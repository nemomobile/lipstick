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
#ifndef NOTIFICATIONSINK_STUB
#define NOTIFICATIONSINK_STUB

#include "notificationsink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationSinkStub : public StubBase
{
public:
    virtual void NotificationSinkConstructor();
    virtual bool applicationEventsEnabled();
    virtual void setApplicationEventsEnabled(bool enabled);
    virtual bool canAddNotification(const Notification &notification);
    void addNotification(const Notification &notification);
    void removeNotification(uint notificationId);
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    virtual void cancelNotification(uint notificationId);
    bool appEventsEnabled ;
};

// 2. IMPLEMENT STUB
void NotificationSinkStub::NotificationSinkConstructor()
{

}
bool NotificationSinkStub::applicationEventsEnabled()
{
    stubMethodEntered("applicationEventsEnabled");
    return stubReturnValue<bool>("applicationEventsEnabled");
}

void NotificationSinkStub::setApplicationEventsEnabled(bool enabled)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enabled));
    stubMethodEntered("setApplicationEventsEnabled", params);
}

bool NotificationSinkStub::canAddNotification(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("canAddNotification", params);
    return stubReturnValue<bool>("canAddNotification");
}

void NotificationSinkStub::addGroup(uint groupId, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("addGroup", params);
}

void NotificationSinkStub::removeGroup(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup", params);
}

void NotificationSinkStub::cancelNotification(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("cancelNotification", params);
}



// 3. CREATE A STUB INSTANCE
NotificationSinkStub gDefaultNotificationSinkStub;
NotificationSinkStub *gNotificationSinkStub = &gDefaultNotificationSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationSink::NotificationSink()
{
    gNotificationSinkStub->NotificationSinkConstructor();
}

bool NotificationSink::applicationEventsEnabled()
{
    return gNotificationSinkStub->applicationEventsEnabled();
}

void NotificationSink::setApplicationEventsEnabled(bool enabled)
{
    gNotificationSinkStub->setApplicationEventsEnabled(enabled);
}

bool NotificationSink::canAddNotification(const Notification &notification)
{
    return gNotificationSinkStub->canAddNotification(notification);
}

void NotificationSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    gNotificationSinkStub->addGroup(groupId, parameters);
}

void NotificationSink::removeGroup(uint groupId)
{
    gNotificationSinkStub->removeGroup(groupId);
}


#endif
