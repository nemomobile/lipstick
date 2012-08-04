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

#include "components/windowmanager.h"
#include "xtools/xwindowmanager.h"

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

    XWindowManager::excludeFromTaskBar(this->effectiveWinId());

    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    setResizeMode(SizeRootObjectToView);

    QGLFormat fmt;
    fmt.setSamples(0);
    fmt.setSampleBuffers(false);

    QGLWidget *glw = new QGLWidget(fmt, this);
    setViewport(glw);

    this->setAutoFillBackground(false);
    this->setAttribute(Qt::WA_OpaquePaintEvent);
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->viewport()->setAutoFillBackground(false);
    this->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    this->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    QObject::connect(this->engine(), SIGNAL(quit()), QApplication::instance(), SLOT(quit()));
    rootContext()->setContextProperty("initialSize", QApplication::desktop()->screenGeometry(this).size());
    rootContext()->setContextProperty("windowManager", new WindowManager(this));

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Don't allow closing the main window
    event->ignore();
}
