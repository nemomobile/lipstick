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

#include "ut_homenotificationsink.h"

#include <QtTest/QtTest>
#include <DuiRemoteAction>
#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiScene>
#include <DuiSceneManager>
#include <DuiInfoBanner>
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "homenotificationsink.h"
#include "notificationmanager_stub.h"
#include "notificationwidgetparameterfactory.h"
#include "../stubs/testnotificationparameters.h"
#include "x11wrapper_stub.h"
#include "eventtypestore_stub.h"

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

maemosec::storage::~storage()
{
}

// DuiSceneManager stubs
void DuiSceneManager::showWindow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy)
{
    window->show();

    DuiInfoBanner *n = dynamic_cast<DuiInfoBanner *>(window);
    if (n != NULL) {
        Ut_HomeNotificationSink::types.append(n->bannerType());
        Ut_HomeNotificationSink::icons.append(n->model()->imageID());
        Ut_HomeNotificationSink::bodies.append(n->model()->bodyText());
        Ut_HomeNotificationSink::buttonIcons.append(n->model()->iconID());
        Ut_HomeNotificationSink::notifications.append(n);
    }

    // Emit the windowShown() signal for "in animation done"
    emit window->windowShown();

    if (Ut_HomeNotificationSink::instantAnimation) {
        hideWindow(window);
    }
}

void DuiSceneManager::hideWindow(DuiSceneWindow *window)
{
    window->hide();
    DuiInfoBanner *n = dynamic_cast<DuiInfoBanner *>(window);
    if (n != NULL) {
        int index = Ut_HomeNotificationSink::notifications.indexOf(n);

        Ut_HomeNotificationSink::types.removeAt(index);
        Ut_HomeNotificationSink::icons.removeAt(index);
        Ut_HomeNotificationSink::bodies.removeAt(index);
        Ut_HomeNotificationSink::contents.removeAt(index);
        Ut_HomeNotificationSink::actions[window].clear();
        Ut_HomeNotificationSink::buttonIcons.removeAt(index);
        Ut_HomeNotificationSink::notifications.removeAt(index);
    }

    emit window->windowHidden();
}

// DuiWindow stubs
DuiSceneManager *DuiWindow::sceneManager()
{
    return NULL;
}

// QGraphicsItem stubs (used by DuiSceneManager)
void QGraphicsItem::setZValue(qreal z)
{
    Q_UNUSED(z);
}

// QGraphicsWidget stubs (used by HomeNotificationSink)
void QGraphicsWidget::addAction(QAction *action)
{
    Ut_HomeNotificationSink::actions[this].append(action);

    DuiRemoteAction *dra = dynamic_cast<DuiRemoteAction *>(action);
    if (dra != NULL) {
        Ut_HomeNotificationSink::contents.append(dra->toString());
    }
}

void QGraphicsWidget::removeAction(QAction *action)
{
    Ut_HomeNotificationSink::actions[this].removeAll(action);

    DuiRemoteAction *dra = dynamic_cast<DuiRemoteAction *>(action);
    if (dra != NULL) {
        Ut_HomeNotificationSink::contents.removeAll(dra->toString());
    }
}

QList<QAction *> QGraphicsWidget::actions() const
{
    return Ut_HomeNotificationSink::actions[this];
}

// QTimer stubs (used by HomeNotificationSink)
void QTimer::start(int msec)
{
    Ut_HomeNotificationSink::lastTimeout = msec;
    Ut_HomeNotificationSink::lastTimer = this;

    if (msec <= 1) {
        emit timeout();
    }
}

// DuiSceneWindow stubs (used by HomeNotificationSink)
void DuiSceneWindow::appear(DeletionPolicy policy)
{
    Q_UNUSED(policy);

    DuiInfoBanner *n = dynamic_cast<DuiInfoBanner *>(this);
    if (n != NULL) {
        Ut_HomeNotificationSink::types.append(n->bannerType());
        Ut_HomeNotificationSink::icons.append(n->model()->imageID());
        Ut_HomeNotificationSink::bodies.append(n->model()->bodyText());
        Ut_HomeNotificationSink::buttonIcons.append(n->model()->iconID());
        Ut_HomeNotificationSink::notifications.append(n);
    }

    // Emit the windowShown() signal for "in animation done"
    emit this->windowShown();

    if (Ut_HomeNotificationSink::instantAnimation) {
        disappear();
    }
}

void DuiSceneWindow::disappear()
{
    DuiInfoBanner *n = dynamic_cast<DuiInfoBanner *>(this);
    if (n != NULL) {
        int index = Ut_HomeNotificationSink::notifications.indexOf(n);

        Ut_HomeNotificationSink::types.removeAt(index);
        Ut_HomeNotificationSink::icons.removeAt(index);
        Ut_HomeNotificationSink::bodies.removeAt(index);
        Ut_HomeNotificationSink::contents.removeAt(index);
        Ut_HomeNotificationSink::actions[this].clear();
        Ut_HomeNotificationSink::buttonIcons.removeAt(index);
        Ut_HomeNotificationSink::notifications.removeAt(index);
    }

    emit windowHidden();
}

// DuiInfoBanner stubs (used by DuiCompositorNotificationSink)
void DuiInfoBanner::setImageID(const QString &image)
{
    int index = Ut_HomeNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_HomeNotificationSink::icons.replace(index, image);
    }
    model()->setImageID(image);
}

void DuiInfoBanner::setBodyText(const QString &body)
{
    int index = Ut_HomeNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_HomeNotificationSink::bodies.replace(index, body);
    }
    model()->setBodyText(body);
}

void DuiInfoBanner::setIconID(const QString &iconId)
{
    int index = Ut_HomeNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_HomeNotificationSink::buttonIcons.replace(index, iconId);
    }
    model()->setIconID(iconId);
}

QList<DuiInfoBanner::BannerType> Ut_HomeNotificationSink::types;
QList<QString> Ut_HomeNotificationSink::icons;
QList<QString> Ut_HomeNotificationSink::bodies;
QList<QString> Ut_HomeNotificationSink::buttonIcons;
QList<QString> Ut_HomeNotificationSink::contents;
QHash<const QGraphicsWidget *, QList<QAction *> > Ut_HomeNotificationSink::actions;
QList<DuiInfoBanner *> Ut_HomeNotificationSink::notifications;
bool Ut_HomeNotificationSink::instantAnimation;
int Ut_HomeNotificationSink::lastTimeout;
QTimer *Ut_HomeNotificationSink::lastTimer;
MainWindow *Ut_HomeNotificationSink::mainWin = NULL;

// Tests
void Ut_HomeNotificationSink::initTestCase()
{
    // Create a DuiAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_homenotificationsink";
    app = new DuiApplication(argc, &app_name);
    mainWin = MainWindow::instance(true);
}

void Ut_HomeNotificationSink::cleanupTestCase()
{
    delete mainWin;
    // Destroy DuiApplication
    delete app;
}

void Ut_HomeNotificationSink::init()
{
    sink = new HomeNotificationSink();
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    connect(sink, SIGNAL(notificationRemovalRequested(uint)), sink, SLOT(removeNotification(uint)));
    instantAnimation = false;
    lastTimeout = -1;
    types.clear();
    icons.clear();
    bodies.clear();
    buttonIcons.clear();
    contents.clear();
    actions.clear();
    notifications.clear();
}

void Ut_HomeNotificationSink::cleanup()
{
    delete sink;
}

void Ut_HomeNotificationSink::testAddNotification()
{

    // Check that notifications are added when preview is enabled by default
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    emit addNotification(Notification(0, 0, 0, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    emit addNotification(Notification(1, 0, 0, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    QCOMPARE(notifications.count(), 2);

    // When previews are disabled, the notification should be transferred immediately
    QSignalSpy spy(sink, SIGNAL(transferNotification(Notification)));
    TestNotificationParameters parameters3("icon3", "body3", "buttonicon3", "content3 3 3 3");
    sink->setNotificationAreaVisible(true);
    emit addNotification(Notification(3, 0, 0, parameters3, NotificationManagerInterface::ApplicationEvent, 1000));
    QApplication::processEvents();
    QCOMPARE(notifications.count(), 2);
    QCOMPARE(spy.count(), 1);

    // When only the sink is disabled, the notification should be transferred immediately
    sink->setNotificationAreaVisible(false);
    sink->setApplicationEventsEnabled(false);
    emit addNotification(Notification(4, 0, 0, parameters3, NotificationManagerInterface::ApplicationEvent, 1000));
    QApplication::processEvents();
    QCOMPARE(notifications.count(), 2);
    QCOMPARE(spy.count(), 2);

    // When the sink and previews are disabled, the notification should be transferred immediately
    sink->setNotificationAreaVisible(true);
    sink->setApplicationEventsEnabled(false);
    emit addNotification(Notification(5, 0, 0, parameters3, NotificationManagerInterface::ApplicationEvent, 1000));
    QApplication::processEvents();
    QCOMPARE(notifications.count(), 2);
    QCOMPARE(spy.count(), 3);

    // SystemEvents should be added always
    sink->setNotificationAreaVisible(false);
    sink->setApplicationEventsEnabled(false);
    emit addNotification(Notification(6, 0, 0, parameters3, NotificationManagerInterface::SystemEvent, 1000));
    QApplication::processEvents();
    QCOMPARE(spy.count(), 3);
    QCOMPARE(notifications.count(), 3);
}

void Ut_HomeNotificationSink::testUpdateNotification()
{
    // Create two notifications
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    emit addNotification(Notification(0, 0, 0, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    emit addNotification(Notification(1, 0, 0, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));

    // Update the second notification
    TestNotificationParameters parametersX("iconX", "bodyX", "buttoniconX", "contentX X X X");
    emit addNotification(Notification(1, 0, 0, parametersX, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that two DuiInfoBanners were created with the given parameters
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("iconX"));
    QCOMPARE(bodies.length(), 2);
    QCOMPARE(bodies[0], QString("body0"));
    QCOMPARE(bodies[1], QString("bodyX"));
    QCOMPARE(buttonIcons.length(), 2);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(buttonIcons[1], QString("buttoniconX"));
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[0], QString("content0 0 0 0"));
    QCOMPARE(contents[1], QString("contentX X X X"));

    // Create a third notification and animate it
    instantAnimation = true;
    TestNotificationParameters parameters2("icon2", "body2", "buttonicon2", "content2 2 2 2");
    emit addNotification(Notification(2, 0, 0, parameters2, NotificationManagerInterface::ApplicationEvent, 1000));
    QApplication::processEvents();

    // Check that there are still only two banners with the given parameters
    QCOMPARE(types.length(), 2);
    QCOMPARE(types[0], DuiInfoBanner::Event);
    QCOMPARE(types[1], DuiInfoBanner::Event);
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("iconX"));
    QCOMPARE(bodies.length(), 2);
    QCOMPARE(bodies[0], QString("body0"));
    QCOMPARE(bodies[1], QString("bodyX"));
    QCOMPARE(buttonIcons.length(), 2);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(buttonIcons[1], QString("buttoniconX"));
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[0], QString("content0 0 0 0"));
    QCOMPARE(contents[1], QString("contentX X X X"));

    // Update the third notification
    emit addNotification(Notification(2, 0, 0, parametersX, NotificationManagerInterface::ApplicationEvent, 1000));
    // Check that there are still only two banners with the given parameters
    QCOMPARE(types.length(), 2);
    QCOMPARE(types[0], DuiInfoBanner::Event);
    QCOMPARE(types[1], DuiInfoBanner::Event);
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("iconX"));
    QCOMPARE(bodies.length(), 2);
    QCOMPARE(bodies[0], QString("body0"));
    QCOMPARE(bodies[1], QString("bodyX"));
    QCOMPARE(buttonIcons.length(), 2);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(buttonIcons[1], QString("buttoniconX"));
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[0], QString("content0 0 0 0"));
    QCOMPARE(contents[1], QString("contentX X X X"));
}

void Ut_HomeNotificationSink::testRemoveNotification()
{
    // Create three notifications
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    emit addNotification(Notification(0, 0, 0, parameters0, NotificationManagerInterface::SystemEvent, 1000));
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    emit addNotification(Notification(1, 0, 0, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    TestNotificationParameters parameters2("icon2", "body2", "buttonicon2", "");
    emit addNotification(Notification(2, 0, 0, parameters2, NotificationManagerInterface::ApplicationEvent, 1000));

    // Cancel the second one
    emit removeNotification(1);
    QApplication::processEvents();

    // Check that two DuiInfoBanners exist
    QCOMPARE(notifications.count(), 2);

    // Recreate the second notification and create an additional one
    emit addNotification(Notification(1, 0, 0, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    TestNotificationParameters parameters3("icon3", "body3", "buttonicon3", "");
    emit addNotification(Notification(3, 0, 0, parameters3, NotificationManagerInterface::SystemEvent, 1000));

    // Check that four DuiInfoBanners exist
    QCOMPARE(notifications.count(), 4);
    QCOMPARE(icons.length(), 4);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("icon2"));
    QCOMPARE(icons[2], QString("icon1"));
    QCOMPARE(icons[3], QString("icon3"));
}

void Ut_HomeNotificationSink::testTransferNotification()
{
    QSignalSpy spy(sink, SIGNAL(transferNotification(Notification)));

    // When an application event notification is canceled before it is animated the transferNotification() signal should not be sent
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    emit addNotification(Notification(0, 0, 0, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    emit removeNotification(0);
    QCOMPARE(spy.count(), 0);

    // When a system event notification has been animated the transfer signal should not be sent
    instantAnimation = true;
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "");
    emit addNotification(Notification(1, 0, 0, parameters0, NotificationManagerInterface::SystemEvent, 1000));
    QCOMPARE(spy.count(), 0);

    // When an application event notification has been animated the transferNotification() signal should be sent
    emit addNotification(Notification(2, 0, 0, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    QApplication::processEvents();
    QCOMPARE(spy.count(), 1);

    // When an already transferred notification is updated the transferNotification() signal should be sent
    emit addNotification(Notification(2, 0, 0, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));
    QCOMPARE(spy.count(), 2);

    // TODO this test could perhaps test that if a notification is updated during the animation the transferred notification reflects the update
}

void Ut_HomeNotificationSink::testTimeout()
{
    // Create a notification with a timeout of 1000 milliseconds
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    emit addNotification(Notification(0, 0, 0, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that the timeout was set
    QCOMPARE(lastTimeout, 1000);

    // The notification should be created
    QCOMPARE(notifications.count(), 1);

    // Create a notification with a timeout of 0 milliseconds
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    emit addNotification(Notification(1, 0, 0, parameters1, NotificationManagerInterface::ApplicationEvent, 0));

    // Check that the timeout was set
    QCOMPARE(lastTimeout, 0);

    // The notification should be immediately destroyed
    QCOMPARE(notifications.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_HomeNotificationSink)
