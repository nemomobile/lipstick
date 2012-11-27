/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "eventeater.h"

EventEater::EventEater(QObject *parent) : QObject(parent)
{
    Display *dpy = QX11Info::display();
    int scr = DefaultScreen(dpy);

    XSetWindowAttributes attr;
    attr.override_redirect = True;

    window = X11Wrapper::XCreateWindow(dpy, DefaultRootWindow(dpy),
                                       0, 0,
                                       DisplayWidth(dpy, scr), DisplayHeight(dpy, scr),
                                       0,
                                       CopyFromParent,
                                       InputOnly,
                                       CopyFromParent,
                                       CWOverrideRedirect,
                                       &attr);

    long inputMask = ButtonPressMask|KeyPressMask;
    X11Wrapper::XSelectInput(dpy, window, inputMask);
    X11Wrapper::XStoreName(dpy, window, const_cast<char*>("EventEater"));

    // Set the stacking layer
    Atom stackingLayerAtom = X11Wrapper::XInternAtom(dpy, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None) {
        long layer = 6;
        X11Wrapper::XChangeProperty(dpy, window, stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &layer, 1);
    }
}

EventEater::~EventEater()
{
    X11Wrapper::XDestroyWindow(QX11Info::display(), window);
}

void EventEater::show()
{
    Display *dpy = QX11Info::display();

    X11Wrapper::XMapRaised(dpy, window);

    // Grabs are released automatically at unmap
    X11Wrapper::XGrabKeyboard(dpy, window, False, GrabModeAsync, GrabModeAsync, CurrentTime);
    X11Wrapper::XGrabPointer(dpy, window, False, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
}

void EventEater::hide()
{
    X11Wrapper::XUnmapWindow(QX11Info::display(), window);
}

bool EventEater::handleXEvent(const XEvent &event)
{
    bool handled = false;

    if ((event.xany.window == window) && (event.type == ButtonPress || event.type == KeyPress)) {
        handled = true;
        emit inputEventReceived();
    }

    return handled;
}
