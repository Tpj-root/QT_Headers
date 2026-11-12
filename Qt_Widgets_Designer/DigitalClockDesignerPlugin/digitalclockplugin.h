
#ifndef DIGITALCLOCKPLUGIN_H
#define DIGITALCLOCKPLUGIN_H

#include <QObject>
#include <QIcon>
#include <QWidget>

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class DigitalClockPlugin : public QObject,
                           public QDesignerCustomWidgetInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(
        IID "org.qt-project.Qt.QDesignerCustomWidgetInterface"
    )

    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:

    explicit DigitalClockPlugin(QObject *parent = nullptr);

    // Plugin initialization.
    void initialize(QDesignerFormEditorInterface *core) override;

    bool isInitialized() const override;

    // Widget information.
    bool isContainer() const override;

    QIcon icon() const override;

    QString domXml() const override;

    QString group() const override;

    QString includeFile() const override;

    QString name() const override;

    QString toolTip() const override;

    QString whatsThis() const override;

    // Create DigitalClock widget.
    QWidget *createWidget(QWidget *parent) override;

private:

    bool initialized = false;
};

#endif
