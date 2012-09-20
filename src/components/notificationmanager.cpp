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

#include "notificationmanageradaptor.h"
#include "notificationmanager.h"

NotificationManager::NotificationManager(QObject *parent) :
    QObject(parent)
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
    Q_UNUSED(app_name)
    Q_UNUSED(replaces_id)
    Q_UNUSED(app_icon)
    Q_UNUSED(summary)
    Q_UNUSED(body)
    Q_UNUSED(actions)
    Q_UNUSED(hints)
    Q_UNUSED(expire_timeout)
    return 0;
}

void NotificationManager::CloseNotification(uint id)
{
    Q_UNUSED(id)
}

QString NotificationManager::GetServerInformation(QString &name, QString &vendor, QString &version)
{
    Q_UNUSED(name)
    Q_UNUSED(vendor)
    Q_UNUSED(version)
    return QString();
}
