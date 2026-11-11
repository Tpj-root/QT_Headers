#include "plugininterface.h"

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

class CalculatorPlugin : public QObject, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)

public:

    QString pluginName() const override
    {
        return "Calculator";
    }

    QWidget *createWidget(QWidget *parent) override
    {
        QWidget *page = new QWidget(parent);

        auto *layout = new QVBoxLayout(page);
        auto *grid = new QGridLayout();

        auto *number1 = new QDoubleSpinBox(page);
        auto *number2 = new QDoubleSpinBox(page);

        number1->setRange(-1000000, 1000000);
        number2->setRange(-1000000, 1000000);

        number1->setValue(10);
        number2->setValue(5);

        grid->addWidget(new QLabel("Number 1:"), 0, 0);
        grid->addWidget(number1, 0, 1);

        grid->addWidget(new QLabel("Number 2:"), 1, 0);
        grid->addWidget(number2, 1, 1);

        auto *result = new QLabel("Result: 0", page);
        result->setStyleSheet("font-size: 20px; font-weight: bold;");

        auto *add = new QPushButton("+", page);
        auto *subtract = new QPushButton("-", page);
        auto *multiply = new QPushButton("*", page);
        auto *divide = new QPushButton("/", page);

        auto calculate = [=](char operation)
        {
            double a = number1->value();
            double b = number2->value();
            double answer = 0;

            switch (operation)
            {
                case '+':
                    answer = a + b;
                    break;

                case '-':
                    answer = a - b;
                    break;

                case '*':
                    answer = a * b;
                    break;

                case '/':
                    if (b == 0)
                    {
                        result->setText("Cannot divide by zero!");
                        return;
                    }

                    answer = a / b;
                    break;
            }

            result->setText(
                "Result: " + QString::number(answer)
            );
        };

        connect(add, &QPushButton::clicked, page,
                [=]() { calculate('+'); });

        connect(subtract, &QPushButton::clicked, page,
                [=]() { calculate('-'); });

        connect(multiply, &QPushButton::clicked, page,
                [=]() { calculate('*'); });

        connect(divide, &QPushButton::clicked, page,
                [=]() { calculate('/'); });

        auto *buttons = new QHBoxLayout();

        buttons->addWidget(add);
        buttons->addWidget(subtract);
        buttons->addWidget(multiply);
        buttons->addWidget(divide);

        layout->addLayout(grid);
        layout->addLayout(buttons);
        layout->addWidget(result);
        layout->addStretch();

        return page;
    }
};

#include "calculatorplugin.moc"