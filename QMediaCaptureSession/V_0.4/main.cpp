// ============================================================
// main.cpp
// ============================================================
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setOrganizationName(QStringLiteral("MyCompany"));
    QApplication::setApplicationName(QStringLiteral("Qt Webcam Measurement System"));

    MainWindow window;
    window.show();

    return app.exec();
}