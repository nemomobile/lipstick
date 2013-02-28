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
#include "qdbusargument_fake.h"
#include "ut_notification.h"
#include "notification.h"
#include "notificationmanager_stub.h"

void Ut_Notification::testGettersAndSetters()
{
    QString appName = "appName1";
    uint replacesId = 1;
    QString appIcon = "appIcon1";
    QString summary = "summary1";
    QString body = "body1";
    QString previewIcon = "previewIcon1";
    QString previewSummary = "previewSummary1";
    QString previewBody = "previewBody1";
    int urgency = 1;
    int itemCount = 1;
    int priority = 1;
    QString category = "category1";
    QStringList actions = QStringList() << "action1a" << "action1b";
    QDateTime timestamp = QDateTime::currentDateTime();
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_TIMESTAMP, timestamp);
    hints.insert(NotificationManager::HINT_ITEM_COUNT, itemCount);
    hints.insert(NotificationManager::HINT_PRIORITY, priority);
    hints.insert(NotificationManager::HINT_PREVIEW_ICON, previewIcon);
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, previewSummary);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, previewBody);
    hints.insert(NotificationManager::HINT_URGENCY, urgency);
    hints.insert(NotificationManager::HINT_CATEGORY, category);
    int expireTimeout = 1;

    // Ensure that the constructor puts things in place
    Notification notification(appName, replacesId, appIcon, summary, body, actions, hints, expireTimeout);
    QCOMPARE(notification.appName(), appName);
    QCOMPARE(notification.replacesId(), replacesId);
    QCOMPARE(notification.appIcon(), appIcon);
    QCOMPARE(notification.summary(), summary);
    QCOMPARE(notification.body(), body);
    QCOMPARE(notification.actions(), actions);
    QCOMPARE(notification.expireTimeout(), expireTimeout);
    QCOMPARE(notification.timestamp(), timestamp);
    QCOMPARE(notification.previewIcon(), previewIcon);
    QCOMPARE(notification.previewSummary(), previewSummary);
    QCOMPARE(notification.previewBody(), previewBody);
    QCOMPARE(notification.urgency(), urgency);
    QCOMPARE(notification.itemCount(), itemCount);
    QCOMPARE(notification.priority(), priority);
    QCOMPARE(notification.category(), category);

    appName = "appName2";
    appIcon = "appIcon2";
    summary = "summary2";
    body = "body2";
    previewIcon = "previewIcon2";
    previewSummary = "previewSummary2";
    previewBody = "previewBody2";
    urgency = 2;
    itemCount = 2;
    priority = 2;
    category = "category2";
    actions = QStringList() << "action2a" << "action2b" << "action2c";
    timestamp = QDateTime::currentDateTime();
    hints.insert(NotificationManager::HINT_TIMESTAMP, timestamp);
    hints.insert(NotificationManager::HINT_ITEM_COUNT, itemCount);
    hints.insert(NotificationManager::HINT_PRIORITY, priority);
    hints.insert(NotificationManager::HINT_PREVIEW_ICON, previewIcon);
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, previewSummary);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, previewBody);
    hints.insert(NotificationManager::HINT_URGENCY, urgency);
    hints.insert(NotificationManager::HINT_CATEGORY, category);
    expireTimeout = 2;
    notification.setAppName(appName);
    notification.setAppIcon(appIcon);
    notification.setSummary(summary);
    notification.setBody(body);
    notification.setActions(actions);
    notification.setHints(hints);
    notification.setExpireTimeout(expireTimeout);
    QCOMPARE(notification.appName(), appName);
    QCOMPARE(notification.appIcon(), appIcon);
    QCOMPARE(notification.summary(), summary);
    QCOMPARE(notification.body(), body);
    QCOMPARE(notification.actions(), actions);
    QCOMPARE(notification.expireTimeout(), expireTimeout);
    QCOMPARE(notification.timestamp(), timestamp);
    QCOMPARE(notification.previewIcon(), previewIcon);
    QCOMPARE(notification.previewSummary(), previewSummary);
    QCOMPARE(notification.previewBody(), previewBody);
    QCOMPARE(notification.urgency(), urgency);
    QCOMPARE(notification.itemCount(), itemCount);
    QCOMPARE(notification.priority(), priority);
    QCOMPARE(notification.category(), category);
}

void Ut_Notification::testIcon_data()
{
    QTest::addColumn<QString>("appIcon");
    QTest::addColumn<QString>("hintIcon");
    QTest::addColumn<QString>("icon");

    QTest::newRow("No app_icon, no hint") << QString() << QString() << QString();
    QTest::newRow("No app_icon, hint") << QString() << QString("hintIcon") << QString("hintIcon");
    QTest::newRow("app_icon, no hint") << QString("appIcon") << QString() << QString("appIcon");
    QTest::newRow("app_icon, hint") << QString("appIcon") << QString("hintIcon") << QString("appIcon");
}

void Ut_Notification::testIcon()
{
    QFETCH(QString, appIcon);
    QFETCH(QString, hintIcon);
    QFETCH(QString, icon);

    QVariantHash hints;
    hints.insert(NotificationManager::HINT_ICON, hintIcon);

    Notification notification1(QString(), 0, appIcon, QString(), QString(), QStringList(), hints, 0);
    QCOMPARE(notification1.icon(), icon);
    Notification notification2(QString(), 0, QString(), QString(), QString(), QStringList(), QVariantHash(), 0);
    notification2.setAppIcon(appIcon);
    notification2.setHints(hints);
    QCOMPARE(notification2.icon(), icon);
}

void Ut_Notification::testSignals()
{
    QVariantHash hints;
    Notification notification(QString(), 0, QString(), QString(), QString(), QStringList(), hints, 0);
    QSignalSpy summarySpy(&notification, SIGNAL(summaryChanged()));
    QSignalSpy bodySpy(&notification, SIGNAL(bodyChanged()));
    QSignalSpy iconSpy(&notification, SIGNAL(iconChanged()));
    QSignalSpy timestampSpy(&notification, SIGNAL(timestampChanged()));
    QSignalSpy previewIconSpy(&notification, SIGNAL(previewIconChanged()));
    QSignalSpy previewSummarySpy(&notification, SIGNAL(previewSummaryChanged()));
    QSignalSpy previewBodySpy(&notification, SIGNAL(previewBodyChanged()));
    QSignalSpy urgencySpy(&notification, SIGNAL(urgencyChanged()));

    notification.setSummary("summary");
    QCOMPARE(summarySpy.count(), 1);
    notification.setSummary("summary");
    QCOMPARE(summarySpy.count(), 1);

    notification.setBody("body");
    QCOMPARE(bodySpy.count(), 1);
    notification.setBody("body");
    QCOMPARE(bodySpy.count(), 1);

    hints.insert(NotificationManager::HINT_ICON, "icon");
    notification.setHints(hints);
    QCOMPARE(iconSpy.count(), 1);

    hints.insert(NotificationManager::HINT_TIMESTAMP, "2012-10-01 18:04:19");
    notification.setHints(hints);
    QCOMPARE(iconSpy.count(), 1);
    QCOMPARE(timestampSpy.count(), 1);

    hints.insert(NotificationManager::HINT_PREVIEW_ICON, "previewIcon");
    notification.setHints(hints);
    QCOMPARE(timestampSpy.count(), 1);
    QCOMPARE(previewIconSpy.count(), 1);

    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "previewSummary");
    notification.setHints(hints);
    QCOMPARE(previewIconSpy.count(), 1);
    QCOMPARE(previewSummarySpy.count(), 1);

    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "previewBody");
    notification.setHints(hints);
    QCOMPARE(previewSummarySpy.count(), 1);
    QCOMPARE(previewBodySpy.count(), 1);

    hints.insert(NotificationManager::HINT_URGENCY, 2);
    notification.setHints(hints);
    QCOMPARE(previewBodySpy.count(), 1);
    QCOMPARE(urgencySpy.count(), 1);
}

void Ut_Notification::testSerialization()
{
    QString appName = "appName1";
    uint replacesId = 1;
    QString appIcon = "appIcon1";
    QString summary = "summary1";
    QString body = "body1";
    QStringList actions = QStringList() << "action1a" << "action1b";
    QString icon = "icon1";
    QDateTime timestamp = QDateTime::currentDateTime();
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_ICON, icon);
    hints.insert(NotificationManager::HINT_TIMESTAMP, timestamp);
    int expireTimeout = 1;

    Notification n1(appName, replacesId, appIcon, summary, body, actions, hints, expireTimeout);
    Notification n2;

    // Transfer a Notification from n1 to n2 by serializing it to a QDBusArgument and unserializing it
    QDBusArgument arg;
    arg << n1;
    arg >> n2;

    QCOMPARE(n2.appName(), n1.appName());
    QCOMPARE(n2.replacesId(), n1.replacesId());
    QCOMPARE(n2.appIcon(), n1.appIcon());
    QCOMPARE(n2.summary(), n1.summary());
    QCOMPARE(n2.body(), n1.body());
    QCOMPARE(n2.actions(), n1.actions());
    QCOMPARE(n2.expireTimeout(), n1.expireTimeout());
    QCOMPARE(n2.icon(), n1.icon());
    QCOMPARE(n2.timestamp(), n1.timestamp());
}

QTEST_MAIN(Ut_Notification)
