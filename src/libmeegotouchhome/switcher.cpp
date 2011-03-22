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

Switcher::Switcher(MWidget *parent, SwitcherModel *model) :
        MWidgetController(model, parent)
{

    // Get the X11 Atoms for closing and activating a window and for other switcher functionalities
    Display *display = QX11Info::display();
    closeWindowAtom = X11Wrapper::XInternAtom(display, "_NET_CLOSE_WINDOW", False);
    activeWindowAtom = X11Wrapper::XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    clientListAtom = X11Wrapper::XInternAtom(display, "_NET_CLIENT_LIST", False);
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

    connect(HomeWindowMonitor::instance(), SIGNAL(windowStackingOrderChanged(QList<WindowInfo>)),
            this, SLOT(handleWindowInfoList(QList<WindowInfo>)));


    // This stuff is necessary to receive touch events
    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);
}

Switcher::~Switcher()
{
    // Close all applications when the home screen shuts down
    foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
        closeWindow(button->xWindow());
    }
}

bool Switcher::handleXEvent(const XEvent &event)
{
    bool eventWasHandled = false;

    if (event.type == PropertyNotify) {
        if (event.xproperty.atom == WindowInfo::TypeAtom || event.xproperty.atom == WindowInfo::StateAtom || event.xproperty.atom == XA_WM_TRANSIENT_FOR) {
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

bool Switcher::addWindows(const QSet<WindowInfo> &windows)
{
    bool applicationWindowListChanged = false;
    foreach(const WindowInfo &wi, windows) {
        applicationWindowListChanged |= addWindowInfo(wi);
    }
    return applicationWindowListChanged;
}

bool Switcher::addWindowInfo(const WindowInfo &wi)
{
    bool applicationWindowListChanged = false;
    if (!windowInfoSet.contains(wi)) {
        windowInfoSet.insert(wi);
        if (wi.transientFor() != 0) {
            markWindowTransientFor(wi.window(), wi.transientFor());
            applicationWindowListChanged = true;
        } else if (isApplicationWindow(wi)) {
            // Add the window to the application window list in case it is one
            applicationWindows.append(wi);
            applicationWindowListChanged = true;
        }

        if (!HomeWindowMonitor::instance()->isOwnWindow(wi.window())) {
            // The Switcher needs to know about Visibility and
            // property changes of other applications' windows
            // (but not of the homescreen window).
            X11Wrapper::XSelectInput(QX11Info::display(), wi.window(),
                                     VisibilityChangeMask | PropertyChangeMask);
        }
    }
    return applicationWindowListChanged;
}

bool Switcher::removeWindows(const QSet<WindowInfo> &windows)
{
    bool applicationWindowListChanged = false;
    foreach(const WindowInfo &windowInfo, windows) {
        applicationWindowListChanged |= removeWindow(windowInfo.window());
    }
    return applicationWindowListChanged;
}

bool Switcher::removeWindow(Window window)
{
    bool applicationWindowListChanged = false;
    const WindowInfo *windowInfo = windowInfoFromSet(windowInfoSet, window);
    if (windowInfo != NULL) {
        if (windowInfo->transientFor() != 0) {
            unmarkWindowTransientFor(windowInfo->window(), windowInfo->transientFor());
            applicationWindowListChanged = true;
        } else if (applicationWindows.contains(*windowInfo)) {
            applicationWindows.removeOne(*windowInfo);
            applicationWindowListChanged = true;
        }
        windowsBeingClosed.remove(windowInfo->window());
        windowInfoSet.remove(*windowInfo);
    }

    return applicationWindowListChanged;
}

const WindowInfo *Switcher::windowInfoFromSet(const QSet<WindowInfo> &windowInfos, Window window)
{
    foreach (const WindowInfo &windowInfo, windowInfos) {
        if (windowInfo.window() == window) {
            return &windowInfo;
        }
    }
    return NULL;
}

void Switcher::markWindowBeingClosed(Window window)
{
    // Add the window to the windows being closed list - but only if it is a
    // known window, as only known windows can be removed from the list
    if (windowInfoFromSet(windowInfoSet, window) != NULL) {
        windowsBeingClosed.insert(window);
        // Remove the closed window immediately by updating the buttons
        updateButtons();
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

void Switcher::handleWindowInfoList(QList<WindowInfo> newWindowList)
{
    foreach(WindowInfo wi, newWindowList) {
        if (!isRelevantWindow(wi.window())) {
            newWindowList.removeOne(wi);
        }
    }

    QSet<WindowInfo> newWindowSet = newWindowList.toSet();
    QSet<WindowInfo> oldWindowSet = windowInfoSet;
    QSet<WindowInfo> closedWindowSet = oldWindowSet - newWindowSet;
    QSet<WindowInfo> openedWindowSet = newWindowSet - oldWindowSet;

    bool added = addWindows(openedWindowSet);
    bool removed = removeWindows(closedWindowSet);

    // The stacking order needs to be cleared out before we start updating the
    // buttons as the topmostWindow is set in the 'updateButtons()' method and
    // the method call is scheduled with a timer in the case of an addition
    QList<WindowInfo> stackingWindowList;
    foreach (WindowInfo window, newWindowList) {
        stackingWindowList.append(window);
    }

    if (!stackingWindowList.isEmpty()){
        topmostWindow = stackingWindowList.last().window();
        // Restore a possible window that was being removed but has now come on top
        added |= restoreButtonBeingRemoved(topmostWindow, false);
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
        if (!HomeWindowMonitor::instance()->isOwnWindow(topmostWindow)) {
            // The view might also need to react (== pan to the correct page) if no buttons were added
            // but the stacking order was changed, i.e. due to app chaining or some other activity
            model()->setTopmostWindow(topmostWindow);
        }
    }
}

bool Switcher::sceneEvent(QEvent *event)
{
    bool handled = false;

    if (event->type() == QEvent::TouchEnd) {
        foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
            button->removeSceneEventFilter(this);
        }
        handled = true;
    }

    return handled || MWidgetController::sceneEvent(event);
}

void Switcher::updateWindowTitle(Window window)
{
    if (windowInfoFromSet(windowInfoSet, window) != NULL) {
        WindowInfo windowInfo = WindowInfo(window);
        if (windowInfo.updateWindowTitle() && switcherButtonMap.contains(window)) {
            switcherButtonMap.value(window)->setText(windowInfo.title());
            switcherButtonMap.value(window)->update();
        }
    }
}

void Switcher::updateWindowProperties(Window window)
{
    if (windowInfoFromSet(windowInfoSet, window) != NULL) {
        WindowInfo windowInfo = WindowInfo(window);

        // Get the old properties
        bool wasApplication = isApplicationWindow(windowInfo);
        Window wasTransientFor = windowInfo.transientFor();

        // Update the properties
        windowInfo.updateWindowProperties();

        // Get the current properties
        bool isApplication = isApplicationWindow(windowInfo);
        Window isTransientFor = windowInfo.transientFor();

        if (wasApplication != isApplication) {
            // Update the window list if the window has changed from an application window to something else or vice versa
            if (isApplication) {
                applicationWindows.append(windowInfo);
            } else {
                applicationWindows.removeOne(windowInfo);
            }
        }

        if (wasTransientFor != isTransientFor) {
            if (wasTransientFor != 0) {
                // Remove this window from the list of transient windows for the previous transient for window
                unmarkWindowTransientFor(windowInfo.window(), wasTransientFor);
            }

            if (isTransientFor != 0) {
                // Add this window as the transient window for another window
                markWindowTransientFor(windowInfo.window(), isTransientFor);
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
    QList<QSharedPointer<SwitcherButton> > validOldButtons;

    // List of newly created buttons
    QList< QSharedPointer<SwitcherButton> > newButtons;

    // The new mapping of known windows to the buttons
    QHash<Window, QSharedPointer<SwitcherButton> > newSwitcherButtonMap;

    // Go through the windows and create new buttons for new windows
    foreach (const WindowInfo &windowInfo, applicationWindows) {
        Window w = topmostTransientWindowFor(windowInfo.window());
        WindowInfo topmostWindowInfo = WindowInfo(w);
        if (windowInfoSet.contains(topmostWindowInfo)) {
            if (switcherButtonMap.contains(windowInfo.window())) {
                QSharedPointer<SwitcherButton> button = switcherButtonMap[windowInfo.window()];

                // Button already exists - set title (as it may have changed)
                button->setText(topmostWindowInfo.title());

                // Change the window id if replaced by a transient
                if (button->xWindow() != topmostWindowInfo.window()) {
                    button->setXWindow(topmostWindowInfo.window());
                }
                // Only add to model if button is not being closed
                if (!windowsBeingClosed.contains(windowInfo.window())) {
                    validOldButtons.append(button);
                }
                newSwitcherButtonMap.insert(windowInfo.window(), button);
            } else {
                QSharedPointer<SwitcherButton> button = createSwitcherButton();
                button->setText(topmostWindowInfo.title());
                button->setXWindow(topmostWindowInfo.window());
                connect(button.data(), SIGNAL(windowToFront(Window)), this, SLOT(windowToFront(Window)));
                connect(button.data(), SIGNAL(closeWindow(Window)), this, SLOT(closeWindow(Window)));
                connect(button.data(), SIGNAL(closeAllWindows()), this, SLOT(closeAllWindows()));
                connect(button.data(), SIGNAL(closeTimedOutForWindow(Window)), this, SLOT(restoreButtonBeingRemoved(Window)));

                newButtons.append(button);
                newSwitcherButtonMap.insert(windowInfo.window(), button);
            }
        }
    }

    foreach(QSharedPointer<SwitcherButton> button, model()->buttons()) {
        if (!validOldButtons.contains(button)) {
            // If button that still has a window is removed from switcher (e.g. window acquires a SkipTaskbarAtom)
            // _MEEGOTOUCH_VISIBLE_IN_SWITCHER should be set to 0 for the window.
            button->setVisibleInSwitcherProperty(false);
        }
    }

    newButtons.append(validOldButtons);

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
    WindowInfo windowInfo = WindowInfo(window);
    if (windowInfo.transientFor() != 0 && windowInfo.transientFor() != window) {
        closeWindow(windowInfo.transientFor());
    }

    windowsBeingClosed.insert(window);
    // Remove the closed window immediately by updating the buttons
    updateButtons();
}

void Switcher::closeAllWindows()
{
    foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
        closeWindow(button->xWindow());
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

void Switcher::updateAnimationStatus(bool animating)
{
    foreach(const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
        button->setVisibilityPropertyEnabled(!animating);
    }

    emit animationStateChanged(animating);
}

QSharedPointer<SwitcherButton> Switcher::createSwitcherButton()
{
     return QSharedPointer<SwitcherButton>(new SwitcherButton);
}

bool Switcher::restoreButtonBeingRemoved(Window window, bool forceUpdateButtons)
{
    bool removed = windowsBeingClosed.remove(window);
    if (removed && forceUpdateButtons) {
        updateButtons();
    }
    return removed;
}
