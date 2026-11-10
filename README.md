

# Top Qt headers – Qt6

## 1. Core Qt GUI headers

| No. | Header             | Main purpose                  | Common class     |
| --- | ------------------ | ----------------------------- | ---------------- |
| 1   | `<QApplication>`   | Starts a GUI application      | `QApplication`   |
| 2   | `<QWidget>`        | Base class for GUI widgets    | `QWidget`        |
| 3   | `<QMainWindow>`    | Main application window       | `QMainWindow`    |
| 4   | `<QDialog>`        | Creates dialog windows        | `QDialog`        |
| 5   | `<QLabel>`         | Displays text and images      | `QLabel`         |
| 6   | `<QPushButton>`    | Creates buttons               | `QPushButton`    |
| 7   | `<QToolButton>`    | Creates toolbar-style buttons | `QToolButton`    |
| 8   | `<QLineEdit>`      | Single-line text input        | `QLineEdit`      |
| 9   | `<QTextEdit>`      | Multi-line rich text input    | `QTextEdit`      |
| 10  | `<QPlainTextEdit>` | Multi-line plain text input   | `QPlainTextEdit` |

## 2. Layout and container headers

| No. | Header          | Main purpose           | Common class  |
| --- | --------------- | ---------------------- | ------------- |
| 11  | `<QVBoxLayout>` | Vertical layout        | `QVBoxLayout` |
| 12  | `<QHBoxLayout>` | Horizontal layout      | `QHBoxLayout` |
| 13  | `<QGridLayout>` | Grid layout            | `QGridLayout` |
| 14  | `<QFormLayout>` | Label and input layout | `QFormLayout` |
| 15  | `<QGroupBox>`   | Groups related widgets | `QGroupBox`   |
| 16  | `<QTabWidget>`  | Tab-based interface    | `QTabWidget`  |
| 17  | `<QSplitter>`   | Resizable panels       | `QSplitter`   |
| 18  | `<QScrollArea>` | Scrollable widget area | `QScrollArea` |

## 3. Input and selection headers

| No. | Header             | Main purpose              | Common class     |
| --- | ------------------ | ------------------------- | ---------------- |
| 19  | `<QComboBox>`      | Dropdown selection        | `QComboBox`      |
| 20  | `<QCheckBox>`      | Multiple option selection | `QCheckBox`      |
| 21  | `<QRadioButton>`   | Single option selection   | `QRadioButton`   |
| 22  | `<QSpinBox>`       | Integer input             | `QSpinBox`       |
| 23  | `<QDoubleSpinBox>` | Decimal input             | `QDoubleSpinBox` |
| 24  | `<QSlider>`        | Slider control            | `QSlider`        |
| 25  | `<QDateEdit>`      | Date input                | `QDateEdit`      |
| 26  | `<QTimeEdit>`      | Time input                | `QTimeEdit`      |

## 4. Data and table headers

| No. | Header                 | Main purpose              | Common class         |
| --- | ---------------------- | ------------------------- | -------------------- |
| 27  | `<QTableWidget>`       | Displays editable tables  | `QTableWidget`       |
| 28  | `<QTableView>`         | Model-based table display | `QTableView`         |
| 29  | `<QTreeWidget>`        | Displays tree structures  | `QTreeWidget`        |
| 30  | `<QListWidget>`        | Displays item lists       | `QListWidget`        |
| 31  | `<QStandardItemModel>` | Stores model-based data   | `QStandardItemModel` |

## 5. Core programming and utility headers

| No. | Header               | Main purpose                 | Common class       |
| --- | -------------------- | ---------------------------- | ------------------ |
| 32  | `<QObject>`          | Signals and slots foundation | `QObject`          |
| 33  | `<QString>`          | String handling              | `QString`          |
| 34  | `<QTimer>`           | Executes timed operations    | `QTimer`           |
| 35  | `<QFile>`            | File operations              | `QFile`            |
| 36  | `<QTextStream>`      | Reads and writes text        | `QTextStream`      |
| 37  | `<QDebug>`           | Debug output                 | `QDebug`           |
| 38  | `<QLoggingCategory>` | Custom logging categories    | `QLoggingCategory` |
| 39  | `<QSettings>`        | Saves application settings   | `QSettings`        |
| 40  | `<QThread>`          | Multithreading               | `QThread`          |

## 6. Bonus: Important advanced Qt headers

These are particularly useful for larger applications.

| Header                    | Purpose                   | Example project      |
| ------------------------- | ------------------------- | -------------------- |
| `<QTcpSocket>`            | TCP client connections    | Trading API          |
| `<QTcpServer>`            | TCP server                | Local trading server |
| `<QNetworkAccessManager>` | HTTP requests             | REST API             |
| `<QNetworkReply>`         | HTTP response             | API response         |
| `<QJsonDocument>`         | JSON parsing              | Market data          |
| `<QJsonObject>`           | JSON objects              | Account information  |
| `<QJsonArray>`            | JSON arrays               | Candle data          |
| `<QPainter>`              | Custom graphics drawing   | Candlestick chart    |
| `<QTimerEvent>`           | Timer event handling      | Animation            |
| `<QPropertyAnimation>`    | Property animations       | Animated buttons     |
| `<QGraphicsView>`         | Graphics scene display    | Interactive chart    |
| `<QGraphicsScene>`        | Graphics scene management | Drawing application  |
| `<QChartView>`            | Chart display (Qt Charts) | Trading chart        |
| `<QProcess>`              | Run external programs     | Python integration   |
| `<QThreadPool>`           | Thread pool management    | Background tasks     |

## 7. Which headers should you learn first?

For your Qt GUI and C++ trading application development, I recommend this learning order:

Level 1 – GUI fundamentals

Start here

`QApplication` → `QWidget` → `QMainWindow` → `QLabel` → `QPushButton` → `QLineEdit`

Level 2 – GUI layouts

`QVBoxLayout` → `QHBoxLayout` → `QGridLayout` → `QFormLayout` → `QSplitter`

Level 3 – Signals and slots

`QObject` → `QTimer` → `QDebug` → `QLoggingCategory`

Level 4 – Data management

`QTableWidget` → `QTableView` → `QStandardItemModel` → `QJsonDocument`

Level 5 – Advanced development

`QNetworkAccessManager` → `QThread` → `QPainter` → `QPropertyAnimation`

## 8. Example: Include multiple Qt headers

A simple Qt6 GUI application:

```
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    QWidget *central = new QWidget;

    QLabel *label = new QLabel("Enter Symbol:");

    QLineEdit *input = new QLineEdit;
    input->setPlaceholderText("XAUUSD");

    QPushButton *button = new QPushButton("BUY");

    QVBoxLayout *layout = new QVBoxLayout(central);

    layout->addWidget(label);
    layout->addWidget(input);
    layout->addWidget(button);

    window.setCentralWidget(central);

    QObject::connect(button, &QPushButton::clicked,
                     [&]() {
        qDebug() << "Symbol:" << input->text();
    });

    window.resize(400, 300);
    window.show();

    return app.exec();
}
```

My recommendation: Learn the purpose of each header, its important class functions, signals, slots, and properties. Don't try to memorize all the functions at once.

I can also teach you the top 100 Qt classes with their most-used functions, arguments, return types, and practical examples, organized as a step-by-step reference.# QT_Headers






## install help

sudo apt install qt6-svg-dev
