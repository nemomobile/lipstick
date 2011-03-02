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
#include "ut_switcherbutton.h"
#include "switcherbutton.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include <MScene>
#include <MSceneManager>
#include <cstring>
#include <QSharedPointer>
#include "windowinfo_stub.h"

#define TEST_ANY_OTHER_ATOM 1
#define TEST_MEEGOTOUCH_VISIBLE_IN_SWITCHER_ATOM 304

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// QTimer stubs (used by Ut_SwitcherButton)
void QTimer::start(int)
{
    if (Ut_SwitcherButton::timerImmediateTimeout) {
        emit timeout();
    }

    id = 0;
}

void QTimer::stop()
{
    id = -1;
}

void QGraphicsItem::prepareGeometryChange()
{
    Ut_SwitcherButton::prepareGeometryChangeCalled = true;
}

Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    if (strcmp(atom_name, "_MEEGOTOUCH_VISIBLE_IN_SWITCHER") == 0) {
        return TEST_MEEGOTOUCH_VISIBLE_IN_SWITCHER_ATOM;
    } else {
        return TEST_ANY_OTHER_ATOM;
    }
}

int X11Wrapper::XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements)
{
    Ut_SwitcherButton::xChangePropertyDisplay = display;
    Ut_SwitcherButton::xChangePropertyWindow = w;
    Ut_SwitcherButton::xChangePropertyProperty = property;
    Ut_SwitcherButton::xChangePropertyType = type;
    Ut_SwitcherButton::xChangePropertyFormat = format;
    Ut_SwitcherButton::xChangePropertyMode = mode;
    Ut_SwitcherButton::xChangePropertyNElements = nelements;

    if (data != NULL) {
        memcpy(Ut_SwitcherButton::xChangePropertyData, data, nelements * format / 8);
    }
    return 0;
}

void TestSignalReceiver::windowToFront(Window window)
{
    switchedWindow = window;
}

void TestSignalReceiver::closeWindow(Window window)
{
    closedWindow = window;
}

bool Ut_SwitcherButton::timerImmediateTimeout = false;
bool Ut_SwitcherButton::prepareGeometryChangeCalled = false;
Display *Ut_SwitcherButton::xChangePropertyDisplay;
Window Ut_SwitcherButton::xChangePropertyWindow;
Atom Ut_SwitcherButton::xChangePropertyProperty;
Atom Ut_SwitcherButton::xChangePropertyType;
int Ut_SwitcherButton::xChangePropertyFormat;
int Ut_SwitcherButton::xChangePropertyMode;
unsigned char Ut_SwitcherButton::xChangePropertyData[16];
int Ut_SwitcherButton::xChangePropertyNElements;
MainWindow *Ut_SwitcherButton::mainWindow;

void Ut_SwitcherButton::init()
{
    button = new SwitcherButton;
    button->setText("Test");
    // TODO This really shouldn't be necessary, but apparently libdui needs something
    // as stupid as this since commit 9aa354a239 to get the updateData signal
    // connected. If libdui gets fixed this can be removed.
    QCoreApplication::processEvents();
    // TODO this REALLY REALLY shouldn't be necessary, but apparently there is a
    // bug in Qt 4.6 as of 20090930 that requires this. If Qt gets fixed this
    // can be removed.
    QCoreApplication::processEvents();

    signalReceiver.closedWindow = 0;
    signalReceiver.switchedWindow = 0;
    timerImmediateTimeout = false;
    prepareGeometryChangeCalled = false;
    xChangePropertyDisplay = 0;
    xChangePropertyWindow = 0;
    xChangePropertyProperty = 0;
    xChangePropertyType = 0;
    xChangePropertyFormat = 0;
    xChangePropertyMode = 0;
    xChangePropertyNElements = 0;
    memset(xChangePropertyData, 0, 16);
    connect(button, SIGNAL(windowToFront(Window)), &signalReceiver, SLOT(windowToFront(Window)));
    connect(button, SIGNAL(closeWindow(Window)), &signalReceiver, SLOT(closeWindow(Window)));
}

void Ut_SwitcherButton::cleanup()
{
    delete button;
}

void Ut_SwitcherButton::initTestCase()
{
    qRegisterMetaType<Window>("Window");

    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherbutton";
    app = new HomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);
}

void Ut_SwitcherButton::cleanupTestCase()
{
    delete mainWindow;
    delete app;
}

void Ut_SwitcherButton::testXWindow()
{
    button->setXWindow(1);
    QCOMPARE(button->xWindow(), (Window)1);
}

void Ut_SwitcherButton::testSwitchToWindow()
{
    button->setXWindow(2);
    button->switchToWindow();
    QCOMPARE(signalReceiver.switchedWindow, (Window)2);
}

void Ut_SwitcherButton::testClosingWithTimeout()
{
    timerImmediateTimeout = true;
    button->setXWindow(3);
    button->close();
    QCOMPARE(signalReceiver.closedWindow, (Window)3);

    // The window close timeout occurs immediately: check that after the timeout button is not closed and still visible
    QCOMPARE(button->isVisible(), true);
}

void Ut_SwitcherButton::testClosingWithoutTimeout()
{
    timerImmediateTimeout = false;
    button->setXWindow(3);
    button->close();
    QCOMPARE(signalReceiver.closedWindow, (Window)3);

    // The window close timeout does not occur: check that button is closed and invisible
    QCOMPARE(button->isVisible(), false);
}

void Ut_SwitcherButton::testPrepareGeometryChange()
{
    button->prepareGeometryChange();
    QVERIFY(prepareGeometryChangeCalled);
}

void Ut_SwitcherButton::testSetVisibleInSwitcherProperty_data()
{
    QTest::addColumn<bool>("visibleInSwitcher");
    QTest::addColumn<unsigned char>("propertyData");

    QTest::newRow("Visible") << true << (unsigned char)1;
    QTest::newRow("Not visible") << false << (unsigned char)0;
}

void Ut_SwitcherButton::testSetVisibleInSwitcherProperty()
{
    QFETCH(bool, visibleInSwitcher);
    QFETCH(unsigned char, propertyData);

    // Set window visible in the Switcher
    button->setVisibleInSwitcherProperty(visibleInSwitcher);

    // Check correct values passed to X11Wrapper::XChangeProperty()
    QCOMPARE(Ut_SwitcherButton::xChangePropertyWindow, button->xWindow());
    QCOMPARE(Ut_SwitcherButton::xChangePropertyProperty, (Atom)TEST_MEEGOTOUCH_VISIBLE_IN_SWITCHER_ATOM);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyFormat, 8);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyMode, PropModeReplace);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyType, XA_CARDINAL);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyNElements, 1);

    unsigned char *data = (unsigned char *)xChangePropertyData;
    QCOMPARE(data[0], propertyData);
}

void Ut_SwitcherButton::testSetVisibleInSwitcherPropertyNotUpdatedWhenValueDoesNotChange()
{
    // Set visible and after that set the window to -1
    button->setVisibleInSwitcherProperty(true);
    Ut_SwitcherButton::xChangePropertyWindow = -1;

    button->setVisibleInSwitcherProperty(true);

    // Verify that XChangeProperty wasn't called
    QCOMPARE(Ut_SwitcherButton::xChangePropertyWindow, Window(-1));
}

void Ut_SwitcherButton::testWhenVisibilityPropertyDisabledThenPropertyChangesOnlyWhenEnabledAgain()
{
    // Initialize to invisible
    button->setVisibleInSwitcherProperty(false);

    button->setVisibilityPropertyEnabled(false);
    button->setVisibleInSwitcherProperty(true);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyWindow, Window(0));
    QCOMPARE(bool(Ut_SwitcherButton::xChangePropertyData[0]), false);
    button->setVisibilityPropertyEnabled(true);
    // TODO: this should actually check that setVisibilityPropertyEnabled(true) causes the visibility to be set based on isOnDisplay()
    button->setVisibleInSwitcherProperty(true);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyWindow, button->xWindow());
    QCOMPARE(bool(Ut_SwitcherButton::xChangePropertyData[0]), true);
}

QTEST_APPLESS_MAIN(Ut_SwitcherButton)
