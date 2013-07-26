/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "notificationmanager.h"
#include "notificationlistmodel.h"

NotificationListModel::NotificationListModel(QObject *parent) :
    QObjectListModel(parent)
{
    connect(NotificationManager::instance(), SIGNAL(notificationModified(uint)), this, SLOT(updateNotification(uint)));
    connect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), this, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(clearRequested()), NotificationManager::instance(), SLOT(removeUserRemovableNotifications()));

    QTimer::singleShot(0, this, SLOT(init()));
}

NotificationListModel::~NotificationListModel()
{
}

void NotificationListModel::init()
{
    foreach(uint id, NotificationManager::instance()->notificationIds()) {
        updateNotification(id);
    }
}

void NotificationListModel::updateNotification(uint id)
{
    LipstickNotification *notification = NotificationManager::instance()->notification(id);
    // This can happen if the notification is removed by someone else connected to
    // the notificationModified() signal
    if (!notification)
        return;

    int index = indexOf(notification);
    if (notificationShouldBeShown(notification)) {
        // Place the notifications in the model latest first, moving existing notifications if necessary
        int expectedIndex = indexFor(notification);
        if (index < 0) {
            insertItem(expectedIndex, notification);
        } else if (index != expectedIndex) {
            move(index, expectedIndex);
        }
    } else if (index >= 0) {
        removeItem(notification);
    }
}

int NotificationListModel::indexFor(LipstickNotification *notification)
{
    for (int index = 0; index < itemCount(); index++) {
        LipstickNotification *notificationAtIndex = static_cast<LipstickNotification *>(get(index));
        if (notificationAtIndex->timestamp() <= notification->timestamp()) {
            return index;
        }
    }
    return itemCount();
}

void NotificationListModel::removeNotification(uint id)
{
    removeItem(NotificationManager::instance()->notification(id));
}

bool NotificationListModel::notificationShouldBeShown(LipstickNotification *notification)
{
    return !notification->hints().value(NotificationManager::HINT_HIDDEN).toBool() && !(notification->body().isEmpty() && notification->summary().isEmpty()) && notification->hints().value(NotificationManager::HINT_URGENCY).toInt() < 2;
}
