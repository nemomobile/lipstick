/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QApplication>
#include <QX11Info>

#include "homewindowmonitor.h"
#include "xtools/xatomcache.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>


QSharedPointer<HomeWindowMonitor> HomeWindowMonitor::windowMonitorInstance = QSharedPointer<HomeWindowMonitor>();

const HomeWindowMonitor *HomeWindowMonitor::instance()
{
    if (windowMonitorInstance.isNull()) {
        windowMonitorInstance = QSharedPointer<HomeWindowMonitor>(new HomeWindowMonitor());
    }

    return windowMonitorInstance.data();
}

HomeWindowMonitor::HomeWindowMonitor() :
        nonFullscreenApplicationWindowTypes(QSet<Atom>() << AtomCache::atom("_NET_WM_WINDOW_TYPE_NOTIFICATION") <<
                                                            AtomCache::atom("_NET_WM_WINDOW_TYPE_DIALOG") <<
                                                            AtomCache::atom("_NET_WM_WINDOW_TYPE_MENU")),
        netClientListStacking(XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False))
{
}

HomeWindowMonitor::~HomeWindowMonitor()
{
}

bool HomeWindowMonitor::isOwnWindow(WId wid) const
{
    foreach (QWidget *window, QApplication::topLevelWidgets()) {
        if (window->winId() == wid) {
            return true;
        }
    }

    return false;
}

bool HomeWindowMonitor::handleXEvent(const XEvent& event)
{
    bool eventHandled = false;

    if (event.type == PropertyNotify && event.xproperty.atom == netClientListStacking && event.xproperty.window == DefaultRootWindow(QX11Info::display())) {
        int numWindowStackingOrderReceivers = receivers(SIGNAL(windowStackingOrderChanged(QList<WindowInfo*>)));
        int numFullscreenWindowReceivers = receivers(SIGNAL(fullscreenWindowOnTopOfOwnWindow()));
        int numAnyWindowReceivers = receivers(SIGNAL(anyWindowOnTopOfOwnWindow(WindowInfo*)));

        if (numWindowStackingOrderReceivers + numFullscreenWindowReceivers + numAnyWindowReceivers > 0) {
            QList<Window> windowOrder = windowStackingOrder();

            if (numWindowStackingOrderReceivers > 0) {
                QList<WindowInfo *> windowStackingList;
                foreach (Window wid, windowOrder) {
                    windowStackingList.append(WindowInfo::windowFor(wid));
                }

                emit windowStackingOrderChanged(windowStackingList);
            }

            if (numFullscreenWindowReceivers + numAnyWindowReceivers > 0) {
                if (!windowOrder.isEmpty()) {
                    QListIterator<Window> iter(windowOrder);
                    iter.toBack();
                    bool anyWindowSignalEmitted = false;
                    while (iter.hasPrevious()) {
                        WindowInfo *windowInfo = WindowInfo::windowFor(iter.previous());
                        if (isOwnWindow(windowInfo->window())) {
                            break;
                        }
                        if (numAnyWindowReceivers > 0 && !anyWindowSignalEmitted) {
                            emit anyWindowOnTopOfOwnWindow(windowInfo);
                            // signal is sent only once per each XEvent, mark signal sent
                            anyWindowSignalEmitted = true;
                            if (numFullscreenWindowReceivers < 1) {
                                // no listeners for fullscreenWindowOnTopOfOwnWindow() signal
                                break;
                            }
                        }
                        if (windowInfo->types().toSet().intersect(nonFullscreenApplicationWindowTypes).isEmpty()) {
                            emit fullscreenWindowOnTopOfOwnWindow();
                            break;
                        }
                    }
                }
            }
        }
        eventHandled = true;
        emit this->isHomeWindowOnTopChanged();
    }

    return eventHandled;
}

QList<Window> HomeWindowMonitor::windowStackingOrder() const
{
    Display *display = QX11Info::display();
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;
    Status result = XGetWindowProperty(display, DefaultRootWindow(display), netClientListStacking,
                                                   0, 0x7fffffff, False, XA_WINDOW,
                                                   &actualType, &actualFormat, &numWindowItems, &bytesLeft, &windowData);

    QList<Window> stackingWindowList;

    if (result == Success && windowData != None) {
        Window *windows = (Window *)windowData;
        for (unsigned int i = 0; i < numWindowItems; i++) {
            stackingWindowList.append(windows[i]);
        }
    }

    return stackingWindowList;
}

bool HomeWindowMonitor::isHomeWindowOnTop() const
{
    QList<Window> windowOrder = windowStackingOrder();
    return isOwnWindow(windowOrder.last());
}

bool HomeWindowMonitor::isHomeWindowOnTop(QSet<Atom> ignoredWindows) const
{
    QList<Window> windowOrder = windowStackingOrder();
    for (int i = windowOrder.length() - 1; i >= 0; --i)
    {
        if (isOwnWindow(windowOrder[i])) {
            return true;
        }
        QSet<Atom> windowTypes = WindowInfo::windowFor(windowOrder[i])->types().toSet();
        if (windowTypes.intersect(ignoredWindows).isEmpty()) {
             break;
        }
    }
    return false;
}
