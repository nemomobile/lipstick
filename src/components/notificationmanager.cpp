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

#include <QCoreApplication>
#include "notificationmanageradaptor.h"
#include "notificationmanager.h"

NotificationManager::NotificationManager(QObject *parent) :
    QObject(parent),
    previousNotificationID(0)
{
    new NotificationManagerAdaptor(this);
    QDBusConnection::sessionBus().registerService("org.freedesktop.Notifications");
    QDBusConnection::sessionBus().registerObject("/org/freedesktop/Notifications", this);
}

QStringList NotificationManager::GetCapabilities()
{
    return QStringList() << "body";
}

uint NotificationManager::Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, NotificationHints hints, int expire_timeout)
{
    uint id = replaces_id != 0 ? replaces_id : nextAvailableNotificationID();

    if (replaces_id == 0 || notifications.contains(id)) {
        // Create a new notification if not replacing an existing one. Only replace an existing one if it really exists.
        Notification notification(app_name, app_icon, summary, body, actions, hints, expire_timeout);
        notifications.insert(id, notification);
    }

    return id;
}

void NotificationManager::CloseNotification(uint id)
{
    if (notifications.contains(id)) {
        notifications.remove(id);
        emit NotificationClosed(id, CloseNotificationCalled);
    }
}

QString NotificationManager::GetServerInformation(QString &name, QString &vendor, QString &version)
{
    name = qApp->applicationName();
    vendor = "Nemo Mobile";
    version = qApp->applicationVersion();
    return QString();
}

uint NotificationManager::nextAvailableNotificationID()
{
    // Try to find an unused ID but only do it up to 2^32-1 times
    for (uint i = 0; i < UINT32_MAX && notifications.contains(previousNotificationID); i++) {
        previousNotificationID++;

        if (previousNotificationID == 0) {
            // 0 is not a valid ID so skip it
            previousNotificationID = 1;
        }
    }

    return previousNotificationID;
}
