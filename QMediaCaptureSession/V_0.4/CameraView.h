// ============================================================
// CameraView.h  (updated – added grabViewImage)
// ============================================================
#pragma once

#include <QWidget>
#include <QImage>
#include <QElapsedTimer>
#include <QString>

class QVideoSink;
class QVideoFrame;
class RulerOverlay;
class ShapeOverlay;

class CameraView : public QWidget
{
    Q_OBJECT

public:
    explicit CameraView(QWidget *parent = nullptr);

    QVideoSink   *videoSink() const { return m_sink; }
    RulerOverlay *ruler()     const { return m_ruler; }
    ShapeOverlay *shape()     const { return m_shape; }   // NEW

    QImage  currentImage() const { return m_image; }
    QSize   frameSize() const { return m_image.size(); }
    QString pixelFormatName() const { return m_pixelFormatName; }
    double  frameRate() const { return m_fps; }

    QImage  grabViewImage();

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
    ShapeOverlay *m_shape = nullptr;   // NEW

    QImage  m_image;
    QString m_pixelFormatName = QStringLiteral("n/a");

    QElapsedTimer m_fpsTimer;
    int           m_frameCount = 0;
    double        m_fps        = 0.0;
};