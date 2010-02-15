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

#include "notifiernotificationsink.h"
#include "genericnotificationparameterfactory.h"

NotifierNotificationSink::NotifierNotificationSink() :
    notificationCount(0)
{
}

NotifierNotificationSink::~NotifierNotificationSink()
{
}


void NotifierNotificationSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    // Set the notification count for the group
    notificationCountForGroup.insert(groupId, parameters.value(GenericNotificationParameterFactory::countKey()).toUInt());

    updateNotificationCount();
}

void NotifierNotificationSink::removeGroup(uint groupId)
{
    // Remove all notifications in the group
    QList<uint> notificationsToRemove;
    foreach(uint notificationId, groupForNotification.keys()) {
        if (groupForNotification.value(notificationId) == groupId) {
            notificationsToRemove.append(notificationId);
        }
    }

    foreach(uint notificationId, notificationsToRemove) {
        groupForNotification.remove(notificationId);
    }

    // Reset the notification count of the group
    notificationCountForGroup.remove(groupId);

    updateNotificationCount();
}

void NotifierNotificationSink::addNotification(const Notification &notification)
{
    // Remove any previous notification with the same ID (so that updates work properly)
    removeNotification(notification.notificationId());

    // Create a mapping between the notification ID and its group ID
    groupForNotification.insert(notification.notificationId(), notification.groupId());

    // Create a mapping between the notification ID and its notification count
    uint count = notification.parameters().value(GenericNotificationParameterFactory::countKey()).toUInt();
    notificationCountForNotification.insert(notification.notificationId(), count);

    // Only the group 0 (no group) notification count is affected by the item count of the notification
    if (notification.groupId() == 0) {
        // Increase the notification count for the "no group" group
        notificationCountForGroup.insert(notification.groupId(), notificationCountForGroup.value(notification.groupId()) + count);
    }

    updateNotificationCount();
}

void NotifierNotificationSink::removeNotification(uint notificationId)
{
    if (groupForNotification.contains(notificationId)) {
        // Remove the mapping between the notification ID and its notification count
        uint count = notificationCountForNotification.take(notificationId);

        // Remove the mapping between the notification ID and its group ID
        uint groupId = groupForNotification.take(notificationId);

        // Only the group 0 (no group) notification count is affected by the item count of the notification
        if (groupId == 0) {
            // Decrease the notification count for the group
            notificationCountForGroup.insert(groupId, notificationCountForGroup.value(groupId) - count);
        }

        updateNotificationCount();
    }
}

void NotifierNotificationSink::updateNotificationCount()
{
    uint count = 0;

    foreach(uint groupId, notificationCountForGroup.keys()) {
        uint c = notificationCountForGroup.value(groupId);

        if (c > 0 && groupForNotification.values().contains(groupId)) {
            // Increase the count if there are notifications in the group
            count += c;
        }
    }

    if (count != notificationCount) {
        // The notification count has changed: signal it
        notificationCount = count;

        emit notificationCountChanged(notificationCount);
    }
}
