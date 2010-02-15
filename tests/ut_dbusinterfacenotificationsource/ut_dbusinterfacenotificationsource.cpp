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

#include "ut_dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsourceadaptor.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"

MockNotificationManager::MockNotificationManager() :
    addNotificationNotificationUserId(12345),
    addNotificationGroupId(12345),
    addNotificationParameters(),
    addNotificationPersistent(false),
    addNotificationType(NotificationManagerInterface::ApplicationEvent),
    updateNotificationNotificationUserId(12345),
    updateNotificationId(12345),
    updateNotificationParameters(),
    removeNotificationNotificationUserId(12345),
    removeNotificationId(12345),
    addGroupNotificationUserId(12345),
    addGroupParameters(),
    addGroupPersistent(false),
    updateGroupNotificationUserId(12345),
    updateGroupId(12345),
    updateGroupParameters(),
    removeGroupNotificationUserId(12345),
    removeGroupId(12345),
    lastNotificationUserId(0),
    queryNotificationUserId(0)
{
}

uint MockNotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type)
{
    addNotificationNotificationUserId = notificationUserId;
    addNotificationGroupId = groupId;
    addNotificationParameters = parameters;
    addNotificationPersistent = persistent;
    addNotificationType = type;
    return 0;
}

bool MockNotificationManager::updateNotification(uint notificationUserId, uint id, const NotificationParameters &parameters)
{
    updateNotificationNotificationUserId = notificationUserId;
    updateNotificationId = id;
    updateNotificationParameters = parameters;
    return true;
}

bool MockNotificationManager::removeNotification(uint notificationUserId, uint id)
{
    removeNotificationNotificationUserId = notificationUserId;
    removeNotificationId = id;
    return true;
}

uint MockNotificationManager::addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent)
{
    addGroupNotificationUserId = notificationUserId;
    addGroupParameters = parameters;
    addGroupPersistent = persistent;
    return 0;
}

bool MockNotificationManager::updateGroup(uint notificationUserId, uint id, const NotificationParameters &parameters)
{
    updateGroupNotificationUserId = notificationUserId;
    updateGroupId = id;
    updateGroupParameters = parameters;
    return true;
}

bool MockNotificationManager::removeGroup(uint notificationUserId, uint id)
{
    removeGroupNotificationUserId = notificationUserId;
    removeGroupId = id;
    return true;
}

uint MockNotificationManager::notificationUserId()
{
    return lastNotificationUserId + 1;
}

QList< uint > MockNotificationManager::notificationIdList(uint notificationUserId)
{
    queryNotificationUserId = notificationUserId;

    QList< uint > tmp;
    tmp << 1 << 2 << 3 << 4 << 5;
    return tmp;
}

// DBusInterfaceNotificationSourceAdaptor stubs (used by NotificationManager)
DBusInterfaceNotificationSourceAdaptor::DBusInterfaceNotificationSourceAdaptor(DBusInterfaceNotificationSource *parent) : QDBusAbstractAdaptor(parent)
{
}

DBusInterfaceNotificationSourceAdaptor::~DBusInterfaceNotificationSourceAdaptor()
{
}

bool DBusInterfaceNotificationSourceAdaptor::removeNotification(uint)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::addNotification(uint, const QString &)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addNotification(uint, const QString &, const QString &, const QString &, const QString &, const QString &)
{
    return 0;
}

bool DBusInterfaceNotificationSourceAdaptor::updateNotification(uint, const QString &)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateNotification(uint, const QString &, const QString &, const QString &, const QString &, const QString &)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::addGroup(const QString &)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addGroup(const QString &, const QString &, const QString &, const QString &, const QString &)
{
    return 0;
}

bool DBusInterfaceNotificationSourceAdaptor::removeGroup(uint)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateGroup(uint, const QString &)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateGroup(uint, const QString &, const QString &, const QString &, const QString &, const QString &)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::removeNotification(uint, uint)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::addNotification(uint, uint, const QString &, bool)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addNotification(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint, bool)
{
    return 0;
}

bool DBusInterfaceNotificationSourceAdaptor::updateNotification(uint, uint, const QString &)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateNotification(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::addGroup(uint, const QString &, bool)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addGroup(uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint, bool)
{
    return 0;
}

bool DBusInterfaceNotificationSourceAdaptor::removeGroup(uint, uint)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateGroup(uint, uint, const QString &)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateGroup(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::notificationUserId()
{
    return 1;
}

QList<uint> DBusInterfaceNotificationSourceAdaptor::notificationIdList(uint notificationUserId)
{
    QList<uint> tmp;
    return tmp;
}

void Ut_DBusInterfaceNotificationSource::initTestCase()
{
}

void Ut_DBusInterfaceNotificationSource::cleanupTestCase()
{
}

void Ut_DBusInterfaceNotificationSource::init()
{
    manager = new MockNotificationManager();
    source = new DBusInterfaceNotificationSource(*manager);
}

void Ut_DBusInterfaceNotificationSource::cleanup()
{
    delete source;
    delete manager;
}

void Ut_DBusInterfaceNotificationSource::testAddNotification()
{
    source->addNotification(1, 2, "event", true, NotificationManagerInterface::ApplicationEvent);
    QCOMPARE(manager->addNotificationNotificationUserId, (uint)1);
    QCOMPARE(manager->addNotificationGroupId, (uint)2);
    QCOMPARE(manager->addNotificationParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QVERIFY(manager->addNotificationPersistent);
    QCOMPARE(manager->addNotificationType, NotificationManagerInterface::ApplicationEvent);

    source->addNotification(3, 4, "event", "summary", "body", "action", "imageURI", 1, true, NotificationManagerInterface::ApplicationEvent);
    QCOMPARE(manager->addNotificationNotificationUserId, (uint)3);
    QCOMPARE(manager->addNotificationGroupId, (uint)4);
    QCOMPARE(manager->addNotificationParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(manager->addNotificationParameters.value(GenericNotificationParameterFactory::countKey()), QVariant("1"));
    QCOMPARE(manager->addNotificationParameters.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("<p><b>summary</b></p><p>body</p>"));
    QCOMPARE(manager->addNotificationParameters.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(manager->addNotificationParameters.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));

    QVERIFY(manager->addNotificationPersistent);
    QCOMPARE(manager->addNotificationType, NotificationManagerInterface::ApplicationEvent);

    source->addNotification(5, 6, "event", "summary", "body", "action", "imageURI", 42, true, NotificationManagerInterface::ApplicationEvent);
    QCOMPARE(manager->addNotificationNotificationUserId, (uint)5);
    QCOMPARE(manager->addNotificationGroupId, (uint)6);
    QCOMPARE(manager->addNotificationParameters.value(GenericNotificationParameterFactory::countKey()), QVariant("42"));

}

void Ut_DBusInterfaceNotificationSource::testUpdateNotification()
{
    source->updateNotification(1, 2, "event");
    QCOMPARE(manager->updateNotificationNotificationUserId, (uint)1);
    QCOMPARE(manager->updateNotificationId, (uint)2);
    QCOMPARE(manager->updateNotificationParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));

    source->updateNotification(3, 4, "event", "summary", "body", "action", "imageURI", 42);
    QCOMPARE(manager->updateNotificationNotificationUserId, (uint)3);
    QCOMPARE(manager->updateNotificationId, (uint)4);
    QCOMPARE(manager->updateNotificationParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(manager->updateNotificationParameters.value(GenericNotificationParameterFactory::countKey()), QVariant("42"));
    QCOMPARE(manager->updateNotificationParameters.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("<p><b>summary</b></p><p>body</p>"));
    QCOMPARE(manager->updateNotificationParameters.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(manager->updateNotificationParameters.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));
}

void Ut_DBusInterfaceNotificationSource::testRemoveNotification()
{
    source->removeNotification(1, 2);
    QCOMPARE(manager->removeNotificationNotificationUserId, (uint)1);
    QCOMPARE(manager->removeNotificationId, (uint)2);
}

void Ut_DBusInterfaceNotificationSource::testAddGroup()
{
    source->addGroup(1, "event", true);
    QCOMPARE(manager->addGroupNotificationUserId, (uint)1);
    QCOMPARE(manager->addGroupParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QVERIFY(manager->addGroupPersistent);
    source->addGroup(2, "event", "summary", "body", "action", "imageURI", 1, true);
    QCOMPARE(manager->addGroupNotificationUserId, (uint)2);
    QCOMPARE(manager->addGroupParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(manager->addGroupParameters.value(GenericNotificationParameterFactory::countKey()), QVariant("1"));
    QCOMPARE(manager->addGroupParameters.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("<p><b>summary</b></p><p>body</p>"));
    QCOMPARE(manager->addGroupParameters.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(manager->addGroupParameters.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));
    QVERIFY(manager->addGroupPersistent);
}

void Ut_DBusInterfaceNotificationSource::testUpdateGroup()
{
    source->updateGroup(1, 2, "event");
    QCOMPARE(manager->updateGroupNotificationUserId, (uint)1);
    QCOMPARE(manager->updateGroupId, (uint)2);
    QCOMPARE(manager->updateGroupParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    source->updateGroup(3, 4, "event", "summary", "body", "action", "imageURI", 42);
    QCOMPARE(manager->updateGroupNotificationUserId, (uint)3);
    QCOMPARE(manager->updateGroupId, (uint)4);
    QCOMPARE(manager->updateGroupParameters.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(manager->updateGroupParameters.value(GenericNotificationParameterFactory::countKey()), QVariant("42"));
    QCOMPARE(manager->updateGroupParameters.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("<p><b>summary</b></p><p>body</p>"));
    QCOMPARE(manager->updateGroupParameters.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(manager->updateGroupParameters.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));
}

void Ut_DBusInterfaceNotificationSource::testRemoveGroup()
{
    source->removeGroup(1, 2);
    QCOMPARE(manager->removeGroupNotificationUserId, (uint)1);
    QCOMPARE(manager->removeGroupId, (uint)2);
}

void Ut_DBusInterfaceNotificationSource::testNotificationIdList()
{
    QList<uint> result = source->notificationIdList(10);
    QCOMPARE(manager->queryNotificationUserId, (uint)10);
    QCOMPARE(result.count(), (int)(5));
    QCOMPARE(result.at(0), (uint)1);
    QCOMPARE(result.at(1), (uint)2);
    QCOMPARE(result.at(2), (uint)3);
    QCOMPARE(result.at(3), (uint)4);
    QCOMPARE(result.at(4), (uint)5);
}

QTEST_APPLESS_MAIN(Ut_DBusInterfaceNotificationSource)
