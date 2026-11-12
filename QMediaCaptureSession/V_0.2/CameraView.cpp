// ============================================================
// CameraView.cpp
// ============================================================
#include "CameraView.h"
#include "RulerOverlay.h"

#include <QPainter>
#include <QResizeEvent>

#include <QVideoSink>
#include <QVideoFrame>
#include <QVideoFrameFormat>

CameraView::CameraView(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(320, 240);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_sink = new QVideoSink(this);

    connect(m_sink, &QVideoSink::videoFrameChanged,
            this, &CameraView::onVideoFrameChanged);

    m_ruler = new RulerOverlay(this);
    m_ruler->setGeometry(rect());
    m_ruler->raise();

    m_fpsTimer.start();
}

void CameraView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    m_ruler->setGeometry(rect());
    m_ruler->raise();
}

void CameraView::onVideoFrameChanged(const QVideoFrame &frame)
{
    if (!frame.isValid())
        return;

    m_pixelFormatName = QVideoFrameFormat::pixelFormatToString(
        frame.surfaceFormat().pixelFormat());

    const QImage img = frame.toImage();
    if (!img.isNull())
        m_image = img;

    // ---- FPS measurement (once per second) ----
    ++m_frameCount;
    if (m_fpsTimer.elapsed() >= 1000)
    {
        m_fps = (m_frameCount * 1000.0) / double(m_fpsTimer.elapsed());
        m_frameCount = 0;
        m_fpsTimer.restart();
        emit frameRateChanged(m_fps);
    }

    update();          // repaint the video
    m_ruler->update(); // repaint the grid on top of it
}

void CameraView::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // Black background (also covers the letterbox area)
    p.fillRect(rect(), Qt::black);

    if (m_image.isNull())
    {
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, tr("Waiting for camera..."));
        return;
    }

    // Aspect-fit the camera frame into this widget
    QSize scaled = m_image.size();
    scaled.scale(size(), Qt::KeepAspectRatio);

    QRect target(QPoint(0, 0), scaled);
    target.moveCenter(rect().center());

    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.drawImage(target, m_image);
}