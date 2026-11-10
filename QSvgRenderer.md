You want to use your six SVG files:

`ad-circle_1.svg` → `ad-circle_6.svg`

When you click the icon, the SVG images should play one after another like a frame-by-frame animation.

For this, use QSvgRenderer + QTimer + a custom QWidget. This is better than `QPropertyAnimation` for playing six SVG frames.

The animation will:

1. Display `ad-circle_1.svg` initially.
2. On click, play frames 1, 2, 3, 4, 5, 6.
3. Automatically stop at frame 6.
4. Allow clicking again to replay.

## 1. Project folder structure

Create this structure:

```
AnimatedIcons/
│
├── CMakeLists.txt
├── main.cpp
├── resources.qrc
│
└── icons/
    ├── ad-circle_1.svg
    ├── ad-circle_2.svg
    ├── ad-circle_3.svg
    ├── ad-circle_4.svg
    ├── ad-circle_5.svg
    └── ad-circle_6.svg
```

## 2. Complete main.cpp

This version uses a custom clickable widget. It renders your SVG frames directly using `QSvgRenderer`.&#x20;

[image](https://www.google.com/s2/favicons?domain=https://doc.qt.io\&sz=32)

Qt 6.12.0+1



```

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

```

## 3. resources.qrc

This embeds all six SVG files into your executable.

```

<RCC>
    <qresource prefix="/icons">

        <file alias="ad-circle_1.svg">icons/ad-circle_1.svg</file>
        <file alias="ad-circle_2.svg">icons/ad-circle_2.svg</file>
        <file alias="ad-circle_3.svg">icons/ad-circle_3.svg</file>
        <file alias="ad-circle_4.svg">icons/ad-circle_4.svg</file>
        <file alias="ad-circle_5.svg">icons/ad-circle_5.svg</file>
        <file alias="ad-circle_6.svg">icons/ad-circle_6.svg</file>

    </qresource>
</RCC>

```

## 4. CMakeLists.txt

```

cmake_minimum_required(VERSION 3.16)

project(AnimatedIcons LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Svg
)

add_executable(AnimatedIcons
    main.cpp
    resources.qrc
)

target_link_libraries(AnimatedIcons PRIVATE
    Qt6::Widgets
    Qt6::Svg
)

```

Install SVG development package if needed:

```
sudo apt install qt6-svg-dev
```

Build:

```
cmake -S . -B build
cmake --build build
./build/AnimatedIcons
```

## 5. Change animation speed

Change this line:

```
int frameDelay = 100;
```

| Delay  | Animation speed |
| ------ | --------------- |
| 50 ms  | Very fast       |
| 100 ms | Smooth          |
| 150 ms | Medium          |
| 250 ms | Slow            |
| 500 ms | Very slow       |

Important: This is frame-by-frame animation, not an animated SVG file. Each SVG is a separate frame.

## 6. How the animation works

Click

QTimer starts

# 1

SVG frame

# 2

SVG frame

# 3

SVG frame

# 4

SVG frame

# 5

SVG frame

# 6

SVG frame

Animation completed

One useful next improvement: We can add a reverse animation (frame 6 → frame 1) and a continuous loop, while keeping the same custom widget reusable throughout your Qt projects.



For your Debian 13 Linux + Qt6 environment, install the Qt SVG development package.

### 1. Install

```
sudo apt update
sudo apt install qt6-svg-dev
```

### 2. Check installation

```
dpkg -l | grep qt6-svg
```

### 3. Include header

```
#include <QSvgRenderer>
```

### 4. Add to CMakeLists.txt

```
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

target_link_libraries(AnimatedIcons PRIVATE
    Qt6::Widgets
    Qt6::Svg
)
```

### 5. Simple test

```
#include <QApplication>
#include <QSvgRenderer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSvgRenderer svg("icons/ad-circle_1.svg");

    qDebug() << "SVG valid:" << svg.isValid();

    return 0;
}
```

Expected

```
SVG valid: true
```

Remember: `QSvgRenderer` belongs to the Qt6 Svg module, not Qt6 Widgets.




Your installation is correct. Both packages are installed:

- `qt6-svg-dev` — provides the C++ headers and development files.
- `qt6-svg-plugins` — provides SVG-related plugins.

So the problem is probably Qt Creator's project configuration, not the installation.

For `#include <QSvgRenderer>` to work, your project must link against the Qt6 Svg module.

## 1. Check whether the header exists

Run:

```
dpkg -L qt6-svg-dev | grep QSvgRenderer
```

You should see something similar to:

```
/usr/include/x86_64-linux-gnu/qt6/QtSvg/QSvgRenderer
/usr/include/x86_64-linux-gnu/qt6/QtSvg/qsvgrenderer.h
```

If those files exist, Qt has the header installed.

## 2. Fix your CMakeLists.txt

Make sure you have `Svg` in `find_package()`.

```
cmake_minimum_required(VERSION 3.16)

project(QT_Headers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Svg
)

add_executable(QT_Headers
    main.cpp
)

target_link_libraries(QT_Headers PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
)
```

## 3. Test in Qt Creator

Your C++ file:

```
#include <QApplication>
#include <QSvgRenderer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSvgRenderer svg("test.svg");

    qDebug() << svg.isValid();

    return 0;
}
```

## 4. Reload the project

In Qt Creator:

1. Open your project.
2. Open `CMakeLists.txt`.
3. Save the file.
4. Go to Build → Clear CMake Configuration if needed.
5. Select Build → Run CMake.
6. Rebuild the project.

## 5. Verify Qt6 Svg CMake configuration

Run:

```
find /usr/lib -name Qt6SvgConfig.cmake
```

Expected location will be similar to:

```
/usr/lib/x86_64-linux-gnu/cmake/Qt6Svg/Qt6SvgConfig.cmake
```

One question: What exactly happens in Qt Creator?

Red underline under `#include`

Compiler error: `No such file or directory`

CMake says Qt6 Svg package not found

Autocomplete does not recognize `QSvgRenderer`

Submit

These are different problems, and I can show you the exact fix.


This happens because Qt cannot find your SVG files inside its Qt Resource System (QRC).

Your C++ code is correct:

```
":/icons/ad-circle_1.svg"
```

But Qt doesn't automatically include files just because they exist in your project folder.

The resource file must be registered and compiled into your executable.

## 1. Check your project structure

Make sure it looks like this:

```
AnimatedIcons/
│
├── CMakeLists.txt
├── main.cpp
├── resources.qrc
│
└── icons/
    ├── ad-circle_1.svg
    ├── ad-circle_2.svg
    ├── ad-circle_3.svg
    ├── ad-circle_4.svg
    ├── ad-circle_5.svg
    └── ad-circle_6.svg
```

## 2. Check resources.qrc

Your file should contain:

```
<RCC>
    <qresource prefix="/icons">

        <file>icons/ad-circle_1.svg</file>
        <file>icons/ad-circle_2.svg</file>
        <file>icons/ad-circle_3.svg</file>
        <file>icons/ad-circle_4.svg</file>
        <file>icons/ad-circle_5.svg</file>
        <file>icons/ad-circle_6.svg</file>

    </qresource>
</RCC>
```

Notice:

- `prefix="/icons"` creates the resource directory.
- `icons/ad-circle_1.svg` is the path relative to the `.qrc` file.

Therefore, the resource path becomes:

```
":/icons/icons/ad-circle_1.svg"
```

Important: Your previous resource file used `alias`, which creates a different path. Both approaches work, but you must match the C++ path.

## 3. Recommended solution: Use alias

I recommend this because your original C++ paths remain unchanged.

resources.qrc

```
<RCC>
    <qresource prefix="/icons">

        <file alias="ad-circle_1.svg">icons/ad-circle_1.svg</file>
        <file alias="ad-circle_2.svg">icons/ad-circle_2.svg</file>
        <file alias="ad-circle_3.svg">icons/ad-circle_3.svg</file>
        <file alias="ad-circle_4.svg">icons/ad-circle_4.svg</file>
        <file alias="ad-circle_5.svg">icons/ad-circle_5.svg</file>
        <file alias="ad-circle_6.svg">icons/ad-circle_6.svg</file>

    </qresource>
</RCC>
```

Now these paths are correct:

```
frames = {
    ":/icons/ad-circle_1.svg",
    ":/icons/ad-circle_2.svg",
    ":/icons/ad-circle_3.svg",
    ":/icons/ad-circle_4.svg",
    ":/icons/ad-circle_5.svg",
    ":/icons/ad-circle_6.svg"
};
```

## 4. Check CMakeLists.txt

You must have:

```
set(CMAKE_AUTORCC ON)

add_executable(AnimatedIcons
    main.cpp
    resources.qrc
)
```

## 5. Clean and rebuild

In Qt Creator:

1. Save `resources.qrc`.
2. Save `CMakeLists.txt`.
3. Select Build → Clean All.
4. Select Build → Run CMake.
5. Select Build → Rebuild All.

Or use terminal:

```
rm -rf build

cmake -S . -B build

cmake --build build

./build/AnimatedIcons
```

## 6. Test whether Qt can find your resource

Before starting the animation, add:

```
#include <QFile>
```

Inside your constructor:

```
for (const QString &file : frames)
{
    qDebug() << file
             << "Exists:"
             << QFile::exists(file);
}
```

Expected output:

```
":/icons/ad-circle_1.svg" Exists: true
":/icons/ad-circle_2.svg" Exists: true
":/icons/ad-circle_3.svg" Exists: true
":/icons/ad-circle_4.svg" Exists: true
":/icons/ad-circle_5.svg" Exists: true
":/icons/ad-circle_6.svg" Exists: true
```



Correct result

&#x20;All six should report `true`.



Why did copying the files make it work?

Because copying them probably placed the SVG files in your application's working directory, allowing a normal filesystem path to find them. But `:/icons/...` is a Qt resource path, not a normal Linux filesystem path. It requires the `.qrc` resource to be compiled into the executable.

My recommendation: use `.qrc` for your six animation frames. Your executable will then contain the SVG files and won't depend on their original folder after deployment.