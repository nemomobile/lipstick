
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2011, Robin Burchell
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include <QDebug>
#include <QX11Info>

#include "components/windowinfo.h"
#include "xtools/xwindowmanager.h"
#include "xtools/xatomcache.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

void XWindowManager::windowToFront(Qt::HANDLE window)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = AtomCache::atom("_NET_ACTIVE_WINDOW");
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1;
    ev.xclient.data.l[1]    = CurrentTime;

    XSendEvent(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), False, StructureNotifyMask, &ev);
    qDebug() << Q_FUNC_INFO << "Foregrounded " << window;
}

void XWindowManager::closeWindow(Qt::HANDLE window)
{
    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = AtomCache::atom("_NET_CLOSE_WINDOW");
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = CurrentTime;
    ev.xclient.data.l[1]    = rootWin;

    XSendEvent(QX11Info::display(), rootWin, False, SubstructureRedirectMask, &ev);
    qDebug() << Q_FUNC_INFO << "Closed " << window;

    // Close also the window this one is transient for, if any
    WindowInfo *windowInfo = WindowInfo::windowFor(window);

    if (windowInfo->transientFor() != 0 && windowInfo->transientFor() != window)
    {
        qDebug() << Q_FUNC_INFO << "Closing transient " << windowInfo->transientFor();
        closeWindow(windowInfo->transientFor());
    }
}

void XWindowManager::excludeFromTaskBar(Qt::HANDLE windowId)
{
    // Tell the window to not to be shown in the switcher
    changeNetWmState(windowId, true, AtomCache::atom("_NET_WM_STATE_SKIP_TASKBAR"));

    // Also set the _NET_WM_STATE window property to ensure Home doesn't try to
    // manage this window in case the window manager fails to set the property in time
    QVector<Atom> atoms;
    atoms.append(AtomCache::atom("_NET_WM_STATE_SKIP_TASKBAR"));
    XChangeProperty(QX11Info::display(), windowId, AtomCache::atom("_NET_WM_STATE"), XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());
}

void XWindowManager::changeNetWmState(Qt::HANDLE windowId, bool enable, Qt::HANDLE first, Qt::HANDLE second)
{
    XEvent event;
    memset(&event, 0, sizeof(event));
    event.xclient.type = ClientMessage;
    event.xclient.message_type = AtomCache::atom("_NET_WM_STATE");
    event.xclient.display = QX11Info::display();
    event.xclient.window = windowId;
    event.xclient.format = 32;
    event.xclient.data.l[0] = enable ? 1 : 0;
    event.xclient.data.l[1] = first;
    event.xclient.data.l[2] = second;
    event.xclient.data.l[3] = 0;
    event.xclient.data.l[4] = 0;
    XSendEvent(QX11Info::display(), windowId, FALSE, (SubstructureNotifyMask | SubstructureRedirectMask), &event);
}
