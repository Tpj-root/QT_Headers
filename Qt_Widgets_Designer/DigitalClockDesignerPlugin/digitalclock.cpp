
#include "digitalclock.h"

#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QFont>
#include <QPaintEvent>

DigitalClock::DigitalClock(QWidget *parent)
    : QWidget(parent),
      m_timer(new QTimer(this)),
      m_timeFormat("HH:mm:ss"),
      m_textColor(Qt::green),
      m_backgroundColor(Qt::black),
      m_fontSize(32)
{
    // Update clock every second.
    connect(m_timer, &QTimer::timeout,
            this, qOverload<>(&QWidget::update));

    m_timer->start(1000);

    // Initial widget size.
    resize(250, 100);

    setMinimumSize(150, 60);

    setWindowTitle("Digital Clock");
}

// ---------------------------------------
// TIME FORMAT
// ---------------------------------------

QString DigitalClock::timeFormat() const
{
    return m_timeFormat;
}

void DigitalClock::setTimeFormat(const QString &format)
{
    m_timeFormat = format;
    update();
}

// ---------------------------------------
// TEXT COLOR
// ---------------------------------------

QColor DigitalClock::textColor() const
{
    return m_textColor;
}

void DigitalClock::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

// ---------------------------------------
// BACKGROUND COLOR
// ---------------------------------------

QColor DigitalClock::backgroundColor() const
{
    return m_backgroundColor;
}

void DigitalClock::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

// ---------------------------------------
// FONT SIZE
// ---------------------------------------

int DigitalClock::fontSize() const
{
    return m_fontSize;
}

void DigitalClock::setFontSize(int size)
{
    m_fontSize = qMax(1, size);
    update();
}

// ---------------------------------------
// PAINT EVENT
// ---------------------------------------

void DigitalClock::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Enable smooth text rendering.
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setRenderHint(QPainter::TextAntialiasing);

    // Draw background.
    painter.fillRect(rect(), m_backgroundColor);

    // Configure font.
    QFont font("DejaVu Sans Mono");

    font.setBold(true);

    font.setPointSize(m_fontSize);

    painter.setFont(font);

    // Configure text color.
    painter.setPen(m_textColor);

    // Get current time.
    QString currentTime =
        QTime::currentTime().toString(m_timeFormat);

    // Draw centered digital time.
    painter.drawText(
        rect(),
        Qt::AlignCenter,
        currentTime
    );
}
