#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QList>

class QPluginLoader;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    QTabWidget *tabs;

    // Keep loaders alive while plugin widgets are being used.
    QList<QPluginLoader *> pluginLoaders;

    void loadPlugins();
};

#endif