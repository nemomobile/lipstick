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

#include "notificationparameters.h"
#include "ut_duihomesystemnotificationpluginbase.h"

const uint NOTIFICATION_USER_ID_CREATED = 100;
uint notificationUserIdReceived;
NotificationParameters notificationParametersReceived;
bool notificationUsedIdCalled = false;
QString eventTypeReceived;
QString summaryReceived;
QString bodyReceived;
QString actionReceived;
QString imageURIReceived;
uint countReceived;

// DuiSystemNotificationSource stubs
SystemNotificationSource::SystemNotificationSource(NotificationManagerInterface &interface): NotificationSource(interface)
{
}

uint SystemNotificationSource::notificationUserId()
{
    notificationUsedIdCalled = true;
    return NOTIFICATION_USER_ID_CREATED;
}

// DuiSystemNotificationSource mocks
uint SystemNotificationSource::addNotification(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    notificationUserIdReceived = notificationUserId;
    eventTypeReceived = eventType;
    summaryReceived = summary;
    bodyReceived = body;
    actionReceived = action;
    imageURIReceived = imageURI;
    countReceived = count;
    return 200;
}

//Notification manager dummy implementation
uint MockNotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type)
{
    Q_UNUSED(notificationUserId);
    Q_UNUSED(parameters);
    Q_UNUSED(groupId);
    Q_UNUSED(persistent);
    Q_UNUSED(type);
    return 0;
}

bool MockNotificationManager::updateNotification(uint notificationUserId, uint id, const NotificationParameters &parameters)
{
    Q_UNUSED(notificationUserId);
    Q_UNUSED(id);
    Q_UNUSED(parameters);
    return true;
}

bool MockNotificationManager::removeNotification(uint notificationUserId, uint id)
{
    Q_UNUSED(notificationUserId);
    Q_UNUSED(id);
    return true;
}

uint MockNotificationManager::addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent)
{
    Q_UNUSED(notificationUserId);
    Q_UNUSED(parameters);
    Q_UNUSED(persistent);
    return 0;
}

bool MockNotificationManager::updateGroup(uint notificationUserId, uint id, const NotificationParameters &parameters)
{
    Q_UNUSED(notificationUserId);
    Q_UNUSED(id);
    Q_UNUSED(parameters);
    return true;
}

bool MockNotificationManager::removeGroup(uint notificationUserId, uint id)
{
    Q_UNUSED(notificationUserId);
    Q_UNUSED(id);
    return true;
}

uint MockNotificationManager::notificationUserId()
{
    return 0;
}

QList< uint > MockNotificationManager::notificationIdList(uint notificationUserId)
{
    Q_UNUSED(notificationUserId);
    QList< uint > tmp;
    tmp << 1 << 2 << 3 << 4 << 5;
    return tmp;
}


void Ut_DuiHomeSystemNotificationPluginBase::initTestCase()
{
}

void Ut_DuiHomeSystemNotificationPluginBase::cleanupTestCase()
{
}

void Ut_DuiHomeSystemNotificationPluginBase::init()
{
    manager = new MockNotificationManager();
    testPluginBase = new TestDuiSystemNotificationPluginBase();
    systemNotificationSource = new SystemNotificationSource(*manager);
}

void Ut_DuiHomeSystemNotificationPluginBase::cleanup()
{
    notificationUsedIdCalled = false;
    eventTypeReceived.clear();;
    summaryReceived.clear();
    bodyReceived.clear();
    actionReceived.clear();
    imageURIReceived.clear();
    countReceived = 0;
    delete testPluginBase;
}

void Ut_DuiHomeSystemNotificationPluginBase::testPluginInitialization()
{
    testPluginBase->initialize(*systemNotificationSource);
    QCOMPARE(testPluginBase->source(), systemNotificationSource);
    QCOMPARE(notificationUsedIdCalled, true);
}

void Ut_DuiHomeSystemNotificationPluginBase::testAddNotification()
{
    testPluginBase->initialize(*systemNotificationSource);

    testPluginBase->addNotification("event", "summary", "body", "action", "imageURI", 1);
    QCOMPARE(notificationUserIdReceived, NOTIFICATION_USER_ID_CREATED);
    QCOMPARE(eventTypeReceived, QString("event"));
    QCOMPARE(summaryReceived, QString("summary"));
    QCOMPARE(bodyReceived, QString("body"));
    QCOMPARE(actionReceived, QString("action"));
    QCOMPARE(imageURIReceived, QString("imageURI"));
    QCOMPARE(countReceived, (uint)1);
}

QTEST_APPLESS_MAIN(Ut_DuiHomeSystemNotificationPluginBase)
