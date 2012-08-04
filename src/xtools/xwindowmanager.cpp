
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

#include "windowinfo.h"
#include "xtools/xwindowmanager.h"
#include "xtools/xatomcache.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

XWindowManager::XWindowManager(QObject *parent)
    : QObject(parent)
{
}

void XWindowManager::windowToFront(qulonglong window)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = AtomCache::ActiveWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1;
    ev.xclient.data.l[1]    = CurrentTime;

    XSendEvent(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), False, StructureNotifyMask, &ev);
    qDebug() << Q_FUNC_INFO << "Foregrounded " << window;
}

void XWindowManager::closeWindow(qulonglong window)
{
    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = AtomCache::CloseWindowAtom;
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
