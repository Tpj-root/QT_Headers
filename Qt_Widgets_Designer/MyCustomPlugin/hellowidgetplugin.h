
#ifndef HELLOWIDGETPLUGIN_H
#define HELLOWIDGETPLUGIN_H

#include <QObject>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class HelloWidgetPlugin : public QObject,
                          public QDesignerCustomWidgetInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID
        "org.qt-project.Qt.QDesignerCustomWidgetInterface")

    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:

    explicit HelloWidgetPlugin(QObject *parent = nullptr);

    // Plugin initialization.
    void initialize(QDesignerFormEditorInterface *core) override;

    bool isInitialized() const override;

    // Widget information.
    QString name() const override;

    QString group() const override;

    QString includeFile() const override;

    QIcon icon() const override;

    QString toolTip() const override;

    QString whatsThis() const override;

    bool isContainer() const override;

    // Create the actual widget.
    QWidget *createWidget(QWidget *parent) override;

    // XML description for Qt Designer.
    QString domXml() const override;

private:

    bool m_initialized = false;
};

#endif
