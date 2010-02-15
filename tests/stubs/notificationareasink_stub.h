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
#ifndef NOTIFICATIONAREASINK_STUB
#define NOTIFICATIONAREASINK_STUB

#include "notificationareasink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationAreaSinkStub : public StubBase
{
public:
    virtual void NotificationAreaSinkConstructor();
    virtual void NotificationAreaSinkDestructor();
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    virtual void notificationClicked();
    virtual void addNotification(DuiInfoBanner &notification);
    virtual void removeNotification(DuiInfoBanner &notification);
    virtual void setupInfoBanner(DuiInfoBanner *infoBanner);
    virtual DuiInfoBanner *updateNotification(DuiInfoBanner *dn, const NotificationParameters &parameters);
};

// 2. IMPLEMENT STUB
void NotificationAreaSinkStub::NotificationAreaSinkConstructor()
{

}
void NotificationAreaSinkStub::NotificationAreaSinkDestructor()
{

}
void NotificationAreaSinkStub::addGroup(uint groupId, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("addGroup", params);
}

void NotificationAreaSinkStub::removeGroup(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup", params);
}

void NotificationAreaSinkStub::addNotification(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("addNotification", params);
}

void NotificationAreaSinkStub::removeNotification(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification", params);
}

void NotificationAreaSinkStub::notificationClicked()
{
    stubMethodEntered("notificationClicked");
}

void NotificationAreaSinkStub::addNotification(DuiInfoBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner & >(notification));
    stubMethodEntered("addNotification", params);
}

void NotificationAreaSinkStub::removeNotification(DuiInfoBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner & >(notification));
    stubMethodEntered("removeNotification", params);
}

void NotificationAreaSinkStub::setupInfoBanner(DuiInfoBanner *infoBanner)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner * >(infoBanner));
    stubMethodEntered("setupInfoBanner", params);
}

DuiInfoBanner *NotificationAreaSinkStub::updateNotification(DuiInfoBanner *dn, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner * >(dn));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("updateNotification", params);
    return stubReturnValue<DuiInfoBanner *>("updateNotification");
}



// 3. CREATE A STUB INSTANCE
NotificationAreaSinkStub gDefaultNotificationAreaSinkStub;
NotificationAreaSinkStub *gNotificationAreaSinkStub = &gDefaultNotificationAreaSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationAreaSink::NotificationAreaSink()
{
    gNotificationAreaSinkStub->NotificationAreaSinkConstructor();
}

NotificationAreaSink::~NotificationAreaSink()
{
    gNotificationAreaSinkStub->NotificationAreaSinkDestructor();
}

void NotificationAreaSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    gNotificationAreaSinkStub->addGroup(groupId, parameters);
}

void NotificationAreaSink::removeGroup(uint groupId)
{
    gNotificationAreaSinkStub->removeGroup(groupId);
}

void NotificationAreaSink::addNotification(const Notification &notification)
{
    gNotificationAreaSinkStub->addNotification(notification);
}

void NotificationAreaSink::removeNotification(uint notificationId)
{
    gNotificationAreaSinkStub->removeNotification(notificationId);
}

void NotificationAreaSink::notificationClicked()
{
    gNotificationAreaSinkStub->notificationClicked();
}

void NotificationAreaSink::setupInfoBanner(DuiInfoBanner *infoBanner)
{
    gNotificationAreaSinkStub->setupInfoBanner(infoBanner);
}

DuiInfoBanner *NotificationAreaSink::updateNotification(DuiInfoBanner *dn, const NotificationParameters &parameters)
{
    return gNotificationAreaSinkStub->updateNotification(dn, parameters);
}


#endif
