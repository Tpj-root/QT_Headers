#ifndef SET_SHORTCUT_H
#define SET_SHORTCUT_H

#include "app_actions.h"

#include <QKeySequence>

// Assign keyboard shortcuts to actions.
// QKeySequence provides standard key combinations such as Ctrl+N.
inline void configureShortcuts(const AppActions &actions)
{
    actions.newAction->setShortcut(QKeySequence("Ctrl+N"));
    actions.openAction->setShortcut(QKeySequence("Ctrl+O"));
    actions.saveAction->setShortcut(QKeySequence("Ctrl+S"));

    // Ctrl+Q is a common shortcut for Exit.
    actions.exitAction->setShortcut(QKeySequence("Ctrl+Q"));

    // This is a separate Edit-menu action, so give it a different shortcut.
    actions.newActionEdit->setShortcut(QKeySequence("Ctrl+Alt+N"));
}

#endif // SET_SHORTCUT_H
