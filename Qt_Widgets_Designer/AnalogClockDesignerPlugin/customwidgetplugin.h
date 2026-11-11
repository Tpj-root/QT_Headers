
#ifndef CUSTOMWIDGETPLUGIN_H
#define CUSTOMWIDGETPLUGIN_H

#include <QObject>
#include <QIcon>
#include <QWidget>

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class AnalogClockPlugin : public QObject,
                          public QDesignerCustomWidgetInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(
        IID "org.qt-project.Qt.QDesignerCustomWidgetInterface"
    )

    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:

    explicit AnalogClockPlugin(QObject *parent = nullptr);

    // Initialize the Designer plugin.
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

    // Create AnalogClock widget.
    QWidget *createWidget(QWidget *parent) override;

private:

    bool initialized = false;
};

#endif
