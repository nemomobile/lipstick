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
#ifndef UT_NOTIFICATIONMANAGER_H
#define UT_NOTIFICATIONMANAGER_H

#include <QObject>

class Ut_NotificationManager : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testManagerIsSingleton();
    void testDatabaseConnectionSucceedsAndTablesAreOk();
    void testDatabaseConnectionSucceedsAndTablesAreNotOk();
    void testFirstDatabaseConnectionFails();
    void testNotEnoughDiskSpaceToOpenDatabase();
    void testNotificationsAreRestoredOnConstruction();
    void testDatabaseCommitIsDoneOnDestruction();
    void testCapabilities();
    void testAddingNotification();
    void testUpdatingExistingNotification();
    void testUpdatingInexistingNotification();
    void testRemovingExistingNotification();
    void testRemovingInexistingNotification();
    void testServerInformation();
    void testModifyingCategoryDefinitionUpdatesNotifications();
    void testUninstallingCategoryDefinitionRemovesNotifications();
    void testActionIsInvokedIfDefined();
    void testActionIsNotInvokedIfIncomplete();
    void testRemoteActionIsInvokedIfDefined();
    void testInvokingActionClosesNotificationIfUserRemovable();
    void testInvokingActionRemovesNotificationIfUserRemovableAndNotCloseable();
    void testListingNotifications();
    void testRemoveUserRemovableNotifications();

signals:
    void actionInvoked(QString action);
};

#endif
