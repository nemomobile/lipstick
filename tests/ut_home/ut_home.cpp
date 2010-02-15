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
#include "launcher_stub.h"
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

// DuiSceneManager stubs
void DuiSceneManager::showWindow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy)
{
    window->show();
}

void DuiSceneManager::hideWindow(DuiSceneWindow *window)
{
    window->hide();
}

// QGraphicsItem stubs (used by DuiSceneManager)
void QGraphicsItem::setZValue(qreal z)
{
    Q_UNUSED(z);
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

// DuiButton stubs
void DuiButton::setIconID(const QString &iconID)
{
    if (objectName() == "ShowLauncherButton") {
        Ut_Home::launcherButtonWindow = dynamic_cast<DuiSceneWindow *>(parentItem());
        Ut_Home::launcherButtonIconID = iconID;
    }
}

QString DuiButton::iconID() const
{
    return Ut_Home::launcherButtonIconID;
}


// Tests
DuiSceneWindow *Ut_Home::launcherButtonWindow = NULL;
QString Ut_Home::launcherButtonIconID;
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
    launcherButtonWindow = NULL;
    launcherButtonIconID = QString();
    timeLineStarted = false;
    desktopLauncherIconID = "";

    statusArea = new StatusArea;
    statusArea->setGeometry(QRectF(0, 0, 200, 20));
    gStatusAreaWindowStub->stubSetReturnValue("statusArea", statusArea);

    home = new Home();
    home->setGeometry(QRectF(0, 0, 864, 480));
    connect(this, SIGNAL(launcherButtonClicked()), home, SLOT(launcherButtonClicked()));
    connect(this, SIGNAL(hideLauncherButton(bool)), home, SLOT(hideLauncherButton(bool)));

    notificationAreaWidget = gNotificationAreaStub->stubLastCallTo("notificationAreaConstructor").parameter<NotificationArea *>(0);
}

void Ut_Home::cleanup()
{
    delete home;
    delete statusArea;
}

void Ut_Home::testSetGeometry()
{
    home->launcherViewport->minimumSize();

    QRectF rect(0, 0, 200, 100);
    home->setGeometry(rect);

    // Test that the launcher viewport size is adjusted to be the same as home's geometry
    QCOMPARE(home->launcherViewport->minimumSize(), rect.size());
    QCOMPARE(home->launcherViewport->maximumSize(), rect.size());
}

void Ut_Home::testBoundingRect()
{
    QCOMPARE(home->boundingRect(), home->desktop->boundingRect());
}

void Ut_Home::testShowingHidingLauncher()
{
    QCOMPARE(home->launcherWindow->isVisible(), false);

    // Show launcher
    emit launcherButtonClicked();
    QCOMPARE(home->launcherWindow->isVisible(), true);
    QCOMPARE(gLauncherStub->stubLastCallTo("setEnabled").parameter<bool>(0), true);
    QCOMPARE(home->launcherButton->iconID(), QString("Icon-home"));

    // Hide launcher
    emit launcherButtonClicked();
    QCOMPARE(home->launcherWindow->isVisible(), false);
    QCOMPARE(gLauncherStub->stubLastCallTo("setEnabled").parameter<bool>(0), false);
    QCOMPARE(home->launcherButton->iconID(), QString("Icon-menu"));
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
    // and the launcher button window should be invisible
    launcherButtonIconID = "foo";
    QVERIFY(launcherButtonWindow->isVisible());
    home->setNotificationAreaVisible(true);
    QVERIFY(home->notificationAreaVisible);
    QCOMPARE(gHomeNotificationSinkStub->stubCallCount("setNotificationAreaVisible"), 1);
    QCOMPARE(gHomeNotificationSinkStub->stubLastCallTo("setNotificationAreaVisible").parameter<bool>(0), true);
    QVERIFY(!launcherButtonWindow->isVisible());
    QCOMPARE(desktopLauncherIconID, launcherButtonIconID);

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

void Ut_Home::testHideLauncherButton()
{
    // Check that the launcher button hiding slot hides and shows the launcher button
    QVERIFY(launcherButtonWindow != NULL);
    emit hideLauncherButton(true);
    QVERIFY(!launcherButtonWindow->isVisible());
    emit hideLauncherButton(false);
    QVERIFY(launcherButtonWindow->isVisible());
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
    QVERIFY(!launcherButtonWindow->isVisible());
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
    QVERIFY(launcherButtonWindow->isVisible());
}

QTEST_APPLESS_MAIN(Ut_Home)
