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
#include <QDebug>
#include "categorydefinitionstore.h"
#include "notificationmanageradaptor.h"
#include "notificationmanager.h"

// Define this if you'd like to see debug messages from the notification manager
#ifdef DEBUG_NOTIFICATIONS
#define NOTIFICATIONS_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define NOTIFICATIONS_DEBUG(things)
#endif

//! The category definitions directory
static const char *CATEGORY_DEFINITION_FILE_DIRECTORY = "/usr/share/nemo/notifications/categories";

//! The number configuration files to load into the event type store.
static const uint MAX_CATEGORY_DEFINITION_FILES = 100;

NotificationManager *NotificationManager::instance_ = 0;

NotificationManager *NotificationManager::instance()
{
    if (instance_ == 0) {
        instance_ = new NotificationManager(qApp);
    }
    return instance_;
}

NotificationManager::NotificationManager(QObject *parent) :
    QObject(parent),
    previousNotificationID(1),
    categoryDefinitionStore(new CategoryDefinitionStore(CATEGORY_DEFINITION_FILE_DIRECTORY, MAX_CATEGORY_DEFINITION_FILES, this))
{
    qDBusRegisterMetaType<NotificationHints>();
    new NotificationManagerAdaptor(this);
    QDBusConnection::sessionBus().registerService("org.freedesktop.Notifications");
    QDBusConnection::sessionBus().registerObject("/org/freedesktop/Notifications", this);

    connect(categoryDefinitionStore, SIGNAL(eventTypeUninstalled(QString)), this, SLOT(removeNotificationsWithEventType(QString)));
    connect(categoryDefinitionStore, SIGNAL(eventTypeModified(QString)), this, SLOT(updateNotificationsWithEventType(QString)));
}

QStringList NotificationManager::GetCapabilities()
{
    return QStringList() << "body";
}

uint NotificationManager::Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, NotificationHints hints, int expireTimeout)
{
    uint id = replacesId != 0 ? replacesId : nextAvailableNotificationID();

    if (replacesId == 0 || notifications.contains(id)) {
        // Create a new notification if not replacing an existing one. Only replace an existing one if it really exists.
        Notification notification(appName, appIcon, summary, body, actions, hints, expireTimeout);
        applyCategoryDefinition(notification, hints.hintValue(NotificationHints::HINT_CATEGORY).toString());
        notifications.insert(id, notification);

        NOTIFICATIONS_DEBUG("NOTIFY:" << notification.appName() << notification.appIcon() << notification.summary() << notification.body() << notification.actions() << notification.hints() << notification.expireTimeout() << "->" << id);
        emit notificationModified(id);
    }

    return id;
}

void NotificationManager::CloseNotification(uint id)
{
    if (notifications.contains(id)) {
        notifications.remove(id);
        emit NotificationClosed(id, CloseNotificationCalled);

        NOTIFICATIONS_DEBUG("REMOVE:" << id);
        emit notificationRemoved(id);
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

void NotificationManager::removeNotificationsWithEventType(const QString &eventType)
{
    foreach(uint id, notifications.keys()) {
        if (notifications[id].hints().hintValue("category").toString() == eventType) {
            CloseNotification(id);
        }
    }
}

void NotificationManager::updateNotificationsWithEventType(const QString &eventType)
{
    foreach(uint id, notifications.keys()) {
        if (notifications[id].hints().hintValue("category").toString() == eventType) {
            Notify(notifications[id].appName(), id, notifications[id].appIcon(), notifications[id].summary(), notifications[id].body(), notifications[id].actions(), notifications[id].hints(), notifications[id].expireTimeout());
        }
    }
}

void NotificationManager::applyCategoryDefinition(Notification &notification, const QString &category)
{
    if (!category.isEmpty()) {
        NotificationHints hints = notification.hints();
        foreach (const QString &key, categoryDefinitionStore->allKeys(category)) {
            if (!hints.hintValue(key).isValid()) {
                hints.setHint(key, categoryDefinitionStore->value(category, key));
            }
        }
        notification.setHints(hints);
    }
}
