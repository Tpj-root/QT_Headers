
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QSvgRenderer>
#include <QTimer>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

// ─────────────────────────────────────────────
// CUSTOM SVG ANIMATION WIDGET
// ─────────────────────────────────────────────

class AnimatedIcon : public QWidget
{
private:

    QSvgRenderer renderer;

    QTimer *timer;

    QStringList frames;

    int currentFrame = 0;

    int frameDelay = 100;

    bool playing = false;

public:

    AnimatedIcon(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setFixedSize(110, 110);

        setCursor(Qt::PointingHandCursor);

        // SVG animation frames
        frames = {
            ":/icons/ad-circle_1.svg",
            ":/icons/ad-circle_2.svg",
            ":/icons/ad-circle_3.svg",
            ":/icons/ad-circle_4.svg",
            ":/icons/ad-circle_5.svg",
            ":/icons/ad-circle_6.svg"
        };


        // Load first SVG
        renderer.load(frames[0]);

        // Create animation timer
        timer = new QTimer(this);

        timer->setInterval(frameDelay);

        // Frame-by-frame animation
        connect(timer, &QTimer::timeout, this, [this]()
                {
                    currentFrame++;

                    // Stop at last frame
                    if (currentFrame >= frames.size())
                    {
                        currentFrame = frames.size() - 1;

                        timer->stop();

                        playing = false;

                        qDebug() << "Animation completed";

                        return;
                    }

                    // Load next SVG
                    renderer.load(frames[currentFrame]);

                    update();
                });
    }

protected:

    // Draw SVG
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // Background
        painter.setPen(Qt::NoPen);

        painter.setBrush(QColor("#252936"));

        painter.drawRoundedRect(rect(), 15, 15);

        // Render SVG
        QRectF target(
            15,
            15,
            width() - 30,
            height() - 30
            );

        if (renderer.isValid())
        {
            renderer.render(&painter, target);
        }
    }

    // Mouse click event
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            startAnimation();
        }
    }

private:

    // Start animation
    void startAnimation()
    {
        timer->stop();

        currentFrame = 0;

        playing = true;

        renderer.load(frames[currentFrame]);

        update();

        timer->start();

        qDebug() << "Animation started";
    }
};


// ─────────────────────────────────────────────
// MAIN APPLICATION
// ─────────────────────────────────────────────

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;

    window.setWindowTitle("Custom SVG Animation");

    window.resize(400, 250);

    window.setStyleSheet(
        "background-color: #171b26;"
        );

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *title = new QLabel("CLICK TO ANIMATE");

    title->setAlignment(Qt::AlignCenter);

    title->setStyleSheet(
        "color: white;"
        "font-size: 20px;"
        "font-weight: bold;"
        );

    layout->addWidget(title);

    // Custom animated icon
    AnimatedIcon *icon = new AnimatedIcon;

    layout->addWidget(icon, 0, Qt::AlignCenter);

    window.show();

    return app.exec();
}
