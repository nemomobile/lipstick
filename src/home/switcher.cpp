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
#include <QEvent>
#include <MApplication>
#include <MLayout>
#include <MFlowLayoutPolicy>
#include "switcher.h"
#include "switcherbutton.h"
#include "windowinfo.h"
#include "mainwindow.h"
#include "x11wrapper.h"

// The time to wait until updating the model when a new application is started
#define UPDATE_DELAY_MS 700

Switcher *Switcher::switcher = NULL;

Switcher *Switcher::instance()
{
    if (switcher == NULL) {
        switcher = new Switcher;
    }
    return switcher;
}

Switcher::Switcher(MWidget *parent) :
    MWidgetController(new SwitcherModel, parent)
{
    // Get the X11 Atoms for closing and activating a window and for different window types
    Display *dpy = QX11Info::display();
    closeWindowAtom  = X11Wrapper::XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
    activeWindowAtom = X11Wrapper::XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
    windowTypeAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    clientListAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLIENT_LIST", False);
    stackedClientListAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLIENT_LIST_STACKING", False);
    closeWindowAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
    windowStateAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_STATE", False);
    netWindowNameAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_NAME", False);
    windowNameAtom = X11Wrapper::XInternAtom(dpy, "WM_NAME", False);

    // Put the atoms for window types that should be excluded from the switcher into a list
    excludeAtoms.insert(WindowInfo::DesktopAtom);
    excludeAtoms.insert(WindowInfo::MenuAtom);
    excludeAtoms.insert(WindowInfo::DockAtom);
    excludeAtoms.insert(WindowInfo::DialogAtom);
    excludeAtoms.insert(WindowInfo::NotificationAtom);
    excludeAtoms.insert(WindowInfo::SkipTaskbarAtom);

    // This stuff is necessary to receive touch events
    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);
}

Switcher::~Switcher()
{
    switcher = NULL;
}

void Switcher::windowToFront(Window window)
{
    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = activeWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1;
    ev.xclient.data.l[1]    = CurrentTime;

    X11Wrapper::XSendEvent(QX11Info::display(), rootWin, False, StructureNotifyMask, &ev);
}

void Switcher::closeWindow(Window window)
{
    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = closeWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = CurrentTime;
    ev.xclient.data.l[1]    = rootWin;

    X11Wrapper::XSendEvent(QX11Info::display(), rootWin, False, SubstructureRedirectMask, &ev);
}

void Switcher::updateButtons()
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
    foreach(WindowInfo wi, applicationWindows) {
        if (switcherButtonMap.contains(wi.window())) {
            SwitcherButton *b = switcherButtonMap[wi.window()];

            // Button already exists - set title (as it may have changed)
            b->setText(wi.title());

            newWindowMap.insert(wi.window(), b);
        } else {
            QSharedPointer<SwitcherButton> b(new SwitcherButton(wi.title(), NULL, wi.window()));
            connect(b.data(), SIGNAL(windowToFront(Window)), this, SLOT(windowToFront(Window)));
            connect(b.data(), SIGNAL(closeWindow(Window)), this, SLOT(closeWindow(Window)));

            newButtons.append(b);

            newWindowMap.insert(wi.window(), b.data());
        }
    }

    foreach(QSharedPointer<SwitcherButton> b, oldButtons) {
        // Keep only the buttons for which a window still exists
        if (newWindowMap.contains(b.data()->xWindow())) {
            currentButtons.append(b);
        }
    }

    switcherButtonMap = newWindowMap;
    nextButtons.append(currentButtons);
    nextButtons.append(newButtons);

    // Take the new set of buttons into use
    model()->setButtons(nextButtons);

    // Let interested parties know about the updated window list
    emit windowListUpdated(applicationWindows);
}

bool Switcher::handleX11Event(XEvent *event)
{
    if (event->type == PropertyNotify) {
        if (event->xproperty.atom == stackedClientListAtom && event->xproperty.window == DefaultRootWindow(QX11Info::display())) {
            // The client list of the root window has changed so update the window list
            updateWindowMapping();
            return true;
        } else if (event->xproperty.atom == windowTypeAtom || event->xproperty.atom == windowStateAtom) {
            // The type or state of a window has changed so update that window's properties
            updateWindowProperties(event->xproperty.window);
            return true;
        } else if (event->xproperty.atom == windowNameAtom || event->xproperty.atom == netWindowNameAtom) {
            // The title of a window has changed so update that window's title
            updateWindowTitle(event->xproperty.window);
            return true;
        }
    } else if (event->type == VisibilityNotify) {
        if (event->xvisibility.state == VisibilityFullyObscured) {
            // A window was obscured: was it a homescreen window?
            bool homescreenWindowVisibilityChanged = false;
            Q_FOREACH(MWindow *window, MApplication::windows()) {
                if (event->xvisibility.window == window->winId()) {
                    homescreenWindowVisibilityChanged = true;
                    break;
                }
            }

            if (!homescreenWindowVisibilityChanged) {
                // It was some other window, so let interested parties (the SwitcherButtons) know about it
                if (event->xvisibility.send_event) {
                    emit windowVisibilityChanged(event->xvisibility.window);
                }
                return true;
            }
        }
    } else if (event->type == ClientMessage && event->xclient.message_type == closeWindowAtom) {
        // A _NET_CLOSE_WINDOW message was caught so a window is being closed; add it to windows being closed list and update the window list
        if (!windowsBeingClosed.contains(event->xclient.window)) {
            windowsBeingClosed.insert(event->xclient.window);
        }
        updateWindowMapping();
        return true;
    }

    return false;
}

void Switcher::updateWindowTitle(Window window)
{
    QMap<Window, WindowInfo>::iterator i = windowMap.find(window);
    if (i != windowMap.end()) {
        if ((*i).updateWindowTitle() && switcherButtonMap.contains(window)) {
            switcherButtonMap.value(window)->setText((*i).title());
            switcherButtonMap.value(window)->update();
        }
    }
}

void Switcher::updateWindowProperties(Window window)
{
    QMap<Window, WindowInfo>::iterator i = windowMap.find(window);
    if (i != windowMap.end()) {
        bool wasApplication = isApplicationWindow(*i);
        (*i).updateWindowProperties();
        bool isApplication = isApplicationWindow(*i);

        if (wasApplication != isApplication) {
            // Update the window list if the window has changed from an app window to some thing else or vice versa
            if (isApplication) {
                applicationWindows.append(*i);
            } else {
                applicationWindows.removeOne(*i);
            }

            updateButtons();
        }
    }
}

void Switcher::updateWindowMapping()
{
    // Get a list of all windows
    Display *dpy = QX11Info::display();
    XWindowAttributes wAttributes;
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;
    Status result = X11Wrapper::XGetWindowProperty(dpy, DefaultRootWindow(dpy), stackedClientListAtom,
                                                0, 0x7fffffff, False, XA_WINDOW,
                                                &actualType, &actualFormat, &numWindowItems, &bytesLeft, &windowData);

    if (result == Success && windowData != None) {
        // We need to keep the stacking order of the windows -> hence the list
        QList<Window> newWindowList;
        Window *wins = (Window *)windowData;
        for (unsigned int i = 0; i < numWindowItems; i++) {
            result = X11Wrapper::XGetWindowAttributes(dpy, wins[i], &wAttributes);
            // The windows that are bigger than 0x0, are Input/Output windows and are not unmapped are interesting
            if (result != 0 &&
                wAttributes.width > 0 && wAttributes.height > 0 &&
                wAttributes.c_class == InputOutput &&
                wAttributes.map_state != IsUnmapped) {
                newWindowList.append(wins[i]);
            }
        }
        QSet<Window> newWindowSet = newWindowList.toSet() - windowsBeingClosed;
        QSet<Window> oldWindowSet = windowMap.keys().toSet();
        QSet<Window> closedWindowSet = oldWindowSet - newWindowSet;
        windowsBeingClosed -= closedWindowSet;
        QSet<Window> openedWindowSet = newWindowSet - oldWindowSet;

        bool added = createWindowInfos(openedWindowSet);
        bool removed = removeWindowInfos(closedWindowSet);
        if (added || removed) {
            if (!removed) {
                // If windows have been added but not removed, update the switcher with a delay
                QTimer::singleShot(UPDATE_DELAY_MS, this, SLOT(updateButtons()));
            } else {
                // If windows have been removed update the switcher instantly
                updateButtons();
            }
        }
        windowsBeingClosed -= closedWindowSet;

        QList<WindowInfo> stackingWindowList;
        foreach (Window w, newWindowList) {
            if (!windowsBeingClosed.contains(w)) {
                stackingWindowList.append(windowMap.value(w));
            }
        }
        emit windowStackingOrderChanged(stackingWindowList);
    }
}

bool Switcher::createWindowInfos(QSet<Window> openedWindowSet)
{
    bool appWindowsAppeared = false;
    foreach(Window w, openedWindowSet) {
        WindowInfo newWindow(w);
        windowMap.insert(w, newWindow);
        if (MainWindow::instance() != NULL && MainWindow::instance()->winId() != w) {
            X11Wrapper::XSelectInput(QX11Info::display(), w, VisibilityChangeMask | PropertyChangeMask);
        }

        if (isApplicationWindow(newWindow)) {
            applicationWindows.append(newWindow);
            appWindowsAppeared = true;
        }
    }
    return appWindowsAppeared;
}

bool Switcher::removeWindowInfos(QSet<Window> closedWindowSet)
{
    bool appWindowsDisappeared = false;
    foreach(Window w, closedWindowSet) {
        if (windowMap.contains(w)) {
            WindowInfo removedWindow = windowMap.take(w);
            if (applicationWindows.removeOne(removedWindow)) {
                appWindowsDisappeared = true;
            }
        }
    }
    return appWindowsDisappeared;
}

bool Switcher::isApplicationWindow(const WindowInfo &wi)
{
    QSet<Atom> excludeSet;
    excludeSet += wi.types().toSet();
    excludeSet += wi.states().toSet();
    return excludeSet.intersect(excludeAtoms).isEmpty();
}
