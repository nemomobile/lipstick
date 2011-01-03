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

#include "mapplication_stub.h"
#include "mwindow_stub.h"

#include <QtTest/QtTest>

#include "ut_homewindowmonitor.h"
#include "homewindowmonitor.h"
#include "xeventlistener_stub.h"
#include "windowinfo_stub.h"
#include "x11wrapper.h"

const WId OWN_WINDOW_ID = 2002;

const Atom NET_CLIENT_LIST_STACKING = 1001;
const Atom NOT_EXPECTED_ATOM = 999;

const Window NON_ROOT_WINDOW = 2;

const Window APPLICATION_WINDOW_ID_1 = 101;
const Window APPLICATION_WINDOW_ID_2 = 102;

QList<Window> gActiveWindows;
void addWindowInfoToActiveWindows(Window window)
{
    gActiveWindows.append(window);
}


Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    if (strcmp(atom_name, "_NET_CLIENT_LIST_STACKING") == 0) {
        return NET_CLIENT_LIST_STACKING;
    }

    return None;
}

int X11Wrapper::XGetWindowProperty(Display */*dpy*/, Window /*w*/, Atom property, long /*long_offset*/, long /*long_length*/, Bool /*del*/, Atom /*req_type*/, Atom */*actual_type_return*/, int */*actual_format_return*/, unsigned long *nitems_return, unsigned long */*bytes_after_return*/, unsigned char **prop_return)
{
    if (property == NET_CLIENT_LIST_STACKING) {
        *nitems_return = gActiveWindows.count();
        *prop_return = new unsigned char[gActiveWindows.count() * sizeof(Window)];

        Window *windows = (Window *) *prop_return;
        for (int w = 0; w < gActiveWindows.count(); ++w) {
            windows[w] = gActiveWindows[w];
        }
        return Success;
    }

    return BadAtom;
}

WId QWidget::winId() const
{
    return OWN_WINDOW_ID;
}

void WindowStackingOrderListener::stackingOrderChanged(QList<WindowInfo> windowList)
{
    append(windowList);
}

void Ut_HomeWindowMonitor::initTestCase()
{
}

void Ut_HomeWindowMonitor::cleanupTestCase()
{
}

void Ut_HomeWindowMonitor::init()
{
    qRegisterMetaType<QModelIndex>("WindowInfo");

    m_subject = new HomeWindowMonitor();

    gMApplicationStub->stubReset();

    windowList.append(new MWindow);
    gMApplicationStub->stubSetReturnValue<QList<MWindow*> >("windows", windowList);

    gWindowInfoStub->stubReset();

    clientListStackingEvent.type = PropertyNotify;
    clientListStackingEvent.xproperty.atom = NET_CLIENT_LIST_STACKING;
    clientListStackingEvent.xproperty.window = DefaultRootWindow(QX11Info::display());
}

void Ut_HomeWindowMonitor::cleanup()
{
    delete m_subject;

    qDeleteAll(windowList);
    windowList.clear();

    gActiveWindows.clear();
}

void Ut_HomeWindowMonitor::testWindowIdInWindowListIsRecognizedAsOwnWindow()
{
    QCOMPARE(m_subject->isOwnWindow(OWN_WINDOW_ID), true);
}

void Ut_HomeWindowMonitor::testWindowIdNotInWindowListIsNotRecognizedAsOwnWindow()
{
    const WId SOME_WINDOW_ID = 1001;
    QCOMPARE(m_subject->isOwnWindow(SOME_WINDOW_ID), false);
}

void Ut_HomeWindowMonitor::testReceivingNonStackingOrderXEventDoesNotEmitSignal()
{
    WindowStackingOrderListener listener;
    QObject::connect(m_subject, SIGNAL(windowStackingOrderChanged(QList<WindowInfo>)), &listener, SLOT(stackingOrderChanged(QList<WindowInfo>)));

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);
    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_2);

    XEvent event;
    event.type = CreateNotify;

    QCOMPARE(m_subject->handleXEvent(event), false);
    QCOMPARE(listener.count(), 0);

    event.type = PropertyNotify;
    event.xproperty.atom = NOT_EXPECTED_ATOM;

    QCOMPARE(m_subject->handleXEvent(event), false);
    QCOMPARE(listener.count(), 0);

    event.xproperty.atom = NET_CLIENT_LIST_STACKING;
    event.xproperty.window = NON_ROOT_WINDOW;
    QCOMPARE(m_subject->handleXEvent(event), false);
    QCOMPARE(listener.count(), 0);
}

void Ut_HomeWindowMonitor::testReceivingStackingOrderXEventEmitsStackingOrderSignal()
{
    WindowStackingOrderListener listener;
    QObject::connect(m_subject, SIGNAL(windowStackingOrderChanged(QList<WindowInfo>)), &listener, SLOT(stackingOrderChanged(QList<WindowInfo>)));

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);
    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_2);

    QCOMPARE(m_subject->handleXEvent(clientListStackingEvent), true);
    QCOMPARE(listener.count(), 1);
    QList<WindowInfo> signaledStackingOrder = listener.at(0);
    QCOMPARE(signaledStackingOrder.count(), 2);
    QCOMPARE(gWindowInfoStub->stubCallCount("WindowInfoConstructor(Window)"), 2);
    QCOMPARE(gWindowInfoStub->stubCallsTo("WindowInfoConstructor(Window)").at(0)->parameter<Window>(0), APPLICATION_WINDOW_ID_1);
    QCOMPARE(gWindowInfoStub->stubCallsTo("WindowInfoConstructor(Window)").at(1)->parameter<Window>(0), APPLICATION_WINDOW_ID_2);
}

void Ut_HomeWindowMonitor::testReceivingStackingOrderXEventEmitsFullscreenWindowOnTopSignal()
{
    QSignalSpy listener(m_subject, SIGNAL(fullscreenWindowOnTopOfOwnWindow()));

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);
    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_2);

    QCOMPARE(m_subject->handleXEvent(clientListStackingEvent), true);
    QCOMPARE(listener.count(), 1);
}

void Ut_HomeWindowMonitor::testReceivingStackingOrderXEventEmitsAnyWindowOnTopSignal()
{
    QSignalSpy listener(m_subject, SIGNAL(anyWindowOnTopOfOwnWindow(WindowInfo)));

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);
    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_2);

    QCOMPARE(m_subject->handleXEvent(clientListStackingEvent), true);
    QCOMPARE(listener.count(), 1);
}

void Ut_HomeWindowMonitor::testNonFullscreenWindowOnTopDoesNotEmitFullscreenWindowOnTopSignal_data()
{
    QTest::addColumn<Atom>("windowType");

    QTest::newRow("Notification") << WindowInfo::NotificationAtom;
    QTest::newRow("Dialog") << WindowInfo::DialogAtom;
    QTest::newRow("Menu") << WindowInfo::MenuAtom;
}

void Ut_HomeWindowMonitor::testNonFullscreenWindowOnTopDoesNotEmitFullscreenWindowOnTopSignal()
{
    QSignalSpy listener(m_subject, SIGNAL(fullscreenWindowOnTopOfOwnWindow()));

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);

    QFETCH(Atom, windowType);

    QList<Atom> types;
    types << windowType;
    gWindowInfoStub->stubSetReturnValue("types", types);

    QCOMPARE(m_subject->handleXEvent(clientListStackingEvent), true);
    QCOMPARE(listener.count(), 0);
}

void Ut_HomeWindowMonitor::testNonFullscreenWindowOnTopEmitsAnyWindowOnTopSignal_data()
{
    QTest::addColumn<Atom>("windowType");

    QTest::newRow("Notification") << WindowInfo::NotificationAtom;
    QTest::newRow("Dialog") << WindowInfo::DialogAtom;
    QTest::newRow("Menu") << WindowInfo::MenuAtom;
}

void Ut_HomeWindowMonitor::testNonFullscreenWindowOnTopEmitsAnyWindowOnTopSignal()
{
    QSignalSpy listener(m_subject, SIGNAL(anyWindowOnTopOfOwnWindow(WindowInfo)));

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);

    QFETCH(Atom, windowType);

    QList<Atom> types;
    types << windowType;
    gWindowInfoStub->stubSetReturnValue("types", types);

    QCOMPARE(m_subject->handleXEvent(clientListStackingEvent), true);
    QCOMPARE(listener.count(), 1);
}

void Ut_HomeWindowMonitor::testOwnWindowOnTopDoesNotEmitFullscreenWindowOnTopSignal()
{
    QSignalSpy listener(m_subject, SIGNAL(fullscreenWindowOnTopOfOwnWindow()));
    gWindowInfoStub->stubSetReturnValue("window", OWN_WINDOW_ID);

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);
    addWindowInfoToActiveWindows(OWN_WINDOW_ID);

    QCOMPARE(m_subject->handleXEvent(clientListStackingEvent), true);
    QCOMPARE(listener.count(), 0);
}

void Ut_HomeWindowMonitor::testOwnWindowOnTopDoesNotEmitAnyWindowOnTopSignal()
{
    QSignalSpy listener(m_subject, SIGNAL(anyWindowOnTopOfOwnWindow(WindowInfo)));
    gWindowInfoStub->stubSetReturnValue("window", OWN_WINDOW_ID);

    addWindowInfoToActiveWindows(APPLICATION_WINDOW_ID_1);
    addWindowInfoToActiveWindows(OWN_WINDOW_ID);

    QCOMPARE(m_subject->handleXEvent(clientListStackingEvent), true);
    QCOMPARE(listener.count(), 0);
}

QTEST_MAIN(Ut_HomeWindowMonitor)
