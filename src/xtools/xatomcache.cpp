
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

#include <QX11Info>
#include "xatomcache.h"

/* TODO: Should we care about these too?

    "_MEEGO_ORIENTATION",
    "_MEEGO_INHIBIT_SCREENSAVER",
    "_MEEGO_TABLET_NOTIFY",
    "_MEEGO_STACKING_LAYER",
    "_MEEGOTOUCH_SKIP_ANIMATIONS",
    "_NET_ACTIVE_WINDOW",
    "_NET_CLIENT_LIST",
    "_NET_CLOSE_WINDOW",
    "_NET_WM_WINDOW_TYPE",
    "_NET_WM_WINDOW_TYPE_NORMAL",
    "_NET_WM_WINDOW_TYPE_DESKTOP",
    "_NET_WM_WINDOW_TYPE_NOTIFICATION",
    "_NET_WM_WINDOW_TYPE_DOCK",
    "_NET_WM_STATE_SKIP_TASKBAR",
    "_NET_WM_STATE",
    "_NET_WM_PID",
    "_NET_WM_ICON_GEOMETRY",
    "_NET_WM_ICON_NAME",
    "Backlight",
    "BACKLIGHT",
    "UTF8_STRING",
    "WM_CHANGE_STATE",
    "_MEEGO_SYSTEM_DIALOG"
*/

static bool atomsInitialized;

Atom AtomCache::TypeAtom;
Atom AtomCache::StateAtom;
Atom AtomCache::SkipTaskbarAtom;
Atom AtomCache::NameAtom;
Atom AtomCache::ClientListAtom;
Atom AtomCache::CloseWindowAtom;
Atom AtomCache::ActiveWindowAtom;
Atom AtomCache::Utf8StringAtom;
Atom AtomCache::WindowPidAtom;

Atom AtomCache::WindowTypeNormalAtom;
Atom AtomCache::WindowTypeDesktopAtom;
Atom AtomCache::WindowTypeNotificationAtom;
Atom AtomCache::WindowTypeDialogAtom;
Atom AtomCache::WindowTypeCallAtom;
Atom AtomCache::WindowTypeDockAtom;
Atom AtomCache::WindowTypeMenuAtom;
Atom AtomCache::WindowTypeInputAtom;

void AtomCache::initializeAtoms()
{
    if (!atomsInitialized)
    {
        Display *dpy = QX11Info::display();

        TypeAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
        StateAtom = XInternAtom(dpy, "_NET_WM_STATE", False);
        SkipTaskbarAtom = XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", False);
        NameAtom = XInternAtom(dpy, "_NET_WM_NAME", False);
        ClientListAtom = XInternAtom(dpy, "_NET_CLIENT_LIST", False);
        CloseWindowAtom = XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
        ActiveWindowAtom = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
        Utf8StringAtom = XInternAtom(dpy, "UTF8_STRING", False);
        WindowPidAtom = XInternAtom(dpy, "_NET_WM_PID", False);

        WindowTypeNormalAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
        WindowTypeDesktopAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
        WindowTypeNotificationAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
        WindowTypeDialogAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
        WindowTypeCallAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_CALL", False);
        WindowTypeDockAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
        WindowTypeMenuAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_MENU", False);
        WindowTypeInputAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_INPUT", False);

        atomsInitialized = true;
    }
}
