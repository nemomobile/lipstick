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

#include <NgfClient>
#include <QWaylandSurface>
#include "notificationmanager.h"
#include "notificationfeedbackplayer.h"
#include "lipstickcompositor.h"

enum PreviewMode {
    AllNotificationsEnabled = 0,
    ApplicationNotificationsDisabled,
    SystemNotificationsDisabled,
    AllNotificationsDisabled
};

NotificationFeedbackPlayer::NotificationFeedbackPlayer(QObject *parent) :
    QObject(parent),
    ngfClient(new Ngf::Client(this))
{
    ngfClient->connect();

    connect(NotificationManager::instance(), SIGNAL(notificationModified(uint)), this, SLOT(addNotification(uint)));
    connect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), this, SLOT(removeNotification(uint)));
}

void NotificationFeedbackPlayer::addNotification(uint id)
{
    LipstickNotification *notification = NotificationManager::instance()->notification(id);

    if (notification != 0) {
        // Play the feedback related to the notification if any
        if (!idToEventId.contains(notification) && isEnabled(notification)) {
            QString feedback = notification->hints().value(NotificationManager::HINT_FEEDBACK).toString();
            if (!feedback.isEmpty()) {
                idToEventId.insert(notification, ngfClient->play(feedback, QMap<QString, QVariant>()));
            }
        }
    }
}

void NotificationFeedbackPlayer::removeNotification(uint id)
{
    LipstickNotification *notification = NotificationManager::instance()->notification(id);

    if (notification != 0) {
        // Stop the feedback related to the notification, if any
        uint eventId = idToEventId.take(notification);
        if (eventId != 0) {
            ngfClient->stop(eventId);
        }
    }
}

bool NotificationFeedbackPlayer::isEnabled(LipstickNotification *notification)
{
    uint mode = AllNotificationsEnabled;
    QWaylandSurface *surface = LipstickCompositor::instance()->surfaceForId(LipstickCompositor::instance()->topmostWindowId());
    if (surface != 0) {
        mode = surface->windowProperties().value("NOTIFICATION_PREVIEWS_DISABLED", uint(AllNotificationsEnabled)).toUInt();
    }

    return mode == AllNotificationsEnabled ||
           (mode == ApplicationNotificationsDisabled && notification->hints().value(NotificationManager::HINT_URGENCY).toInt() >= 2) ||
           (mode == SystemNotificationsDisabled && notification->hints().value(NotificationManager::HINT_URGENCY).toInt() < 2);
}
