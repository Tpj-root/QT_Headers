#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QWidget>
#include <QtPlugin>
#define PluginInterface_iid "org.example.MyQtSoftware.PluginInterface/1.0"


class PluginInterface
{
public:
    virtual ~PluginInterface() = default;

    virtual QString pluginName() const = 0;

    virtual QWidget *createWidget(QWidget *parent) = 0;
};

#define PluginInterface_iid "org.example.MyQtSoftware.PluginInterface/1.0"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif