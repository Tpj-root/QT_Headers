#include "plugininterface.h"

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabasePlugin : public QObject, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)

private:

    QSqlDatabase db;

public:

    QString pluginName() const override
    {
        return "Database";
    }

    QWidget *createWidget(QWidget *parent) override
    {
        QWidget *page = new QWidget(parent);

        auto *layout = new QVBoxLayout(page);

        auto *title = new QLabel("SQLite Database Plugin");
        title->setStyleSheet("font-size: 20px; font-weight: bold;");

        auto *nameInput = new QLineEdit(page);
        nameInput->setPlaceholderText("Enter a name");

        auto *addButton = new QPushButton("Add Record", page);

        auto *table = new QTableWidget(page);

        table->setColumnCount(2);
        table->setHorizontalHeaderLabels(
            {"ID", "Name"}
        );

        table->horizontalHeader()->setSectionResizeMode(
            QHeaderView::Stretch
        );

        table->setEditTriggers(
            QAbstractItemView::NoEditTriggers
        );

        layout->addWidget(title);
        layout->addWidget(nameInput);
        layout->addWidget(addButton);
        layout->addWidget(table);

        // Create application data folder
        QString dataPath =
            QStandardPaths::writableLocation(
                QStandardPaths::AppDataLocation
            );

        if (!QDir().mkpath(dataPath))
        {
            title->setText("Cannot create database folder");
            addButton->setEnabled(false);
            return page;
        }

        // Create SQLite connection
        const QString connectionName =
            "MyQtSoftwareDatabasePlugin";

        db = QSqlDatabase::addDatabase(
            "QSQLITE",
            connectionName
        );

        db.setDatabaseName(dataPath + "/mydatabase.db");

        // Open database
        if (!db.open())
        {
            title->setText("Database error: " + db.lastError().text());
            addButton->setEnabled(false);
            return page;
        }

        // Create table
        QSqlQuery createQuery(db);

        if (!createQuery.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL)"
        ))
        {
            title->setText("Table error: " + createQuery.lastError().text());
            addButton->setEnabled(false);
            return page;
        }

        // Function to load database records
        auto refreshTable = [=]()
        {
            QSqlQuery query(db);

            if (!query.exec(
                "SELECT id, name FROM users ORDER BY id"
            ))
            {
                QMessageBox::warning(
                    page,
                    "Database Error",
                    query.lastError().text()
                );

                return;
            }

            table->setRowCount(0);

            while (query.next())
            {
                int row = table->rowCount();

                table->insertRow(row);

                table->setItem(
                    row, 0,
                    new QTableWidgetItem(
                        query.value(0).toString()
                    )
                );

                table->setItem(
                    row, 1,
                    new QTableWidgetItem(
                        query.value(1).toString()
                    )
                );
            }
        };

        // Add record button
        connect(addButton, &QPushButton::clicked,
                page, [=]()
        {
            QString name = nameInput->text().trimmed();

            if (name.isEmpty())
            {
                QMessageBox::information(
                    page,
                    "Input",
                    "Please enter a name."
                );

                return;
            }

            QSqlQuery insertQuery(db);

            insertQuery.prepare(
                "INSERT INTO users (name) VALUES (:name)"
            );

            insertQuery.bindValue(":name", name);

            if (!insertQuery.exec())
            {
                QMessageBox::warning(
                    page,
                    "Database Error",
                    insertQuery.lastError().text()
                );

                return;
            }

            nameInput->clear();

            refreshTable();
        });

        // Load existing records
        refreshTable();

        return page;
    }
};

#include "databaseplugin.moc"