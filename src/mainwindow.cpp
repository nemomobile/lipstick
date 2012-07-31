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
#include "homeapplication.h"

#include <QDBusInterface>
#include <QX11Info>
#include <QFile>
#include <QGLWidget>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDesktopWidget>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

MainWindow *MainWindow::mainWindowInstance = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent)
{
    mainWindowInstance = this;
    if (qgetenv("MEEGOHOME_DESKTOP") != "0") {
        // Dont Set the window type to desktop if MEEGOHOME_DESKTOP is set to 0
        setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    }

#ifdef Q_WS_X11
    // Visibility change messages are required to make the appVisible() signal work
    WId window = winId();
    XWindowAttributes attributes;
    Display *display = QX11Info::display();
    XGetWindowAttributes(display, window, &attributes);
    XSelectInput(display, window, attributes.your_event_mask | VisibilityChangeMask);
#endif

    excludeFromTaskBar();

    setResizeMode(SizeRootObjectToView);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setViewport(new QGLWidget);

    QObject::connect(this->engine(), SIGNAL(quit()), QApplication::instance(), SLOT(quit()));
    rootContext()->setContextProperty("initialSize", QApplication::desktop()->screenGeometry(this).size());

    // TODO: disable this for non-debug builds
    if (QFile::exists("main.qml"))
        setSource(QUrl::fromLocalFile("./main.qml"));
    else
        setSource(QUrl("qrc:/qml/main.qml"));
}

MainWindow::~MainWindow()
{
    mainWindowInstance = NULL;
}

MainWindow *MainWindow::instance(bool create)
{
    if (mainWindowInstance == NULL && create) {
        // The static instance variable is set in the constructor
        new MainWindow;
    }

    return mainWindowInstance;
}

void MainWindow::excludeFromTaskBar()
{
    // Tell the window to not to be shown in the switcher
    Atom skipTaskbarAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    changeNetWmState(true, skipTaskbarAtom);

    // Also set the _NET_WM_STATE window property to ensure Home doesn't try to
    // manage this window in case the window manager fails to set the property in time
    Atom netWmStateAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVector<Atom> atoms;
    atoms.append(skipTaskbarAtom);
    XChangeProperty(QX11Info::display(), internalWinId(), netWmStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());
}

void MainWindow::changeNetWmState(bool set, Atom one, Atom two)
{
    XEvent e;
    e.xclient.type = ClientMessage;
    Display *display = QX11Info::display();
    Atom netWmStateAtom = XInternAtom(display, "_NET_WM_STATE", FALSE);
    e.xclient.message_type = netWmStateAtom;
    e.xclient.display = display;
    e.xclient.window = internalWinId();
    e.xclient.format = 32;
    e.xclient.data.l[0] = set ? 1 : 0;
    e.xclient.data.l[1] = one;
    e.xclient.data.l[2] = two;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;
    XSendEvent(display, RootWindow(display, x11Info().screen()), FALSE, (SubstructureNotifyMask | SubstructureRedirectMask), &e);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Don't allow closing the main window
    event->ignore();
}
