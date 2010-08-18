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
#include "switcher.h"
#include "windowmonitor.h"
#include "switcherbutton.h"
#include "windowinfo.h"
#include "homewindowmonitor.h"
#include "x11wrapper.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET(Switcher)

// The time to wait until updating the model when a new application is started
#define UPDATE_DELAY_MS 700

// For getting a singleton Switcher instance
Switcher *Switcher::switcher = NULL;
Switcher *Switcher::instance()
{
    if (switcher == NULL) {
        qFatal("Switcher::instance called before it was constructed");
    }
    return switcher;
}

Switcher::Switcher(const WindowMonitor *windowMonitor, MWidget *parent) : MWidgetController(new SwitcherModel, parent),
        windowMonitor(windowMonitor)
{
    switcher = this;

    if (this->windowMonitor == NULL) {
        this->windowMonitor = new HomeWindowMonitor;
    }

    // Get the X11 Atoms for closing and activating a window and for other switcher functionalities
    Display *display = QX11Info::display();
    closeWindowAtom = X11Wrapper::XInternAtom(display, "_NET_CLOSE_WINDOW", False);
    activeWindowAtom = X11Wrapper::XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    clientListAtom = X11Wrapper::XInternAtom(display, "_NET_CLIENT_LIST", False);
    stackedClientListAtom = X11Wrapper::XInternAtom(display, "_NET_CLIENT_LIST_STACKING", False);
    netWindowNameAtom = X11Wrapper::XInternAtom(display, "_NET_WM_NAME", False);
    windowNameAtom = X11Wrapper::XInternAtom(display, "WM_NAME", False);

    // Put the atoms for window types that should be excluded from the switcher into a list
    excludeAtoms.insert(WindowInfo::DesktopAtom);
    excludeAtoms.insert(WindowInfo::MenuAtom);
    excludeAtoms.insert(WindowInfo::DockAtom);
    excludeAtoms.insert(WindowInfo::DialogAtom);
    excludeAtoms.insert(WindowInfo::NotificationAtom);
    excludeAtoms.insert(WindowInfo::SkipTaskbarAtom);
    excludeAtoms.insert(WindowInfo::InputWindowAtom);

    // Configure the update buttons timer
    updateButtonsTimer.setSingleShot(true);
    updateButtonsTimer.setInterval(UPDATE_DELAY_MS);
    connect(&updateButtonsTimer, SIGNAL(timeout()), this, SLOT(updateButtons()));

    // This stuff is necessary to receive touch events
    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);
}

Switcher::~Switcher()
{
    delete windowMonitor;

    qDeleteAll(windowInfoMap);

    switcher = NULL;
}

bool Switcher::handleXEvent(const XEvent &event)
{
    bool eventWasHandled = false;

    if (event.type == CreateNotify) {
        // A window has been created so add it to the switcher if it has not been added already
        if (isRelevantWindow(event.xcreatewindow.window) && addWindow(event.xcreatewindow.window)) {
            scheduleUpdateButtons();
        }
        eventWasHandled = true;
    } else if (event.type == DestroyNotify) {
        // A window has been destroyed so completely remove it from the switcher
        if (removeWindow(event.xdestroywindow.window)) {
            // Update the switcher buttons instantly
            updateButtons();
        }
        eventWasHandled = true;
    } else if (event.type == PropertyNotify) {
        if (event.xproperty.atom == stackedClientListAtom && event.xproperty.window == DefaultRootWindow(QX11Info::display())) {
            // The client list of the root window has changed so update the window list
            updateWindowInfoMap();
            eventWasHandled = true;
        } else if (event.xproperty.atom == WindowInfo::TypeAtom || event.xproperty.atom == WindowInfo::StateAtom || event.xproperty.atom == XA_WM_TRANSIENT_FOR) {
            // The type, state or transiency of a window has changed so update that window's properties
            updateWindowProperties(event.xproperty.window);
            eventWasHandled = true;
        } else if (event.xproperty.atom == windowNameAtom || event.xproperty.atom == netWindowNameAtom) {
            // The title of a window has changed so update that window's title
            updateWindowTitle(event.xproperty.window);
            eventWasHandled = true;
        }
    } else if (event.type == ClientMessage && event.xclient.message_type == closeWindowAtom) {
        // A _NET_CLOSE_WINDOW message was caught so a window is being closed; add it to windows being closed list
        markWindowBeingClosed(event.xclient.window);
        eventWasHandled = true;
    }

    return eventWasHandled;
}

bool Switcher::addWindows(const QSet<Window> &windows)
{
    bool applicationWindowListChanged = false;
    foreach(Window window, windows) {
        applicationWindowListChanged |= addWindow(window);
    }
    return applicationWindowListChanged;
}

bool Switcher::addWindow(Window window)
{
    bool applicationWindowListChanged = false;

    // Window that are already tracked can be ignored
    if (!windowInfoMap.contains(window)) {
        // Add the window to the map of tracked windows
        WindowInfo *windowInfo = new WindowInfo(window);
        windowInfoMap.insert(window, windowInfo);

        if (windowInfo->transientFor() != 0) {
            markWindowTransientFor(window, windowInfo->transientFor());
            applicationWindowListChanged = true;
        } else if (isApplicationWindow(*windowInfo)) {
            // Add the window to the application window list in case it is one
            applicationWindows.append(*windowInfo);

            if (windowMonitor != NULL && !windowMonitor->isOwnWindow(window)) {
                // The Switcher needs to know about Visibility and property changes of other applications' windows (but not of the homescreen window)
                X11Wrapper::XSelectInput(QX11Info::display(), window, VisibilityChangeMask | PropertyChangeMask);
            }

            applicationWindowListChanged = true;
        }
    }

    return applicationWindowListChanged;
}

bool Switcher::removeWindows(const QSet<Window> &windows)
{
    bool applicationWindowListChanged = false;
    foreach(Window window, windows) {
        applicationWindowListChanged |= removeWindow(window);
    }
    return applicationWindowListChanged;
}

bool Switcher::removeWindow(Window window)
{
    bool applicationWindowListChanged = false;

    // Remove the window from the tracked windows
    windowsBeingClosed.remove(window);
    WindowInfo *windowInfo = windowInfoMap.take(window);
    if (windowInfo != NULL) {
        if (windowInfo->transientFor() != 0) {
            unmarkWindowTransientFor(window, windowInfo->transientFor());
            applicationWindowListChanged = true;
        } else if (applicationWindows.contains(*windowInfo)) {
            applicationWindows.removeOne(*windowInfo);
            applicationWindowListChanged = true;
        }
        delete windowInfo;
    }

    return applicationWindowListChanged;
}

void Switcher::markWindowBeingClosed(Window window)
{
    // Add the window to the windows being closed list
    if (!windowsBeingClosed.contains(window)) {
        windowsBeingClosed.insert(window);
    }
}

void Switcher::markWindowTransientFor(Window window, Window transientFor)
{
    transientMap[transientFor].append(window);
}

void Switcher::unmarkWindowTransientFor(Window window, Window transientFor)
{
    transientMap[transientFor].removeOne(window);
    if (transientMap[transientFor].isEmpty()) {
        // The window has no more transient windows for it so remove the list completely
        transientMap.remove(transientFor);
    }
}

bool Switcher::isRelevantWindow(Window window)
{
    // Only windows that are bigger than 0x0, are Input/Output windows and are not unmapped are interesting
    XWindowAttributes attributes;
    Status result = X11Wrapper::XGetWindowAttributes(QX11Info::display(), window, &attributes);

    return result != 0 &&
           attributes.width > 0 && attributes.height > 0 &&
           attributes.c_class == InputOutput && attributes.map_state != IsUnmapped;
}

bool Switcher::isApplicationWindow(const WindowInfo &windowInfo)
{
    QSet<Atom> windowAtomSet;
    windowAtomSet += windowInfo.types().toSet();
    windowAtomSet += windowInfo.states().toSet();
    return windowAtomSet.intersect(excludeAtoms).isEmpty() && windowInfo.transientFor() == 0;
}

void Switcher::scheduleUpdateButtons()
{
    if (!updateButtonsTimer.isActive()) {
        updateButtonsTimer.start();
    }
}

void Switcher::updateWindowInfoMap()
{
    // Get a list of all windows
    Display *display = QX11Info::display();
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;
    Status result = X11Wrapper::XGetWindowProperty(display, DefaultRootWindow(display), stackedClientListAtom,
                                                0, 0x7fffffff, False, XA_WINDOW,
                                                &actualType, &actualFormat, &numWindowItems, &bytesLeft, &windowData);

    if (result == Success && windowData != None) {
        // We need to keep the stacking order of the windows -> hence the list
        QList<Window> newWindowList;
        Window *windows = (Window *)windowData;
        for (unsigned int i = 0; i < numWindowItems; i++) {
            if (isRelevantWindow(windows[i])) {
                newWindowList.append(windows[i]);
            }
        }

        // Check whether windows have been opened or closed
        QSet<Window> newWindowSet = newWindowList.toSet() - windowsBeingClosed;
        QSet<Window> oldWindowSet = windowInfoMap.keys().toSet();
        QSet<Window> closedWindowSet = oldWindowSet - newWindowSet;
        QSet<Window> openedWindowSet = newWindowSet - oldWindowSet;
        windowsBeingClosed -= closedWindowSet;

        bool added = addWindows(openedWindowSet);
        bool removed = removeWindows(closedWindowSet);

        // The stacking order needs to be cleared out before we start updating the
        // buttons as the topmostWindow is set in the 'updateButtons()' method and
        // the method call is scheduled with a timer in the case of an addition
        QList<WindowInfo> stackingWindowList;
        foreach (Window window, newWindowList) {
            if (!windowsBeingClosed.contains(window)) {
                stackingWindowList.append(*windowInfoMap.value(window));
            }
        }

        if (!stackingWindowList.isEmpty()){
            topmostWindow = stackingWindowList.last().window();
        }

        if (added || removed) {
            if (!removed) {
                // If windows have been added but not removed, update the switcher with a delay
                scheduleUpdateButtons();
            } else {
                // If windows have been removed update the switcher instantly
                updateButtons();
            }
        } else if (!stackingWindowList.isEmpty()) {
            if (!windowMonitor->isOwnWindow(topmostWindow)) {
                // The view might also need to react (== pan to the correct page) if no buttons were added
                // but the stacking order was changed, i.e. due to app chaining or some other activity
                model()->setTopmostWindow(topmostWindow);
            }
        }

        emit windowStackingOrderChanged(stackingWindowList);
    }
}

void Switcher::updateWindowTitle(Window window)
{
    WindowInfo *windowInfo = windowInfoMap.value(window);
    if (windowInfo != NULL && windowInfo->updateWindowTitle() && switcherButtonMap.contains(window)) {
        switcherButtonMap.value(window)->setText(windowInfo->title());
        switcherButtonMap.value(window)->update();
    }
}

void Switcher::updateWindowProperties(Window window)
{
    WindowInfo *windowInfo = windowInfoMap.value(window);
    if (windowInfo != NULL) {
        // Get the old properties
        bool wasApplication = isApplicationWindow(*windowInfo);
        Window wasTransientFor = windowInfo->transientFor();

        // Update the properties
        windowInfo->updateWindowProperties();

        // Get the current properties
        bool isApplication = isApplicationWindow(*windowInfo);
        Window isTransientFor = windowInfo->transientFor();

        if (wasApplication != isApplication) {
            // Update the window list if the window has changed from an application window to something else or vice versa
            if (isApplication) {
                applicationWindows.append(*windowInfo);
            } else {
                applicationWindows.removeOne(*windowInfo);
            }
        }

        if (wasTransientFor != isTransientFor) {
            if (wasTransientFor != 0) {
                // Remove this window from the list of transient windows for the previous transient for window
                unmarkWindowTransientFor(windowInfo->window(), wasTransientFor);
            }

            if (isTransientFor != 0) {
                // Add this window as the transient window for another window
                markWindowTransientFor(windowInfo->window(), isTransientFor);
            }
        }

        if (wasApplication != isApplication || wasTransientFor != isTransientFor) {
            // Update the buttons if the properties have changed
            updateButtons();
        }
    }
}

void Switcher::updateButtons()
{
    // List of existing buttons for which a window still exists
    QList<SwitcherButton *> validOldButtons;

    // List of newly created buttons
    QList< QSharedPointer<SwitcherButton> > newButtons;

    // The new mapping of known windows to the buttons
    QMap<Window, SwitcherButton *> newSwitcherButtonMap;

    // Go through the windows and create new buttons for new windows
    foreach (const WindowInfo &windowInfo, applicationWindows) {
        WindowInfo *topmostWindowInfo = windowInfoMap.value(topmostTransientWindowFor(windowInfo.window()));
        if (topmostWindowInfo != NULL) {
            if (switcherButtonMap.contains(windowInfo.window())) {
                SwitcherButton *button = switcherButtonMap[windowInfo.window()];

                // Button already exists - set title (as it may have changed)
                button->setText(topmostWindowInfo->title());

                // Change the window id if replaced by a transient
                if (button->xWindow() != topmostWindowInfo->window()) {
                    button->setXWindow(topmostWindowInfo->window());
                }

                validOldButtons.append(button);
                newSwitcherButtonMap.insert(windowInfo.window(), button);
            } else {
                QSharedPointer<SwitcherButton> button(new SwitcherButton);
                button->setText(topmostWindowInfo->title());
                button->setXWindow(topmostWindowInfo->window());
                connect(button.data(), SIGNAL(windowToFront(Window)), this, SLOT(windowToFront(Window)));
                connect(button.data(), SIGNAL(closeWindow(Window)), this, SLOT(closeWindow(Window)));

                newButtons.append(button);
                newSwitcherButtonMap.insert(windowInfo.window(), button.data());
            }
        }
    }

    int firstNewButtonIndex = 0;
    foreach(QSharedPointer<SwitcherButton> button, model()->buttons()) {
        // Keep only the buttons for which a window still exists
        if (validOldButtons.contains(button.data())) {
            newButtons.insert(firstNewButtonIndex++, button);
        }
    }

     // Take the new set of buttons into use
    switcherButtonMap = newSwitcherButtonMap;
    model()->setButtons(newButtons);

    // Let interested parties know about the updated window list
    emit windowListUpdated(applicationWindows);

    model()->setTopmostWindow(topmostWindow);
}

void Switcher::windowToFront(Window window)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = activeWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1;
    ev.xclient.data.l[1]    = CurrentTime;
    X11Wrapper::XSendEvent(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), False, StructureNotifyMask, &ev);
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

    // Close also the window this one is transient for, if any
    WindowInfo *windowInfo = windowInfoMap.value(window);
    if (windowInfo != NULL && windowInfo->transientFor() != 0 && windowInfo->transientFor() != window) {
        closeWindow(windowInfo->transientFor());
    }
}

Window Switcher::topmostTransientWindowFor(Window window)
{
    if (transientMap.contains(window) && !transientMap[window].isEmpty()) {
        return topmostTransientWindowFor(transientMap[window].last());
    } else {
        return window;
    }
}
