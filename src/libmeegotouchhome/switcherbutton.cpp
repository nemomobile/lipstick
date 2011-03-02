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
#include "switcherbutton.h"
#include <MApplication>
#include "homeapplication.h"
#include "switcherbuttonview.h"
#include "mainwindow.h"
#include "x11wrapper.h"
#include <QX11Info>

#include <MWidgetCreator>
M_REGISTER_WIDGET(SwitcherButton)

Atom SwitcherButton::visibleAtom = 0;

SwitcherButton::SwitcherButton(QGraphicsItem *parent, SwitcherButtonModel *model) :
    MButton(parent, model),
    visibility(false),
    visibilityPropertyEnabled(true),
    visibilityInitialized(false)
{
    // Configure timers
    windowCloseTimer.setSingleShot(true);
    connect(&windowCloseTimer, SIGNAL(timeout()), this, SLOT(resetState()));

    if (visibleAtom == 0) {
        // Get the icon geometry X11 Atom if it doesn't exist yet
        visibleAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_MEEGOTOUCH_VISIBLE_IN_SWITCHER", False);
    }

    connect(this, SIGNAL(clicked()), this, SLOT(switchToWindow()));
}

SwitcherButton::~SwitcherButton()
{
}

Window SwitcherButton::xWindow()
{
    return model()->xWindow();
}

void SwitcherButton::setXWindow(Window window)
{
    model()->setXWindow(window);
}

void SwitcherButton::switchToWindow()
{
    emit windowToFront(model()->xWindow());
}

void SwitcherButton::close()
{
    setVisible(false);
    windowCloseTimer.start(5000);

    emit closeWindow(model()->xWindow());
}

void SwitcherButton::prepareGeometryChange()
{
    QGraphicsItem::prepareGeometryChange();
}

void SwitcherButton::resetState()
{
    setVisible(true);
    prepareGeometryChange();
    emit closeTimedOutForWindow(model()->xWindow());
}

void SwitcherButton::enterDisplayEvent()
{
    setVisibleInSwitcherProperty(true);
}

void SwitcherButton::exitDisplayEvent()
{
    setVisibleInSwitcherProperty(false);
}

void SwitcherButton::setVisibleInSwitcherProperty(bool set)
{
    if (visibilityPropertyEnabled && (visibility != set || !visibilityInitialized)) {
        Display *dpy = QX11Info::display();
        if (set) {
            unsigned char data = 1;
            X11Wrapper::XChangeProperty(dpy, xWindow(), visibleAtom, XA_CARDINAL, 8, PropModeReplace, &data, 1);
        } else {
            unsigned char data = 0;
            X11Wrapper::XChangeProperty(dpy, xWindow(), visibleAtom, XA_CARDINAL, 8, PropModeReplace, &data, 1);
        }

        visibility = set;
        visibilityInitialized = true;
    }
}

void SwitcherButton::setVisibilityPropertyEnabled(bool enable)
{
    visibilityPropertyEnabled = enable;

    if (enable) {
        setVisibleInSwitcherProperty(isOnDisplay());
    }
}
