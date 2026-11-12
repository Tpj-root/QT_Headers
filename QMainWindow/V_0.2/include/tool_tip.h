#ifndef TOOL_TIP_H
#define TOOL_TIP_H

#include "app_actions.h"

// Configure tooltip and status-tip text for the actions.
inline void configureToolTips(const AppActions &actions)
{
    // ToolTip appears when the mouse pointer rests over an action.
    actions.newActionEdit->setToolTip("Create a new file");

    // Use actual newline escape sequences for multi-line tooltip text.
    actions.openActionEdit->setToolTip("Open a file\nChoose a file to open");

    actions.saveAction->setToolTip("Save the current file");
    actions.exitAction->setToolTip("Close the application");

    // StatusTip text is displayed in the QMainWindow status bar
    // when the user hovers over the action.
    actions.newActionEdit->setStatusTip("Create a new file");
    actions.openActionEdit->setStatusTip("Open an existing file");
    actions.saveActionEdit->setStatusTip("Save the current file");
    actions.aboutAction->setStatusTip("Show information about this application");
}

#endif // TOOL_TIP_H
