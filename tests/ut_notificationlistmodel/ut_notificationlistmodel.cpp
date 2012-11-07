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
#include "ut_notificationlistmodel.h"
#include "notificationlistmodel.h"
#include "notificationmanager_stub.h"
#include "qobjectlistmodel_stub.h"

void Ut_NotificationListModel::init()
{
}

void Ut_NotificationListModel::cleanup()
{
    gNotificationManagerStub->stubReset();
    gQObjectListModelStub->stubReset();
}

void Ut_NotificationListModel::testModelPopulatesOnConstruction()
{
    Notification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1 << 2);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", -1);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("addItem"), 2);
    QCOMPARE(gQObjectListModelStub->stubCallsTo("addItem").at(0)->parameter<QObject *>(0), &notification);
    QCOMPARE(gQObjectListModelStub->stubCallsTo("addItem").at(1)->parameter<QObject *>(0), &notification);
}

void Ut_NotificationListModel::testNotificationIsOnlyAddedIfNotAlreadyAdded()
{
    Notification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", 0);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("addItem"), 0);
}

void Ut_NotificationListModel::testNotificationIsNotAddedIfNoSummaryOrBody_data()
{
    QTest::addColumn<QString>("summary");
    QTest::addColumn<QString>("body");
    QTest::addColumn<int>("addItemCount");
    QTest::newRow("No summary, no body") << "" << "" << 0;
    QTest::newRow("Summary, no body") << "summary" << "" << 1;
    QTest::newRow("No summary, body") << "" << "body" << 1;
    QTest::newRow("Summary, body") << "summary" << "body" << 1;
}

void Ut_NotificationListModel::testNotificationIsNotAddedIfNoSummaryOrBody()
{
    QFETCH(QString, summary);
    QFETCH(QString, body);
    QFETCH(int, addItemCount);

    Notification notification("appName", 1, "appIcon", summary, body, QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", -1);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("addItem"), addItemCount);
}

void Ut_NotificationListModel::testNotificationIsNotAddedIfUrgencyIsSystem()
{
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_URGENCY, 3);
    Notification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", hints, 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", -1);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("addItem"), 0);
}

void Ut_NotificationListModel::testAlreadyAddedNotificationIsRemovedIfNoLongerAddable()
{
    Notification notification("appName", 1, "appIcon", "", "", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", 0);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("removeItem"), 1);
    QCOMPARE(gQObjectListModelStub->stubCallsTo("removeItem").at(0)->parameter<QObject *>(0), &notification);
}

void Ut_NotificationListModel::testNotificationRemoval()
{
    NotificationListModel model;
    Notification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    model.removeNotification(1);
    QCOMPARE(gQObjectListModelStub->stubCallCount("removeItem"), 1);
    QCOMPARE(gQObjectListModelStub->stubCallsTo("removeItem").at(0)->parameter<QObject *>(0), &notification);
}

QTEST_MAIN(Ut_NotificationListModel)
