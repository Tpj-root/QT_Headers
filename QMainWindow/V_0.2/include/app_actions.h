#ifndef APP_ACTIONS_H
#define APP_ACTIONS_H

#include <QAction>
#include <QMenu>
#include <QToolBar>

// This structure stores pointers to the actions and menus used by the app.
// The same QAction pointer can be shared by a menu and a toolbar.
struct AppActions
{
    // Menu pointers.
    QMenu *fileMenu = nullptr;
    QMenu *editMenu = nullptr;
    QMenu *helpMenu = nullptr;

    // File menu actions.
    QAction *newAction = nullptr;
    QAction *openAction = nullptr;
    QAction *saveAction = nullptr;
    QAction *saveAction1 = nullptr;
    QAction *saveAction2 = nullptr;
    QAction *saveAction3 = nullptr;
    QAction *exitAction = nullptr;

    // Edit menu actions.
    QAction *newActionEdit = nullptr;
    QAction *openActionEdit = nullptr;
    QAction *saveActionEdit = nullptr;
    QAction *saveAction1Edit = nullptr;
    QAction *saveAction2Edit = nullptr;
    QAction *saveAction3Edit = nullptr;

    // Help menu action.
    QAction *aboutAction = nullptr;
};

#endif // APP_ACTIONS_H
