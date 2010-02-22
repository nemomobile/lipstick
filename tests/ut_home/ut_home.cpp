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

#include <DuiPannableViewport>
#include <DuiModalSceneWindow>
#include <DuiWidgetView>
#include <DuiButton>
#include <DuiScene>
#include <DuiSceneManager>
#include "ut_home.h"
#include "homeapplication_stub.h"
#include "home.h"
#include "notificationarea_stub.h"
#include "statusarea_stub.h"
#include "statusareawindow_stub.h"
#include "desktop.h"
#include "mainwindow_stub.h"
#include "x11wrapper_stub.h"
#include "homenotificationsink_stub.h"
#include "widgetnotificationsink_stub.h"
#include "notificationsink_stub.h"
#include "applicationcontext.h"

// X stubs to avoid crashes
void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

// QTimeLine stubs
bool timeLineStarted = false;
void QTimeLine::start()
{
    timeLineStarted = true;
}

// Desktop stubs
Desktop *desktopWidget = NULL;

Desktop::Desktop(DuiWidget *parent) : DuiWidgetController(new DesktopModel, parent)
{
    desktopWidget = this;
}

Desktop::~Desktop()
{
}

QString desktopLauncherIconID = "";

void Desktop::setNotificationAreaOpen(bool open, const QString &launcherIconID)
{
    model()->setNotificationAreaOpen(open);
    desktopLauncherIconID = launcherIconID;
}

// Tests
MainWindow *Ut_Home::mainWin = NULL;

void Ut_Home::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_home";
    app = new HomeApplication(argc, &app_name);
    mainWin = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWin);
}

void Ut_Home::cleanupTestCase()
{
    delete mainWin;
    delete app;
}

void Ut_Home::init()
{
    timeLineStarted = false;
    desktopLauncherIconID = "";

    statusArea = new StatusArea;
    statusArea->setGeometry(QRectF(0, 0, 200, 20));
    gStatusAreaWindowStub->stubSetReturnValue("statusArea", statusArea);

    home = new Home();
    home->setGeometry(QRectF(0, 0, 864, 480));

    notificationAreaWidget = gNotificationAreaStub->stubLastCallTo("notificationAreaConstructor").parameter<NotificationArea *>(0);
}

void Ut_Home::cleanup()
{
    delete home;
    delete statusArea;
}

void Ut_Home::testBoundingRect()
{
    QCOMPARE(home->boundingRect(), home->desktop->boundingRect());
}

void Ut_Home::testNotificationAreaVisibility()
{
    // test initial notification area visibility
    QVERIFY(!home->notificationAreaVisible);
    home->notificationCountChanged(0);
    QVERIFY(!home->notificationAreaVisible);
    home->notificationCountChanged(1);
    QVERIFY(!home->notificationAreaVisible);

    // when notification area is visible, notification previews should be disabled
    home->setNotificationAreaVisible(true);
    QVERIFY(home->notificationAreaVisible);
    QCOMPARE(gHomeNotificationSinkStub->stubCallCount("setNotificationAreaVisible"), 1);
    QCOMPARE(gHomeNotificationSinkStub->stubLastCallTo("setNotificationAreaVisible").parameter<bool>(0), true);

    // the situation should be reversed
    home->setNotificationAreaVisible(false);
    QVERIFY(!home->notificationAreaVisible);
    QCOMPARE(gHomeNotificationSinkStub->stubCallCount("setNotificationAreaVisible"), 2);
    QCOMPARE(gHomeNotificationSinkStub->stubLastCallTo("setNotificationAreaVisible").parameter<bool>(0), false);

    // when notification area is visible and notification count becomes 0, it should close
    home->setNotificationAreaVisible(true);
    home->notificationCountChanged(0);
    QVERIFY(!home->notificationAreaVisible);

    // test toggling of the notification area
    home->notificationCountChanged(1);
    home->toggleNotificationArea();
    QVERIFY(home->notificationAreaVisible);
    home->toggleNotificationArea();
    QVERIFY(!home->notificationAreaVisible);
}

void Ut_Home::testNotificationAreaPanning()
{
    home->setGeometry(QRectF(0, 0, 200, 100));

    // if notifications fit into view, the area should not be pannable and viewport is at top
    notificationAreaWidget->setPreferredHeight(100);
    home->setNotificationAreaVisible(true);
    QCOMPARE(home->notificationAreaViewport->position(), QPointF(0, 0));
    QVERIFY(!home->notificationAreaViewport->isEnabled());

    // if notifications don't fit into view, the area should be pannable and viewport is at bottom
    notificationAreaWidget->setPreferredHeight(800);
    home->setNotificationAreaVisible(true);
    QCOMPARE(home->notificationAreaViewport->position(), QPointF(0, home->notificationArea->preferredHeight() - home->notificationAreaViewport->size().height()));
    QVERIFY(home->notificationAreaViewport->isEnabled());

    // the notification area viewport should not grow over the status bar
    home->setNotificationAreaVisible(false);
    home->toggleNotificationArea();
    QCOMPARE(home->notificationAreaViewport->size().height(), home->size().height() - statusArea->boundingRect().height());
}

void Ut_Home::testNotificationAreaTransition()
{
    notificationAreaWidget->setPreferredHeight(100);

    // Test that the notification area will slide down when opened
    home->setNotificationAreaVisible(true);
    QVERIFY(timeLineStarted);
    home->notificationAreaTimeLineValueChanged(0);
    QVERIFY(home->notificationAreaViewport->isVisible());
    QCOMPARE(home->notificationAreaViewport->pos(), QPointF(0, -100));
    QCOMPARE(home->pos(), QPointF(0, 0));
    home->notificationAreaTimeLineValueChanged(0.5);
    QCOMPARE(home->notificationAreaViewport->pos(), QPointF(0, -50));
    QCOMPARE(home->pos(), QPointF(0, 50));
    home->notificationAreaTimeLineValueChanged(1);
    QCOMPARE(home->notificationAreaViewport->pos(), QPointF(0, 0));
    QCOMPARE(home->pos(), QPointF(0, 100));
    home->notificationAreaTimeLineFinished();

    // Test that the notification area will adjust it's size when notification count changes
    timeLineStarted = false;
    notificationAreaWidget->setPreferredHeight(50);
    home->notificationCountChanged(1);
    QVERIFY(timeLineStarted);
    home->notificationAreaTimeLineValueChanged(0);
    QCOMPARE(home->pos(), QPointF(0, 100));
    QCOMPARE(home->notificationAreaViewport->rect().height(), (qreal)100);
    home->notificationAreaTimeLineValueChanged(0.5);
    QCOMPARE(home->pos(), QPointF(0, 75));
    QCOMPARE(home->notificationAreaViewport->rect().height(), (qreal)75);
    home->notificationAreaTimeLineValueChanged(1.0);
    QCOMPARE(home->pos(), QPointF(0, 50));
    QCOMPARE(home->notificationAreaViewport->rect().height(), (qreal)50);
    home->notificationAreaTimeLineFinished();

    // Test that the notification area will slide up when closed
    timeLineStarted = false;
    home->setNotificationAreaVisible(false);
    QVERIFY(timeLineStarted);
    home->notificationAreaTimeLineValueChanged(0);
    QVERIFY(home->notificationAreaViewport->isVisible());
    QVERIFY(desktopWidget->model()->notificationAreaOpen());
    QCOMPARE(home->notificationAreaViewport->pos(), QPointF(0, 0));
    QCOMPARE(home->pos(), QPointF(0, 50));
    home->notificationAreaTimeLineValueChanged(0.5);
    QCOMPARE(home->notificationAreaViewport->pos(), QPointF(0, -25));
    QCOMPARE(home->pos(), QPointF(0, 25));
    home->notificationAreaTimeLineValueChanged(1);
    QCOMPARE(home->notificationAreaViewport->pos(), QPointF(0, -50));
    QCOMPARE(home->pos(), QPointF(0, 0));
    home->notificationAreaTimeLineFinished();
    QVERIFY(!home->notificationAreaViewport->isVisible());
    QVERIFY(!desktopWidget->model()->notificationAreaOpen());
    home->notificationAreaTimeLineFinished();
}

QTEST_APPLESS_MAIN(Ut_Home)
