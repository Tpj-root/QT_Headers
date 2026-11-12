
#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QWidget>
#include <QColor>

class QTimer;
class QPaintEvent;

class DigitalClock : public QWidget
{
    Q_OBJECT

    // Properties visible in Qt Designer.
    Q_PROPERTY(QString timeFormat READ timeFormat WRITE setTimeFormat)

    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)

public:

    explicit DigitalClock(QWidget *parent = nullptr);

    // Time format.
    QString timeFormat() const;
    void setTimeFormat(const QString &format);

    // Text color.
    QColor textColor() const;
    void setTextColor(const QColor &color);

    // Background color.
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    // Font size.
    int fontSize() const;
    void setFontSize(int size);

protected:

    void paintEvent(QPaintEvent *event) override;

private:

    QTimer *m_timer;

    QString m_timeFormat;

    QColor m_textColor;

    QColor m_backgroundColor;

    int m_fontSize;
};

#endif
