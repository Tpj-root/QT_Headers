#ifndef SET_ICON_H
#define SET_ICON_H

#include <QAction>
#include <QIcon>
#include <QString>

// Assign an icon to an existing QAction.
// resourcePath should match an image path registered in a Qt resource (.qrc) file.
inline void setActionIcon(QAction *action, const QString &resourcePath)
{
    // Do nothing if the caller passes a null QAction pointer.
    if (action == nullptr)
        return;

    // QIcon loads the image from the resource path.
    // If the resource does not exist, the action still works but has no icon.
    action->setIcon(QIcon(resourcePath));
}

#endif // SET_ICON_H
