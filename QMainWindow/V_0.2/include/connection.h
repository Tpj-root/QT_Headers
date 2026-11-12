#ifndef CONNECTION_H
#define CONNECTION_H

#include "app_actions.h"
#include "enable_disable.h"
#include "status_bar.h"

#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>
#include <QObject>

// Connect application actions and the central button to their behavior.
inline void connectApplicationSignals(QMainWindow &window,
                                      QPushButton *button,
                                      AppActions &actions,
                                      QToolBar *mainToolbar)
{
    // Exit closes the main window.
    QObject::connect(actions.exitAction, &QAction::triggered,
                     &window, &QMainWindow::close);

    // Clicking the central button displays a temporary status message.
    QObject::connect(button, &QPushButton::clicked,
                     [&window]()
    {
        showStatusMessage(window, "Button Clicked!", 3000);
    });

    // This helper connects several actions to one reusable lambda.
    // The lambda displays the action's name in the status bar.
    auto showActionMessage = [&window](const QString &actionName)
    {
        showStatusMessage(window, actionName + " Action Triggered!", 3000);
    };

    // File menu actions.
    QObject::connect(actions.newAction, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("New"); });

    QObject::connect(actions.openAction, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Open"); });

    QObject::connect(actions.saveAction, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Save"); });

    QObject::connect(actions.saveAction1, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Save1"); });

    QObject::connect(actions.saveAction2, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Save2"); });

    QObject::connect(actions.saveAction3, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Save3"); });

    // Edit menu actions.
    QObject::connect(actions.newActionEdit, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Edit New"); });

    QObject::connect(actions.openActionEdit, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Edit Open"); });

    QObject::connect(actions.saveActionEdit, &QAction::triggered,
                     [showActionMessage]() { showActionMessage("Edit Save"); });

    QObject::connect(actions.aboutAction, &QAction::triggered,
                     [&window]()
    {
        showStatusMessage(window, "My Qt Application", 3000);
    });

    // Keep the enable/disable behavior in its own header.
    configureEnableDisable(actions, mainToolbar);
}

#endif // CONNECTION_H
