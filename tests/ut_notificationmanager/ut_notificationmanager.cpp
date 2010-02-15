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

#include "ut_notificationmanager.h"

#include <QtTest/QtTest>
#include <QMetaType>
#include <duinotification.h>
#include <duifiledatastore.h>
#include "notificationmanager.h"
#include "notification.h"
#include "notificationgroup.h"
#include "dbusinterfacenotificationsource.h"
#include "systemnotificationsource.h"

Q_DECLARE_METATYPE(Notification)
Q_DECLARE_METATYPE(NotificationGroup)
Q_DECLARE_METATYPE(NotificationParameters)

bool Ut_NotificationManager::catchTimerTimeouts;
QList<int> Ut_NotificationManager::timerTimeouts;
QBuffer gStateBuffer;
QBuffer gNotificationBuffer;
QSet<uint> gPersistentGroupSet;
QList<NotificationGroup> gGroupList;
QList<Notification> gNotificationList;
quint32 gLastUserId;
bool gTestingPersistent;


// Test Notification Manager
class TestNotificationManager : public NotificationManager
{
public:
    TestNotificationManager(int notificationRelayInterval, uint maxWaitQueueSize = 100);
    void relayNextNotification();
};

TestNotificationManager::TestNotificationManager(int notificationRelayInterval, uint maxWaitQueueSize)
    : NotificationManager(notificationRelayInterval, maxWaitQueueSize)
{
}

void TestNotificationManager::relayNextNotification()
{
    NotificationManager::relayNextNotification();
}

DBusInterfaceNotificationSource::DBusInterfaceNotificationSource(NotificationManagerInterface &manager) : NotificationSource(manager)
{
}

// maemosec stubs
int maemosec::storage::get_file(const char *pathname, unsigned char **to_buf, ssize_t *bytes)
{
    if (QString(pathname).contains("state.data")) {
        *to_buf = (unsigned char *)gStateBuffer.buffer().data();
        *bytes = gStateBuffer.buffer().size();
    } else if (QString(pathname).contains("notifications.data")) {
        *to_buf = (unsigned char *)gNotificationBuffer.buffer().data();
        *bytes = gNotificationBuffer.buffer().size();
    }

    return 0;
}

int maemosec::storage::put_file(const char *pathname, unsigned char *data, ssize_t bytes)
{
    if (QString(pathname).contains("state.data")) {
        gStateBuffer.close();
        gStateBuffer.setData((char *)data, bytes);
    } else if (QString(pathname).contains("notifications.data")) {
        gNotificationBuffer.close();
        gNotificationBuffer.setData((char *)data, bytes);
    }

    return 0;
}

void maemosec::storage::release_buffer(unsigned char *data)
{
    Q_UNUSED(data);
    gNotificationBuffer.close();
    gStateBuffer.close();
}

bool maemosec::storage::contains_file(const char *pathname)
{
    Q_UNUSED(pathname);

    return true;
}

void maemosec::storage::remove_file(const char *pathname)
{
    Q_UNUSED(pathname);
}

void maemosec::storage::commit()
{
}

maemosec::storage::storage(const char *name, maemosec::storage::visibility_t visibility, maemosec::storage::protection_t protection)
{
    Q_UNUSED(name);
    Q_UNUSED(visibility);
    Q_UNUSED(protection);
}

maemosec::storage::~storage()
{
}

// Helper function for loading the last given user id and group information from gStateBuffer
// and repopulating gPersistentGroupSet and gGroupList accordingly
void loadStateData()
{
    gStateBuffer.open(QIODevice::ReadOnly);
    QDataStream gds;
    gds.setDevice(&gStateBuffer);

    gPersistentGroupSet.clear();
    gGroupList.clear();

    gStateBuffer.seek(0);

    gds >> gLastUserId;
    gds >> gPersistentGroupSet;

    NotificationGroup ng;

    while (!gds.atEnd()) {
        gds >> ng;
        gGroupList.append(ng);
    }
}

// Helper function for loading notifications from gNotificationBuffer
// and repopulating gNotificationList
void loadNotifications()
{
    gNotificationBuffer.open(QIODevice::ReadOnly);
    QDataStream nds;
    nds.setDevice(&gNotificationBuffer);

    gNotificationList.clear();

    gNotificationBuffer.seek(0);

    Notification notification;

    while (!nds.atEnd()) {
        nds >> notification;
        gNotificationList.append(notification);
    }
}



// QTimer stubs (used by NotificationManager)
void QTimer::start(int msec)
{
    if (Ut_NotificationManager::catchTimerTimeouts) {
        Ut_NotificationManager::timerTimeouts.append(msec);
    }
}

// QDir stubs (used by NotificationManager)
bool QDir::exists(const QString &) const
{
    return gTestingPersistent;
}

bool QDir::mkpath(const QString &) const
{
    return true;
}

// Tests
void Ut_NotificationManager::initTestCase()
{
    qRegisterMetaType<Notification>();
    qRegisterMetaType<NotificationParameters>();
}

void Ut_NotificationManager::cleanupTestCase()
{
}

void Ut_NotificationManager::init()
{
    // Create test notification manager as a pass-through manager (no notifications will be queued).
    manager = new TestNotificationManager(0);
    timerTimeouts.clear();
    catchTimerTimeouts = false;

    gStateBuffer.open(QIODevice::ReadWrite);
    gNotificationBuffer.open(QIODevice::ReadWrite);
}

void Ut_NotificationManager::cleanup()
{
    delete manager;
}

void Ut_NotificationManager::testNotificationUserId()
{
    uint id1 = manager->notificationUserId();
    uint id2 = manager->notificationUserId();
    QVERIFY(id1 != 0);
    QVERIFY(id2 != 0);
    QVERIFY(id1 != id2);

    delete manager;

    gTestingPersistent = true;
    manager = new TestNotificationManager(0);

    uint id3 = manager->notificationUserId();
    QVERIFY(id3 != id1);
    QVERIFY(id3 != id2);

    loadStateData();

    QVERIFY(id3 == gLastUserId);

    gTestingPersistent = false;

    gNotificationBuffer.buffer().clear();
    gStateBuffer.buffer().clear();

    delete manager;
    manager = new TestNotificationManager(0);
}

void Ut_NotificationManager::testAddNotification()
{
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    catchTimerTimeouts = true;

    // Create three notifications - two with a content link and one without
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addNotification(0, parameters1, 0, true, NotificationManager::SystemEvent);
    NotificationParameters parameters2;
    parameters2.add("iconId", "buttonicon2");
    uint id2 = manager->addNotification(0, parameters2);

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    // Test that we didn't get any invalid ids
    QVERIFY(id0 != 0);
    QVERIFY(id1 != 0);
    QVERIFY(id2 != 0);

    // Check that three notifications were created with the given parameters
    QCOMPARE(spy.count(), 3);

    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value("imageId").toString(), QString("icon0"));
    QCOMPARE(n.type(), NotificationManager::ApplicationEvent);
    QCOMPARE(n.timeout(), 0);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value("body").toString(), QString("body1"));
    QCOMPARE(n.type(), NotificationManager::SystemEvent);
    QCOMPARE(n.timeout(), 0);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id2);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value("iconId").toString(), QString("buttonicon2"));
    QCOMPARE(n.type(), NotificationManager::ApplicationEvent);
    QCOMPARE(n.timeout(), 0);

    // Verify that the IDs are unique
    QVERIFY(id0 != id1);
    QVERIFY(id0 != id2);
    QVERIFY(id1 != id2);
}

void Ut_NotificationManager::testUpdateNotification()
{
    // Create one notification
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);

    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Update the notification
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    manager->updateNotification(0, id0, parameters1);

    // Test that the relevant signals are sent
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.parameters().value("imageId").isNull(), true);
    QCOMPARE(n.parameters().value("body").toString(), QString("body1"));
    QCOMPARE(n.type(), NotificationManagerInterface::ApplicationEvent);
    QCOMPARE(n.timeout(), 0);

    // TODO this cannot be fully tested until DuiInfoBanner supports updates
}

void Ut_NotificationManager::testRemoveNotification()
{
    QSignalSpy removeSpy(manager, SIGNAL(notificationRemoved(uint)));

    // Create three notifications
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    manager->addNotification(0, parameters0, 0, NotificationManager::SystemEvent);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addNotification(0, parameters1);
    NotificationParameters parameters2;
    parameters2.add("iconId", "buttonicon2");
    manager->addNotification(0, parameters2);

    // Cancel the second one
    manager->removeNotification(0, id1);

    // Test that the relevant signals are sent
    QCOMPARE(removeSpy.count(), 1);
    QList<QVariant> arguments = removeSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
}

void Ut_NotificationManager::testAddGroup()
{
    QSignalSpy addGroupSpy(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)));

    NotificationParameters inParameters1;
    inParameters1.add("imageId", "icon1");
    uint id1 = manager->addGroup(0, inParameters1);
    NotificationParameters inParameters2;
    inParameters2.add("body", "body2");
    uint id2 = manager->addGroup(0, inParameters2);

    // Check that we didn't get invalid numbers
    QVERIFY(id1 != 0);
    QVERIFY(id2 != 0);
    QVERIFY(id1 != id2);

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    QCOMPARE(addGroupSpy.count(), 2);

    QList<QVariant> arguments = addGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
    NotificationParameters outParameters1 = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters1.value("imageId").toString(), QString("icon1"));
    arguments = addGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id2);
    NotificationParameters outParameters2 = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters2.value("body").toString(), QString("body2"));
}

void Ut_NotificationManager::testUpdateGroup()
{
    NotificationParameters inParameters1;
    inParameters1.add("imageId", "icon1");
    uint id1 = manager->addGroup(0, inParameters1);

    QSignalSpy updateGroupSpy(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)));

    NotificationParameters inParameters2;
    inParameters2.add("body", "body2");
    manager->updateGroup(0, id1, inParameters2);
    QCOMPARE(updateGroupSpy.count(), 1);
    QList<QVariant> arguments = updateGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
    NotificationParameters outParameters2 = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters2.value("body").toString(), QString("body2"));
}

void Ut_NotificationManager::testUpdateNonexistingGroup()
{
    QSignalSpy updateGroupSpy(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)));

    // Try to update a group that doesn't exist
    NotificationParameters parameters1;
    parameters1.add("imageId", "icon1");
    manager->updateGroup(0, 1, parameters1);
    QCOMPARE(updateGroupSpy.count(), 0);

    // Add a group...
    uint id1 = manager->addGroup(0, parameters1);
    // Need to clear the spy because addGroup sends the same signal
    updateGroupSpy.clear();
    // ...but try to update some other group
    NotificationParameters parameters2;
    parameters2.add("body", "body2");
    manager->updateGroup(0, id1 + 1, parameters2);
    QCOMPARE(updateGroupSpy.count(), 0);
}

void Ut_NotificationManager::testRemoveGroup()
{
    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    NotificationParameters parameters1;
    parameters1.add("imageId", "icon1");
    uint id1 = manager->addGroup(0, parameters1);
    manager->removeGroup(0, id1);
    QCOMPARE(removeGroupSpy.count(), 1);
    QList<QVariant> arguments = removeGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
}

void Ut_NotificationManager::testRemoveGroupWithNotifications()
{
    NotificationParameters gparameters1;
    gparameters1.add("imageId", "gicon1");
    uint groupId = manager->addGroup(0, gparameters1);
    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));
    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    QSignalSpy removeNotificationSpy(manager, SIGNAL(notificationRemoved(uint)));
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0, groupId);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addNotification(0, parameters1, groupId);
    QCOMPARE(addSpy.count(), 2);

    manager->removeGroup(0, groupId);
    QCOMPARE(removeGroupSpy.count(), 1);
    QCOMPARE(removeNotificationSpy.count(), 2);
    QList<QVariant> arguments = removeNotificationSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id0);
    arguments = removeNotificationSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
}

void Ut_NotificationManager::testRemovingAllNotificationsFromAGroup()
{
    NotificationParameters gparameters1;
    gparameters1.add("imageId", "gicon1");
    uint groupId = manager->addGroup(0, gparameters1);

    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0, groupId);
    manager->removeNotification(0, id0);

    // Check that group is not removed even if the all notifications from it are removed
    QCOMPARE(removeGroupSpy.count(), 0);

    // Check that the group could be reused for showing notifications
    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));
    manager->addNotification(0, parameters0, groupId);
    QCOMPARE(addSpy.count(), 1);
}

void Ut_NotificationManager::testRemoveNonexistingGroup()
{
    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    manager->removeGroup(0, 1);
    QCOMPARE(removeGroupSpy.count(), 0);

    // Add a group and then try to remove it twice
    NotificationParameters gparameters1;
    gparameters1.add("imageId", "gicon1");
    uint id1 = manager->addGroup(0, gparameters1);
    manager->removeGroup(0, id1);
    removeGroupSpy.clear();
    manager->removeGroup(0, id1);
    QCOMPARE(removeGroupSpy.count(), 0);
}

void Ut_NotificationManager::testAddNotificationInGroup()
{
    NotificationParameters gparameters1;
    gparameters1.add("imageId", "gicon1");
    uint groupId = manager->addGroup(0, gparameters1);
    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0, groupId);

    QCOMPARE(addSpy.count(), 1);
    QList<QVariant> arguments = addSpy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.groupId(), groupId);
    QCOMPARE(n.parameters().value("imageId").toString(), QString("icon0"));
}

void Ut_NotificationManager::testAddNotificationInNonexistingGroup()
{
    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));

    // Add a notification to a group that doesn't exist
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    manager->addNotification(0, parameters0, 1);
    QCOMPARE(addSpy.count(), 0);
}

void Ut_NotificationManager::testSecondSimultaneousNotificationIsLeftInQueue()
{
    // Create a new notification manager with manual notification relay.
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create a signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Create two notifications
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addNotification(0, parameters1);

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    // Check that notification sink was signaled with one notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value("imageId").toString(), QString("icon0"));
    QCOMPARE(n.type(), NotificationManagerInterface::ApplicationEvent);
    QCOMPARE(n.timeout(), -1);

    // Relay next notification
    manager->relayNextNotification();

    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value("body").toString(), QString("body1"));
    QCOMPARE(n.type(), NotificationManagerInterface::ApplicationEvent);
    QCOMPARE(n.timeout(), -1);

    // Create third notification. This should be left in the queue since the second notification is being shown.
    NotificationParameters parameters2;
    parameters0.add("iconId", "buttonicon2");
    manager->addNotification(0, parameters2);
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationManager::testCancellingNotificationInQueue()
{
    // Create a new notification manager with manual notification relay.
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create signal spies to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));
    QSignalSpy removeSpy(manager, SIGNAL(notificationRemoved(uint)));

    // Create three notifications
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addNotification(0, parameters1);
    NotificationParameters parameters2;
    parameters2.add("iconId", "buttonicon2");
    uint id2 = manager->addNotification(0, parameters2);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Cancel the middle notification
    manager->removeNotification(0, id1);

    // Check that notification sink was not signaled with cancel.
    QCOMPARE(removeSpy.count(), 0);

    // Relay next notification.
    manager->relayNextNotification();

    // Check that notification sink was signaled with third notification.
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id2);
}

void Ut_NotificationManager::testUpdatingNotificationInQueue()
{
    // Create a new notification manager with manual notification relay.
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Create two notifications
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addNotification(0, parameters1);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Update the second notification
    NotificationParameters parameters2;
    parameters2.add("iconId", "newicon");
    manager->updateNotification(0, id1, parameters2);

    // Check that notification sink was not signaled with update.
    QCOMPARE(spy.count(), 0);

    // Relay next notification.
    manager->relayNextNotification();

    // Check that notification sink was signaled with updated notification data.
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);
    QCOMPARE(n.parameters().value("imageId").isNull(), true);
    QCOMPARE(n.parameters().value("body").isNull(), true);
    QCOMPARE(n.parameters().value("iconId").toString(), QString("newicon"));
    QCOMPARE(n.parameters().value("contentId").isNull(), true);
    QCOMPARE(n.type(), NotificationManagerInterface::ApplicationEvent);
}

// Test that nothing happens if wait queue is empty and relay notification is called
void Ut_NotificationManager::testRelayInEmptyQueue()
{
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Try to relay notification even though it does not exist
    manager->relayNextNotification();
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationManager::testDroppingNotificationsIfQueueIsFull()
{
    // Create notification manager with wait queue of 5 notifications
    delete manager;
    manager = new TestNotificationManager(-1, 5);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Add 7 notifications to the notification manager.
    // First notification will be relayed to the sink.
    // Next five notifications will fill the wait queue.
    // Last notification will be dropped.
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters15;
    parameters15.add("body", "queuefillerbody");
    for (int i = 0; i < 5; ++i) {
        manager->addNotification(0, parameters15);
    }
    NotificationParameters parameters6;
    parameters0.add("icondId", "droppedbuttonicon");
    manager->addNotification(0, parameters6);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Relay the next 5 notifications
    for (int i = 0; i < 5; ++i) {
        manager->relayNextNotification();
        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        n = qvariant_cast<Notification>(arguments.at(0));
        QCOMPARE(n.parameters().value("body").toString(), QString("queuefillerbody"));
    }

    // The queue should be empty. Try relaying.
    manager->relayNextNotification();
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationManager::testWaitQueueTimer()
{
    // Create notification manager with relay interval
    delete manager;
    manager = new TestNotificationManager(3000);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    catchTimerTimeouts = true;

    // Add two notifications to the notification manager.
    // First notification should pass through.
    // Second is left in wait queue
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addNotification(0, parameters1);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Check that timer was started.
    QCOMPARE(timerTimeouts.count(), 1);
    QCOMPARE(timerTimeouts.at(0), 3000);

    // Expire timer
    manager->relayNextNotification();

    // Check that notification sink was signaled with second notification.
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);

    // Check that timer was re-started.
    QCOMPARE(timerTimeouts.count(), 2);
    QCOMPARE(timerTimeouts.at(1), 3000);

    // Expire timer
    manager->relayNextNotification();

    // Check that notification sink was not signaled anymore.
    QCOMPARE(spy.count(), 0);

    // Check that timer was not re-started.
    QCOMPARE(timerTimeouts.count(), 2);
}

void Ut_NotificationManager::testRemoveNotificationsInGroup()
{
    QSignalSpy removeSpy(manager, SIGNAL(notificationRemoved(uint)));

    NotificationParameters gparameters1;
    gparameters1.add("imageId", "gicon1");
    uint groupId = manager->addGroup(0, gparameters1);
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addNotification(0, parameters0, groupId);
    NotificationParameters parameters1;
    parameters0.add("imageId", "icon1");
    uint id1 = manager->addNotification(0, parameters0, groupId);

    manager->removeNotificationsInGroup(groupId);

    // Test that the relevant signals are sent
    QCOMPARE(removeSpy.count(), 2);
    QCOMPARE(removeSpy.at(0).at(0).toUInt(), id0);
    QCOMPARE(removeSpy.at(1).at(0).toUInt(), id1);
}

void Ut_NotificationManager::testNotificationIdList()
{
    NotificationParameters parameters;
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(3, parameters);
    manager->addNotification(2, parameters);
    manager->addNotification(1, parameters);

    uint notificationUserId = 1;
    QList<uint> list;
    list = manager->notificationIdList(notificationUserId);
    QCOMPARE(list.size(), 4);

    QCOMPARE(list.at(0), (uint)1);
    QCOMPARE(list.at(1), (uint)2);
    QCOMPARE(list.at(2), (uint)3);
    QCOMPARE(list.at(3), (uint)6);

}

void Ut_NotificationManager::testNotificationIdListZeroNotificationUserId()
{
    NotificationParameters parameters;
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(3, parameters);
    manager->addNotification(2, parameters);
    manager->addNotification(1, parameters);

    uint notificationUserId = 0;
    QList<uint> list;
    list = manager->notificationIdList(notificationUserId);
    QCOMPARE(list.size(), 0);
}

void Ut_NotificationManager::testNotificationIdListNotificationListEmpty()
{
    uint notificationUserId = 0;
    QList<uint> list;
    list = manager->notificationIdList(notificationUserId);
    QCOMPARE(list.size(), 0);
}


void Ut_NotificationManager::testGroupInfoPersistentStorage()
{
    gTestingPersistent = true;

    gNotificationBuffer.buffer().clear();
    gStateBuffer.buffer().clear();

    delete manager;
    manager = new TestNotificationManager(3000);

    // Add two groups to the notification manager.
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    uint id0 = manager->addGroup(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    uint id1 = manager->addGroup(0, parameters1, true);

    loadStateData();

    QCOMPARE((uint)gPersistentGroupSet.count(), (uint)1);
    QCOMPARE((bool)gPersistentGroupSet.contains(id1), (bool)true);
    QCOMPARE((uint)gGroupList.count(), (uint)2);

    NotificationGroup   ng = gGroupList.at(0);
    QCOMPARE(ng.parameters().value("imageId").toString(), QString("icon0"));
    ng = gGroupList.at(1);
    QCOMPARE(ng.parameters().value("body").toString(), QString("body1"));


    manager->removeGroup(0, id0);

    loadStateData();

    QCOMPARE((uint)gPersistentGroupSet.count(), (uint)1);
    QCOMPARE((bool)gPersistentGroupSet.contains(id1), (bool)true);
    QCOMPARE((uint)gGroupList.count(), (uint)1);

    ng = gGroupList.at(0);
    QCOMPARE(ng.parameters().value("body").toString(), QString("body1"));


    manager->updateGroup(0, id1, parameters0);

    loadStateData();

    QCOMPARE((uint)gPersistentGroupSet.count(), (uint)1);
    QCOMPARE((bool)gPersistentGroupSet.contains(id1), (bool)true);
    QCOMPARE((uint)gGroupList.count(), (uint)1);

    ng = gGroupList.at(0);
    QCOMPARE(ng.parameters().value("imageId").toString(), QString("icon0"));
}

void Ut_NotificationManager::testPersistentNotificationStorage()
{
    delete manager;
    manager = new TestNotificationManager(3000);

    // Add two groups to the notification manager,
    // the other one as persistent
    NotificationParameters gparameters0;
    gparameters0.add("imageId", "icon0");
    uint gid0 = manager->addGroup(0, gparameters0);
    NotificationParameters gparameters1;
    gparameters1.add("body", "body1");
    uint gid1 = manager->addGroup(0, gparameters1, true);

    // Create three notifications - two persistent and one non-persistent
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    // Non-persistent
    manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    // Persistent in a non-persistent group
    uint id1 = manager->addNotification(0, parameters1, gid0, true);
    NotificationParameters parameters2;
    parameters2.add("iconId", "buttonicon2");
    // Non-persistent in a persistent group
    uint id2 = manager->addNotification(0, parameters2, gid1, false);

    loadNotifications();

    QCOMPARE((uint)gNotificationList.count(), (uint)2);
    Notification notification = gNotificationList.at(0);
    QCOMPARE(notification.parameters().value("body").toString(), QString("body1"));
    notification = gNotificationList.at(1);
    QCOMPARE(notification.parameters().value("iconId").toString(), QString("buttonicon2"));


    manager->removeNotification(0, id1);

    loadNotifications();

    QCOMPARE((uint)gNotificationList.count(), (uint)1);
    notification = gNotificationList.at(0);
    QCOMPARE(notification.parameters().value("iconId").toString(), QString("buttonicon2"));


    manager->updateNotification(0, id2, parameters0);

    loadNotifications();

    QCOMPARE((uint)gNotificationList.count(), (uint)1);
    notification = gNotificationList.at(0);
    QCOMPARE(notification.parameters().value("imageId").toString(), QString("icon0"));
}

void Ut_NotificationManager::testPersistentNotificationRestoration()
{
    delete manager;

    gTestingPersistent = true;

    gNotificationBuffer.buffer().clear();
    gNotificationBuffer.open(QIODevice::WriteOnly);
    QDataStream stream(&gNotificationBuffer);

    // Create three notifications
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    stream << Notification(0, 3, 0, parameters0, NotificationManagerInterface::ApplicationEvent, 0);

    NotificationParameters parameters1;
    parameters1.add("body", "body1");
    stream << Notification(1, 4, 0, parameters1, NotificationManagerInterface::SystemEvent, 1000);

    NotificationParameters parameters2;
    parameters2.add("iconId", "buttonicon2");
    stream << Notification(2, 5, 0, parameters2, NotificationManagerInterface::ApplicationEvent, 2000);

    gNotificationBuffer.close();

    manager = new TestNotificationManager(0);
    QSignalSpy spy(manager, SIGNAL(notificationRestored(Notification)));
    manager->restorePersistentData();

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    // Check that three notifications were created with the given parameters
    QCOMPARE(spy.count(), 3);

    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), (uint)0);
    QCOMPARE(n.groupId(), (uint)3);
    QCOMPARE(n.parameters().value("imageId").toString(), QString("icon0"));
    QCOMPARE(n.type(), NotificationManager::ApplicationEvent);
    QCOMPARE(n.timeout(), 0);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), (uint)1);
    QCOMPARE(n.groupId(), (uint)4);
    QCOMPARE(n.parameters().value("body").toString(), QString("body1"));
    QCOMPARE(n.type(), NotificationManager::SystemEvent);
    QCOMPARE(n.timeout(), 1000);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), (uint)2);
    QCOMPARE(n.groupId(), (uint)5);
    QCOMPARE(n.parameters().value("iconId").toString(), QString("buttonicon2"));
    QCOMPARE(n.type(), NotificationManager::ApplicationEvent);
    QCOMPARE(n.timeout(), 2000);

    gTestingPersistent = false;
}

QTEST_MAIN(Ut_NotificationManager)
