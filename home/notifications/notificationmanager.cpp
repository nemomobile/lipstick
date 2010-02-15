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

#include "notificationmanager.h"
#include "notification.h"
#include "dbusinterfacenotificationsource.h"
#include "systemnotificationsource.h"
#include "contextframeworkcontext.h"
#include <QDBusConnection>
#include <QDir>
#include <duifiledatastore.h>
#include <eventtypestore.h>

//! Directory in which the persistent data files are located
static const QString PERSISTENT_DATA_PATH = QDir::homePath() + QString("/.config/duihome/notificationmanager/");

//! The number configuration files to load into the event type store.
static const uint MAX_EVENT_TYPE_CONF_FILES = 100;

//! Name of the file where persistent status data is stored
static const QString STATE_DATA_FILE_NAME = PERSISTENT_DATA_PATH + QString("state.data");

//! Name of the file where persistent notifications are stored
static const QString NOTIFICATIONS_FILE_NAME = PERSISTENT_DATA_PATH + QString("notifications.data");


NotificationManager::NotificationManager(int relayInterval, uint maxWaitQueueSize) :
    notifications(),
    groups(),
    maxWaitQueueSize(maxWaitQueueSize),
    notificationInProgress(false),
    relayInterval(relayInterval),
    context(new ContextFrameworkContext()),
    lastUsedNotificationUserId(0),
    persistentDataRestored(false),
    persistentStorage(new maemosec::storage("org.maemo.dui.NotificationManager", maemosec::storage::vis_private, maemosec::storage::prot_encrypted))
{
    dBusSource = new DBusInterfaceNotificationSource(*this);
    systemNotificationSource = NULL;

    waitQueueTimer.setSingleShot(true);
    connect(&waitQueueTimer, SIGNAL(timeout()), this, SLOT(relayNextNotification()));

    if (!QDir::root().exists(PERSISTENT_DATA_PATH)) {
        // No data to restore exists yet
        persistentDataRestored = true;
    }

    //Initialize the event type store
    initializeEventTypeStore();

    // Connect to D-Bus and register the DBus source as an object
    QDBusConnection::sessionBus().registerService("org.maemo.dui.NotificationManager");
    QDBusConnection::sessionBus().registerObject("/", dBusSource);
}

NotificationManager::~NotificationManager()
{
    delete dBusSource;
    delete context;
}

void NotificationManager::loadSystemNotificationSource()
{
    if (systemNotificationSource == NULL) {
        systemNotificationSource = new SystemNotificationSource(*this);
    }
}

bool NotificationManager::ensurePersistentDataPath()
{
    // Create the data store path if it does not exist yet
    if (!QDir::root().exists(PERSISTENT_DATA_PATH)) {
        return QDir::root().mkpath(PERSISTENT_DATA_PATH);
    }

    return true;
}

void NotificationManager::saveStateData()
{
    if (ensurePersistentDataPath()) {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QDataStream stream(&buffer);

        stream << lastUsedNotificationUserId;
        stream << persistentGroups;

        foreach(const NotificationGroup & group, groups) {
            stream << group;
        }

        persistentStorage->put_file(STATE_DATA_FILE_NAME.toAscii(),
                                    (unsigned char *)buffer.data().data(),
                                    buffer.data().length());

        persistentStorage->commit();
    }
}


void NotificationManager::savePersistentNotifications()
{
    if (ensurePersistentDataPath()) {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QDataStream stream(&buffer);

        if (persistentNotifications.size()) {
            foreach(uint notificationId, persistentNotifications) {
                stream << notifications[notificationId];
            }

            persistentStorage->put_file(NOTIFICATIONS_FILE_NAME.toAscii(),
                                        (unsigned char *)buffer.data().data(),
                                        buffer.data().length());
        } else {
            persistentStorage->remove_file(NOTIFICATIONS_FILE_NAME.toAscii());
        }


        persistentStorage->commit();
    }
}


void NotificationManager::restorePersistentData()
{
    if (!persistentDataRestored && ensurePersistentDataPath()) {
        unsigned char *data;
        ssize_t dataLength;

        // The persistent storage automatically verifies the data.
        // If the data is corrupted or tampered with, the previous state
        // is lost and there's nothing we can do.
        persistentDataRestored = true;

        if (persistentStorage->contains_file(STATE_DATA_FILE_NAME.toAscii()) &&
                persistentStorage->get_file(STATE_DATA_FILE_NAME.toAscii(), &data, &dataLength) == 0) {

            QBuffer buffer;
            buffer.setData((char *)data, dataLength);
            buffer.open(QIODevice::ReadOnly);
            persistentStorage->release_buffer(data);

            QDataStream stream(&buffer);

            stream >> lastUsedNotificationUserId;
            stream >> persistentGroups;

            NotificationGroup group;

            while (!stream.atEnd()) {
                stream >> group;
                groups.insert(group.groupId(), group);
                emit groupUpdated(group.groupId(), group.parameters());
            }
        }

        if (persistentStorage->contains_file(NOTIFICATIONS_FILE_NAME.toAscii()) &&
                persistentStorage->get_file(NOTIFICATIONS_FILE_NAME.toAscii(), &data, &dataLength) == 0) {

            QBuffer buffer;
            buffer.setData((char *)data, dataLength);
            buffer.open(QIODevice::ReadOnly);
            persistentStorage->release_buffer(data);

            QDataStream stream(&buffer);

            Notification notification;

            while (!stream.atEnd()) {
                stream >> notification;
                notifications.insert(notification.notificationId(), notification);
                // Notifications in the persistent storage must have been persistent before
                persistentNotifications.insert(notification.notificationId());
                emit notificationRestored(notification);
            }
        }
    }
}


void NotificationManager::initializeEventTypeStore()
{
    if (notificationEventTypeStore) {
        return;
    }
    notificationEventTypeStore = QSharedPointer<EventTypeStore> (new EventTypeStore(NOTIFICATIONS_EVENT_TYPES, MAX_EVENT_TYPE_CONF_FILES));
}


uint NotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type)
{
    restorePersistentData();

    if (groupId == 0 || groups.contains(groupId)) {
        uint notificationId = nextAvailableNotificationID();

        // Mark the notification used
        Notification notification(notificationId, groupId, notificationUserId, parameters, type, relayInterval);
        notifications.insert(notificationId, notification);
        submitNotification(notification);

        // If a group is persistent, all the notifications in the group are persistent too
        if (persistent || persistentGroups.contains(groupId)) {
            persistentNotifications.insert(notificationId);
            savePersistentNotifications();
        }

        return notificationId;
    }

    return 0;
}

bool NotificationManager::updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters)
{
    Q_UNUSED(notificationUserId);

    restorePersistentData();

    QHash<uint, Notification>::iterator ni = notifications.find(notificationId);

    if (ni != notifications.end()) {
        (*ni).setParameters(parameters);

        int waitQueueIndex = findNotificationFromWaitQueue(notificationId);
        if (waitQueueIndex >= 0) {
            waitQueue[waitQueueIndex].setParameters(parameters);
        } else {
            // Inform the sinks about the update
            emit notificationUpdated(notifications.value(notificationId));
        }

        // Also checks if the notification's group is persistent
        if (persistentNotifications.contains(notificationId) ||
                persistentGroups.contains((*ni).groupId())) {
            savePersistentNotifications();
        }

        return true;
    } else {
        return false;
    }
}

bool NotificationManager::removeNotification(uint notificationUserId, uint notificationId)
{
    Q_UNUSED(notificationUserId);

    return removeNotification(notificationId);
}

bool NotificationManager::removeNotification(uint notificationId)
{
    restorePersistentData();

    if (notifications.contains(notificationId)) {
        int waitQueueIndex = findNotificationFromWaitQueue(notificationId);
        if (waitQueueIndex >= 0) {
            waitQueue.removeAt(waitQueueIndex);
        } else {
            // Inform the sinks about the removal
            emit notificationRemoved(notificationId);
        }

        // Mark the notification unused
        notifications.take(notificationId);

        if (persistentNotifications.contains(notificationId)) {
            persistentNotifications.remove(notificationId);
            savePersistentNotifications();
        }

        return true;
    } else {
        return false;
    }
}

bool NotificationManager::removeNotificationsInGroup(uint groupId)
{
    QList<uint> notificationIds;

    foreach(const Notification & notification, notifications.values()) {
        if (notification.groupId() == groupId) {
            notificationIds.append(notification.notificationId());
        }
    }

    bool result = !notificationIds.isEmpty();
    foreach(uint notificationId, notificationIds) {
        result &= removeNotification(notificationId);
    }
    return result;
}

uint NotificationManager::addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent)
{
    restorePersistentData();

    uint groupID = nextAvailableGroupID();
    NotificationGroup group(groupID, notificationUserId, parameters);
    groups.insert(groupID, group);
    emit groupUpdated(groupID, parameters);

    if (persistent) {
        persistentGroups.insert(groupID);
    }

    saveStateData();

    return groupID;
}

bool NotificationManager::updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters)
{
    Q_UNUSED(notificationUserId);

    restorePersistentData();

    QHash<uint, NotificationGroup>::iterator gi = groups.find(groupId);

    if (gi != groups.end()) {
        (*gi).setParameters(parameters);
        emit groupUpdated(groupId, parameters);

        saveStateData();

        return true;
    } else {
        return false;
    }
}

bool NotificationManager::removeGroup(uint notificationUserId, uint groupId)
{
    restorePersistentData();

    if (groups.remove(groupId)) {
        foreach(const Notification & notification, notifications) {
            if (notification.groupId() == groupId) {
                removeNotification(notificationUserId, notification.notificationId());
            }
        }

        emit groupRemoved(groupId);

        if (persistentGroups.contains(groupId)) {
            persistentGroups.remove(groupId);
        }

        saveStateData();

        return true;
    } else {
        return false;
    }
}

uint NotificationManager::notificationUserId()
{
    restorePersistentData();

    lastUsedNotificationUserId++;
    saveStateData();

    return lastUsedNotificationUserId;
}

const EventTypeStore &NotificationManager::eventTypeStore() const
{
    return *notificationEventTypeStore.data();
}

QList<uint> NotificationManager::notificationIdList(uint notificationUserId)
{
    QList<uint> listOfNotificationIds;

    foreach(const Notification & notification, notifications) {
        if (notification.userId() == notificationUserId) {
            listOfNotificationIds.append(notification.notificationId());
        }
    }

    return listOfNotificationIds;
}

void NotificationManager::relayNextNotification()
{
    notificationInProgress = false;
    if (!waitQueue.isEmpty()) {
        submitNotification(waitQueue.takeFirst());
    }
}

void NotificationManager::submitNotification(const Notification &notification)
{
    if (!notificationInProgress) {
        // Inform about the new notification
        emit notificationUpdated(notification);

        if (relayInterval != 0) {
            notificationInProgress = true;
            if (relayInterval > 0) {
                waitQueueTimer.start(relayInterval);
            }
        }
    } else {
        // Store new notification in the notification wait queue
        if ((uint)waitQueue.size() < maxWaitQueueSize) {
            waitQueue.append(notification);
        }
    }
}

int NotificationManager::findNotificationFromWaitQueue(uint notificationId)
{
    for (int i = 0; i < waitQueue.count(); ++i) {
        if (waitQueue[i].notificationId() == notificationId) {
            return i;
        }
    }
    return -1;
}

uint NotificationManager::nextAvailableNotificationID()
{
    unsigned int i = 1;
    // Try to find an unused ID but only do it up to 2^32-1 times
    while (i != 0 && notifications.contains(i)) {
        ++i;
    }
    return i;
}

uint NotificationManager::nextAvailableGroupID()
{
    unsigned int i = 1;
    // Try to find an unused ID but only do it up to 2^32-1 times
    while (i != 0 && groups.contains(i)) {
        ++i;
    }
    return i;
}
