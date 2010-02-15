/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef UT_DUIHOMESYSTEMNOTIFICATIONPLUGINBASE_H
#define UT_DUIHOMESYSTEMNOTIFICATIONPLUGINBASE_H

#include <QtTest/QtTest>
#include <QObject>
#include "notificationmanagerinterface.h"
#include "systemnotificationsource.h"
#include "duihomesystemnotificationpluginbase.h"

class MockNotificationManager : public NotificationManagerInterface
{
public:
    uint addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type);
    bool updateNotification(uint notificationUserId, uint id, const NotificationParameters &parameters);
    bool removeNotification(uint notificationUserId, uint id);
    uint addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent);
    bool updateGroup(uint notificationUserId, uint id, const NotificationParameters &parameters);
    bool removeGroup(uint notificationUserId, uint id);
    uint notificationUserId();
    QList< uint > notificationIdList(uint notificationUserId);
};

class TestDuiSystemNotificationPluginBase : public DuiHomeSystemNotificationPluginBase
{
public:
    // Accessor to protected method in DuiSystemNotificationPluginBase
    uint addNotification(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count) {
        return DuiHomeSystemNotificationPluginBase::addNotification(eventType, summary, body, action, imageURI, count);
    }
};

class Ut_DuiHomeSystemNotificationPluginBase : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test plugin initialization
    void testPluginInitialization();
    // Test notification adding
    void testAddNotification();

private:
    // system notitification source
    SystemNotificationSource *systemNotificationSource;
    // Notification Manager
    MockNotificationManager *manager;
    // Class under test
    TestDuiSystemNotificationPluginBase *testPluginBase;
};
#endif //UT_DUIHOMESYSTEMNOTIFICATIONPLUGINBASE_H
