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