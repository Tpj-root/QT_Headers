
#include "hellowidget.h"

#include <QVBoxLayout>

HelloWidget::HelloWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create the label.
    m_label = new QLabel("Hello from my plugin!", this);

    // Center the label.
    m_label->setAlignment(Qt::AlignCenter);

    // Create the layout.
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Add the label to the layout.
    layout->addWidget(m_label);

    // Set minimum size.
    setMinimumSize(150, 80);

    // Set default widget text.
    setText("Hello from my plugin!");
}

QString HelloWidget::text() const
{
    return m_label->text();
}

void HelloWidget::setText(const QString &text)
{
    m_label->setText(text);
}
