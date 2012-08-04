

#include "windowmanager.h"
#include "xtools/xwindowmanager.h"

WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
}

void WindowManager::windowToFront(qulonglong windowId)
{
    XWindowManager::windowToFront(windowId);
}

void WindowManager::closeWindow(qulonglong windowId)
{
    XWindowManager::closeWindow(windowId);
}
