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
#include <cstring>
#include "windowinfo.h"
#include "x11wrapper.h"
#include <QX11Info>


static bool atomsInitialized;

Atom WindowInfo::TypeAtom;
Atom WindowInfo::StateAtom;
Atom WindowInfo::NormalAtom;
Atom WindowInfo::DesktopAtom;
Atom WindowInfo::NotificationAtom;
Atom WindowInfo::DialogAtom;
Atom WindowInfo::CallAtom;
Atom WindowInfo::DockAtom;
Atom WindowInfo::MenuAtom;
Atom WindowInfo::SkipTaskbarAtom;
Atom WindowInfo::NameAtom;

void WindowInfo::initializeAtoms()
{
    if (!atomsInitialized) {
        Display *dpy = QX11Info::display();
        TypeAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
        StateAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE", False);
        NormalAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
        DesktopAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
        NotificationAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
        DialogAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
        CallAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_CALL", False);
        DockAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
        MenuAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_MENU", False);
        SkipTaskbarAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", False);
        NameAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_NAME", False);
        atomsInitialized = true;
    }
}

WindowInfo::WindowInfo(Window window)
{
    d = new WindowData;
    d->window = window;
    updateWindowTitle();
    updateWindowProperties();
}

WindowInfo::WindowInfo()
{
    d = new WindowData;
}

WindowInfo::~WindowInfo()
{
}

const QString& WindowInfo::title() const
{
    return d->title;
}

Window WindowInfo::window() const
{
    return d->window;
}

Window WindowInfo::transientFor() const
{
    return d->transientFor;
}

QList<Atom> WindowInfo::types() const
{
    return d->types;
}

QList<Atom> WindowInfo::states() const
{
    return d->states;
}

bool operator==(const WindowInfo &wi1, const WindowInfo &wi2)
{
    return wi1.window() == wi2.window();
}

bool WindowInfo::updateWindowTitle()
{
    Display *dpy = QX11Info::display();
    XTextProperty textProperty;
    bool updated = false;
    int result = X11Wrapper::XGetTextProperty(dpy, d->window, &textProperty, WindowInfo::NameAtom);
    if (result == 0) {
        result = X11Wrapper::XGetWMName(dpy, d->window, &textProperty);
    }

    if (result != 0) {
        d->title = QString::fromUtf8((const char *)textProperty.value);
        X11Wrapper::XFree(textProperty.value);
        updated = true;
    }
    return updated;
}

void WindowInfo::updateWindowProperties()
{
    d->types = getWindowProperties(d->window, WindowInfo::TypeAtom);
    d->states = getWindowProperties(d->window, WindowInfo::StateAtom);

    if (!X11Wrapper::XGetTransientForHint(QX11Info::display(), d->window, &d->transientFor) || d->transientFor == d->window) {
        d->transientFor = 0;
    }
}

QList<Atom> WindowInfo::getWindowProperties(Window winId, Atom propertyAtom, long maxCount)
{
    QList<Atom> properties;
    Atom actualType;
    int actualFormat;
    unsigned long numTypeItems, bytesLeft;
    unsigned char *typeData = NULL;

    Status result = X11Wrapper::XGetWindowProperty(QX11Info::display(), winId, propertyAtom, 0L, maxCount, False, XA_ATOM, &actualType, &actualFormat, &numTypeItems, &bytesLeft, &typeData);
    if (result == Success) {
        Atom *type = (Atom *) typeData;
        for (unsigned int n = 0; n < numTypeItems; n++) {
            properties.append(type[n]);
        }
        X11Wrapper::XFree(typeData);
    }
    return properties;
}
