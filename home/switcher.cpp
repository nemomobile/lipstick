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

#include <QX11Info>
#include <QApplication>
#include <MLayout>
#include <MFlowLayoutPolicy>

#include "switcher.h"
#include "switcherbutton.h"
#include "windowinfo.h"

Switcher::Switcher(MWidget *parent) :
    MWidgetController(new SwitcherModel, parent)
{
    // Connect to the windowListUpdated signal of the HomeApplication to get information about window list changes
    connect(qApp, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), this, SLOT(windowListUpdated(const QList<WindowInfo> &)));
    connect(qApp, SIGNAL(windowTitleChanged(Window, QString)), this, SLOT(changeWindowTitle(Window, QString)));

    // Get the X11 Atoms for closing and activating a window
    closeWindowAtom  = XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW",  False);
    activeWindowAtom = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", False);
}

Switcher::~Switcher()
{
}

void Switcher::windowListUpdated(const QList<WindowInfo> &windowList)
{
    QList< QSharedPointer<SwitcherButton> > oldButtons(model()->buttons());

    // List of existing buttons for which a window still exists
    QList< QSharedPointer<SwitcherButton> > currentButtons;

    // List of newly created buttons
    QList< QSharedPointer<SwitcherButton> > newButtons;

    // List to be set as the new list in the model
    QList< QSharedPointer<SwitcherButton> > nextButtons;

    // The new mapping of known windows to the buttons
    QMap<Window, SwitcherButton *> newWindowMap;

    // Go through the windows and create new buttons for new windows
    foreach(WindowInfo wi, windowList) {
        if (windowMap.contains(wi.window())) {
            SwitcherButton *b = windowMap[wi.window()];

            // Button already exists - set title and priority (as they may have changed)
            b->setText(wi.title());
            b->setWindowPriority(wi.windowPriority());

            newWindowMap[wi.window()] = b;
        } else {
            QSharedPointer<SwitcherButton> b(new SwitcherButton(wi.title(), NULL, wi.window(), wi.windowPriority()));
            connect(b.data(), SIGNAL(windowToFront(Window)), this, SLOT(windowToFront(Window)));
            connect(b.data(), SIGNAL(closeWindow(Window)), this, SLOT(closeWindow(Window)));

            // Put the new high priority buttons straight to the top
            if (wi.windowPriority() == WindowInfo::Call) {
                nextButtons.append(b);
            } else {
                newButtons.append(b);
            }

            newWindowMap[wi.window()] = b.data();
        }
    }

    foreach(QSharedPointer<SwitcherButton> b, oldButtons) {
        // Keep only the buttons for which a window still exists
        if (newWindowMap.contains(b.data()->xWindow())) {
            if (b.data()->windowPriority() == WindowInfo::Call) {
                currentButtons.prepend(b);
            } else {
                currentButtons.append(b);
            }
        }
    }

    windowMap = newWindowMap;
    nextButtons.append(currentButtons);
    nextButtons.append(newButtons);

    // Take the new set of buttons into use
    model()->setButtons(nextButtons);
}

void Switcher::windowToFront(Window window)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    Display *display = QX11Info::display();

    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = activeWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1;
    ev.xclient.data.l[1]    = CurrentTime;
    ev.xclient.data.l[2]    = 0;

    XSendEvent(display,
               rootWin, False,
               SubstructureRedirectMask, &ev);
}

void Switcher::closeWindow(Window window)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));

    Display *display = QX11Info::display();

    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = closeWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = CurrentTime;
    ev.xclient.data.l[1]    = rootWin;

    XSendEvent(display,
               rootWin, False,
               SubstructureRedirectMask, &ev);
}

void Switcher::viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)
{
    foreach(QSharedPointer<SwitcherButton> b, model()->buttons()) {
        // Update the icon geometry of each button when the viewport is panned
        b->updateIconGeometry();
    }
}

void Switcher::changeWindowTitle(Window window,  const QString &title)
{
    if (windowMap.contains(window)) {
        windowMap.value(window)->setText(title);
        windowMap.value(window)->update();
    }
}
