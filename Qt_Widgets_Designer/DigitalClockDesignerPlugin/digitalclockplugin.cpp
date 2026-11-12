
#include "digitalclockplugin.h"
#include "digitalclock.h"

#include <QtPlugin>

// ---------------------------------------
// CONSTRUCTOR
// ---------------------------------------

DigitalClockPlugin::DigitalClockPlugin(QObject *parent)
    : QObject(parent)
{
}

// ---------------------------------------
// INITIALIZE PLUGIN
// ---------------------------------------

void DigitalClockPlugin::initialize(
    QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);

    if (initialized)
        return;

    initialized = true;
}

// ---------------------------------------
// INITIALIZATION STATUS
// ---------------------------------------

bool DigitalClockPlugin::isInitialized() const
{
    return initialized;
}

// ---------------------------------------
// CREATE WIDGET
// ---------------------------------------

QWidget *DigitalClockPlugin::createWidget(QWidget *parent)
{
    return new DigitalClock(parent);
}

// ---------------------------------------
// WIDGET NAME
// ---------------------------------------

QString DigitalClockPlugin::name() const
{
    return QStringLiteral("DigitalClock");
}

// ---------------------------------------
// WIDGET GROUP
// ---------------------------------------

QString DigitalClockPlugin::group() const
{
    return QStringLiteral("My Custom Widgets");
}

// ---------------------------------------
// HEADER FILE
// ---------------------------------------

QString DigitalClockPlugin::includeFile() const
{
    return QStringLiteral("digitalclock.h");
}

// ---------------------------------------
// ICON
// ---------------------------------------

QIcon DigitalClockPlugin::icon() const
{
    return QIcon();
}

// ---------------------------------------
// TOOLTIP
// ---------------------------------------

QString DigitalClockPlugin::toolTip() const
{
    return QStringLiteral("Digital Clock Widget");
}

// ---------------------------------------
// WHAT'S THIS
// ---------------------------------------

QString DigitalClockPlugin::whatsThis() const
{
    return QStringLiteral(
        "A customizable digital clock displaying current time."
    );
}

// ---------------------------------------
// CONTAINER STATUS
// ---------------------------------------

bool DigitalClockPlugin::isContainer() const
{
    return false;
}

// ---------------------------------------
// DESIGNER XML
// ---------------------------------------

QString DigitalClockPlugin::domXml() const
{
    return QString::fromUtf8(R"(
<ui language="c++">
    <widget class="DigitalClock" name="digitalClock">
        <property name="geometry">
            <rect>
                <x>0</x>
                <y>0</y>
                <width>250</width>
                <height>100</height>
            </rect>
        </property>
        <property name="toolTip">
            <string>Digital Clock</string>
        </property>
    </widget>
</ui>
)");
}
