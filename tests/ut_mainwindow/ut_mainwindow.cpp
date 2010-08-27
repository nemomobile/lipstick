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

#include "ut_mainwindow.h"
#include "x11wrapper_stub.h"
#include "mainwindow.h"
#include <MApplication>
#include <MApplicationPage>
#include <QDBusInterface>

// Stub the Dbus call used by status bar to open a menu
bool dBusCallMade = false;
QString dBusService = QString();
QString dBusPath = QString();
QString dBusInterface = QString();
QDBus::CallMode callMode = QDBus::Block;
QString dBusMethod = QString();
QList<QVariant> dBusArguments = QList<QVariant>();
QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode callmode, const QString &method,
        const QVariant &arg1,
        const QVariant &arg2,
        const QVariant &arg3,
        const QVariant &arg4,
        const QVariant &arg5,
        const QVariant &arg6,
        const QVariant &arg7,
        const QVariant &arg8)
{
    dBusCallMade = true;
    dBusInterface = this->interface();
    dBusService = this->service();
    dBusPath = this->path();
    callMode = callmode;
    dBusMethod = method;
    dBusArguments << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8;
    QDBusMessage message;
    return message;
}

void resetDBusStub()
{
    dBusCallMade = false;
    dBusService = QString();
    dBusPath = QString();
    dBusInterface = QString();
    callMode = QDBus::Block;
    dBusMethod = QString();
    dBusArguments = QList<QVariant>();
}

// Home stubs
class Home : public MApplicationPage
{
public:
    Home(QGraphicsItem *parent = 0);
};

Home::Home(QGraphicsItem *parent) : MApplicationPage(parent)
{
}

void Ut_MainWindow::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mainwindow";
    app = new MApplication(argc, &app_name);

    mainWindow = MainWindow::instance(true);

    // Check that receiving of VisibilityChange events was requested
    QCOMPARE(true, (bool)(gX11WrapperStub->stubLastCallTo("XSelectInput").parameter<long>(2) & VisibilityChangeMask));

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

void Ut_MainWindow::testCallUILaunching()
{
}

#undef KeyPress
void Ut_MainWindow::testContentSearchLaunchedWhenAToZPressed()
{
    for (int key = Qt::Key_A; key <= Qt::Key_Z; key++) {
        resetDBusStub();
        QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier);
        mainWindow->keyPressEvent(&keyEvent);
        QCOMPARE(dBusCallMade, true);
        QCOMPARE(dBusService, MainWindow::CONTENT_SEARCH_DBUS_SERVICE);
        QCOMPARE(dBusPath, MainWindow::CONTENT_SEARCH_DBUS_PATH);
        QCOMPARE(dBusInterface, MainWindow::CONTENT_SEARCH_DBUS_INTERFACE);
        QCOMPARE(dBusMethod, QString("launch"));
        QCOMPARE(dBusArguments.at(0).toString(), keyEvent.text());
    }
}

void addIntRangeToSet(QSet<int> &set, int from, int to)
{
    for (int i = from; i <= to; i++) {
        set.insert(i);
    }
}

void Ut_MainWindow::testNothingLaunchedWhenSomethingElsePressed()
{
    QSet<int> keys;
    addIntRangeToSet(keys, Qt::Key_Space, Qt::Key_Slash);
    addIntRangeToSet(keys, Qt::Key_Colon, Qt::Key_At);
    addIntRangeToSet(keys, Qt::Key_BracketLeft, Qt::Key_ydiaeresis);
    addIntRangeToSet(keys, Qt::Key_Escape, Qt::Key_Dead_Horn);
    addIntRangeToSet(keys, Qt::Key_Context1, Qt::Key_Hangup);

    foreach (int key, keys.values()) {
        resetDBusStub();
        QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier);
        mainWindow->keyPressEvent(&keyEvent);
        QCOMPARE(dBusCallMade, false);
    }
}

QTEST_APPLESS_MAIN(Ut_MainWindow)
