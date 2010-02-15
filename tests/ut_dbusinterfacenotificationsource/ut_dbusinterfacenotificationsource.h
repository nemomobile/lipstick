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

#ifndef UT_DBUSINTERFACENOTIFICATIONSOURCE_H
#define UT_DBUSINTERFACENOTIFICATIONSOURCE_H

#include <QtTest/QtTest>
#include <QObject>
#include "notificationmanagerinterface.h"

// class DuiApplication;
class DBusInterfaceNotificationSource;

// MockNotificationManager used by the DBusInterfaceNotificationSource
class MockNotificationManager : public NotificationManagerInterface
{
public:
    MockNotificationManager();
    uint addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId = 0, bool persistent = false, NotificationType type = ApplicationEvent);
    bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters);
    bool removeNotification(uint notificationUserId, uint notificationId);
    uint addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent = false);
    bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters);
    bool removeGroup(uint notificationUserId, uint groupId);
    uint notificationUserId();
    QList< uint > notificationIdList(uint notificationUserId);

    uint addNotificationNotificationUserId;
    uint addNotificationGroupId;
    NotificationParameters addNotificationParameters;
    bool addNotificationPersistent;
    NotificationManagerInterface::NotificationType addNotificationType;
    uint updateNotificationNotificationUserId;
    uint updateNotificationId;
    NotificationParameters updateNotificationParameters;
    uint removeNotificationNotificationUserId;
    uint removeNotificationId;
    uint addGroupNotificationUserId;
    NotificationParameters addGroupParameters;
    bool addGroupPersistent;
    uint updateGroupNotificationUserId;
    uint updateGroupId;
    NotificationParameters updateGroupParameters;
    uint removeGroupNotificationUserId;
    uint removeGroupId;
    uint lastNotificationUserId;
    uint queryNotificationUserId;
};

class Ut_DBusInterfaceNotificationSource : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test notification adding
    void testAddNotification();
    // Test notification updating
    void testUpdateNotification();
    // Test notification removal
    void testRemoveNotification();
    // Test notification group adding
    void testAddGroup();
    // Test notification group updating
    void testUpdateGroup();
    // Test notification group removal
    void testRemoveGroup();
    //Test the query of notifications
    void testNotificationIdList();

private:
    // Notification manager interface used by the test subject
    MockNotificationManager *manager;
    // The object being tested
    DBusInterfaceNotificationSource *source;
};

#endif
