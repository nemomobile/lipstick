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
const char *NotificationManager::HINT_HIDDEN = "x-nemo-hidden";
const char *NotificationManager::HINT_REMOTE_ACTION_PREFIX = "x-nemo-remote-action-";
const char *NotificationManager::HINT_REMOTE_ACTION_ICON_PREFIX = "x-nemo-remote-action-icon-";
const char *NotificationManager::HINT_FEEDBACK = "x-nemo-feedback";
const char *NotificationManager::HINT_USER_REMOVABLE = "x-nemo-user-removable";
const char *NotificationManager::HINT_DISPLAY_ON = "x-nemo-display-on";
const char *NotificationManager::HINT_LED_DISABLED_WITHOUT_BODY_AND_SUMMARY = "x-nemo-led-disabled-without-body-and-summary";
const char *NotificationManager::HINT_ORIGIN = "x-nemo-origin";
const char *NotificationManager::HINT_OWNER = "x-nemo-owner";
const char *NotificationManager::HINT_MAX_CONTENT_LINES = "x-nemo-max-content-lines";

NotificationManager::NotificationManager(QObject *parent) : QObject(parent)
{
}

NotificationManager::~NotificationManager()
{
}

void NotificationManager::invokeAction(const QString &)
{
}

void NotificationManager::removeNotificationIfUserRemovable(uint)
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

void NotificationManager::expire()
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

LipstickNotification *createNotification(uint id, int urgency = 0, QVariant priority = QVariant())
{
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_FEEDBACK, "feedback");
    hints.insert(NotificationManager::HINT_URGENCY, urgency);
    if (priority.isValid()) {
        hints.insert(NotificationManager::HINT_PRIORITY, priority);
    }
    LipstickNotification *notification = new LipstickNotification("ut_notificationfeedbackplayer", id, "", "", "", QStringList(), hints, -1);
    notificationManagerNotification.insert(id, notification);
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
    player = new NotificationFeedbackPlayer();
}

void Ut_NotificationFeedbackPlayer::cleanup()
{
    delete player;

    gClientStub->stubReset();
    gLipstickCompositorStub->stubSetReturnValue("surfaceForId", (QWaylandSurface *)0);
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
    player = new NotificationFeedbackPlayer();

    // Update the notification
    player->addNotification(1);

    // Check that NGFAdapter::play() was not called
    QCOMPARE(gClientStub->stubCallCount("play"), 0);
}

QWaylandSurface *surface = (QWaylandSurface *)1;
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
    QTest::newRow("Surface, no properties, application notification") << surface << QVariantMap() << 1 << 1;
    QTest::newRow("Surface, all notifications enabled, application notification") << surface << allNotificationsEnabled << 1 << 1;
    QTest::newRow("Surface, application notifications disabled, application notification") << surface << applicationNotificationsDisabled << 1 << 0;
    QTest::newRow("Surface, system notifications disabled, application notification") << surface << systemNotificationsDisabled << 1 << 1;
    QTest::newRow("Surface, all notifications disabled, application notification") << surface << allNotificationsDisabled << 1 << 0;
    QTest::newRow("No surface, system notification") << (QWaylandSurface *)0 << QVariantMap() << 2 << 1;
    QTest::newRow("Surface, no properties, system notification") << surface << QVariantMap() << 2 << 1;
    QTest::newRow("Surface, all notifications enabled, system notification") << surface << allNotificationsEnabled << 2 << 1;
    QTest::newRow("Surface, application notifications disabled, system notification") << surface << applicationNotificationsDisabled << 2 << 1;
    QTest::newRow("Surface, system notifications disabled, system notification") << surface << systemNotificationsDisabled << 2 << 0;
    QTest::newRow("Surface, all notifications disabled, system notification") << surface << allNotificationsDisabled << 2 << 0;
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

void Ut_NotificationFeedbackPlayer::testNotificationPriority_data()
{
    QTest::addColumn<int>("minimumPriority");
    QTest::addColumn<int>("urgency");
    QTest::addColumn<QVariant>("priority");
    QTest::addColumn<int>("playCount");

    QTest::newRow("Minimum priority 50, urgency 1, priority not defined") << 50 << 1 << QVariant() << 0;
    QTest::newRow("Minimum priority 50, urgency 1, priority 49") << 50 << 1 << QVariant(49) << 0;
    QTest::newRow("Minimum priority 50, urgency 1, priority 50") << 50 << 1 << QVariant(50) << 1;
    QTest::newRow("Minimum priority 50, urgency 2, priority not defined") << 50 << 2 << QVariant() << 1;
    QTest::newRow("Minimum priority 50, urgency 2, priority 49") << 50 << 2 << QVariant(49) << 1;
    QTest::newRow("Minimum priority 50, urgency 2, priority 50") << 50 << 2 << QVariant(50) << 1;
}

void Ut_NotificationFeedbackPlayer::testNotificationPriority()
{
    QFETCH(int, minimumPriority);
    QFETCH(int, urgency);
    QFETCH(QVariant, priority);
    QFETCH(int, playCount);

    player->setMinimumPriority(minimumPriority);
    createNotification(1, urgency, priority);
    player->addNotification(1);

    QCOMPARE(gClientStub->stubCallCount("play"), playCount);
}

void Ut_NotificationFeedbackPlayer::testLEDDisabledWhenNoSummaryAndBody_data()
{
    QTest::addColumn<QVariant>("disableHint");
    QTest::addColumn<bool>("mediaParametersDefined");

    QTest::newRow("LED disabled without body and summary not defined") << QVariant() << true;
    QTest::newRow("LED disabled without body and summary false") << QVariant(false) << false;
    QTest::newRow("LED disabled without body and summary true") << QVariant(true) << true;
}

void Ut_NotificationFeedbackPlayer::testLEDDisabledWhenNoSummaryAndBody()
{
    QFETCH(QVariant, disableHint);
    QFETCH(bool, mediaParametersDefined);

    QVariantHash hints;
    hints.insert(NotificationManager::HINT_FEEDBACK, "feedback");
    if (disableHint.isValid()) {
        hints.insert(NotificationManager::HINT_LED_DISABLED_WITHOUT_BODY_AND_SUMMARY, disableHint);
    }
    LipstickNotification *notification1 = new LipstickNotification("ut_notificationfeedbackplayer", 1, "", "", "", QStringList(), hints, -1);
    LipstickNotification *notification2 = new LipstickNotification("ut_notificationfeedbackplayer", 2, "", "summary", "", QStringList(), hints, -1);
    LipstickNotification *notification3 = new LipstickNotification("ut_notificationfeedbackplayer", 3, "", "", "body", QStringList(), hints, -1);
    notificationManagerNotification.insert(1, notification1);
    notificationManagerNotification.insert(2, notification2);
    notificationManagerNotification.insert(3, notification3);

    player->addNotification(1);
    QCOMPARE(gClientStub->stubCallCount("play"), 1);
    QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QVariantMap>(1).contains("media.leds"), mediaParametersDefined);
    if (mediaParametersDefined) {
        QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QVariantMap>(1).value("media.leds").toBool(), false);
        QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QVariantMap>(1).value("media.audio").toBool(), true);
        QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QVariantMap>(1).value("media.vibra").toBool(), true);
        QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QVariantMap>(1).value("media.backlight").toBool(), true);
    }

    player->addNotification(2);
    QCOMPARE(gClientStub->stubCallCount("play"), 2);
    QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QVariantMap>(1).isEmpty(), true);

    player->addNotification(3);
    QCOMPARE(gClientStub->stubCallCount("play"), 3);
    QCOMPARE(gClientStub->stubLastCallTo("play").parameter<QVariantMap>(1).isEmpty(), true);
}

QTEST_MAIN(Ut_NotificationFeedbackPlayer)
