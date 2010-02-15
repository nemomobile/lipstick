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

#ifndef UT_NOTIFICATIONMANAGER_H
#define UT_NOTIFICATIONMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QVariant>
#include <duinotification.h>

class NotificationManager;
class TestNotificationManager;

class Ut_NotificationManager : public QObject
{
    Q_OBJECT

public:
    static bool catchTimerTimeouts;
    static QList<int> timerTimeouts;
    static QHash<QString, QVariant> dataStore;

private:
    TestNotificationManager *manager;

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test that notification system user IDs are allocated properly
    void testNotificationUserId();
    // Test that correct kinds of notification widgets are displayed that their IDs are unique
    void testAddNotification();
    // Test that updating with an ID updates the correct notification
    void testUpdateNotification();
    // Test that canceling with an ID cancels the correct notification
    void testRemoveNotification();

    // Tests adding a notification group
    void testAddGroup();
    // Tests updating a notification group
    void testUpdateGroup();
    // Tests updating a notification group that doesn't exist
    void testUpdateNonexistingGroup();
    // Tests removing of a notification group
    void testRemoveGroup();
    // Tests removing a notification group with notifications
    void testRemoveGroupWithNotifications();
    // Test that group still exists when all notifications from it are removed
    void testRemovingAllNotificationsFromAGroup();
    // Tests removing of a notification group that doesn't exist
    void testRemoveNonexistingGroup();
    // Test adding of notification into a notification group
    void testAddNotificationInGroup();
    // Test adding of notification into a notification group that doesn't exist
    void testAddNotificationInNonexistingGroup();
    // Test that second notification is left in a queue
    void testSecondSimultaneousNotificationIsLeftInQueue();
    // Test that notification canceled in the wait queue is not relayed to sinks
    void testCancellingNotificationInQueue();
    // Test that notification can be updated while its in the wait queue
    void testUpdatingNotificationInQueue();
    // Test that notification is not realyed when the queue is empty
    void testRelayInEmptyQueue();
    // Test that new notifications are dropped if wait queue is full
    void testDroppingNotificationsIfQueueIsFull();
    // Test that wait queue timer relays notifications from the wait queue
    void testWaitQueueTimer();
    // Test removing notifications in a group
    void testRemoveNotificationsInGroup();
    // Test quering notifications
    void testNotificationIdList();
    // Test with wrong/zero notification user id
    void testNotificationIdListZeroNotificationUserId();
    // Test with empty notifications list
    void testNotificationIdListNotificationListEmpty();
    // Test that the group info is saved to the persistent storage
    void testGroupInfoPersistentStorage();
    // Test that the persistent notifications are saved to the persistent storage
    void testPersistentNotificationStorage();
    // Test that the persistent notifications are restored from the persistent storage
    void testPersistentNotificationRestoration();
};

#endif // UT_NOTIFICATIONMANAGER_H
