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

#include <QtTest/QtTest>
#include "notificationmanager.h"
#include "notificationfeedbackplayer.h"
#include "ngfclient_stub.h"
#include "ut_notificationfeedbackplayer.h"

const char *NotificationManager::HINT_CATEGORY = "category";
const char *NotificationManager::HINT_URGENCY = "urgency";
const char *NotificationManager::HINT_ICON = "x-nemo-icon";
const char *NotificationManager::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationManager::HINT_PRIORITY = "x-nemo-priority";
const char *NotificationManager::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationManager::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationManager::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationManager::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationManager::HINT_FEEDBACK = "x-nemo-feedback";

NotificationManager::NotificationManager(QObject *parent) : QObject(parent)
{
}

NotificationManager::~NotificationManager()
{
}

void NotificationManager::invokeAction(const QString &)
{
}

void NotificationManager::removeNotificationsWithCategory(const QString &)
{
}

void NotificationManager::updateNotificationsWithCategory(const QString &)
{
}

void NotificationManager::commit()
{
}

void NotificationManager::removeUserRemovableNotifications()
{
}

NotificationManager *notificationManagerInstance = 0;
NotificationManager *NotificationManager::instance()
{
    if (notificationManagerInstance == 0) {
        notificationManagerInstance = new NotificationManager;
    }
    return notificationManagerInstance;
}

QHash<uint, Notification *> notificationManagerNotification;
Notification *NotificationManager::notification(uint id) const
{
    return notificationManagerNotification.value(id);
}

Notification *createNotification(uint id)
{
    Notification *notification = new Notification;
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_FEEDBACK, "feedback");
    notification->setHints(hints);
    notificationManagerNotification.insert(id, notification);
    return notification;
}

void Ut_NotificationFeedbackPlayer::init()
{
    player = new NotificationFeedbackPlayer;
}

void Ut_NotificationFeedbackPlayer::cleanup()
{
    delete player;

    gClientStub->stubReset();
}

void Ut_NotificationFeedbackPlayer::testAddAndRemoveNotification()
{
    gClientStub->stubSetReturnValue("play", (quint32)1);

    // Create a notification
    createNotification(1);
    player->addNotification(1);

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(gClientStub->stubCallCount("play"), 1);
    QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QString>(0), QString("feedback"));

    // Remove the notification
    player->removeNotification(1);

    // Check that NGFAdapter::stop() was called for the notification
    QCOMPARE(gClientStub->stubCallCount("stop"), 1);
    QCOMPARE(gClientStub->stubLastCallTo("stop").parameter<quint32>(0), (quint32)1);

    // Check that NGFAdapter::stop() is not called for an already stopped notification
    player->removeNotification(1);
    QCOMPARE(gClientStub->stubCallCount("stop"), 1);
}

void Ut_NotificationFeedbackPlayer::testWithoutFeedbackId()
{
    // Create a notification
    Notification *notification = createNotification(1);
    notification->setHints(QVariantHash());
    player->addNotification(1);

    // Check that NGFAdapter::play() was not called for the feedback
    QCOMPARE(gClientStub->stubCallCount("play"), 0);
}

void Ut_NotificationFeedbackPlayer::testUpdateNotificationIsNotPossible()
{
    // Create a notification
    Notification *notification = createNotification(1);
    player->addNotification(1);

    // Update the notification
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_FEEDBACK, "feedback2");
    notification->setHints(hints);
    player->addNotification(1);

    // Check that NGFAdapter::play() was only called for the first feedback
    QCOMPARE(gClientStub->stubCallCount("play"), 1);
    QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QString>(0), QString("feedback"));
}

QTEST_MAIN(Ut_NotificationFeedbackPlayer)
