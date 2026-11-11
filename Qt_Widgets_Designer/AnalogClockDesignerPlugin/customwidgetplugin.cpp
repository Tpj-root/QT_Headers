
#include "customwidgetplugin.h"
#include "analogclock.h"

#include <QtPlugin>

// Constructor.
AnalogClockPlugin::AnalogClockPlugin(QObject *parent)
    : QObject(parent)
{
}

// Initialize plugin.
void AnalogClockPlugin::initialize(
    QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);

    if (initialized)
        return;

    initialized = true;
}

// Check initialization.
bool AnalogClockPlugin::isInitialized() const
{
    return initialized;
}

// Create the actual widget.
QWidget *AnalogClockPlugin::createWidget(QWidget *parent)
{
    return new AnalogClock(parent);
}

// Widget name in Designer.
QString AnalogClockPlugin::name() const
{
    return QStringLiteral("AnalogClock");
}

// Widget Box category.
QString AnalogClockPlugin::group() const
{
    return QStringLiteral("My Custom Widgets");
}

// Header included in generated UI code.
QString AnalogClockPlugin::includeFile() const
{
    return QStringLiteral("analogclock.h");
}

// Widget icon.
QIcon AnalogClockPlugin::icon() const
{
    return QIcon();
}

// Tooltip.
QString AnalogClockPlugin::toolTip() const
{
    return QStringLiteral("Analog Clock Widget");
}

// Description.
QString AnalogClockPlugin::whatsThis() const
{
    return QStringLiteral(
        "A custom analog clock with hour, minute and second hands."
    );
}

// Not a container widget.
bool AnalogClockPlugin::isContainer() const
{
    return false;
}

// Designer XML description.
QString AnalogClockPlugin::domXml() const
{
    return QString::fromUtf8(R"(
<ui language="c++">
    <widget class="AnalogClock" name="analogClock">
        <property name="geometry">
            <rect>
                <x>0</x>
                <y>0</y>
                <width>200</width>
                <height>200</height>
            </rect>
        </property>
        <property name="toolTip">
            <string>Analog Clock</string>
        </property>
        <property name="whatsThis">
            <string>Custom Analog Clock Widget</string>
        </property>
    </widget>
</ui>
)");
}
