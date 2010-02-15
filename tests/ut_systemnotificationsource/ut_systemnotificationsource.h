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
#ifndef UT_SYSTEMNOTIFICATIONSOURCE_H
#define UT_SYSTEMNOTIFICATIONSOURCE_H

#include <QtTest/QtTest>
#include <QObject>
#include "systemnotificationsource.h"
#include "notificationmanagerinterface.h"

class DuiHomeSystemNotificationPluginBase;

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

    uint notificationUserId_;
    NotificationParameters parameters;
    uint groupId;
    bool persistent;
    NotificationType type;
};

class Ut_DuiSystemNotificationSource : public QObject
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

    // Test notification adding
    void testAddNotification();

private:
    // Class under test
    SystemNotificationSource *systemNotificationSource;
    // Notification Manager
    MockNotificationManager *manager;
};
#endif //UT_SYSTEMNOTIFICATIONSOURCE_H
