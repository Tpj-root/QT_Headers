#include "mainwindow.h"
#include "plugininterface.h"
#include <QCoreApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("My Qt Software");
    resize(900, 600);

    // Central tab area
    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    // Welcome page
    QLabel *welcome = new QLabel(
        "Welcome to My Qt Software\n\n"
        "Your future features will be added through plugins.",
        this
    );

    welcome->setAlignment(Qt::AlignCenter);
    tabs->addTab(welcome, "Home");

    // Menu bar
    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered,
            this, &QWidget::close);

    // Toolbar
    QToolBar *toolbar = addToolBar("Main Toolbar");
    toolbar->addAction(exitAction);

    // Status bar
    statusBar()->showMessage("Application ready");

    // Load installed plugins
    loadPlugins();
}

void MainWindow::loadPlugins()
{
    QDir pluginDir(QCoreApplication::applicationDirPath()
                   + "/plugins");

    if (!pluginDir.exists()) {
        statusBar()->showMessage("Plugin folder not found");
        return;
    }

    const QStringList files =
        pluginDir.entryList(QDir::Files);

    for (const QString &file : files) {

        auto *loader = new QPluginLoader(
            pluginDir.absoluteFilePath(file), this
        );

        QObject *pluginObject = loader->instance();

        if (!pluginObject) {
            qWarning() << "Plugin load failed:"
                       << file << loader->errorString();

            delete loader;
            continue;
        }

        PluginInterface *plugin =
            qobject_cast<PluginInterface *>(pluginObject);

        if (!plugin) {
            qWarning() << "Invalid plugin:" << file;
            delete loader;
            continue;
        }

        QWidget *widget = plugin->createWidget(tabs);

        if (!widget) {
            qWarning() << "Plugin returned no widget:" << file;
            delete loader;
            continue;
        }

        tabs->addTab(widget, plugin->pluginName());

        // Keep the loader alive for the plugin's lifetime.
        pluginLoaders.append(loader);

        qDebug() << "Loaded plugin:" << plugin->pluginName();
    }
}