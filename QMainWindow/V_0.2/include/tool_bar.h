#ifndef TOOL_BAR_H
#define TOOL_BAR_H

#include "app_actions.h"

#include <QMainWindow>
#include <QToolBar>

// Create both toolbars and add the existing menu actions to them.
// QMainWindow takes ownership of toolbars added with addToolBar().
inline void createToolBars(QMainWindow &window,
                           const AppActions &actions,
                           QToolBar *&mainToolbar,
                           QToolBar *&editToolbar)
{
    // Create the main toolbar.
    mainToolbar = window.addToolBar("Main Toolbar");

    // Add the same QAction objects used by the File menu.
    // This makes menu and toolbar behavior stay synchronized.
    mainToolbar->addAction(actions.newAction);
    mainToolbar->addSeparator();

    mainToolbar->addAction(actions.openAction);
    mainToolbar->addSeparator();

    mainToolbar->addAction(actions.saveAction);
    mainToolbar->addSeparator();

    mainToolbar->addAction(actions.saveAction1);
    mainToolbar->addAction(actions.saveAction2);
    mainToolbar->addAction(actions.saveAction3);
    mainToolbar->addSeparator();

    mainToolbar->addAction(actions.exitAction);

    // Create a second toolbar for Edit actions.
    editToolbar = window.addToolBar("Edit Toolbar");

    // Add the existing Edit menu actions to the Edit toolbar.
    editToolbar->addAction(actions.newActionEdit);
    editToolbar->addAction(actions.openActionEdit);
    editToolbar->addAction(actions.saveActionEdit);
    editToolbar->addAction(actions.saveAction1Edit);
    editToolbar->addAction(actions.saveAction2Edit);
    editToolbar->addAction(actions.saveAction3Edit);
}

#endif // TOOL_BAR_H
