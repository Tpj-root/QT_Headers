#ifndef CONNECTIONSTATUSPLUGIN_H
#define CONNECTIONSTATUSPLUGIN_H

#include <QObject>
#include <QDesignerCustomWidgetInterface>

class ConnectionStatusPlugin :
        public QObject,
        public QDesignerCustomWidgetInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID
        "org.qt-project.Qt.QDesignerCustomWidgetInterface")

    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:

    explicit ConnectionStatusPlugin(QObject *parent = nullptr);

    bool isInitialized() const override;

    void initialize(
        QDesignerFormEditorInterface *core) override;

    QWidget *createWidget(QWidget *parent) override;

    QString name() const override;

    QString group() const override;

    QIcon icon() const override;

    QString toolTip() const override;

    QString whatsThis() const override;

    bool isContainer() const override;

    QString domXml() const override;

    QString includeFile() const override;

private:

    bool m_initialized = false;
};

#endif