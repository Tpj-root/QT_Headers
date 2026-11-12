// ============================================================
// MainWindow.cpp  (fixed)
// ============================================================
#include "MainWindow.h"
#include "CameraView.h"
#include "RulerOverlay.h"
#include "ShapeOverlay.h"          // FIX #3: full type for defaultStepProfileCm()

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
#include <QDialog>                 // FIX #3
#include <QVBoxLayout>             // FIX #3
#include <QPlainTextEdit>          // FIX #3
#include <QDialogButtonBox>        // FIX #3
#include <QPushButton>             // FIX #3
#include <QFontDatabase>           // FIX #3
#include <QStringList>             // FIX #3

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Qt Webcam Measurement System"));

    m_view = new CameraView(this);
    setCentralWidget(m_view);

    buildToolBars();

    refreshShapeInfo();            // FIX #6: populate the label initially

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

void MainWindow::refreshShapeInfo()
{
    if (m_shapeInfoLabel && m_view && m_view->shape())
        m_shapeInfoLabel->setText(m_view->shape()->infoString());
}

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

    connect(scaleUp,   &QAction::triggered, this, [this]() { nudgeScale(+0.1); });
    connect(scaleDown, &QAction::triggered, this, [this]() { nudgeScale(-0.1); });

    QAction *reset = gridBar->addAction(tr("Reset"));
    connect(reset, &QAction::triggered, this, &MainWindow::resetGridDefaults);

    // ==================== SHAPE TOOLBAR ====================
    QToolBar *shapeBar = addToolBar(tr("Shape"));
    shapeBar->setObjectName(QStringLiteral("shapeToolBar"));
    shapeBar->setMovable(false);

    m_shapeAction = shapeBar->addAction(tr("Show Shape"));
    m_shapeAction->setCheckable(true);
    m_shapeAction->setChecked(true);
    connect(m_shapeAction, &QAction::toggled, this,
            [this](bool on) { m_view->shape()->setShapeEnabled(on); });

    shapeBar->addWidget(new QLabel(tr("  Line: ")));
    m_shapeLineBox = new QSpinBox(shapeBar);
    m_shapeLineBox->setRange(1, 10);
    m_shapeLineBox->setValue(2);
    m_shapeLineBox->setSuffix(tr(" px"));
    shapeBar->addWidget(m_shapeLineBox);
    connect(m_shapeLineBox, &QSpinBox::valueChanged, this,
            [this](int v) { m_view->shape()->setShapeLineWidth(v); });

    QAction *editPts = shapeBar->addAction(tr("Edit Points…"));
    connect(editPts, &QAction::triggered, this, [this]()
    {
        QVector<QPointF> pts = m_view->shape()->profilePointsCm();

        QDialog dlg(this);
        dlg.setWindowTitle(tr("Edit Shape Profile — upper half (cm)"));
        dlg.resize(420, 460);
        auto *lay = new QVBoxLayout(&dlg);

        auto *hint = new QLabel(
            tr("One point per line:  X, Y  (cm)\n"
               "Y must be >= 0. First and last points should be on Y = 0\n"
               "(they close the mirrored contour). Snapped to 1 µm."));
        hint->setWordWrap(true);
        lay->addWidget(hint);

        auto *edit = new QPlainTextEdit(&dlg);
        QString s;
        for (const QPointF& p : pts)
            s += QString::number(p.x(), 'f', 6) + ", "
               + QString::number(p.y(), 'f', 6) + "\n";
        edit->setPlainText(s);
        edit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        lay->addWidget(edit);

        auto *btns = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel |
            QDialogButtonBox::RestoreDefaults, &dlg);
        lay->addWidget(btns);
        connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

        // FIX #5: connect from the actual RestoreDefaults button,
        //         not from an expression like &QDialogButtonBox::button(...)
        if (QPushButton *restoreBtn =
                btns->button(QDialogButtonBox::RestoreDefaults))
        {
            connect(restoreBtn, &QPushButton::clicked, &dlg, [&]() {
                QString d;
                for (const QPointF& p : ShapeOverlay::defaultStepProfileCm())
                    d += QString::number(p.x(), 'f', 6) + ", "
                       + QString::number(p.y(), 'f', 6) + "\n";
                edit->setPlainText(d);
            });
        }

        if (dlg.exec() != QDialog::Accepted) return;

        QVector<QPointF> parsed;
        for (const QString& line : edit->toPlainText().split('\n'))
        {
            const QString t = line.trimmed();
            if (t.isEmpty()) continue;
            const QStringList parts = t.split(',');
            if (parts.size() != 2) continue;
            bool okx = false, oky = false;
            const double x = parts[0].trimmed().toDouble(&okx);
            const double y = parts[1].trimmed().toDouble(&oky);
            if (okx && oky && y >= 0.0)
                parsed.append(QPointF(x, y));
        }
        if (parsed.size() >= 3)
        {
            m_view->shape()->setProfilePointsCm(parsed);
            refreshShapeInfo();
        }
    });

    shapeBar->addSeparator();

    m_shapeInfoLabel = new QLabel;
    m_shapeInfoLabel->setStyleSheet(
        "QLabel { color: #00dcff; font-weight: bold; }");
    shapeBar->addWidget(m_shapeInfoLabel);

    // ==================== CAMERA TOOLBAR ====================
    QToolBar *camBar = addToolBar(tr("Camera"));
    camBar->setObjectName(QStringLiteral("cameraToolBar"));
    camBar->setMovable(false);

    m_captureAction = camBar->addAction(tr("Save Capture"));
    m_captureAction->setShortcut(QKeySequence::Save);
    connect(m_captureAction, &QAction::triggered,
            this, &MainWindow::captureImage);

    m_includeGridBox = new QCheckBox(tr("Include Grid"), camBar);
    m_includeGridBox->setChecked(true);
    m_includeGridBox->setToolTip(
        tr("Save the image exactly as displayed (camera picture + ruler "
           "overlay). Unchecked = save raw camera frame at full resolution."));
    camBar->addWidget(m_includeGridBox);

    // FIX #4: actually create the Include-Shape checkbox
    m_includeShapeBox = new QCheckBox(tr("Include Shape"), camBar);
    m_includeShapeBox->setChecked(true);
    m_includeShapeBox->setToolTip(
        tr("Also render the step-cut shape into the saved image."));
    camBar->addWidget(m_includeShapeBox);

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

    refreshShapeInfo();            // FIX #6: keep label in sync
}

// ------------------------------------------------------------------
void MainWindow::captureImage()
{
    const bool wantGrid  = m_includeGridBox  && m_includeGridBox->isChecked();
    const bool wantShape = m_includeShapeBox && m_includeShapeBox->isChecked();

    QImage img;

    if (wantGrid || wantShape)
    {
        RulerOverlay *ruler = m_view->ruler();
        ShapeOverlay *shape = m_view->shape();

        const bool sg = ruler->isGridEnabled();
        const bool sr = ruler->isRulersEnabled();
        const bool ss = shape->isShapeEnabled();

        ruler->setGridEnabled(wantGrid);
        ruler->setRulersEnabled(wantGrid);
        shape->setShapeEnabled(wantShape);

        img = m_view->grabViewImage();

        ruler->setGridEnabled(sg);
        ruler->setRulersEnabled(sr);
        shape->setShapeEnabled(ss);
    }
    else
    {
        img = m_view->currentImage();
    }

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
            tr("Saved [grid=%1 shape=%2]: %3")
                .arg(wantGrid  ? tr("on") : tr("off"),
                     wantShape ? tr("on") : tr("off"),
                     path),
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