
#include "hellowidgetplugin.h"
#include "hellowidget.h"

#include <QIcon>

HelloWidgetPlugin::HelloWidgetPlugin(QObject *parent)
    : QObject(parent)
{
}

void HelloWidgetPlugin::initialize(
    QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);

    if (m_initialized)
        return;

    m_initialized = true;
}

bool HelloWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

// Name displayed in Qt Designer.
QString HelloWidgetPlugin::name() const
{
    return "HelloWidget";
}

// Widget Box category.
QString HelloWidgetPlugin::group() const
{
    return "My Custom Widgets";
}

// Header file included in generated UI code.
QString HelloWidgetPlugin::includeFile() const
{
    return "hellowidget.h";
}

// Icon displayed in Widget Box.
QIcon HelloWidgetPlugin::icon() const
{
    return QIcon();
}

QString HelloWidgetPlugin::toolTip() const
{
    return "My first custom Qt widget";
}

QString HelloWidgetPlugin::whatsThis() const
{
    return "A simple custom widget created by me.";
}

// This widget is not a container.
bool HelloWidgetPlugin::isContainer() const
{
    return false;
}

// Create widget instance.
QWidget *HelloWidgetPlugin::createWidget(QWidget *parent)
{
    return new HelloWidget(parent);
}

// Designer's widget XML description.
QString HelloWidgetPlugin::domXml() const
{
    return R"(
        <ui language="c++">
            <widget class="HelloWidget" name="helloWidget">
                <property name="geometry">
                    <rect>
                        <x>0</x>
                        <y>0</y>
                        <width>180</width>
                        <height>80</height>
                    </rect>
                </property>
                <property name="text">
                    <string>Hello from my plugin!</string>
                </property>
            </widget>
        </ui>
    )";
}
