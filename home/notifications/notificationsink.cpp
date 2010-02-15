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

#include "notificationsink.h"

NotificationSink::NotificationSink() : appEventsEnabled(true)
{
}

void NotificationSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    Q_UNUSED(groupId)
    Q_UNUSED(parameters)
}

void NotificationSink::removeGroup(uint groupId)
{
    Q_UNUSED(groupId)
}

bool NotificationSink::applicationEventsEnabled()
{
    return appEventsEnabled;
}

void NotificationSink::setApplicationEventsEnabled(bool enabled)
{
    appEventsEnabled = enabled;
}

bool NotificationSink::canAddNotification(const Notification &notification)
{
    return appEventsEnabled || notification.type() == NotificationManagerInterface::SystemEvent;
}
