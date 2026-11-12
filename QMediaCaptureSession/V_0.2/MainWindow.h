// ============================================================
// MainWindow.h
// ============================================================
#pragma once

#include <QMainWindow>
#include <QMediaCaptureSession>

class QCamera;
class QLabel;
class QAction;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QTimer;
class CameraView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void startCamera();
    void updateInfo();
    void captureImage();
    void resetGridDefaults();

private:
    void buildToolBars();

    CameraView          *m_view   = nullptr;
    QCamera             *m_camera = nullptr;
    QMediaCaptureSession m_session;

    QLabel *m_infoLabel = nullptr;

    QAction *m_gridAction    = nullptr;
    QAction *m_rulerAction   = nullptr;
    QAction *m_captureAction = nullptr;

    QDoubleSpinBox *m_xStepBox     = nullptr;
    QDoubleSpinBox *m_yStepBox     = nullptr;
    QSpinBox       *m_lineWidthBox = nullptr;
    QSpinBox       *m_qualityBox   = nullptr;
    QComboBox      *m_formatBox    = nullptr;

    QTimer *m_infoTimer = nullptr;
};