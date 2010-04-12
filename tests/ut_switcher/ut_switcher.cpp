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
#include <QtTest/QtTest>
#include <MApplication>
#include <MApplicationPage>

#include <QFocusEvent>
#include "ut_switcher.h"
#include "switcher.h"
#include "switcherbutton.h"
#include "switcherview.h"
#include "windowinfo.h"
#include "x11wrapper_stub.h"
#include "mscenemanager_stub.h"
#include "mwindow_stub.h"

QMap<SwitcherButton *, Window> g_windowButtonMap;

// Home stubs
class Home : public MApplicationPage
{
public:
    Home(QGraphicsItem *parent = 0);
};

Home::Home(QGraphicsItem *parent) : MApplicationPage(parent)
{
}


// SwitcherButton stubs (used by Switcher)
SwitcherButton::SwitcherButton(const QString &title, MWidget *parent, Window window, WindowInfo::WindowPriority windowPriority) :
    MButton(title, parent, new SwitcherButtonModel),
    priority(windowPriority)
{
    g_windowButtonMap[this] = window;
}

SwitcherButton::~SwitcherButton()
{
    g_windowButtonMap.remove(this);
}

void SwitcherButton::switchToWindow()
{
}

void SwitcherButton::resetState()
{
}

void SwitcherButton::close()
{
}

void SwitcherButton::prepareGeometryChange()
{
    QGraphicsItem::prepareGeometryChange();
}

void SwitcherButton::setGeometry(const QRectF &rect)
{
    return MButton::setGeometry(rect);
}

void SwitcherButton::updateIconGeometry()
{
    Ut_Switcher::iconGeometryUpdated.append(this);
}

WindowInfo::WindowPriority SwitcherButton::windowPriority() const
{
    return priority;
}

void SwitcherButton::setWindowPriority(WindowInfo::WindowPriority windowPriority)
{
    priority = windowPriority;
}

QList<SwitcherButton *> Ut_Switcher::iconGeometryUpdated;

Window SwitcherButton::xWindow()
{
    return g_windowButtonMap[this];
}

void Ut_Switcher::init()
{
    Ut_Switcher::iconGeometryUpdated.clear();

    // Creating a switcher also creates the switcher view
    switcher = new Switcher;
    switcher->setView(new SwitcherView(switcher));

    // Connect widget add/remove signals
    connect(this, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), switcher, SLOT(windowListUpdated(const QList<WindowInfo> &)));
    connect(this, SIGNAL(sizePosChanged(const QSizeF &, const QRectF &, const QPointF &)), switcher, SLOT(viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)));
}

void Ut_Switcher::cleanup()
{
    // Destroy the switcher (and the associated view)
    delete switcher;
}

void Ut_Switcher::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcher";
    app = new MApplication(argc, &app_name);

    mSceneManager = new MSceneManager(NULL, NULL);
    gMWindowStub->stubSetReturnValue("sceneManager", mSceneManager);
}

void Ut_Switcher::cleanupTestCase()
{
    delete mSceneManager;
    // Destroy the MApplication
    delete app;
}

void Ut_Switcher::testWindowAdding()
{
    // Add three test windows to the window list
    QList<WindowInfo> l = createWindowList(3);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // There should be three items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 3);

    // See that three SwitcherButtons are added to the model with the correct names
    for (int i = 0; i < 3; i++) {
        // The button titles should match the window names (Test0, Test1, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i);
        QCOMPARE(b->text(), title);
    }
}

void Ut_Switcher::testWindowRemoving()
{
    // Add three test windows to the window list
    QList<WindowInfo> l = createWindowList(3);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // Remove the second window
    l.removeAt(1);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // There should be two items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 2);

    // See that there are two SwitcherButtons in the model with the correct names
    for (int i = 0; i < 2; i++) {
        // The button titles should match the window names (Test0, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i == 0 ? 0 : 2);
        QCOMPARE(b->text(), title);
    }
}

void Ut_Switcher::testWindowTitleChange()
{
    // Add three test windows to the window list
    QList<WindowInfo> l = createWindowList(3);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // Change the name of the second window
    QString title("Test3");
    XWindowAttributes a;
    memset(&a, 0, sizeof(XWindowAttributes));
    l[1] = WindowInfo(title, (Window)l[1].window(), a, (Pixmap)0);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // There should be three items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 3);

    // See that three SwitcherButtons are added to the model with the correct names
    for (int i = 0; i < 3; i++) {
        // The button titles should match the window names (Test0, Test3, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i == 1 ? 3 : i);
        QCOMPARE(b->text(), title);
    }
}

void Ut_Switcher::testPanning()
{
    // Add three test windows to the window list
    QList<WindowInfo> l = createWindowList(3);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);
    Ut_Switcher::iconGeometryUpdated.clear();

    // Let the Switcher know about a change in panning
    QSizeF viewportSize;
    QRectF pannedRange;
    QPointF pannedPos;
    emit sizePosChanged(viewportSize, pannedRange, pannedPos);

    // The icon geometry of the three windows should be changed
    QCOMPARE(iconGeometryUpdated.count(), 3);
    for (int i = 0; i < 3; i++) {
        QCOMPARE(l.at(i).window(), iconGeometryUpdated.at(i)->xWindow());
    }
}

QList<WindowInfo> Ut_Switcher::createWindowList(int numWindows)
{
    XWindowAttributes a;
    Pixmap p = 0;
    memset(&a, 0, sizeof(XWindowAttributes));

    QList<WindowInfo> l;
    for (int i = 0; i < numWindows; i++) {
        QString title = QString().sprintf("Test%d", i);
        l.append(WindowInfo(title, (Window)i, a, p));
    }
    return l;
}

void Ut_Switcher::testWindowOrder()
{
    // Add three test windows to the window list
    QList<WindowInfo> l = createWindowList(3);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // There should be three items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 3);

    // See that three SwitcherButtons are added to the model with the correct names
    for (int i = 0; i < 3; i++) {
        // The button titles should match the window names (Test0, Test1, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i);
        QCOMPARE(b->text(), title);
    }

    // Create a new list and shuffle its order
    QList<WindowInfo> sl = createWindowList(3);
    sl.swap(0, 1);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(sl);

    // There should be three items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 3);

    // See that three the SwitcherButtons are still in the same order
    for (int i = 0; i < 3; i++) {
        // The button titles should match the window names (Test0, Test1, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i);
        QCOMPARE(b->text(), title);
        // Check that the window ids are in the same order as in the original list
        QCOMPARE(b->xWindow(), l.at(i).window());
    }
}

void Ut_Switcher::testCallWindowAdding()
{
    // Add three test windows to the window list
    QList<WindowInfo> l = createWindowList(3);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // There should be three items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 3);

    // See that three SwitcherButtons are added to the model with the correct names
    for (int i = 0; i < 3; i++) {
        // The button titles should match the window names (Test0, Test1, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i);
        QCOMPARE(b->text(), title);
    }

    XWindowAttributes a;
    memset(&a, 0, sizeof(XWindowAttributes));
    Window w = 3;
    Pixmap p = 0;
    QString callTitle("Call");

    // Add a new call window
    l.insert(1, WindowInfo(callTitle, w, a, p, WindowInfo::Call));

    emit windowListUpdated(l);

    // There should be four items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 4);

    // The call window should be the first one
    QCOMPARE(switcher->model()->buttons().at(0)->text(), QString("Call"));

    // See that three previous the SwitcherButtons are still in the same order
    for (int i = 1; i < 4; i++) {
        // The button titles should match the window names (Test0, Test1, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i - 1);
        QCOMPARE(b->text(), title);
    }
}

void Ut_Switcher::testCallWindowFromExisistingWindow()
{
    // Add three test windows to the window list
    QList<WindowInfo> l = createWindowList(3);

    // Let the Switcher know about the updated window list
    emit windowListUpdated(l);

    // Take the last window and add it again as a call window
    WindowInfo lwi = l.takeAt(2);
    QString title = lwi.title();
    l.append(WindowInfo(title, lwi.window(), lwi.windowAttributes(), lwi.icon(), WindowInfo::Call));

    emit windowListUpdated(l);

    // There should be three items in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), 3);

    // The last window should now be the first one
    QCOMPARE(switcher->model()->buttons().at(0)->text(), QString("Test2"));

    // See that the rest of the SwitcherButtons are in the right order
    for (int i = 1; i < 3; i++) {
        // The button titles should match the window names (Test0, Test1, Test2)
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QString title = QString().sprintf("Test%d", i - 1);
        QCOMPARE(b->text(), title);
    }
}

QTEST_APPLESS_MAIN(Ut_Switcher)
