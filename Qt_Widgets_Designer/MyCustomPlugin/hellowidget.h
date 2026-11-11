
#ifndef HELLOWIDGET_H
#define HELLOWIDGET_H

#include <QWidget>
#include <QLabel>

class HelloWidget : public QWidget
{
    Q_OBJECT

    // This property will appear in Qt Designer's Property Editor.
    Q_PROPERTY(QString text READ text WRITE setText)

public:

    explicit HelloWidget(QWidget *parent = nullptr);

    QString text() const;

    void setText(const QString &text);

private:

    QLabel *m_label;
};

#endif
