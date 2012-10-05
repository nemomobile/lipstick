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
    foreach(uint id, NotificationManager::instance()->notificationIds()) {
        updateNotification(id);
    }

    connect(NotificationManager::instance(), SIGNAL(notificationModified(uint)), this, SLOT(updateNotification(uint)));
    connect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), this, SLOT(removeNotification(uint)));
}

NotificationListModel::~NotificationListModel()
{
}

void NotificationListModel::updateNotification(uint id)
{
    Notification *notification = NotificationManager::instance()->notification(id);
    if (indexOf(notification) < 0 && notification->hints().value(NotificationManager::HINT_CLASS).toString() != "system") {
        addItem(notification);
    } else if (indexOf(notification) >= 0 && notification->hints().value(NotificationManager::HINT_CLASS).toString() == "system") {
        removeItem(notification);
    }
}

void NotificationListModel::removeNotification(uint id)
{
    removeItem(NotificationManager::instance()->notification(id));
}
