
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QString>

class QLineEdit;
class QPushButton;
class QGridLayout;

class Calculator : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int buttonSize READ buttonSize WRITE setButtonSize)

    Q_PROPERTY(QString displayColor READ displayColor WRITE setDisplayColor)

public:

    explicit Calculator(QWidget *parent = nullptr);

    int buttonSize() const;
    void setButtonSize(int size);

    QString displayColor() const;
    void setDisplayColor(const QString &color);

private:

    QLineEdit *m_display;

    QGridLayout *m_layout;

    int m_buttonSize;

    QString m_displayColor;

    double m_storedValue;

    QString m_pendingOperator;

    bool m_waitingForOperand;

    void createButtons();

    void createButton(const QString &text,
                      int row,
                      int column,
                      int rowSpan = 1,
                      int columnSpan = 1);

    void digitClicked();

    void operatorClicked();

    void calculate();

    void updateDisplay();

private slots:

    void clear();
    void backspace();
    void decimalClicked();
    void equalClicked();
    void percentageClicked();
    void changeSign();

};

#endif
