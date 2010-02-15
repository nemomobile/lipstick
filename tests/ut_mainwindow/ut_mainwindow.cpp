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

#include "ut_mainwindow.h"
#include "x11wrapper_stub.h"

#include "mainwindow.h"

#include <DuiApplication>
#include <DuiApplicationPage>


// Home stubs
class Home : public DuiApplicationPage
{
public:
    Home(QGraphicsItem *parent = 0);
};

Home::Home(QGraphicsItem *parent) : DuiApplicationPage(parent)
{
}

void Ut_MainWindow::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mainwindow";
    app = new DuiApplication(argc, &app_name);

    mainWindow = MainWindow::instance(true);

    QCOMPARE(true, (bool)(gX11WrapperStub->stubLastCallTo("XSelectInput").parameter<long>(2) | VisibilityChangeMask));

    Atom netWmStateAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", FALSE);
    Atom skipTaskbarAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    XEvent event = gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<XEvent>(4);

    QCOMPARE(event.xclient.message_type, (unsigned long)netWmStateAtom);
    QCOMPARE(event.xclient.format, (int)32);
    QCOMPARE(event.xclient.data.l[0], (long)1);
    QCOMPARE(event.xclient.data.l[1], (long)skipTaskbarAtom);
    QCOMPARE(event.xclient.data.l[2], (long)0);
    QCOMPARE(event.xclient.data.l[3], (long)0);
    QCOMPARE(event.xclient.data.l[4], (long)0);
}

void Ut_MainWindow::cleanupTestCase()
{
    delete mainWindow;
    delete app;
}


QTEST_APPLESS_MAIN(Ut_MainWindow)
