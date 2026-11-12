#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QMainWindow>
#include <QStatusBar>
#include <QString>

// Show a message that remains until another message replaces it.
inline void showReadyMessage(QMainWindow &window)
{
    window.statusBar()->showMessage("Application Ready");
}

// Show a temporary message.
// The duration is measured in milliseconds; 3000 means 3 seconds.
inline void showStatusMessage(QMainWindow &window,
                              const QString &message,
                              int durationMs = 3000)
{
    window.statusBar()->showMessage(message, durationMs);
}

#endif // STATUS_BAR_H
