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
#define TEST_NET_WM_ICON_GEOMETRY_ATOM 303

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

void QGraphicsItem::prepareGeometryChange()
{
    Ut_SwitcherButton::prepareGeometryChangeCalled = true;
}

Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    return strcmp(atom_name, "_NET_WM_ICON_GEOMETRY") == 0 ? TEST_NET_WM_ICON_GEOMETRY_ATOM : TEST_ANY_OTHER_ATOM;
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
    button = new SwitcherButton("Test");
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
    button->model()->setXWindow(1);
    QCOMPARE(button->xWindow(), (Window)1);
}

void Ut_SwitcherButton::testSwitchToWindow()
{
    button->model()->setXWindow(2);
    button->switchToWindow();
    QCOMPARE(signalReceiver.switchedWindow, (Window)2);
}

void Ut_SwitcherButton::testClose()
{
    button->model()->setXWindow(3);
    button->close();
    QCOMPARE(signalReceiver.closedWindow, (Window)3);
}

void Ut_SwitcherButton::testPrepareGeometryChange()
{
    button->prepareGeometryChange();
    QVERIFY(prepareGeometryChangeCalled);
}

void Ut_SwitcherButton::testSetGeometry()
{
    // Verify that the icon position for the window is set properly when the switcher button is moved
    QGraphicsScene scene;
    scene.addItem(button);
    button->model()->setXWindow(3);
    button->setGeometry(QRectF(25, 25, 50, 50));

    // Get the expected icon position
    QRectF iconPosition = button->boundingRect();
    iconPosition.moveTo(button->mapToScene(0, 0));

    // Update the icon geometry
    scene.removeItem(button);

    // XChangeProperty should be called for the window of the button and _NET_WM_ICON_GEOMETRY property should be filled with 4 32-bit values which should contain the icon geometry
    QCOMPARE(Ut_SwitcherButton::xChangePropertyWindow, button->xWindow());
    QCOMPARE(Ut_SwitcherButton::xChangePropertyProperty, (Atom)TEST_NET_WM_ICON_GEOMETRY_ATOM);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyFormat, 32);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyNElements, 4);
    unsigned int *iconGeometry = (unsigned int *)xChangePropertyData;
    QCOMPARE(iconGeometry[0], (unsigned int)iconPosition.x());
    QCOMPARE(iconGeometry[1], (unsigned int)iconPosition.y());
    QCOMPARE(iconGeometry[2], (unsigned int)iconPosition.width());
    QCOMPARE(iconGeometry[3], (unsigned int)iconPosition.height());
}

void Ut_SwitcherButton::testUpdateIconGeometry()
{
    // Verify that the icon position for the window is set properly when the switcher button is moved
    QGraphicsScene scene;
    scene.addItem(button);
    button->model()->setXWindow(3);
    button->setGeometry(QRectF(25, 25, 50, 50));

    // Get the expected icon position
    QRectF iconPosition = button->boundingRect();
    iconPosition.moveTo(button->mapToScene(0, 0));

    // XChangeProperty should be called for the window of the button and _NET_WM_ICON_GEOMETRY property should be filled with 4 32-bit values which should contain the icon geometry
    QCOMPARE(Ut_SwitcherButton::xChangePropertyWindow, button->xWindow());
    QCOMPARE(Ut_SwitcherButton::xChangePropertyProperty, (Atom)TEST_NET_WM_ICON_GEOMETRY_ATOM);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyFormat, 32);
    QCOMPARE(Ut_SwitcherButton::xChangePropertyNElements, 4);
    unsigned int *iconGeometry = (unsigned int *)xChangePropertyData;
    QCOMPARE(iconGeometry[0], (unsigned int)iconPosition.x());
    QCOMPARE(iconGeometry[1], (unsigned int)iconPosition.y());
    QCOMPARE(iconGeometry[2], (unsigned int)iconPosition.width());
    QCOMPARE(iconGeometry[3], (unsigned int)iconPosition.height());

    scene.removeItem(button);
}

QTEST_APPLESS_MAIN(Ut_SwitcherButton)
