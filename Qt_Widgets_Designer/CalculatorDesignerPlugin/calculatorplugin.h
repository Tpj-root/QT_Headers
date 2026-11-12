
#ifndef CALCULATORPLUGIN_H
#define CALCULATORPLUGIN_H

#include <QObject>
#include <QIcon>
#include <QWidget>

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class CalculatorPlugin : public QObject,
                         public QDesignerCustomWidgetInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(
        IID "org.qt-project.Qt.QDesignerCustomWidgetInterface"
    )

    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:

    explicit CalculatorPlugin(QObject *parent = nullptr);

    void initialize(QDesignerFormEditorInterface *core) override;

    bool isInitialized() const override;

    bool isContainer() const override;

    QIcon icon() const override;

    QString domXml() const override;

    QString group() const override;

    QString includeFile() const override;

    QString name() const override;

    QString toolTip() const override;

    QString whatsThis() const override;

    QWidget *createWidget(QWidget *parent) override;

private:

    bool initialized = false;
};

#endif
