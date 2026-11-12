
#include <QApplication>
#include <QMainWindow>
#include <QCamera>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QVideoWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    // Detect available webcams
    auto cameras = QMediaDevices::videoInputs();

    if (cameras.isEmpty())
        return 1;

    // Create camera
    QCamera camera(cameras.first());

    // Create capture session
    QMediaCaptureSession session;

    // Create video display
    QVideoWidget videoWidget;

    // Connect camera and video display
    session.setCamera(&camera);
    session.setVideoOutput(&videoWidget);

    window.setCentralWidget(&videoWidget);

    window.resize(800, 600);
    window.show();

    // Start webcam
    camera.start();

    return app.exec();
}
