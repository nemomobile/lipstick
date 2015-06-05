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
#include "lipstickcompositor.h"
#include "notificationmanager.h"
#include "notificationfeedbackplayer.h"

namespace {

enum PreviewMode {
    AllNotificationsEnabled = 0,
    ApplicationNotificationsDisabled,
    SystemNotificationsDisabled,
    AllNotificationsDisabled
};

}

NotificationFeedbackPlayer::NotificationFeedbackPlayer(QObject *parent) :
    QObject(parent),
    ngfClient(new Ngf::Client(this)),
    minimumPriority_(0)
{
    connect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), this, SLOT(removeNotification(uint)));

    QTimer::singleShot(0, this, SLOT(init()));
}

void NotificationFeedbackPlayer::init()
{
    ngfClient->connect();

    foreach(uint id, NotificationManager::instance()->notificationIds()) {
        idToEventId.insert(NotificationManager::instance()->notification(id), 0);
    }
}

void NotificationFeedbackPlayer::addNotification(uint id)
{
    LipstickNotification *notification = NotificationManager::instance()->notification(id);

    if (notification != 0 && !idToEventId.contains(notification) && isEnabled(notification)) {
        // Play the feedback related to the notification if any
        QString feedback = notification->hints().value(NotificationManager::HINT_FEEDBACK).toString();
        if (!feedback.isEmpty()) {
            QMap<QString, QVariant> properties;
            if (notification->hints().value(NotificationManager::HINT_LED_DISABLED_WITHOUT_BODY_AND_SUMMARY, true).toBool() && notification->body().isEmpty() && notification->summary().isEmpty()) {
                properties.insert("media.leds", false);
                properties.insert("media.audio", true);
                properties.insert("media.vibra", true);
                properties.insert("media.backlight", true);
            }
            idToEventId.insert(notification, ngfClient->play(feedback, properties));
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

    int urgency = notification->urgency();
    int priority = notification->priority();
    int notificationIsCritical = urgency >= 2 || notification->hints().value(NotificationManager::HINT_DISPLAY_ON).toBool();

    return !(urgency < 2 && priority < minimumPriority_) &&
           (mode == AllNotificationsEnabled ||
            (mode == ApplicationNotificationsDisabled && notificationIsCritical) ||
            (mode == SystemNotificationsDisabled && urgency < 2));
}

int NotificationFeedbackPlayer::minimumPriority() const
{
    return minimumPriority_;
}

void NotificationFeedbackPlayer::setMinimumPriority(int minimumPriority)
{
    minimumPriority_ = minimumPriority;

    emit minimumPriorityChanged();
}
