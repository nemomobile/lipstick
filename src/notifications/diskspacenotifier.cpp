/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012-2013 Jolla Ltd.
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

#include <QDBusConnection>
#include <QTimer>
#include "homeapplication.h"
#include "lipsticknotification.h"
#include "notificationmanager.h"
#include "diskspacenotifier.h"

DiskSpaceNotifier::DiskSpaceNotifier(QObject *parent) : QObject(parent),
    notificationId(0)
{
    QDBusConnection::systemBus().connect(QString(), "/com/nokia/diskmonitor/signal", "com.nokia.diskmonitor.signal", "disk_space_change_ind", this, SLOT(handleDiskSpaceChange(QString, int)));

    QTimer::singleShot(0, this, SLOT(removeDiskSpaceNotifications()));
}

DiskSpaceNotifier::~DiskSpaceNotifier()
{
}

void DiskSpaceNotifier::handleDiskSpaceChange(const QString &path, int percentage)
{
    bool notificationShouldBeVisible = false;

    if (percentage == 100) {
        // Disk space usage for path is 100%
        if (!notificationsSentForPath[path].second) {
            notificationShouldBeVisible = true;
            notificationsSentForPath[path].second = true;
        }
    } else {
        // Disk space usage for path is above the notification threshold
        if (!notificationsSentForPath[path].first) {
            notificationShouldBeVisible = true;
            notificationsSentForPath[path].first = true;
        }
    }

    if (notificationShouldBeVisible) {
        if (notificationId != 0) {
            // Destroy any previous notification
            NotificationManager *manager = NotificationManager::instance();
            manager->CloseNotification(notificationId);
        }

        // Show a system notification
        NotificationManager *manager = NotificationManager::instance();
        QVariantHash hints;
        hints.insert(NotificationManager::HINT_CATEGORY, "x-nemo.system.diskspace");
        //% "Getting low with storage. Please check."
        QString diskLowText = qtTrId("qtn_memu_memlow_notification_src");
        hints.insert(NotificationManager::HINT_PREVIEW_BODY, diskLowText);
        // TODO go to some relevant place when clicking the notification
        notificationId = manager->Notify(qApp->applicationName(), 0, QString(), QString(), QString(), QStringList(), hints, -1);

        bool nonSystemNotificationFound = false;
        NotificationList notifications = manager->GetNotifications(qApp->applicationName());
        foreach (LipstickNotification* notification, notifications.notifications()) {
            if (notification->category() == "x-nemo.diskspace.low") {
                nonSystemNotificationFound = true;
                break;
            }
        }
        if (!nonSystemNotificationFound) {
            // Show a non-system notification
            // TODO: Figure out if this could be combined with the system notification. Currently
            //       the system notifications are deleted by NotificationPreviewPresenter class
            //       after they've been displayed.
            hints.clear();
            hints.insert(NotificationManager::HINT_CATEGORY, "x-nemo.diskspace.low");
            manager->Notify(qApp->applicationName(), 0, QString(), diskLowText, QString(), QStringList(), hints, -1);
        }
    }
}

void DiskSpaceNotifier::removeDiskSpaceNotifications()
{
    NotificationManager *manager = NotificationManager::instance();
    foreach (uint id, manager->notificationIds()) {
        LipstickNotification *notification = NotificationManager::instance()->notification(id);
        if (notification->appName() == qApp->applicationName() && notification->category() == "x-nemo.system.diskspace") {
            manager->CloseNotification(id);
        }
    }
}
