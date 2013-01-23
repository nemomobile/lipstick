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
#include "notificationmanager.h"
#include "notificationfeedbackplayer.h"

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
    Notification *notification = NotificationManager::instance()->notification(id);

    // Play the feedback related to the notification if any
    if (!idToEventId.contains(notification)) {
        QString feedback = notification->hints().value(NotificationManager::HINT_FEEDBACK).toString();
        if (!feedback.isEmpty()) {
            idToEventId.insert(notification, ngfClient->play(feedback, QMap<QString, QVariant>()));
        }
    }
}

void NotificationFeedbackPlayer::removeNotification(uint id)
{
    Notification *notification = NotificationManager::instance()->notification(id);

    if (notification != 0) {
        // Stop the feedback related to the notification, if any
        uint eventId = idToEventId.take(notification);
        if (eventId != 0) {
            ngfClient->stop(eventId);
        }
    }
}
