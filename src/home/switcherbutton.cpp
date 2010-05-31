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

Atom SwitcherButton::iconGeometryAtom = 0;

SwitcherButton::SwitcherButton(const QString &title, MWidget *parent, Window window, WindowInfo::WindowPriority windowPriority) :
    MButton(title, parent, new SwitcherButtonModel),
    priority(windowPriority)
{
    // Configure timers
    windowCloseTimer.setSingleShot(true);
    connect(&windowCloseTimer, SIGNAL(timeout()), this, SLOT(resetState()));

    if (iconGeometryAtom == 0) {
        // Get the icon geometry X11 Atom if it doesn't exist yet
        iconGeometryAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_ICON_GEOMETRY", False);
    }

    // Update the window title and pixmap
    model()->setXWindow(window);

    connect(MainWindow::instance(), SIGNAL(orientationChangeFinished(const M::Orientation &)), this, SLOT(updateIconGeometry()));

    connect(this, SIGNAL(clicked()), this, SLOT(switchToWindow()));
}

SwitcherButton::~SwitcherButton()
{
}

Window SwitcherButton::xWindow()
{
    return model()->xWindow();
}

WindowInfo::WindowPriority SwitcherButton::windowPriority() const
{
    return priority;
}

void SwitcherButton::setWindowPriority(WindowInfo::WindowPriority windowPriority)
{
    priority = windowPriority;
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

void SwitcherButton::setGeometry(const QRectF &rect)
{
    MButton::setGeometry(rect);

    // When the switcher button's geometry is changed update the icon geometry
    updateIconGeometry();
}

void SwitcherButton::resetState()
{
    setVisible(true);
    prepareGeometryChange();
}

void SwitcherButton::updateIconGeometry()
{
    // Get the position of the Switcher Button in scene coordinates
    QRectF iconPosition = boundingRect();
    iconPosition.moveTo(mapToScene(0, 0));

    // Replace the old X icon geometry property for the window with iconGeometry, which consists of 4 unsigned ints (32 bits)
    unsigned int iconGeometry[4];
    iconGeometry[0] = iconPosition.x();
    iconGeometry[1] = iconPosition.y();
    iconGeometry[2] = iconPosition.width();
    iconGeometry[3] = iconPosition.height();
    X11Wrapper::XChangeProperty(QX11Info::display(), xWindow(), iconGeometryAtom, XA_CARDINAL, sizeof(unsigned int) * 8, PropModeReplace, (unsigned char *)&iconGeometry, 4);
}
