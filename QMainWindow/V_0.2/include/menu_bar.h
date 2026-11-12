#ifndef MENU_BAR_H
#define MENU_BAR_H

#include "app_actions.h"
#include "set_icon.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

// Create the menu bar and all menu actions.
// The actions are stored in AppActions so other headers can reuse them.
inline void createMenuBar(QMainWindow &window, AppActions &actions)
{
    // QMainWindow already owns a menu bar; menuBar() returns it.
    QMenuBar *bar = window.menuBar();

    // Create the three top-level menus.
    actions.fileMenu = bar->addMenu("File");
    actions.editMenu = bar->addMenu("Edit");
    actions.helpMenu = bar->addMenu("Help");

    // ---------------- FILE MENU ----------------

    // Create the New action and assign its resource icon.
    actions.newAction = actions.fileMenu->addAction("New");
    setActionIcon(actions.newAction, ":/new/prefix1/icons/ui-tab--plus.png");

    // Create the Open action.
    actions.openAction = actions.fileMenu->addAction("Open");
    setActionIcon(actions.openAction, ":/new/prefix1/icons/lifebuoy.png");

    // Create the Save actions.
    actions.saveAction = actions.fileMenu->addAction("Save");
    setActionIcon(actions.saveAction, ":/new/prefix1/icons/1.png");

    actions.saveAction1 = actions.fileMenu->addAction("Save1");
    setActionIcon(actions.saveAction1, ":/new/prefix1/icons/2.png");

    actions.saveAction2 = actions.fileMenu->addAction("Save2");
    setActionIcon(actions.saveAction2, ":/new/prefix1/icons/3.png");

    actions.saveAction3 = actions.fileMenu->addAction("Save3");
    setActionIcon(actions.saveAction3, ":/new/prefix1/icons/4.png");

    // A separator visually divides normal actions from Exit.
    actions.fileMenu->addSeparator();

    // Create the Exit action.
    actions.exitAction = actions.fileMenu->addAction("Exit");

    // ---------------- EDIT MENU ----------------

    actions.newActionEdit = actions.editMenu->addAction("New");
    setActionIcon(actions.newActionEdit, ":/new/prefix1/icons/5.png");

    actions.openActionEdit = actions.editMenu->addAction("Open");
    setActionIcon(actions.openActionEdit, ":/new/prefix1/icons/6.png");

    actions.saveActionEdit = actions.editMenu->addAction("Save");
    setActionIcon(actions.saveActionEdit, ":/new/prefix1/icons/7.png");

    actions.saveAction1Edit = actions.editMenu->addAction("Save1");
    setActionIcon(actions.saveAction1Edit, ":/new/prefix1/icons/8.png");

    actions.saveAction2Edit = actions.editMenu->addAction("Save2");
    setActionIcon(actions.saveAction2Edit, ":/new/prefix1/icons/9.png");

    actions.saveAction3Edit = actions.editMenu->addAction("Save3");
    setActionIcon(actions.saveAction3Edit, ":/new/prefix1/icons/10.png");

    // ---------------- HELP MENU ----------------

    actions.aboutAction = actions.helpMenu->addAction("About");
}

#endif // MENU_BAR_H
