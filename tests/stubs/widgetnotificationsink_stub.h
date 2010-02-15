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
#ifndef WIDGETNOTIFICATIONSINK_STUB
#define WIDGETNOTIFICATIONSINK_STUB

#include "widgetnotificationsink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class WidgetNotificationSinkStub : public StubBase
{
public:
    virtual void notificationRemovalRequested(uint notificationId);
    virtual void notificationGroupClearingRequested(uint groupId);
    virtual QString determineIconId(const NotificationParameters &parameters);
    virtual DuiInfoBanner *createInfoBanner(const Notification &notification);
    virtual DuiInfoBanner *createInfoBanner(DuiInfoBanner::BannerType type, uint groupId, const NotificationParameters &parameters);
    virtual void updateActions(DuiInfoBanner *infoBanner, const NotificationParameters &parameters);
    virtual QString determineIconIdFromEventType(const QString &eventType);
    virtual void infoBannerClicked();
};

// 2. IMPLEMENT STUB
void WidgetNotificationSinkStub::notificationRemovalRequested(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("notificationRemovalRequested", params);
}

void WidgetNotificationSinkStub::notificationGroupClearingRequested(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("notificationGroupClearingRequested", params);
}

QString WidgetNotificationSinkStub::determineIconId(const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("determineIconId", params);
    return stubReturnValue<QString>("determineIconId");
}

DuiInfoBanner *WidgetNotificationSinkStub::createInfoBanner(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("createInfoBanner", params);
    return stubReturnValue<DuiInfoBanner *>("createInfoBanner");
}

DuiInfoBanner *WidgetNotificationSinkStub::createInfoBanner(DuiInfoBanner::BannerType type, uint groupId, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner::BannerType >(type));
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("createInfoBanner", params);
    return stubReturnValue<DuiInfoBanner *>("createInfoBanner");
}

void WidgetNotificationSinkStub::updateActions(DuiInfoBanner *infoBanner, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiInfoBanner * >(infoBanner));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("updateActions", params);
}

QString WidgetNotificationSinkStub::determineIconIdFromEventType(const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("determineIconIdFromEventType", params);
    return stubReturnValue<QString>("determineIconIdFromEventType");
}

void WidgetNotificationSinkStub::infoBannerClicked()
{
    stubMethodEntered("infoBannerClicked");
}



// 3. CREATE A STUB INSTANCE
WidgetNotificationSinkStub gDefaultWidgetNotificationSinkStub;
WidgetNotificationSinkStub *gWidgetNotificationSinkStub = &gDefaultWidgetNotificationSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
QString WidgetNotificationSink::determineIconId(const NotificationParameters &parameters)
{
    return gWidgetNotificationSinkStub->determineIconId(parameters);
}

DuiInfoBanner *WidgetNotificationSink::createInfoBanner(const Notification &notification)
{
    return gWidgetNotificationSinkStub->createInfoBanner(notification);
}

DuiInfoBanner *WidgetNotificationSink::createInfoBanner(DuiInfoBanner::BannerType type, uint groupId, const NotificationParameters &parameters)
{
    return gWidgetNotificationSinkStub->createInfoBanner(type, groupId, parameters);
}

void WidgetNotificationSink::updateActions(DuiInfoBanner *infoBanner, const NotificationParameters &parameters)
{
    gWidgetNotificationSinkStub->updateActions(infoBanner, parameters);
}

QString WidgetNotificationSink::determineIconIdFromEventType(const QString &eventType)
{
    return gWidgetNotificationSinkStub->determineIconIdFromEventType(eventType);
}

void WidgetNotificationSink::infoBannerClicked()
{
    gWidgetNotificationSinkStub->infoBannerClicked();
}


#endif
