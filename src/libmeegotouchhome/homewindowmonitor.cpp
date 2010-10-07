/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <MApplication>
#include <MWindow>
#include "homewindowmonitor.h"
#include "x11wrapper.h"

HomeWindowMonitor::HomeWindowMonitor() :
        nonFullscreenApplicationWindowTypes(QSet<Atom>() << WindowInfo::NotificationAtom <<
                                                            WindowInfo::DialogAtom <<
                                                            WindowInfo::MenuAtom),
        netClientListStacking(X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False))
{
}

HomeWindowMonitor::~HomeWindowMonitor()
{
}

bool HomeWindowMonitor::isOwnWindow(WId wid) const
{
    foreach (MWindow *window, MApplication::windows()) {
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
        int numWindowStackingOrderReceivers = receivers(SIGNAL(windowStackingOrderChanged(QList<WindowInfo>)));
        int numFullscreenWindowReceivers = receivers(SIGNAL(fullscreenWindowOnTopOfOwnWindow()));

        if (numWindowStackingOrderReceivers + numFullscreenWindowReceivers > 0) {
            QList<Window> windowOrder = windowStackingOrder();

            if (numWindowStackingOrderReceivers > 0) {
                QList<WindowInfo> windowStackingList;
                foreach (Window window, windowOrder) {
                    windowStackingList.append(WindowInfo(window));
                }

                emit windowStackingOrderChanged(windowStackingList);
            }

            if (numFullscreenWindowReceivers > 0) {
                if (!windowOrder.isEmpty()) {
                    QListIterator<Window> iter(windowOrder);
                    iter.toBack();
                    while (iter.hasPrevious()) {
                        WindowInfo windowInfo(iter.previous());
                        if (windowInfo.types().toSet().intersect(nonFullscreenApplicationWindowTypes).isEmpty()) {
                            if (!isOwnWindow(windowInfo.window())) {
                                emit fullscreenWindowOnTopOfOwnWindow();
                            }
                            break;
                        }
                    }
                }
            }
        }

        eventHandled = true;
    }

    return eventHandled;
}

QList<Window> HomeWindowMonitor::windowStackingOrder()
{
    Display *display = QX11Info::display();
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;
    Status result = X11Wrapper::XGetWindowProperty(display, DefaultRootWindow(display), netClientListStacking,
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
