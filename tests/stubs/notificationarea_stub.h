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
#ifndef NOTIFICATIONAREA_STUB_H
#define NOTIFICATIONAREA_STUB_H

#include <stubbase.h>
#include <notificationarea.h>
#include <DuiInfoBanner>

class NotificationAreaStub : public StubBase
{
public:
    virtual void notificationAreaConstructor(NotificationArea *notificationArea, DuiWidget *parent);
    virtual void notificationAreaDestructor();
    virtual void addNotification(DuiInfoBanner &notification);
    virtual void removeNotification(DuiInfoBanner &notification);
};

void NotificationAreaStub::notificationAreaConstructor(NotificationArea *notificationArea, DuiWidget *parent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<NotificationArea *>(notificationArea));
    params.append(new Parameter<DuiWidget *>(parent));
    stubMethodEntered("notificationAreaConstructor", params);
}

void NotificationAreaStub::notificationAreaDestructor()
{
    stubMethodEntered("notificationAreaDestructor");
}

void NotificationAreaStub::addNotification(DuiInfoBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner &>(notification));
    stubMethodEntered("addNotification", params);
}

void NotificationAreaStub::removeNotification(DuiInfoBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner &>(notification));
    stubMethodEntered("removeNotification", params);
}

NotificationAreaStub gDefaultNotificationAreaStub;
NotificationAreaStub *gNotificationAreaStub = &gDefaultNotificationAreaStub;

NotificationArea::NotificationArea(DuiWidget *parent)
{
    gNotificationAreaStub->notificationAreaConstructor(this, parent);
}

NotificationArea::~NotificationArea()
{
    gNotificationAreaStub->notificationAreaDestructor();
}

void NotificationArea::addNotification(DuiInfoBanner &notification)
{
    gNotificationAreaStub->addNotification(notification);
}

void NotificationArea::removeNotification(DuiInfoBanner &notification)
{
    gNotificationAreaStub->removeNotification(notification);
}

#endif
