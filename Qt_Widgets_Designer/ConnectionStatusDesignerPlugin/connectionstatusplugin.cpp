#include "connectionstatusplugin.h"
#include "connectionstatus.h"

#include <QtPlugin>

ConnectionStatusPlugin::ConnectionStatusPlugin(QObject *parent)
    : QObject(parent)
{
}

bool ConnectionStatusPlugin::isInitialized() const
{
    return m_initialized;
}

void ConnectionStatusPlugin::initialize(
    QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);

    if (m_initialized)
        return;

    m_initialized = true;
}

QWidget *ConnectionStatusPlugin::createWidget(QWidget *parent)
{
    return new ConnectionStatus(parent);
}

QString ConnectionStatusPlugin::name() const
{
    return "ConnectionStatus";
}

QString ConnectionStatusPlugin::group() const
{
    return "My Custom Widgets";
}

QIcon ConnectionStatusPlugin::icon() const
{
    return QIcon();
}

QString ConnectionStatusPlugin::toolTip() const
{
    return "Connection status LED indicator";
}

QString ConnectionStatusPlugin::whatsThis() const
{
    return "Displays connection state using a colored LED.";
}

bool ConnectionStatusPlugin::isContainer() const
{
    return false;
}

QString ConnectionStatusPlugin::domXml() const
{
    return
        "<ui language=\"c++\">"
        "<widget class=\"ConnectionStatus\" name=\"connectionStatus\">"
        "<property name=\"geometry\">"
        "<rect>"
        "<x>0</x><y>0</y>"
        "<width>180</width><height>40</height>"
        "</rect>"
        "</property>"
        "</widget>"
        "</ui>";
}

QString ConnectionStatusPlugin::includeFile() const
{
    return "connectionstatus.h";
}