// ============================================================
// CameraView.h
// ============================================================
#pragma once

#include <QWidget>
#include <QImage>
#include <QElapsedTimer>
#include <QString>

class QVideoSink;
class QVideoFrame;
class RulerOverlay;

/**
 * Shows the live camera picture and keeps a RulerOverlay on top.
 *
 * The frame is rendered by QPainter (NOT QVideoWidget) - this is the
 * only reliable way to have a transparent overlay above video output
 * on Windows / macOS, because QVideoWidget is a native window.
 */
class CameraView : public QWidget
{
    Q_OBJECT

public:
    explicit CameraView(QWidget *parent = nullptr);

    QVideoSink   *videoSink() const { return m_sink; }
    RulerOverlay *ruler()     const { return m_ruler; }

    QImage  currentImage() const { return m_image; }   // last frame, full resolution
    QSize   frameSize() const { return m_image.size(); }
    QString pixelFormatName() const { return m_pixelFormatName; }
    double  frameRate() const { return m_fps; }

signals:
    void frameRateChanged(double fps);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onVideoFrameChanged(const QVideoFrame &frame);

private:
    QVideoSink   *m_sink  = nullptr;
    RulerOverlay *m_ruler = nullptr;

    QImage  m_image;
    QString m_pixelFormatName = QStringLiteral("n/a");

    QElapsedTimer m_fpsTimer;
    int           m_frameCount = 0;
    double        m_fps        = 0.0;
};