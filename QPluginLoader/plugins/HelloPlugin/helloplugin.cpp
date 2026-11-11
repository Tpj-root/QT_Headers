#include "plugininterface.h"

#include <QObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class HelloPlugin : public QObject, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)

public:

    QString pluginName() const override
    {
        return "Hello Plugin";
    }

    QWidget *createWidget(QWidget *parent) override
    {
        QWidget *page = new QWidget(parent);

        auto *layout = new QVBoxLayout(page);

        auto *label = new QLabel("Hello! This feature came from a plugin.");
        auto *button = new QPushButton("Click Me");

        layout->addWidget(label);
        layout->addWidget(button);
        layout->addStretch();

        QObject::connect(button, &QPushButton::clicked,
                         label, [label]() {
            label->setText("Plugin button clicked!");
        });

        return page;
    }
};

#include "helloplugin.moc"