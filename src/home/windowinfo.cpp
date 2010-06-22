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

WindowInfo::WindowInfo(Window window)
{    
    if (!atomsInitialized) {
        Display *dpy = QX11Info::display();
        WindowInfo::TypeAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
        WindowInfo::StateAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE", False);
        WindowInfo::NormalAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);    
        WindowInfo::DesktopAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
        WindowInfo::NotificationAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
        WindowInfo::DialogAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
        WindowInfo::CallAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_CALL", False);
        WindowInfo::DockAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
        WindowInfo::MenuAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_MENU", False);
        WindowInfo::SkipTaskbarAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", False);
        WindowInfo::NameAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_NAME", False);
        atomsInitialized = true;
    }
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

WindowInfo::WindowPriority WindowInfo::windowPriority() const
{
    return d->types.contains(WindowInfo::CallAtom) ? WindowInfo::Call : WindowInfo::Normal;
}

Window WindowInfo::window() const
{
    return d->window;
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
}

QList<Atom> WindowInfo::getWindowProperties(Window winId, Atom propertyAtom, long maxCount)
{
    QList<Atom> to;
    Display *dpy = QX11Info::display();

    Atom actualType;
    int actualFormat;
    unsigned long numTypeItems, bytesLeft;
    unsigned char *typeData = NULL;
    
    Status result = X11Wrapper::XGetWindowProperty(dpy, winId, propertyAtom, 0L, maxCount, False, XA_ATOM,
                                                   &actualType, &actualFormat, 
                                                   &numTypeItems, &bytesLeft, &typeData);

    if (result == Success) {
        to.clear();
        Atom *type = (Atom *)typeData;
        for (unsigned int n = 0; n < numTypeItems; n++) {
            to.append(type[n]);
        }
        X11Wrapper::XFree(typeData);            
    }
    return to;
}
