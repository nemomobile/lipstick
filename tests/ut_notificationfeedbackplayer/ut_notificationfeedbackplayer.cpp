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
#include "notificationpreviewpresenter_stub.h"
#include "lipstickcompositor_stub.h"
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
const char *NotificationManager::HINT_USER_REMOVABLE = "x-nemo-user-removable";

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

QHash<uint, LipstickNotification *> notificationManagerNotification;
LipstickNotification *NotificationManager::notification(uint id) const
{
    return notificationManagerNotification.value(id);
}

QList<uint> NotificationManager::notificationIds() const
{
    return notificationManagerNotification.keys();
}

LipstickNotification *createNotification(uint id, int urgency = 0)
{
    LipstickNotification *notification = new LipstickNotification;
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_FEEDBACK, "feedback");
    hints.insert(NotificationManager::HINT_URGENCY, urgency);
    notification->setHints(hints);
    notificationManagerNotification.insert(id, notification);
    gNotificationPreviewPresenterStub->stubSetReturnValue("notification", notification);
    return notification;
}

QVariantMap qWaylandSurfaceWindowProperties;
QVariantMap QWaylandSurface::windowProperties() const
{
    return qWaylandSurfaceWindowProperties;
}

void QTimer::singleShot(int, const QObject *receiver, const char *member)
{
    // The "member" string is of form "1member()", so remove the trailing 1 and the ()
    int memberLength = strlen(member) - 3;
    char modifiedMember[memberLength + 1];
    strncpy(modifiedMember, member + 1, memberLength);
    modifiedMember[memberLength] = 0;
    QMetaObject::invokeMethod(const_cast<QObject *>(receiver), modifiedMember, Qt::DirectConnection);
}

void Ut_NotificationFeedbackPlayer::initTestCase()
{
    gLipstickCompositorStub->stubSetReturnValue("instance", new LipstickCompositor());
}

void Ut_NotificationFeedbackPlayer::init()
{
    presenter = new NotificationPreviewPresenter();
    player = new NotificationFeedbackPlayer(presenter);
}

void Ut_NotificationFeedbackPlayer::cleanup()
{
    delete player;
    delete presenter;

    gClientStub->stubReset();
    gNotificationPreviewPresenterStub->stubReset();
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
    LipstickNotification *notification = createNotification(1);
    notification->setHints(QVariantHash());
    player->addNotification(1);

    // Check that NGFAdapter::play() was not called for the feedback
    QCOMPARE(gClientStub->stubCallCount("play"), 0);
}

void Ut_NotificationFeedbackPlayer::testUpdateNotificationIsNotPossible()
{
    // Create a notification
    LipstickNotification *notification = createNotification(1);
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

void Ut_NotificationFeedbackPlayer::testUpdateNotificationIsNotPossibleAfterRestart()
{
    delete player;

    // Create a notification
    createNotification(1);
    player = new NotificationFeedbackPlayer(presenter);

    // Update the notification
    player->addNotification(1);

    // Check that NGFAdapter::play() was not called
    QCOMPARE(gClientStub->stubCallCount("play"), 0);
}

QWaylandSurface surface;
void Ut_NotificationFeedbackPlayer::testNotificationPreviewsDisabled_data()
{
    QTest::addColumn<QWaylandSurface *>("surface");
    QTest::addColumn<QVariantMap>("windowProperties");
    QTest::addColumn<int>("urgency");
    QTest::addColumn<int>("playCount");

    QVariantMap allNotificationsEnabled;
    QVariantMap applicationNotificationsDisabled;
    QVariantMap systemNotificationsDisabled;
    QVariantMap allNotificationsDisabled;
    allNotificationsEnabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 0);
    applicationNotificationsDisabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 1);
    systemNotificationsDisabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 2);
    allNotificationsDisabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 3);
    QTest::newRow("No surface, application notification") << (QWaylandSurface *)0 << QVariantMap() << 1 << 1;
    QTest::newRow("Surface, no properties, application notification") << &surface << QVariantMap() << 1 << 1;
    QTest::newRow("Surface, all notifications enabled, application notification") << &surface << allNotificationsEnabled << 1 << 1;
    QTest::newRow("Surface, application notifications disabled, application notification") << &surface << applicationNotificationsDisabled << 1 << 0;
    QTest::newRow("Surface, system notifications disabled, application notification") << &surface << systemNotificationsDisabled << 1 << 1;
    QTest::newRow("Surface, all notifications disabled, application notification") << &surface << allNotificationsDisabled << 1 << 0;
    QTest::newRow("No surface, system notification") << (QWaylandSurface *)0 << QVariantMap() << 2 << 1;
    QTest::newRow("Surface, no properties, system notification") << &surface << QVariantMap() << 2 << 1;
    QTest::newRow("Surface, all notifications enabled, system notification") << &surface << allNotificationsEnabled << 2 << 1;
    QTest::newRow("Surface, application notifications disabled, system notification") << &surface << applicationNotificationsDisabled << 2 << 1;
    QTest::newRow("Surface, system notifications disabled, system notification") << &surface << systemNotificationsDisabled << 2 << 0;
    QTest::newRow("Surface, all notifications disabled, system notification") << &surface << allNotificationsDisabled << 2 << 0;
}

void Ut_NotificationFeedbackPlayer::testNotificationPreviewsDisabled()
{
    QFETCH(QWaylandSurface *, surface);
    QFETCH(QVariantMap, windowProperties);
    QFETCH(int, urgency);
    QFETCH(int, playCount);

    gLipstickCompositorStub->stubSetReturnValue("surfaceForId", surface);
    qWaylandSurfaceWindowProperties = windowProperties;

    createNotification(1, urgency);
    player->addNotification(1);

    QCOMPARE(gClientStub->stubCallCount("play"), playCount);
}

QTEST_MAIN(Ut_NotificationFeedbackPlayer)
