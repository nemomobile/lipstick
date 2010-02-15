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

#include "duihomesystemnotificationpluginbase.h"

DuiHomeSystemNotificationPluginBase::DuiHomeSystemNotificationPluginBase() :
    source_(NULL),
    notificationUserId(0)
{
}

void DuiHomeSystemNotificationPluginBase::initialize(DuiHomeSystemNotificationSourceInterface &sourceInterface)
{
    source_ = &sourceInterface;
    notificationUserId = source_->notificationUserId();
}

uint DuiHomeSystemNotificationPluginBase::addNotification(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return source_->addNotification(notificationUserId, eventType, summary, body, action, imageURI, count);
}

DuiHomeSystemNotificationSourceInterface *DuiHomeSystemNotificationPluginBase::source()
{
    return source_;
}
