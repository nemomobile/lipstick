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

#ifndef UT_DUICOMPOSITORNOTIFICATIONSINK_H
#define UT_DUICOMPOSITORNOTIFICATIONSINK_H

#include <QObject>
#include <QList>
#include <QString>
#include <DuiInfoBanner>
#include "notification.h"

class DuiApplication;
class MainWindow;
class DuiCompositorNotificationSink;

class MockNotificationManager : public QObject, public NotificationManagerInterface
{
    Q_OBJECT
public:
    MockNotificationManager();
    uint addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type, int timeout);
    uint addNotification(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters(), uint groupId = 0, bool persistent = false, NotificationType type = ApplicationEvent);
    bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters = NotificationParameters());
    uint addGroup(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters(), bool persistent = false);
    bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters = NotificationParameters());
    bool removeGroup(uint notificationUserId, uint groupId);
    uint notificationUserId();
    QList<uint> notificationIdList(uint notificationUserId);

    uint nextAvailableNotificationID;
    QList<Notification> notifications;

public slots:
    bool removeNotification(uint notificationUserId, uint notificationId);

signals:
    void notificationRemoved(uint notificationId);
    void notificationUpdated(const Notification &notification);
};

class Ut_DuiCompositorNotificationSink : public QObject
{
    Q_OBJECT

public:
    static QList<DuiInfoBanner::BannerType> types;
    static QList<QString> icons;
    static QList<QString> bodies;
    static QList<QString> buttonIcons;
    static QList<QString> contents;
    static QHash<const QGraphicsWidget *, QList<QAction *> > actions;
    static QList<DuiInfoBanner *> notifications;
    static int lastTimeout;
    static bool desktopIsOnTop;
    static int lastFixedWidth;
    static int lastFixedHeight;
    static QTransform lastTransform;

private:
    DuiApplication *app;
    DuiCompositorNotificationSink *sink;
    MockNotificationManager *notificationManager;
    MainWindow *mainWindow;

signals:
    void orientationChangeFinished(const Dui::Orientation &orientation);

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test that notifications can be added and no notifications are displayed when desktop is on top
    void testAddNotification();
    // Test that updating with an ID updates the correct notification
    void testUpdateNotification();
    // Test that canceling with an ID cancels the correct notification
    void testRemoveNotification();
    // Test that notification windows are destroyed after a timeout
    void testTimeout();
    // Test that changing the orientation rotates the existing notifications
    void testOrientationChanged();
    // Test that notifications are NOT added while recording
    void testNotificationWhileApplicationEventsDisabled();
};

#endif // UT_DUICOMPOSITORNOTIFICATIONSINK_H
