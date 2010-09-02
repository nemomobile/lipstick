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

#include "mainwindow.h"
#include "home.h"

#include <QGLWidget>
#include <QDBusInterface>
#include <MApplication>
#include <MSceneManager>
#include "x11wrapper.h"

MainWindow *MainWindow::mainWindowInstance = NULL;
QGLContext *MainWindow::openGLContext = NULL;
const QString MainWindow::CONTENT_SEARCH_DBUS_SERVICE = "com.nokia.maemo.meegotouch.ContentSearch";
const QString MainWindow::CONTENT_SEARCH_DBUS_PATH = "/";
const QString MainWindow::CONTENT_SEARCH_DBUS_INTERFACE = "com.nokia.maemo.meegotouch.ContentSearchInterface";

const QString MainWindow::CALL_UI_DBUS_SERVICE = "com.nokia.telephony.callhistory";
const QString MainWindow::CALL_UI_DBUS_PATH = "/callhistory";
const QString MainWindow::CALL_UI_DBUS_INTERFACE = "com.nokia.telephony.callhistory";

MainWindow::MainWindow(QWidget *parent) :
    MWindow(parent)
{
    mainWindowInstance = this;
    if (qgetenv("MEEGOHOME_DESKTOP") != "0") {
        // Dont Set the window type to desktop if MEEGOHOME_DESKTOP is set to 0
        setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    }

    if (!MApplication::softwareRendering()) {
        // Get GL context
        QGLWidget *w = dynamic_cast<QGLWidget *>(viewport());
        if (w != NULL) {
            openGLContext = const_cast<QGLContext *>(w->context());
        }
    }

#ifdef Q_WS_X11
    // Visibility change messages are required to make the appVisible() signal work
    WId window = winId();
    XWindowAttributes attributes;
    Display *display = QX11Info::display();
    X11Wrapper::XGetWindowAttributes(display, window, &attributes);
    X11Wrapper::XSelectInput(display, window, attributes.your_event_mask | VisibilityChangeMask);
#endif

    // Create Home; the scene manager must be created before this
    home = new Home;
    sceneManager()->appearSceneWindowNow(home);

    setBackgroundBrush(Qt::black);

    excludeFromTaskBar();
}

MainWindow::~MainWindow()
{
    mainWindowInstance = NULL;
    openGLContext = NULL;
    delete home;
}

MainWindow *MainWindow::instance(bool create)
{
    if (mainWindowInstance == NULL && create) {
        // The static instance variable is set in the constructor
        new MainWindow;
    }

    return mainWindowInstance;
}

QGLContext *MainWindow::glContext()
{
    return openGLContext;
}

void MainWindow::excludeFromTaskBar()
{
    // Tell the window to not to be shown in the switcher
    Atom skipTaskbarAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    changeNetWmState(true, skipTaskbarAtom);

    // Also set the _NET_WM_STATE window property to ensure Home doesn't try to
    // manage this window in case the window manager fails to set the property in time
    Atom netWmStateAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVector<Atom> atoms;
    atoms.append(skipTaskbarAtom);
    X11Wrapper::XChangeProperty(QX11Info::display(), internalWinId(), netWmStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());
}

void MainWindow::changeNetWmState(bool set, Atom one, Atom two)
{
    XEvent e;
    e.xclient.type = ClientMessage;
    Display *display = QX11Info::display();
    Atom netWmStateAtom = X11Wrapper::XInternAtom(display, "_NET_WM_STATE", FALSE);
    e.xclient.message_type = netWmStateAtom;
    e.xclient.display = display;
    e.xclient.window = internalWinId();
    e.xclient.format = 32;
    e.xclient.data.l[0] = set ? 1 : 0;
    e.xclient.data.l[1] = one;
    e.xclient.data.l[2] = two;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;
    X11Wrapper::XSendEvent(display, RootWindow(display, x11Info().screen()), FALSE, (SubstructureNotifyMask | SubstructureRedirectMask), &e);
    X11Wrapper::XSync(display, FALSE);
}

bool MainWindow::isCallUILaunchingKey(int key)
{
    // Numbers, *, + and # will launch the call UI
    return ((key >= Qt::Key_0 && key <= Qt::Key_9) || key == Qt::Key_Asterisk || key == Qt::Key_Plus || key == Qt::Key_NumberSign);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key < Qt::Key_Escape && !event->modifiers().testFlag(Qt::ControlModifier)) {
        // Special keys and CTRL-anything should do nothing
        QString searchString = event->text();
        if (!searchString.isEmpty()) {
            // Only launch something if the key press produced a string
            if (isCallUILaunchingKey(key)) {
                launchCallUI(searchString);
            } else {
                searchStringToBeSent.append(searchString);
                launchContentSearch();
            }
        }
    }
}

void MainWindow::launchContentSearch()
{
    // Only one content search launch may be active at a time
    if (searchStringBeingSent.isEmpty() && !searchStringToBeSent.isEmpty()) {
        // Make an asynchronous call to the content search and send the search string to be sent
        QDBusInterface interface(CONTENT_SEARCH_DBUS_SERVICE, CONTENT_SEARCH_DBUS_PATH, CONTENT_SEARCH_DBUS_INTERFACE, QDBusConnection::sessionBus());
        interface.callWithCallback("launch", (QList<QVariant>() << searchStringToBeSent), this, SLOT(markSearchStringSentAndSendRemainingSearchString()), SLOT(markSearchStringNotSent()));

        searchStringBeingSent = searchStringToBeSent;
        searchStringToBeSent.clear();
    }
}

void MainWindow::launchCallUI(const QString &digits)
{
   QDBusInterface interface(CALL_UI_DBUS_SERVICE, CALL_UI_DBUS_PATH, CALL_UI_DBUS_INTERFACE, QDBusConnection::sessionBus());
   interface.call(QDBus::NoBlock, "dialer", digits);
}

void MainWindow::markSearchStringSentAndSendRemainingSearchString()
{
    searchStringBeingSent.clear();

    // Send the search string still to be sent (if any)
    launchContentSearch();
}

void MainWindow::markSearchStringNotSent()
{
    // Since content search didn't launch prepend the sent search string to the search string to be sent but don't retry
    searchStringToBeSent.prepend(searchStringBeingSent);
    searchStringBeingSent.clear();
}
