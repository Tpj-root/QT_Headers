#include "plugininterface.h"

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QLabel>

class ChartCanvas : public QWidget
{
public:

    explicit ChartCanvas(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setMinimumSize(500, 300);
    }

protected:

    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        // Chart drawing area
        QRect area = rect().adjusted(50, 20, -20, -40);

        // Background
        painter.fillRect(rect(), Qt::white);

        // Grid lines
        painter.setPen(QPen(Qt::lightGray, 1));

        for (int i = 0; i <= 5; ++i)
        {
            int y = area.top() + i * area.height() / 5;

            painter.drawLine(
                area.left(), y,
                area.right(), y
            );
        }

        // Axis
        painter.setPen(QPen(Qt::black, 2));

        painter.drawLine(
            area.bottomLeft(),
            area.bottomRight()
        );

        painter.drawLine(
            area.topLeft(),
            area.bottomLeft()
        );

        // Sample data
        QVector<double> values = {
            20, 45, 30, 70, 55, 90, 65
        };

        QPainterPath path;

        for (int i = 0; i < values.size(); ++i)
        {
            double x = area.left()
                + i * area.width() / (values.size() - 1);

            double y = area.bottom()
                - (values[i] / 100.0) * area.height();

            if (i == 0)
                path.moveTo(x, y);
            else
                path.lineTo(x, y);

            painter.setBrush(Qt::blue);
            painter.drawEllipse(QPointF(x, y), 4, 4);
        }

        // Draw data line
        painter.setPen(QPen(Qt::blue, 3));
        painter.setBrush(Qt::NoBrush);

        painter.drawPath(path);

        // Chart title
        painter.setPen(Qt::black);
        painter.drawText(
            QRect(0, 0, width(), 25),
            Qt::AlignCenter,
            "Sample Data Chart"
        );
    }
};


class ChartPlugin : public QObject, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)

public:

    QString pluginName() const override
    {
        return "Chart";
    }

    QWidget *createWidget(QWidget *parent) override
    {
        QWidget *page = new QWidget(parent);

        auto *layout = new QVBoxLayout(page);

        auto *title = new QLabel("Chart Plugin");
        title->setStyleSheet("font-size: 20px; font-weight: bold;");

        auto *chart = new ChartCanvas(page);

        layout->addWidget(title);
        layout->addWidget(chart);

        return page;
    }
};

#include "chartplugin.moc"