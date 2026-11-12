
#include "calculator.h"

#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QMessageBox>
#include <QRegularExpression>
#include <QApplication>
#include <QKeyEvent>
#include <cmath>

// ---------------------------------------------
// CONSTRUCTOR
// ---------------------------------------------

Calculator::Calculator(QWidget *parent)
    : QWidget(parent),
      m_display(new QLineEdit(this)),
      m_layout(new QGridLayout),
      m_buttonSize(55),
      m_displayColor("#202020"),
      m_storedValue(0.0),
      m_waitingForOperand(true)
{
    setWindowTitle("Calculator");

    setMinimumSize(260, 350);

    // Display settings.
    m_display->setReadOnly(true);

    m_display->setAlignment(Qt::AlignRight);

    m_display->setText("0");

    m_display->setMinimumHeight(65);

    QFont displayFont;
    displayFont.setPointSize(22);
    displayFont.setBold(true);

    m_display->setFont(displayFont);

    // Main layout.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(m_display);

    mainLayout->addLayout(m_layout);

    mainLayout->setContentsMargins(8, 8, 8, 8);

    mainLayout->setSpacing(5);

    createButtons();

    setDisplayColor(m_displayColor);
}

// ---------------------------------------------
// BUTTON CREATION
// ---------------------------------------------

void Calculator::createButtons()
{
    const QString buttons[5][4] = {

        {"C", "⌫", "%", "/"},

        {"7", "8", "9", "*"},

        {"4", "5", "6", "-"},

        {"1", "2", "3", "+"},

        {"+/-", "0", ".", "="}

    };

    for (int row = 0; row < 5; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            createButton(
                buttons[row][column],
                row,
                column
            );
        }
    }
}

// ---------------------------------------------
// CREATE INDIVIDUAL BUTTON
// ---------------------------------------------

void Calculator::createButton(
    const QString &text,
    int row,
    int column,
    int rowSpan,
    int columnSpan)
{
    QPushButton *button = new QPushButton(text, this);

    button->setMinimumSize(
        m_buttonSize,
        m_buttonSize
    );

    QFont font;
    font.setPointSize(15);
    font.setBold(true);

    button->setFont(font);

    m_layout->addWidget(
        button,
        row,
        column,
        rowSpan,
        columnSpan
    );

    // Button signal connections.

    if (text == "C")
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::clear);
    }
    else if (text == "⌫")
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::backspace);
    }
    else if (text == ".")
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::decimalClicked);
    }
    else if (text == "=")
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::equalClicked);
    }
    else if (text == "%")
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::percentageClicked);
    }
    else if (text == "+/-")
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::changeSign);
    }
    else if (QString("+-*/").contains(text))
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::operatorClicked);
    }
    else
    {
        connect(button, &QPushButton::clicked,
                this, &Calculator::digitClicked);
    }
}

// ---------------------------------------------
// DIGIT BUTTON
// ---------------------------------------------

void Calculator::digitClicked()
{
    QPushButton *button =
        qobject_cast<QPushButton *>(sender());

    if (!button)
        return;

    QString digit = button->text();

    if (m_waitingForOperand)
    {
        m_display->setText(digit);
        m_waitingForOperand = false;
    }
    else
    {
        if (m_display->text() == "0")
            m_display->setText(digit);
        else
            m_display->setText(
                m_display->text() + digit
            );
    }
}

// ---------------------------------------------
// OPERATOR BUTTON
// ---------------------------------------------

void Calculator::operatorClicked()
{
    QPushButton *button =
        qobject_cast<QPushButton *>(sender());

    if (!button)
        return;

    QString clickedOperator = button->text();

    if (!m_pendingOperator.isEmpty() &&
        !m_waitingForOperand)
    {
        calculate();
    }
    else
    {
        m_storedValue = m_display->text().toDouble();
    }

    m_pendingOperator = clickedOperator;

    m_waitingForOperand = true;
}

// ---------------------------------------------
// CALCULATION ENGINE
// ---------------------------------------------

void Calculator::calculate()
{
    double operand = m_display->text().toDouble();

    if (m_pendingOperator == "+")
        m_storedValue += operand;

    else if (m_pendingOperator == "-")
        m_storedValue -= operand;

    else if (m_pendingOperator == "*")
        m_storedValue *= operand;

    else if (m_pendingOperator == "/")
    {
        if (operand == 0.0)
        {
            m_display->setText("Error");
            m_pendingOperator.clear();
            m_waitingForOperand = true;
            return;
        }

        m_storedValue /= operand;
    }

    updateDisplay();

    m_pendingOperator.clear();

    m_waitingForOperand = true;
}

// ---------------------------------------------
// UPDATE DISPLAY
// ---------------------------------------------

void Calculator::updateDisplay()
{
    m_display->setText(
        QString::number(m_storedValue, 'g', 15)
    );
}

// ---------------------------------------------
// EQUAL BUTTON
// ---------------------------------------------

void Calculator::equalClicked()
{
    if (!m_pendingOperator.isEmpty() &&
        !m_waitingForOperand)
    {
        calculate();
    }
}

// ---------------------------------------------
// CLEAR BUTTON
// ---------------------------------------------

void Calculator::clear()
{
    m_display->setText("0");

    m_storedValue = 0.0;

    m_pendingOperator.clear();

    m_waitingForOperand = true;
}

// ---------------------------------------------
// BACKSPACE BUTTON
// ---------------------------------------------

void Calculator::backspace()
{
    if (m_waitingForOperand)
        return;

    QString text = m_display->text();

    if (text == "Error")
    {
        clear();
        return;
    }

    text.chop(1);

    if (text.isEmpty() || text == "-")
        text = "0";

    m_display->setText(text);
}

// ---------------------------------------------
// DECIMAL BUTTON
// ---------------------------------------------

void Calculator::decimalClicked()
{
    if (m_waitingForOperand)
    {
        m_display->setText("0.");
        m_waitingForOperand = false;
    }
    else if (!m_display->text().contains('.'))
    {
        m_display->setText(
            m_display->text() + "."
        );
    }
}

// ---------------------------------------------
// PERCENTAGE BUTTON
// ---------------------------------------------

void Calculator::percentageClicked()
{
    double value = m_display->text().toDouble();

    value /= 100.0;

    m_display->setText(
        QString::number(value, 'g', 15)
    );

    m_waitingForOperand = true;
}

// ---------------------------------------------
// POSITIVE / NEGATIVE BUTTON
// ---------------------------------------------

void Calculator::changeSign()
{
    double value = m_display->text().toDouble();

    value = -value;

    m_display->setText(
        QString::number(value, 'g', 15)
    );
}

// ---------------------------------------------
// BUTTON SIZE PROPERTY
// ---------------------------------------------

int Calculator::buttonSize() const
{
    return m_buttonSize;
}

void Calculator::setButtonSize(int size)
{
    m_buttonSize = qMax(25, size);

    const auto buttons = findChildren<QPushButton *>();

    for (QPushButton *button : buttons)
    {
        button->setMinimumSize(
            m_buttonSize,
            m_buttonSize
        );
    }
}

// ---------------------------------------------
// DISPLAY COLOR PROPERTY
// ---------------------------------------------

QString Calculator::displayColor() const
{
    return m_displayColor;
}

void Calculator::setDisplayColor(const QString &color)
{
    QColor selectedColor(color);

    if (!selectedColor.isValid())
        return;

    m_displayColor = color;

    m_display->setStyleSheet(
        QString(
            "QLineEdit {"
            "background-color: %1;"
            "color: white;"
            "padding: 8px;"
            "border: 2px solid #555555;"
            "}"
        ).arg(color)
    );
}
