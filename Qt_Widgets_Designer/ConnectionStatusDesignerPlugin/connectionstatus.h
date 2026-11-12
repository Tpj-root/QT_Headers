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