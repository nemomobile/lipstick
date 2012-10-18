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

void Ut_NotificationListModel::testNotificationIsNotAddedIfClassIsSystem()
{
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_CLASS, "system");
    Notification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", hints, 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", -1);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("addItem"), 0);
}

void Ut_NotificationListModel::testNotificationIsNotAddedIfSummaryIsEmpty()
{
    Notification notification("appName", 1, "appIcon", "", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", -1);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("addItem"), 0);
}

void Ut_NotificationListModel::testNotificationIsNotAddedIfBodyIsEmpty()
{
    Notification notification("appName", 1, "appIcon", "summary", "", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", -1);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("addItem"), 0);
}

void Ut_NotificationListModel::testAlreadyAddedNotificationIsRemovedIfClassChangesToSystem()
{
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_CLASS, "system");
    Notification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", hints, 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", 0);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("removeItem"), 1);
    QCOMPARE(gQObjectListModelStub->stubCallsTo("removeItem").at(0)->parameter<QObject *>(0), &notification);
}

void Ut_NotificationListModel::testAlreadyAddedNotificationIsRemovedIfSummaryChangesToEmpty()
{
    Notification notification("appName", 1, "appIcon", "", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    gQObjectListModelStub->stubSetReturnValue("indexOf", 0);
    NotificationListModel model;
    QCOMPARE(gQObjectListModelStub->stubCallCount("removeItem"), 1);
    QCOMPARE(gQObjectListModelStub->stubCallsTo("removeItem").at(0)->parameter<QObject *>(0), &notification);
}

void Ut_NotificationListModel::testAlreadyAddedNotificationIsRemovedIfBodyChangesToEmpty()
{
    Notification notification("appName", 1, "appIcon", "summary", "", QStringList() << "action", QVariantHash(), 1);
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
