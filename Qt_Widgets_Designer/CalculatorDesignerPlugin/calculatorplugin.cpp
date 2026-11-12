
#include "calculatorplugin.h"
#include "calculator.h"

#include <QtPlugin>

// ---------------------------------------
// CONSTRUCTOR
// ---------------------------------------

CalculatorPlugin::CalculatorPlugin(QObject *parent)
    : QObject(parent)
{
}

// ---------------------------------------
// INITIALIZE
// ---------------------------------------

void CalculatorPlugin::initialize(
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

bool CalculatorPlugin::isInitialized() const
{
    return initialized;
}

// ---------------------------------------
// CREATE WIDGET
// ---------------------------------------

QWidget *CalculatorPlugin::createWidget(QWidget *parent)
{
    return new Calculator(parent);
}

// ---------------------------------------
// WIDGET NAME
// ---------------------------------------

QString CalculatorPlugin::name() const
{
    return QStringLiteral("Calculator");
}

// ---------------------------------------
// WIDGET GROUP
// ---------------------------------------

QString CalculatorPlugin::group() const
{
    return QStringLiteral("My Custom Widgets");
}

// ---------------------------------------
// HEADER FILE
// ---------------------------------------

QString CalculatorPlugin::includeFile() const
{
    return QStringLiteral("calculator.h");
}

// ---------------------------------------
// ICON
// ---------------------------------------

QIcon CalculatorPlugin::icon() const
{
    return QIcon();
}

// ---------------------------------------
// TOOLTIP
// ---------------------------------------

QString CalculatorPlugin::toolTip() const
{
    return QStringLiteral("Custom Calculator Widget");
}

// ---------------------------------------
// DESCRIPTION
// ---------------------------------------

QString CalculatorPlugin::whatsThis() const
{
    return QStringLiteral(
        "A calculator supporting basic arithmetic operations."
    );
}

// ---------------------------------------
// CONTAINER
// ---------------------------------------

bool CalculatorPlugin::isContainer() const
{
    return false;
}

// ---------------------------------------
// DESIGNER XML
// ---------------------------------------

QString CalculatorPlugin::domXml() const
{
    return QString::fromUtf8(R"(
<ui language="c++">
    <widget class="Calculator" name="calculator">
        <property name="geometry">
            <rect>
                <x>0</x>
                <y>0</y>
                <width>300</width>
                <height>400</height>
            </rect>
        </property>
        <property name="toolTip">
            <string>Calculator Widget</string>
        </property>
    </widget>
</ui>
)");
}
