#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");

    QMainWindow window;

    window.setWindowTitle("My Qt Application");
    window.resize(800, 500);

    // MENU BAR
    // QMenu *fileMenu = window.menuBar()->addMenu("File");
    // QMenu *fileMenu = window.menuBar()->addMenu(
    //     QIcon(":/new/prefix1/icons/smiley.png"), "File"
        // );
    QMenu *fileMenu = window.menuBar()->addMenu("File");

    QMenu *editMenu = window.menuBar()->addMenu("Edit");
    QMenu *helpMenu = window.menuBar()->addMenu("Help");

    QAction *newAction = fileMenu->addAction(QIcon(":/new/prefix1/icons/ui-tab--plus.png"), "New");
    QAction *openAction = fileMenu->addAction("Open");
    openAction->setIcon(QIcon(":/new/prefix1/icons/lifebuoy.png"));

    QAction *saveAction = fileMenu->addAction("Save");
    saveAction->setIcon(QIcon(":/new/prefix1/icons/1.png"));
    QAction *saveAction1 = fileMenu->addAction("Save1");
    saveAction1->setIcon(QIcon(":/new/prefix1/icons/2.png"));
    QAction *saveAction2 = fileMenu->addAction("Save2");
    saveAction2->setIcon(QIcon(":/new/prefix1/icons/3.png"));
    QAction *saveAction3 = fileMenu->addAction("Save3");
    saveAction3->setIcon(QIcon(":/new/prefix1/icons/4.png"));


    QAction *newAction_editMenu = editMenu->addAction("New");
    newAction_editMenu->setIcon(QIcon(":/new/prefix1/icons/5.png"));
    newAction_editMenu->setToolTip("Create a new file"
                                   "Create \n"
                                   "Create \n"
                                   "Create /n ");
    // newAction->setText("Create New");
    // newAction->setEnabled(false);
    // newAction->setVisible(false);


    newAction_editMenu->setStatusTip("iam what a new file");
    QAction *openAction_editMenu = editMenu->addAction("Open");
    openAction_editMenu->setIcon(QIcon(":/new/prefix1/icons/6.png"));
    QAction *saveAction_editMenu = editMenu->addAction("Save");
    saveAction_editMenu->setIcon(QIcon(":/new/prefix1/icons/7.png"));
    QAction *saveAction1_editMenu = editMenu->addAction("Save1");
    saveAction1_editMenu->setIcon(QIcon(":/new/prefix1/icons/8.png"));
    QAction *saveAction2_editMenu = editMenu->addAction("Save2");
    saveAction2_editMenu->setIcon(QIcon(":/new/prefix1/icons/9.png"));
    QAction *saveAction3_editMenu = editMenu->addAction("Save3");
    saveAction3_editMenu->setIcon(QIcon(":/new/prefix1/icons/10.png"));

    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("Exit");

    // TOOL BAR
    QToolBar *toolbar = window.addToolBar("Main Toolbar");

    toolbar->addAction(newAction);
    toolbar->addSeparator();

    toolbar->addAction(openAction);
    toolbar->addSeparator();

    toolbar->addAction(saveAction);
    toolbar->addSeparator();

    toolbar->addAction(saveAction1);
    toolbar->addAction(saveAction2);
    toolbar->addAction(saveAction3);
    toolbar->addSeparator();
    toolbar->addAction(exitAction);



    // TOOL BAR EDIT
    QToolBar *toolbarEdit = window.addToolBar("Edit Toolbar");

    toolbarEdit->addAction(newAction_editMenu);

    toolbarEdit->addAction(openAction_editMenu);
    toolbarEdit->addAction(saveAction_editMenu);
    toolbarEdit->addAction(saveAction1_editMenu);
    toolbarEdit->addAction(saveAction2_editMenu);
    toolbarEdit->addAction(saveAction3_editMenu);





    // STATUS BAR
    window.statusBar()->showMessage("Application Ready");

    // CENTRAL WIDGET
    QWidget *central = new QWidget;

    QPushButton *button = new QPushButton("Click Me");

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(button);

    window.setCentralWidget(central);

    // SIGNALS
    QObject::connect(exitAction, &QAction::triggered,
                     &window, &QMainWindow::close);

    QObject::connect(button, &QPushButton::clicked,
                     [&window]()
    {
        window.statusBar()->showMessage("Button Clicked!", 3000);
    });


    QObject::connect(openAction_editMenu, &QAction::triggered,
                     [fileMenu]()
                     {
                         // newAction->setEnabled(false);
                         fileMenu->menuAction()->setEnabled(false);
                     });

    QObject::connect(newAction_editMenu, &QAction::triggered,
                     [fileMenu, toolbar]()
                     {
                         fileMenu->menuAction()->setEnabled(true);
                         toolbar->setEnabled(true);
                     });

    QObject::connect(openAction_editMenu, &QAction::triggered,
                     [fileMenu, toolbar]()
                     {
                         fileMenu->menuAction()->setEnabled(false);
                         toolbar->setEnabled(false);
                     });


    auto showMessage = [&window]()
    {
        window.statusBar()->showMessage("Menu Action Triggered!", 3000);
    };

    QObject::connect(newAction,  &QAction::triggered, showMessage);
    QObject::connect(openAction, &QAction::triggered, showMessage);
    QObject::connect(saveAction, &QAction::triggered, showMessage);

    

    window.show();

    return app.exec();
}
