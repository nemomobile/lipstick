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
    QObjectListModel(parent),
    m_populated(false)
{
    connect(NotificationManager::instance(), SIGNAL(notificationModified(uint)), this, SLOT(updateNotification(uint)));
    connect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), this, SLOT(removeNotification(uint)));
    connect(NotificationManager::instance(), SIGNAL(notificationsRemoved(const QList<uint> &)), this, SLOT(removeNotifications(const QList<uint> &)));
    connect(this, SIGNAL(clearRequested()), NotificationManager::instance(), SLOT(removeUserRemovableNotifications()));

    QTimer::singleShot(0, this, SLOT(init()));
}

NotificationListModel::~NotificationListModel()
{
}

bool NotificationListModel::populated() const
{
    return m_populated;
}

void NotificationListModel::init()
{
    if (m_populated) {
        foreach(uint id, NotificationManager::instance()->notificationIds()) {
            updateNotification(id);
        }
    } else {
        QList<QObject *> initialNotifications;

        foreach(uint id, NotificationManager::instance()->notificationIds()) {
            LipstickNotification *notification = NotificationManager::instance()->notification(id);
            if (notificationShouldBeShown(notification)) {
                initialNotifications.append(notification);
            }
        }

        addItems(initialNotifications);
    }

    m_populated = true;
    emit populatedChanged(m_populated);
}

void NotificationListModel::updateNotification(uint id)
{
    LipstickNotification *notification = NotificationManager::instance()->notification(id);

    if (notification != 0) {
        int index = indexOf(notification);
        if (notificationShouldBeShown(notification)) {
            // Place the notifications in the model latest first, moving existing notifications if necessary
            int expectedIndex = indexFor(notification);
            if (index < 0) {
                insertItem(expectedIndex, notification);
            } else if (index != expectedIndex) {
                move(index, expectedIndex);
            } else {
                update(index);
            }
        } else if (index >= 0) {
            removeItem(notification);
        }
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

void NotificationListModel::refreshModel()
{
    init();
}

void NotificationListModel::markAsDisplayed(uint id)
{
    NotificationManager::instance()->MarkNotificationDisplayed(id);
}

void NotificationListModel::removeNotification(uint id)
{
    if (LipstickNotification *notification = NotificationManager::instance()->notification(id)) {
        removeItem(notification);
    }
}

void NotificationListModel::removeNotifications(const QList<uint> &ids)
{
    if (!ids.isEmpty()) {
        QList<QObject *> items;
        foreach (uint id, ids) {
            if (LipstickNotification *notification = NotificationManager::instance()->notification(id)) {
                items.append(notification);
            }
        }
        removeItems(items);
    }
}

bool NotificationListModel::notificationShouldBeShown(LipstickNotification *notification)
{
    return !notification->hidden() && (!notification->body().isEmpty() || !notification->summary().isEmpty());
}
