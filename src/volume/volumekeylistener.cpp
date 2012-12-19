/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "xtools/homewindowmonitor.h"
#include <QX11Info>
#include "components/windowinfo.h"
#include "xtools/x11wrapper.h"
#include "xtools/xatomcache.h"
#include "volumekeylistener.h"

VolumeKeyListener::VolumeKeyListener(QObject *parent) : QObject(parent)
{
}

bool VolumeKeyListener::handleXEvent(const XEvent &event)
{
    if (event.type == PropertyNotify && event.xproperty.window == DefaultRootWindow(QX11Info::display()) && event.xproperty.atom == AtomCache::atom("_NET_CLIENT_LIST")) {
        selectKeyPressInputForAllWindows();
    } else if ((event.type == KeyPress || event.type == KeyRelease) && event.xkey.keycode >= 8) {
        // Convert the X keycode to a Linux kernel keycode by substracting 8
        emit keyEvent(event.xkey.keycode - 8, event.type);
        return true;
    }

    return false;
}

void VolumeKeyListener::selectKeyPressInputForAllWindows()
{
    Display *dpy = QX11Info::display();
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;

    int result = X11Wrapper::XGetWindowProperty(dpy, DefaultRootWindow(dpy), AtomCache::atom("_NET_CLIENT_LIST"), 0, 0x7fffffff, false, XA_WINDOW, &actualType, &actualFormat, &numWindowItems, &bytesLeft, &windowData);

    if (result != Success || windowData == None) {
        return;
    }

    Window *windows = (Window *)windowData;
    for (unsigned int window = 0; window < numWindowItems; window++) {
        WindowInfo *windowInfo = WindowInfo::windowFor(windows[window]);
        if (!HomeWindowMonitor::instance()->isOwnWindow(windowInfo->window())) {
            X11Wrapper::XSelectInput(dpy, windowInfo->window(), KeyPressMask | KeyReleaseMask);
        }
    }

    X11Wrapper::XFree(windowData);
}
