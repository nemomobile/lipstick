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
#include "duihomesystemnotificationpluginbase.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"
#include "ut_systemnotificationsource.h"

int noOfPluginsLoaded = 0;
// Test plugins
DuiHomeSystemNotificationPluginBase *testPlugins[2];

//Notification manager mocks
uint MockNotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type)
{
    this->notificationUserId_ = notificationUserId;
    this->parameters = parameters;
    this->groupId = groupId;
    this->persistent = persistent;
    this->type = type;
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

// QPluginLoader stubs

QObject *QPluginLoader::instance()
{
    return testPlugins[noOfPluginsLoaded];
}

// DuiSystemNotificationPluginBase stubs
void DuiHomeSystemNotificationPluginBase::initialize(DuiHomeSystemNotificationSourceInterface &sourceInterface)
{
    Q_UNUSED(sourceInterface);
    noOfPluginsLoaded++;
}

DuiHomeSystemNotificationPluginBase::DuiHomeSystemNotificationPluginBase()
{
}

void Ut_DuiSystemNotificationSource::initTestCase()
{
}

void Ut_DuiSystemNotificationSource::cleanupTestCase()
{
}

void Ut_DuiSystemNotificationSource::init()
{
    testPlugins[0] = new DuiHomeSystemNotificationPluginBase();
    testPlugins[1] = new DuiHomeSystemNotificationPluginBase();
    manager = new MockNotificationManager();
    systemNotificationSource = new SystemNotificationSource(*manager);
    QCOMPARE(noOfPluginsLoaded, 2);
}

void Ut_DuiSystemNotificationSource::cleanup()
{
    delete manager;
    delete systemNotificationSource;
    noOfPluginsLoaded = 0;
}

void Ut_DuiSystemNotificationSource::testAddNotification()
{
    systemNotificationSource->addNotification(100, "event", "summary", "body", "action", "imageURI", 1);
    QCOMPARE(manager->notificationUserId_, (uint)100);
    QCOMPARE(manager->parameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(manager->parameters.value(GenericNotificationParameterFactory::countKey()), QVariant("1"));
    QCOMPARE(manager->parameters.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("<p><b>summary</b></p><p>body</p>"));
    QCOMPARE(manager->parameters.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(manager->parameters.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));
    QCOMPARE(manager->groupId, (uint)0);
    QCOMPARE(manager->persistent, false);
    QCOMPARE(manager->type, NotificationManagerInterface::SystemEvent);
}

QTEST_APPLESS_MAIN(Ut_DuiSystemNotificationSource)
