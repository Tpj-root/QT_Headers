My next idea is a Connection Status Indicator Plugin. This one would be very useful in a professional Qt application, especially an MT5 trading dashboard.

Imagine a small widget that shows whether your application is connected to the MT5 server, API, database, or network.

Connection Status Indicator — Preview

Disconnected

Connecting

Connected

Error

## Plugin 5: ConnectionStatus

My recommendation

### Features

- Four states: Disconnected, Connecting, Connected, Error.
- Different LED colors for each state.
- Custom status text.
- Adjustable LED diameter.
- Qt Designer properties.
- Can be updated dynamically from C++ code.

### Project structure

```
ConnectionStatusDesignerPlugin/
│
├── CMakeLists.txt
│
├── connectionstatus.h
├── connectionstatus.cpp
│
├── connectionstatusplugin.h
└── connectionstatusplugin.cpp
```

## 1. connectionstatus.h

```
#ifndef CONNECTIONSTATUS_H
#define CONNECTIONSTATUS_H

#include <QWidget>
#include <QString>

class ConnectionStatus : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel)
    Q_PROPERTY(int ledSize READ ledSize WRITE setLedSize)

public:

    enum State
    {
        Disconnected,
        Connecting,
        Connected,
        Error
    };

    Q_ENUM(State)

    explicit ConnectionStatus(QWidget *parent = nullptr);

    State state() const;
    void setState(State state);

    QString label() const;
    void setLabel(const QString &label);

    int ledSize() const;
    void setLedSize(int size);

    QSize sizeHint() const override;

signals:

    void stateChanged(State state);

protected:

    void paintEvent(QPaintEvent *event) override;

private:

    State m_state = Disconnected;

    QString m_label = "Disconnected";

    int m_ledSize = 16;

    QColor stateColor() const;
};

#endif
```

## 2. connectionstatus.cpp

```
#include "connectionstatus.h"

#include <QPainter>
#include <QFont>
#include <QColor>
#include <QPaintEvent>

// Constructor
ConnectionStatus::ConnectionStatus(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(150, 35);
}

// Return current state
ConnectionStatus::State ConnectionStatus::state() const
{
    return m_state;
}

// Change connection state
void ConnectionStatus::setState(State state)
{
    if (m_state == state)
        return;

    m_state = state;

    switch (m_state)
    {
        case Disconnected:
            m_label = "Disconnected";
            break;

        case Connecting:
            m_label = "Connecting";
            break;

        case Connected:
            m_label = "Connected";
            break;

        case Error:
            m_label = "Error";
            break;
    }

    emit stateChanged(m_state);

    update();
}

// Return label
QString ConnectionStatus::label() const
{
    return m_label;
}

// Change label
void ConnectionStatus::setLabel(const QString &label)
{
    m_label = label;

    update();
}

// Return LED size
int ConnectionStatus::ledSize() const
{
    return m_ledSize;
}

// Change LED size
void ConnectionStatus::setLedSize(int size)
{
    m_ledSize = qMax(4, size);

    update();
}

// LED color depending on state
QColor ConnectionStatus::stateColor() const
{
    switch (m_state)
    {
        case Disconnected:
            return QColor("#DC5555");

        case Connecting:
            return QColor("#D9A441");

        case Connected:
            return QColor("#35B779");

        case Error:
            return QColor("#D94D4D");
    }

    return Qt::gray;
}

// Recommended widget size
QSize ConnectionStatus::sizeHint() const
{
    return QSize(180, 40);
}

// Draw LED and status text
void ConnectionStatus::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    // LED position
    int x = 5;
    int y = (height() - m_ledSize) / 2;

    // LED circle
    painter.setPen(Qt::NoPen);

    painter.setBrush(stateColor());

    painter.drawEllipse(
        x,
        y,
        m_ledSize,
        m_ledSize
    );

    // Status text
    painter.setPen(palette().color(QPalette::WindowText));

    QFont font = painter.font();

    font.setPointSize(10);

    painter.setFont(font);

    painter.drawText(
        m_ledSize + 15,
        0,
        width() - m_ledSize - 15,
        height(),
        Qt::AlignVCenter | Qt::AlignLeft,
        m_label
    );
}
```

## 3. connectionstatusplugin.h

```
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
```

## 4. connectionstatusplugin.cpp

```
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
```

## 5. CMakeLists.txt

```
cmake_minimum_required(VERSION 3.16)

project(ConnectionStatusDesignerPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    UiPlugin
)

add_library(connectionstatusplugin MODULE

    connectionstatus.h
    connectionstatus.cpp

    connectionstatusplugin.h
    connectionstatusplugin.cpp
)

target_link_libraries(connectionstatusplugin PRIVATE

    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::UiPlugin
)

set_target_properties(connectionstatusplugin PROPERTIES

    LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/designer"

    OUTPUT_NAME "connectionstatusplugin"
)
```

## 6. Build and install

```
cd ~/Qt

mkdir -p ConnectionStatusDesignerPlugin

cd ConnectionStatusDesignerPlugin
```

Save the five files above in this directory, then:

```
cmake -S . -B build

cmake --build build -j$(nproc)
```

Copy the plugin:

```
PLUGIN_DIR="$HOME/Qt/Tools/QtDesignStudio/qt6_design_studio_reduced_version/plugins/designer"

cp build/designer/libconnectionstatusplugin.so "$PLUGIN_DIR/"
```

Restart Qt Designer.

## 7. Change status dynamically from your application

Once the widget is added to your `.ui` form and promoted/recognized as `ConnectionStatus`, you can change its state:

```
ui->connectionStatus->setState(ConnectionStatus::Connected);

ui->connectionStatus->setState(ConnectionStatus::Connecting);

ui->connectionStatus->setState(ConnectionStatus::Disconnected);

ui->connectionStatus->setState(ConnectionStatus::Error);
```

My next plugin idea after this: a custom Gauge Meter with a circular scale, needle, minimum/maximum values and live readings. That would be excellent for displaying CPU usage, network speed, or trading account metrics.