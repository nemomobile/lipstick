#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

class WindowManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowManager)

public:
    explicit WindowManager(QObject *parent = 0);
    Q_INVOKABLE void closeWindow(qulonglong windowId);
    Q_INVOKABLE void windowToFront(qulonglong windowId);

};

#endif // WINDOWMANAGER_H
