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
#ifndef HOMENOTIFICATIONSINK_STUB
#define HOMENOTIFICATIONSINK_STUB

#include "homenotificationsink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class HomeNotificationSinkStub : public StubBase
{
public:
    virtual void HomeNotificationSinkConstructor();
    virtual void HomeNotificationSinkDestructor();
    virtual void setNotificationAreaVisible(bool visible);
    virtual bool canAddNotification(const Notification &notification);
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    virtual void notificationAnimationDone();
    virtual void timeout();
    virtual void updateNotification(const Notification &notification);
    virtual void removeNotification(DuiInfoBanner *infoBanner);
    virtual void notificationDone(uint notificationId);
};

// 2. IMPLEMENT STUB
void HomeNotificationSinkStub::HomeNotificationSinkConstructor()
{

}
void HomeNotificationSinkStub::HomeNotificationSinkDestructor()
{

}
void HomeNotificationSinkStub::setNotificationAreaVisible(bool visible)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(visible));
    stubMethodEntered("setNotificationAreaVisible", params);
}

bool HomeNotificationSinkStub::canAddNotification(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("canAddNotification", params);
    return stubReturnValue<bool>("canAddNotification");
}

void HomeNotificationSinkStub::addNotification(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("addNotification", params);
}

void HomeNotificationSinkStub::removeNotification(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification", params);
}

void HomeNotificationSinkStub::notificationAnimationDone()
{
    stubMethodEntered("notificationAnimationDone");
}

void HomeNotificationSinkStub::timeout()
{
    stubMethodEntered("timeout");
}

void HomeNotificationSinkStub::updateNotification(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("updateNotification", params);
}

void HomeNotificationSinkStub::removeNotification(DuiInfoBanner *infoBanner)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner * >(infoBanner));
    stubMethodEntered("removeNotification", params);
}

void HomeNotificationSinkStub::notificationDone(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("notificationDone", params);
}



// 3. CREATE A STUB INSTANCE
HomeNotificationSinkStub gDefaultHomeNotificationSinkStub;
HomeNotificationSinkStub *gHomeNotificationSinkStub = &gDefaultHomeNotificationSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
HomeNotificationSink::HomeNotificationSink()
{
    gHomeNotificationSinkStub->HomeNotificationSinkConstructor();
}

HomeNotificationSink::~HomeNotificationSink()
{
    gHomeNotificationSinkStub->HomeNotificationSinkDestructor();
}

void HomeNotificationSink::setNotificationAreaVisible(bool visible)
{
    gHomeNotificationSinkStub->setNotificationAreaVisible(visible);
}

bool HomeNotificationSink::canAddNotification(const Notification &notification)
{
    return gHomeNotificationSinkStub->canAddNotification(notification);
}

void HomeNotificationSink::addNotification(const Notification &notification)
{
    gHomeNotificationSinkStub->addNotification(notification);
}

void HomeNotificationSink::removeNotification(uint notificationId)
{
    gHomeNotificationSinkStub->removeNotification(notificationId);
}

void HomeNotificationSink::notificationAnimationDone()
{
    gHomeNotificationSinkStub->notificationAnimationDone();
}

void HomeNotificationSink::timeout()
{
    gHomeNotificationSinkStub->timeout();
}

void HomeNotificationSink::updateNotification(const Notification &notification)
{
    gHomeNotificationSinkStub->updateNotification(notification);
}

void HomeNotificationSink::removeNotification(DuiInfoBanner *infoBanner)
{
    gHomeNotificationSinkStub->removeNotification(infoBanner);
}

void HomeNotificationSink::notificationDone(uint notificationId)
{
    gHomeNotificationSinkStub->notificationDone(notificationId);
}


#endif
