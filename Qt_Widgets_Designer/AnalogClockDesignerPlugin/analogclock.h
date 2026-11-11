
#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QWidget>

class QPaintEvent;

class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    explicit AnalogClock(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif
