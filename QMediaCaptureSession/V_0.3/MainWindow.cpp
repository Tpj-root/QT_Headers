// ============================================================
// MainWindow.cpp  (updated – calibration + capture with grid)
// ============================================================
#include "MainWindow.h"
#include "CameraView.h"
#include "RulerOverlay.h"

#include <QCamera>
#include <QMediaDevices>

#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QStatusBar>
#include <QTimer>
#include <QDir>
#include <QDateTime>
#include <QImageWriter>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Qt Webcam Measurement System"));

    m_view = new CameraView(this);
    setCentralWidget(m_view);

    buildToolBars();

    m_infoTimer = new QTimer(this);
    m_infoTimer->setInterval(500);
    connect(m_infoTimer, &QTimer::timeout, this, &MainWindow::updateInfo);
    m_infoTimer->start();

    connect(m_view, &CameraView::frameRateChanged,
            this, &MainWindow::updateInfo);

    startCamera();

    statusBar()->showMessage(
        tr("Camera ready  |  X/Y ruler, origin at center (0,0)  |  1 mm ticks"));

    resize(1100, 750);
}

// ------------------------------------------------------------------
// Toolbars
// ------------------------------------------------------------------
void MainWindow::buildToolBars()
{
    // ==================== GRID TOOLBAR ====================
    QToolBar *gridBar = addToolBar(tr("Grid"));
    gridBar->setObjectName(QStringLiteral("gridToolBar"));
    gridBar->setMovable(false);

    m_gridAction = gridBar->addAction(tr("Show Grid"));
    m_gridAction->setCheckable(true);
    m_gridAction->setChecked(true);
    connect(m_gridAction, &QAction::toggled, this,
            [this](bool on) { m_view->ruler()->setGridEnabled(on); });

    m_rulerAction = gridBar->addAction(tr("Show Rulers"));
    m_rulerAction->setCheckable(true);
    m_rulerAction->setChecked(true);
    connect(m_rulerAction, &QAction::toggled, this,
            [this](bool on) { m_view->ruler()->setRulersEnabled(on); });

    gridBar->addSeparator();

    gridBar->addWidget(new QLabel(tr("  X step: ")));
    m_xStepBox = new QDoubleSpinBox(gridBar);
    m_xStepBox->setRange(0.1, 20.0);
    m_xStepBox->setDecimals(1);
    m_xStepBox->setSingleStep(0.1);
    m_xStepBox->setSuffix(tr(" cm"));
    m_xStepBox->setValue(1.0);
    m_xStepBox->setToolTip(tr("Spacing between VERTICAL grid lines"));
    gridBar->addWidget(m_xStepBox);
    connect(m_xStepBox, &QDoubleSpinBox::valueChanged, this,
            [this](double v) { m_view->ruler()->setXSpacingCm(v); });

    gridBar->addWidget(new QLabel(tr("  Y step: ")));
    m_yStepBox = new QDoubleSpinBox(gridBar);
    m_yStepBox->setRange(0.1, 20.0);
    m_yStepBox->setDecimals(1);
    m_yStepBox->setSingleStep(0.1);
    m_yStepBox->setSuffix(tr(" cm"));
    m_yStepBox->setValue(1.0);
    m_yStepBox->setToolTip(tr("Spacing between HORIZONTAL grid lines"));
    gridBar->addWidget(m_yStepBox);
    connect(m_yStepBox, &QDoubleSpinBox::valueChanged, this,
            [this](double v) { m_view->ruler()->setYSpacingCm(v); });

    gridBar->addWidget(new QLabel(tr("  Line: ")));
    m_lineWidthBox = new QSpinBox(gridBar);
    m_lineWidthBox->setRange(1, 10);
    m_lineWidthBox->setValue(1);
    m_lineWidthBox->setSuffix(tr(" px"));
    m_lineWidthBox->setToolTip(tr("Grid / ruler line thickness"));
    gridBar->addWidget(m_lineWidthBox);
    connect(m_lineWidthBox, &QSpinBox::valueChanged, this,
            [this](int v) { m_view->ruler()->setGridLineWidth(v); });

    gridBar->addSeparator();

    // ==================== CALIBRATION ====================
    // NEW: numeric +/-, fine step
    QAction *scaleUp   = gridBar->addAction(tr("Cal +"));
    QAction *scaleDown = gridBar->addAction(tr("Cal -"));
    scaleUp->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal));
    scaleDown->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus));
    scaleUp->setToolTip(tr("Increase pixels-per-cm by 0.1 (Ctrl+=)"));
    scaleDown->setToolTip(tr("Decrease pixels-per-cm by 0.1 (Ctrl+-)"));

    gridBar->addWidget(new QLabel(tr("  ")));
    m_pxPerCmBox = new QDoubleSpinBox(gridBar);
    m_pxPerCmBox->setRange(5.0, 300.0);
    m_pxPerCmBox->setDecimals(1);
    m_pxPerCmBox->setSingleStep(0.1);
    m_pxPerCmBox->setSuffix(tr(" px/cm"));
    m_pxPerCmBox->setValue(m_view->ruler()->pixelsPerCm());
    m_pxPerCmBox->setToolTip(
        tr("Manual calibration – adjust until the ruler matches a physical "
           "reference in the camera view (e.g. a real ruler on the desk)."));
    m_pxPerCmBox->setMinimumWidth(110);
    gridBar->addWidget(m_pxPerCmBox);

    connect(m_pxPerCmBox, &QDoubleSpinBox::valueChanged, this,
            [this](double v) { m_view->ruler()->setPixelsPerCm(v); });

    // The +/- actions just push the spin box – one source of truth.
    connect(scaleUp,   &QAction::triggered, this, [this]() { nudgeScale(+0.1); });
    connect(scaleDown, &QAction::triggered, this, [this]() { nudgeScale(-0.1); });

    QAction *reset = gridBar->addAction(tr("Reset"));
    connect(reset, &QAction::triggered, this, &MainWindow::resetGridDefaults);

    // ==================== CAMERA TOOLBAR ====================
    QToolBar *camBar = addToolBar(tr("Camera"));
    camBar->setObjectName(QStringLiteral("cameraToolBar"));
    camBar->setMovable(false);

    m_captureAction = camBar->addAction(tr("Save Capture"));
    m_captureAction->setShortcut(QKeySequence::Save);
    connect(m_captureAction, &QAction::triggered,
            this, &MainWindow::captureImage);

    // NEW: capture with grid overlay
    m_includeGridBox = new QCheckBox(tr("Include Grid"), camBar);
    m_includeGridBox->setChecked(true);
    m_includeGridBox->setToolTip(
        tr("Save the image exactly as displayed (camera picture + ruler "
           "overlay). Unchecked = save raw camera frame at full resolution."));
    camBar->addWidget(m_includeGridBox);

    camBar->addSeparator();

    camBar->addWidget(new QLabel(tr("  Format: ")));
    m_formatBox = new QComboBox(camBar);
    m_formatBox->addItem(tr("JPEG"), QStringLiteral("jpg"));
    m_formatBox->addItem(tr("PNG"),  QStringLiteral("png"));
    camBar->addWidget(m_formatBox);

    camBar->addWidget(new QLabel(tr("  Quality: ")));
    m_qualityBox = new QSpinBox(camBar);
    m_qualityBox->setRange(1, 100);
    m_qualityBox->setValue(90);
    camBar->addWidget(m_qualityBox);

    connect(m_formatBox, &QComboBox::currentIndexChanged, this,
            [this](int)
            {
                const bool jpeg =
                    m_formatBox->currentData().toString() == QLatin1String("jpg");
                m_qualityBox->setEnabled(jpeg);
                updateInfo();
            });

    connect(m_qualityBox, &QSpinBox::valueChanged,
            this, &MainWindow::updateInfo);

    camBar->addSeparator();

    m_infoLabel = new QLabel(tr("  FPS: --"));
    m_infoLabel->setMinimumWidth(420);
    camBar->addWidget(m_infoLabel);
}

// ------------------------------------------------------------------
void MainWindow::nudgeScale(double delta)
{
    m_pxPerCmBox->setValue(m_pxPerCmBox->value() + delta);
}

// ------------------------------------------------------------------
void MainWindow::startCamera()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

    if (cameras.isEmpty())
    {
        m_captureAction->setEnabled(false);
        statusBar()->showMessage(tr("No camera device found."));
        return;
    }

    m_camera = new QCamera(cameras.first(), this);

    m_session.setCamera(m_camera);
    m_session.setVideoSink(m_view->videoSink());

    m_camera->start();
}

// ------------------------------------------------------------------
void MainWindow::updateInfo()
{
    if (!m_view || !m_infoLabel)
        return;

    const QSize sz = m_view->frameSize();
    const QString enc = m_view->pixelFormatName().isEmpty()
                            ? QStringLiteral("n/a")
                            : m_view->pixelFormatName();

    const bool jpeg =
        m_formatBox && m_formatBox->currentData().toString() == QLatin1String("jpg");

    const QString quality = jpeg
                                ? QString::number(m_qualityBox->value())
                                : tr("lossless");

    m_infoLabel->setText(
        tr("  FPS: %1 | %2x%3 | Enc: %4 | Quality: %5")
            .arg(m_view->frameRate(), 0, 'f', 1)
            .arg(sz.width())
            .arg(sz.height())
            .arg(enc, quality));
}

// ------------------------------------------------------------------
// Save capture – with grid by default.
// ------------------------------------------------------------------
void MainWindow::captureImage()
{
    const bool withGrid = m_includeGridBox && m_includeGridBox->isChecked();

    QImage img = withGrid
                     ? m_view->grabViewImage()   // video + overlay, screen resolution
                     : m_view->currentImage();   // raw frame, full resolution

    if (img.isNull())
    {
        statusBar()->showMessage(tr("No frame available to capture."), 3000);
        return;
    }

    QString dir = QStandardPaths::writableLocation(
        QStandardPaths::PicturesLocation);
    if (dir.isEmpty())
        dir = QDir::homePath();
    QDir().mkpath(dir);

    const QString ext = m_formatBox->currentData().toString();
    const QString fileName =
        QStringLiteral("capture_%1.%2")
            .arg(QDateTime::currentDateTime()
                     .toString(QStringLiteral("yyyyMMdd_hhmmss_zzz")),
                 ext);

    const QString path = dir + QLatin1Char('/') + fileName;

    QImageWriter writer(path, ext.toLatin1());
    if (ext.compare(QLatin1String("jpg"), Qt::CaseInsensitive) == 0)
        writer.setQuality(m_qualityBox->value());

    if (writer.write(img))
    {
        statusBar()->showMessage(
            tr("Saved (%1 grid): %2")
                .arg(withGrid ? tr("with") : tr("without"), path),
            6000);
    }
    else
    {
        statusBar()->showMessage(
            tr("Save failed: %1").arg(writer.errorString()), 6000);
    }
}

// ------------------------------------------------------------------
void MainWindow::resetGridDefaults()
{
    // Route through the spin box so the toolbar stays in sync.
    m_pxPerCmBox->setValue(38.0);

    RulerOverlay *r = m_view->ruler();
    r->setXSpacingCm(1.0);
    r->setYSpacingCm(1.0);
    r->setGridLineWidth(1);
    r->setGridEnabled(true);
    r->setRulersEnabled(true);

    m_xStepBox->blockSignals(true);
    m_yStepBox->blockSignals(true);
    m_lineWidthBox->blockSignals(true);
    m_gridAction->blockSignals(true);
    m_rulerAction->blockSignals(true);

    m_xStepBox->setValue(1.0);
    m_yStepBox->setValue(1.0);
    m_lineWidthBox->setValue(1);
    m_gridAction->setChecked(true);
    m_rulerAction->setChecked(true);

    m_xStepBox->blockSignals(false);
    m_yStepBox->blockSignals(false);
    m_lineWidthBox->blockSignals(false);
    m_gridAction->blockSignals(false);
    m_rulerAction->blockSignals(false);

    updateInfo();
}