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

void QTimer::singleShot(int, const QObject *receiver, const char *member)
{
    // The "member" string is of form "1member()", so remove the trailing 1 and the ()
    int memberLength = strlen(member) - 3;
    char modifiedMember[memberLength + 1];
    strncpy(modifiedMember, member + 1, memberLength);
    modifiedMember[memberLength] = 0;
    QMetaObject::invokeMethod(const_cast<QObject *>(receiver), modifiedMember, Qt::DirectConnection);
}

void Ut_NotificationListModel::init()
{
}

void Ut_NotificationListModel::cleanup()
{
    gNotificationManagerStub->stubReset();
}

void Ut_NotificationListModel::testSignalConnections()
{
    NotificationListModel model;
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationModified(uint)), &model, SLOT(updateNotification(uint))), true);
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationsModified(const QList<uint> &)), &model, SLOT(updateNotifications(const QList<uint> &))), true);
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), &model, SLOT(removeNotification(uint))), true);
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationsRemoved(const QList<uint> &)), &model, SLOT(removeNotifications(const QList<uint> &))), true);
    QCOMPARE(disconnect(&model, SIGNAL(clearRequested()), NotificationManager::instance(), SLOT(removeUserRemovableNotifications())), true);
}

void Ut_NotificationListModel::testModelPopulatesOnConstruction()
{
    LipstickNotification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    NotificationListModel model;
    QCOMPARE(model.populated(), true);
    QCOMPARE(model.itemCount(), 1);
    QCOMPARE(model.get(0), &notification);
}

void Ut_NotificationListModel::testNotificationIsOnlyAddedIfNotAlreadyAdded()
{
    LipstickNotification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    NotificationListModel model;
    model.updateNotification(1);
    QCOMPARE(model.itemCount(), 1);
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

    LipstickNotification notification("appName", 1, "appIcon", summary, body, QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    NotificationListModel model;
    QCOMPARE(model.itemCount(), addItemCount);
}

void Ut_NotificationListModel::testNotificationIsNotAddedIfHidden()
{
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_HIDDEN, true);
    LipstickNotification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", hints, 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    NotificationListModel model;
    QCOMPARE(model.itemCount(), 0);
}

void Ut_NotificationListModel::testAlreadyAddedNotificationIsRemovedIfNoLongerAddable()
{
    LipstickNotification notification("appName", 1, "appIcon", "", "", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    NotificationListModel model;
    QCOMPARE(model.itemCount(), 0);
}

void Ut_NotificationListModel::testNotificationRemoval()
{
    LipstickNotification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);
    NotificationListModel model;
    model.removeNotification(1);
    QCOMPARE(model.itemCount(), 0);
    QCOMPARE(model.populated(), true);
}

void Ut_NotificationListModel::testNotificationOrdering()
{
    NotificationListModel model;
    QVariantHash hints1;
    QVariantHash hints2;
    QVariantHash hints3;
    hints1.insert(NotificationManager::HINT_TIMESTAMP, QDateTime(QDate(2013, 1, 1), QTime(12, 34, 56)));
    hints2.insert(NotificationManager::HINT_TIMESTAMP, QDateTime(QDate(2013, 1, 3), QTime(12, 34, 56)));
    hints3.insert(NotificationManager::HINT_TIMESTAMP, QDateTime(QDate(2013, 1, 5), QTime(12, 34, 56)));
    LipstickNotification notification1("appName1", 1, "appIcon1", "summary1", "body1", QStringList() << "action1", hints1, 1);
    LipstickNotification notification2("appName2", 2, "appIcon2", "summary2", "body2", QStringList() << "action2", hints2, 1);
    LipstickNotification notification3("appName3", 3, "appIcon3", "summary3", "body3", QStringList() << "action3", hints3, 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification1);
    model.updateNotification(1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification3);
    model.updateNotification(3);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification2);
    model.updateNotification(2);
    QCOMPARE(model.itemCount(), 3);
    QCOMPARE(model.get(0), &notification3);
    QCOMPARE(model.get(1), &notification2);
    QCOMPARE(model.get(2), &notification1);

    hints1.insert(NotificationManager::HINT_TIMESTAMP, QDateTime(QDate(2013, 1, 7), QTime(12, 34, 56)));
    notification1.setHints(hints1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification1);
    model.updateNotification(1);
    QCOMPARE(model.get(0), &notification1);
    QCOMPARE(model.get(1), &notification3);
    QCOMPARE(model.get(2), &notification2);

    hints1.insert(NotificationManager::HINT_TIMESTAMP, QDateTime(QDate(2013, 1, 4), QTime(12, 34, 56)));
    notification1.setHints(hints1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification1);
    model.updateNotification(1);
    QCOMPARE(model.get(0), &notification3);
    QCOMPARE(model.get(1), &notification1);
    QCOMPARE(model.get(2), &notification2);
}

void Ut_NotificationListModel::testNotificationUpdate()
{
    LipstickNotification notification("appName", 1, "appIcon", "summary", "body", QStringList() << "action", QVariantHash(), 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);

    NotificationListModel model;
    QCOMPARE(model.itemCount(), 1);
    QCOMPARE(model.get(0)->property("replacesId").value<uint>(), 1u);

    QSignalSpy dataChangedSpy(&model, SIGNAL(dataChanged(QModelIndex,QModelIndex)));
    QCOMPARE(dataChangedSpy.count(), 0);

    notification.setAppName("differentAppName");
    model.updateNotification(1);
    QCOMPARE(model.itemCount(), 1);
    QCOMPARE(model.get(0)->property("replacesId").value<uint>(), 1u);

    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(dataChangedSpy.at(0).count(), 2);
    QCOMPARE(qvariant_cast<QModelIndex>(dataChangedSpy.at(0).at(0)).parent(), QModelIndex());
    QCOMPARE(qvariant_cast<QModelIndex>(dataChangedSpy.at(0).at(0)).row(), 0);
    QCOMPARE(qvariant_cast<QModelIndex>(dataChangedSpy.at(0).at(0)).column(), 0);
    QCOMPARE(qvariant_cast<QModelIndex>(dataChangedSpy.at(0).at(1)).parent(), QModelIndex());
    QCOMPARE(qvariant_cast<QModelIndex>(dataChangedSpy.at(0).at(1)).row(), 0);
    QCOMPARE(qvariant_cast<QModelIndex>(dataChangedSpy.at(0).at(1)).column(), 0);
}

void Ut_NotificationListModel::testRemoteActions()
{
    QStringList actions;
    actions.append("default");
    actions.append("Default Remote Action");
    actions.append("alternate");
    actions.append("Alternate Remote Action");
    actions.append("invalid");
    actions.append("Invalid Remote Action");
    actions.append("also-invalid");
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_REMOTE_ACTION_PREFIX + QStringLiteral("default"), "dbus-call-default-action");
    hints.insert(NotificationManager::HINT_REMOTE_ACTION_ICON_PREFIX + QStringLiteral("default"), "icon-s-default");
    hints.insert(NotificationManager::HINT_REMOTE_ACTION_PREFIX + QStringLiteral("alternate"), "dbus-call-alternate-action");
    LipstickNotification notification("appName", 1, "appIcon", "summary", "body", actions, hints, 1);
    gNotificationManagerStub->stubSetReturnValue("notificationIds", QList<uint>() << 1);
    gNotificationManagerStub->stubSetReturnValue("notification", &notification);

    NotificationListModel model;
    QCOMPARE(model.itemCount(), 1);
    LipstickNotification *modelNotification(qobject_cast<LipstickNotification *>(model.get(0)));
    QCOMPARE(modelNotification, &notification);
    QVariantList remoteActions(modelNotification->remoteActions());
    QCOMPARE(remoteActions.count(), 2);
    QVariantMap remoteAction(remoteActions.at(0).value<QVariantMap>());
    QCOMPARE(remoteAction["name"].toString(), QStringLiteral("default"));
    QCOMPARE(remoteAction["displayName"].toString(), QStringLiteral("Default Remote Action"));
    QCOMPARE(remoteAction["icon"].toString(), QStringLiteral("icon-s-default"));
    remoteAction = remoteActions.at(1).value<QVariantMap>();;
    QCOMPARE(remoteAction["name"].toString(), QStringLiteral("alternate"));
    QCOMPARE(remoteAction["displayName"].toString(), QStringLiteral("Alternate Remote Action"));
    QCOMPARE(remoteAction["icon"].toString(), QString());
}

QTEST_MAIN(Ut_NotificationListModel)
