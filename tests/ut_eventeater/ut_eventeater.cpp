/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QtTest/QtTest>
#include "ut_eventeater.h"
#include "eventeater.h"
#include <QEvent>
#include <QShowEvent>
#include "x11wrapper_modified_stub.h"

static const Window WINDOW_ID = 69;

XEventListener::XEventListener()
{
}

XEventListener::~XEventListener()
{
}

void Ut_EventEater::init()
{
    gX11WrapperStub->stubReset();
    gX11WrapperStub->stubSetReturnValue("XCreateWindow", WINDOW_ID);

    m_subject = new EventEater;
}

void Ut_EventEater::cleanup()
{
    if (m_subject)
        delete m_subject;
}

void Ut_EventEater::initTestCase()
{
}

void Ut_EventEater::cleanupTestCase()
{
}

void Ut_EventEater::testButtonEvents_data()
{
    QTest::addColumn<int>("eventType");
    QTest::addColumn<Window>("winId");
    QTest::addColumn<int>("verifyButtonEventSpyCount");

    QTest::newRow("Button press") << ButtonPress << WINDOW_ID << 1;
    QTest::newRow("Key press") << KeyPress << WINDOW_ID << 1;
    QTest::newRow("Button press for wrong window id") << ButtonPress << (Window)1 << 0;
    QTest::newRow("Key press for wrong window id") << KeyPress << (Window)1 << 0;
}

void Ut_EventEater::testButtonEvents()
{
    QFETCH(int, eventType);
    QFETCH(Window, winId);
    QFETCH(int, verifyButtonEventSpyCount);

    XEvent event;
    event.type = eventType;
    event.xany.window = winId;

    QSignalSpy spy(m_subject, SIGNAL(inputEventReceived()));
    m_subject->handleXEvent(event);

    QCOMPARE(spy.count(), verifyButtonEventSpyCount);
}

void Ut_EventEater::testWindowIsCreatedProperly()
{
    Display *dpy = QX11Info::display();
    int scr = DefaultScreen(dpy);
    unsigned int width = DisplayWidth(dpy, scr);
    unsigned int height = DisplayHeight(dpy, scr);

    QCOMPARE(gX11WrapperStub->stubCallCount("XCreateWindow"), 1);
    MethodCall &xcw = gX11WrapperStub->stubLastCallTo("XCreateWindow");
    QCOMPARE(xcw.parameter<int>(2), 0);
    QCOMPARE(xcw.parameter<int>(3), 0);
    QCOMPARE(xcw.parameter<unsigned int>(4), width);
    QCOMPARE(xcw.parameter<unsigned int>(5), height);
    QCOMPARE(xcw.parameter<unsigned int>(8), (unsigned int)InputOnly);
    QCOMPARE(xcw.parameter<unsigned long>(10), (unsigned long)CWOverrideRedirect);
    QCOMPARE(xcw.parameter<XSetWindowAttributes>(11).override_redirect, (Bool)True);

    QCOMPARE(gX11WrapperStub->stubCallCount("XSelectInput"), 1);
    MethodCall &xsi = gX11WrapperStub->stubLastCallTo("XSelectInput");
    QCOMPARE(xsi.parameter<Window>(1), WINDOW_ID);
    QCOMPARE(xsi.parameter<long>(2), (long)(ButtonPressMask|KeyPressMask));

    QCOMPARE(gX11WrapperStub->stubCallCount("XStoreName"), 1);
    MethodCall &xsn = gX11WrapperStub->stubLastCallTo("XStoreName");
    QCOMPARE(xsn.parameter<Window>(1), WINDOW_ID);
    QCOMPARE(QString(xsn.parameter<char*>(2)), QString("EventEater"));
}

void Ut_EventEater::testStackingLayerPropertyIsSet()
{
    QCOMPARE(gX11WrapperStub->stubCallCount("XChangeProperty"), 1);
    MethodCall &xcp = gX11WrapperStub->stubLastCallTo("XChangeProperty");

    QCOMPARE(xcp.parameter<Window>(1), WINDOW_ID);
    QCOMPARE(xcp.parameter<Atom>(2), X11Wrapper::XInternAtom(0, "_MEEGO_STACKING_LAYER", False));
    QCOMPARE(xcp.parameter<Atom>(3), XA_CARDINAL);
    QCOMPARE(xcp.parameter<int>(4), 32);
    QCOMPARE(xcp.parameter<int>(5), PropModeReplace);
    QCOMPARE(*reinterpret_cast<long*>(xcp.parameter<QByteArray>(6).data()), (long)6);
    QCOMPARE(xcp.parameter<int>(7), 1);
}

void Ut_EventEater::testWindowIsDestroyed()
{
    delete m_subject;
    m_subject = NULL;

    QCOMPARE(gX11WrapperStub->stubCallCount("XDestroyWindow"), 1);
    MethodCall &xdw = gX11WrapperStub->stubLastCallTo("XDestroyWindow");
    QCOMPARE(xdw.parameter<Window>(1), WINDOW_ID);
}

void Ut_EventEater::testShow()
{
    m_subject->show();

    QCOMPARE(gX11WrapperStub->stubCallCount("XMapRaised"), 1);
    MethodCall &xmr = gX11WrapperStub->stubLastCallTo("XMapRaised");
    QCOMPARE(xmr.parameter<Window>(1), WINDOW_ID);

    QCOMPARE(gX11WrapperStub->stubCallCount("XGrabKeyboard"), 1);
    MethodCall &xgk = gX11WrapperStub->stubLastCallTo("XGrabKeyboard");
    QCOMPARE(xgk.parameter<Window>(1), WINDOW_ID);
    QCOMPARE(xgk.parameter<Bool>(2), False);
    QCOMPARE(xgk.parameter<int>(3), (int)GrabModeAsync);
    QCOMPARE(xgk.parameter<int>(4), (int)GrabModeAsync);
    QCOMPARE(xgk.parameter<Time>(5), (Time)CurrentTime);

    QCOMPARE(gX11WrapperStub->stubCallCount("XGrabPointer"), 1);
    MethodCall &xgp = gX11WrapperStub->stubLastCallTo("XGrabPointer");
    QCOMPARE(xgp.parameter<Window>(1), WINDOW_ID);
    QCOMPARE(xgp.parameter<Bool>(2), False);
    QCOMPARE(xgp.parameter<int>(3), (int)ButtonPressMask);
    QCOMPARE(xgp.parameter<int>(4), (int)GrabModeAsync);
    QCOMPARE(xgp.parameter<int>(5), (int)GrabModeAsync);
    QCOMPARE(xgp.parameter<Window>(6), (Window)None);
    QCOMPARE(xgp.parameter<Cursor>(7), (Cursor)None);
    QCOMPARE(xgp.parameter<Time>(8), (Time)CurrentTime);

    int xmr_i = 0, xgk_i = 0;

    QList<MethodCall*> x11Calls = gX11WrapperStub->stubCallHistory();

    for (int i=0;i<x11Calls.count();i++) {
        if (x11Calls.at(i)->name() == "XMapRaised")
            xmr_i = i;

        if (x11Calls.at(i)->name() == "XGrabKeyboard")
            xgk_i = i;
    }

    // The window must have been mapped before the grab
    QVERIFY(xmr_i < xgk_i);
}

void Ut_EventEater::testHide()
{
    m_subject->hide();

    QCOMPARE(gX11WrapperStub->stubCallCount("XUnmapWindow"), 1);
    MethodCall &xuw = gX11WrapperStub->stubLastCallTo("XUnmapWindow");
    QCOMPARE(xuw.parameter<Window>(1), WINDOW_ID);
}

QTEST_MAIN(Ut_EventEater)
