#ifndef ENABLE_DISABLE_H
#define ENABLE_DISABLE_H

#include "app_actions.h"

#include <QObject>
#include <QToolBar>

// Connect the Edit menu actions to enable/disable the File menu and main toolbar.
inline void configureEnableDisable(AppActions &actions, QToolBar *mainToolbar)
{
    // When Edit -> Open is triggered, disable the File menu and main toolbar.
    QObject::connect(actions.openActionEdit, &QAction::triggered,
                     [fileMenu = actions.fileMenu, mainToolbar]()
    {
        // menuAction() is the QAction representing the whole File menu.
        // Disabling it disables the File menu entry in the menu bar.
        fileMenu->menuAction()->setEnabled(false);

        // Disable the main toolbar as a whole.
        mainToolbar->setEnabled(false);
    });

    // When Edit -> New is triggered, enable them again.
    // The Edit toolbar remains available so the user can restore the UI.
    QObject::connect(actions.newActionEdit, &QAction::triggered,
                     [fileMenu = actions.fileMenu, mainToolbar]()
    {
        fileMenu->menuAction()->setEnabled(true);
        mainToolbar->setEnabled(true);
    });
}

#endif // ENABLE_DISABLE_H
