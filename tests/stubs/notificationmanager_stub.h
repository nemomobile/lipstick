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
#ifndef NOTIFICATIONMANAGER_STUB
#define NOTIFICATIONMANAGER_STUB

#include "notificationmanager.h"
#include <stubbase.h>
#include <notificationmanagerinterface.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationManagerStub : public StubBase
{
public:
    virtual void NotificationManagerConstructor(int relayInterval, uint maxWaitQueueSize);
    virtual void NotificationManagerDestructor();
    virtual void restorePersistentData();
    virtual uint addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationManagerInterface::NotificationType type);
    virtual bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters);
    virtual bool removeNotification(uint notificationUserId, uint notificationId);
    virtual uint addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent);
    virtual bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters);
    virtual bool removeGroup(uint notificationUserId, uint groupId);
    virtual uint notificationUserId();
    virtual QList<uint> notificationIdList(uint notificationUserId);
    virtual bool removeNotification(uint notificationId);
    virtual bool removeNotificationsInGroup(uint groupId);
    virtual const EventTypeStore &eventTypeStore() const;
    virtual void notificationUpdated(const Notification &notification);
    virtual void notificationRemoved(uint notificationId);
    virtual void groupUpdated(uint groupId, const NotificationParameters &parameters);
    virtual void groupRemoved(uint groupId);
    virtual void notificationRestored(const Notification &notification);
    virtual void relayNextNotification();
    virtual void submitNotification(const Notification &notification);
    virtual int findNotificationFromWaitQueue(uint notificationId);
    virtual uint nextAvailableNotificationID();
    virtual uint nextAvailableGroupID();
    virtual void initializeNotificationUserIdDataStore();
    virtual void initializeEventTypeStore();
    virtual bool ensurePersistentDataPath();
    virtual void saveStateData();
    virtual void savePersistentNotifications();
};

// 2. IMPLEMENT STUB
void NotificationManagerStub::NotificationManagerConstructor(int relayInterval, uint maxWaitQueueSize)
{
    Q_UNUSED(relayInterval);
    Q_UNUSED(maxWaitQueueSize);

}
void NotificationManagerStub::NotificationManagerDestructor()
{

}
void NotificationManagerStub::restorePersistentData()
{
    stubMethodEntered("restorePersistentData");
}

uint NotificationManagerStub::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationManagerInterface::NotificationType type)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationUserId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<bool >(persistent));
    params.append(new Parameter<NotificationManagerInterface::NotificationType >(type));
    stubMethodEntered("addNotification", params);
    return stubReturnValue<uint>("addNotification");
}

bool NotificationManagerStub::updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationUserId));
    params.append(new Parameter<uint >(notificationId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("updateNotification", params);
    return stubReturnValue<bool>("updateNotification");
}

bool NotificationManagerStub::removeNotification(uint notificationUserId, uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationUserId));
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification", params);
    return stubReturnValue<bool>("removeNotification");
}

uint NotificationManagerStub::addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationUserId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    params.append(new Parameter<bool >(persistent));
    stubMethodEntered("addGroup", params);
    return stubReturnValue<uint>("addGroup");
}

bool NotificationManagerStub::updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationUserId));
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("updateGroup", params);
    return stubReturnValue<bool>("updateGroup");
}

bool NotificationManagerStub::removeGroup(uint notificationUserId, uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationUserId));
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup", params);
    return stubReturnValue<bool>("removeGroup");
}

uint NotificationManagerStub::notificationUserId()
{
    stubMethodEntered("notificationUserId");
    return stubReturnValue<uint>("notificationUserId");
}

QList<uint> NotificationManagerStub::notificationIdList(uint notificationUserId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationUserId));
    stubMethodEntered("notificationIdList", params);
    return stubReturnValue<QList<uint> >("notificationIdList");
}

bool NotificationManagerStub::removeNotification(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification", params);
    return stubReturnValue<bool>("removeNotification");
}

bool NotificationManagerStub::removeNotificationsInGroup(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeNotificationsInGroup", params);
    return stubReturnValue<bool>("removeNotificationsInGroup");
}

const EventTypeStore &NotificationManagerStub::eventTypeStore() const
{
    stubMethodEntered("eventTypeStore");
    return * stubReturnValue<const EventTypeStore *>("eventTypeStore");
}

void NotificationManagerStub::notificationUpdated(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("notificationUpdated", params);
}

void NotificationManagerStub::notificationRemoved(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("notificationRemoved", params);
}

void NotificationManagerStub::groupUpdated(uint groupId, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("groupUpdated", params);
}

void NotificationManagerStub::groupRemoved(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("groupRemoved", params);
}

void NotificationManagerStub::notificationRestored(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("notificationRestored", params);
}

void NotificationManagerStub::relayNextNotification()
{
    stubMethodEntered("relayNextNotification");
}

void NotificationManagerStub::submitNotification(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("submitNotification", params);
}

int NotificationManagerStub::findNotificationFromWaitQueue(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("findNotificationFromWaitQueue", params);
    return stubReturnValue<int>("findNotificationFromWaitQueue");
}

uint NotificationManagerStub::nextAvailableNotificationID()
{
    stubMethodEntered("nextAvailableNotificationID");
    return stubReturnValue<uint>("nextAvailableNotificationID");
}

uint NotificationManagerStub::nextAvailableGroupID()
{
    stubMethodEntered("nextAvailableGroupID");
    return stubReturnValue<uint>("nextAvailableGroupID");
}

void NotificationManagerStub::initializeNotificationUserIdDataStore()
{
    stubMethodEntered("initializeNotificationUserIdDataStore");
}

void NotificationManagerStub::initializeEventTypeStore()
{
    stubMethodEntered("initializeEventTypeStore");
}

bool NotificationManagerStub::ensurePersistentDataPath()
{
    stubMethodEntered("ensurePersistentDataPath");
    return stubReturnValue<bool>("ensurePersistentDataPath");
}

void NotificationManagerStub::saveStateData()
{
    stubMethodEntered("saveStateData");
}

void NotificationManagerStub::savePersistentNotifications()
{
    stubMethodEntered("savePersistentNotifications");
}



// 3. CREATE A STUB INSTANCE
NotificationManagerStub gDefaultNotificationManagerStub;
NotificationManagerStub *gNotificationManagerStub = &gDefaultNotificationManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationManager::NotificationManager(int relayInterval, uint maxWaitQueueSize) : maxWaitQueueSize(uint())
{
    gNotificationManagerStub->NotificationManagerConstructor(relayInterval, maxWaitQueueSize);
}

NotificationManager::~NotificationManager()
{
    gNotificationManagerStub->NotificationManagerDestructor();
}

void NotificationManager::restorePersistentData()
{
    gNotificationManagerStub->restorePersistentData();
}

uint NotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type)
{
    return gNotificationManagerStub->addNotification(notificationUserId, parameters, groupId, persistent, type);
}

bool NotificationManager::updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters)
{
    return gNotificationManagerStub->updateNotification(notificationUserId, notificationId, parameters);
}

bool NotificationManager::removeNotification(uint notificationUserId, uint notificationId)
{
    return gNotificationManagerStub->removeNotification(notificationUserId, notificationId);
}

uint NotificationManager::addGroup(uint notificationUserId, const NotificationParameters &parameters, bool persistent)
{
    return gNotificationManagerStub->addGroup(notificationUserId, parameters, persistent);
}

bool NotificationManager::updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters)
{
    return gNotificationManagerStub->updateGroup(notificationUserId, groupId, parameters);
}

bool NotificationManager::removeGroup(uint notificationUserId, uint groupId)
{
    return gNotificationManagerStub->removeGroup(notificationUserId, groupId);
}

uint NotificationManager::notificationUserId()
{
    return gNotificationManagerStub->notificationUserId();
}

QList<uint> NotificationManager::notificationIdList(uint notificationUserId)
{
    return gNotificationManagerStub->notificationIdList(notificationUserId);
}

bool NotificationManager::removeNotification(uint notificationId)
{
    return gNotificationManagerStub->removeNotification(notificationId);
}

bool NotificationManager::removeNotificationsInGroup(uint groupId)
{
    return gNotificationManagerStub->removeNotificationsInGroup(groupId);
}

const EventTypeStore &NotificationManager::eventTypeStore() const
{
    return gNotificationManagerStub->eventTypeStore();
}

void NotificationManager::relayNextNotification()
{
    gNotificationManagerStub->relayNextNotification();
}

void NotificationManager::submitNotification(const Notification &notification)
{
    gNotificationManagerStub->submitNotification(notification);
}

int NotificationManager::findNotificationFromWaitQueue(uint notificationId)
{
    return gNotificationManagerStub->findNotificationFromWaitQueue(notificationId);
}

uint NotificationManager::nextAvailableNotificationID()
{
    return gNotificationManagerStub->nextAvailableNotificationID();
}

uint NotificationManager::nextAvailableGroupID()
{
    return gNotificationManagerStub->nextAvailableGroupID();
}

void NotificationManager::initializeNotificationUserIdDataStore()
{
    gNotificationManagerStub->initializeNotificationUserIdDataStore();
}

void NotificationManager::initializeEventTypeStore()
{
    gNotificationManagerStub->initializeEventTypeStore();
}

bool NotificationManager::ensurePersistentDataPath()
{
    return gNotificationManagerStub->ensurePersistentDataPath();
}

void NotificationManager::saveStateData()
{
    gNotificationManagerStub->saveStateData();
}

void NotificationManager::savePersistentNotifications()
{
    gNotificationManagerStub->savePersistentNotifications();
}


#endif
