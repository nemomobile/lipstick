/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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
#include <QApplication>
#include <DuiApplicationWindow>
#include <DuiApplicationPage>
#include <DuiButton>
#include <QDebug>
#include <QX11Info>

#include "windowpropertyhandler.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

WindowPropertyHandler::~WindowPropertyHandler()
{
}

static void setCallMode(WId windowId)
{
    Display *dpy = QX11Info::display();
    Atom windowTypeAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    Atom windowTypeCallAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_CALL", False);
    XChangeProperty(dpy, windowId, windowTypeAtom,
                    XA_ATOM, 32, PropModeReplace,
                    (const unsigned char *)&windowTypeCallAtom,
                    1);
}

static void unsetCallMode(WId windowId)
{
    Display *dpy = QX11Info::display();
    Atom windowTypeAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    Atom windowTypeNormalAtom = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
    XChangeProperty(dpy, windowId, windowTypeAtom,
                    XA_ATOM, 32, PropModeReplace,
                    (const unsigned char *)&windowTypeNormalAtom,
                    1);
}

WindowPropertyHandler::WindowPropertyHandler(DuiApplicationPage *page,
        DuiApplicationWindow *window,
        DuiButton *button)
    : page_(page),
      window_(window),
      button_(button),
      originalTitle_(button_->text()),
      state_(false)
{
}

void WindowPropertyHandler::changeWindowProperty(bool /*isCall*/)
{

    state_ = !state_;
    if (state_) {
        QString newTitle(originalTitle_ + " call");
        page_->setTitle(newTitle);
        button_->setText(newTitle);
        window_->setWindowTitle(newTitle);
        setCallMode(window_->winId());

    } else {
        page_->setTitle(originalTitle_);
        button_->setText(originalTitle_);
        window_->setWindowTitle(originalTitle_);
        unsetCallMode(window_->winId());
    }
}


