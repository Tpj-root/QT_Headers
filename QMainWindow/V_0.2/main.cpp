#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>

// Include all of the project's reusable helper headers.
#include "library.h"

int main(int argc, char *argv[])
{
    // QApplication manages the GUI application's event loop and settings.
    QApplication app(argc, argv);

    // Use Qt's Fusion style for a consistent cross-platform appearance.
    QApplication::setStyle("Fusion");

    // Create the main application window.
    QMainWindow window;

    // Set the title shown in the window title bar.
    window.setWindowTitle("My Qt Application");

    // Set the initial window size: width 800, height 500.
    window.resize(800, 500);

    // This structure stores all menu actions so they can be shared.
    AppActions actions;

    // Build the File, Edit and Help menus.
    createMenuBar(window, actions);

    // Pointers receive the toolbars created by createToolBars().
    // QMainWindow owns the toolbar objects after addToolBar().
    QToolBar *mainToolbar = nullptr;
    QToolBar *editToolbar = nullptr;

    // Build the two toolbars and add the shared QAction objects.
    createToolBars(window, actions, mainToolbar, editToolbar);

    // Configure tooltip and status-tip help text.
    configureToolTips(actions);

    // Configure keyboard shortcuts.
    configureShortcuts(actions);

    // Display the initial status-bar message.
    showReadyMessage(window);

    // Create the central widget that occupies the main window's content area.
    QWidget *central = new QWidget(&window);

    // Create the button and make the central widget its parent.
    QPushButton *button = new QPushButton("Click Me", central);

    // Create a vertical layout and install it on the central widget.
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Add the button to the layout.
    layout->addWidget(button);

    // Tell QMainWindow to display this widget in its central area.
    // QMainWindow takes ownership of the central widget.
    window.setCentralWidget(central);

    // Connect menu actions and the button to their behavior.
    connectApplicationSignals(window, button, actions, mainToolbar);

    // Display the completed window.
    window.show();

    // Start Qt's event loop; it runs until the application exits.
    return app.exec();
}
