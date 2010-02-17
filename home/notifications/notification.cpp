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

#include "notification.h"

Notification::Notification() :
    notificationId_(0),
    groupId_(0),
    userId_(0),
    parameters_(),
    type_(NotificationManagerInterface::ApplicationEvent),
    timeout_(0)
{
}

Notification::Notification(uint notificationId, uint groupId, uint userId, const NotificationParameters &parameters, NotificationManagerInterface::NotificationType type, int timeout) :
    notificationId_(notificationId),
    groupId_(groupId),
    userId_(userId),
    parameters_(parameters),
    type_(type),
    timeout_(timeout)
{
}

Notification::~Notification()
{
}

uint Notification::notificationId() const
{
    return notificationId_;
}

uint Notification::groupId() const
{
    return groupId_;
}

uint Notification::userId() const
{
    return userId_;
}

const NotificationParameters &Notification::parameters() const
{
    return parameters_;
}

void Notification::setParameters(const NotificationParameters &parameters)
{
    parameters_ = parameters;
}

NotificationManagerInterface::NotificationType Notification::type() const
{
    return type_;
}

int Notification::timeout() const
{
    return timeout_;
}


QDataStream &operator<<(QDataStream &datastream, const Notification &notification)
{
    datastream << notification.notificationId_;
    datastream << qint32(notification.type_);
    datastream << notification.groupId_;
    datastream << notification.userId_;
    datastream << notification.timeout_;
    datastream << notification.parameters_;
    return datastream;
}

QDataStream &operator>>(QDataStream &datastream, Notification &notification)
{
    datastream >> notification.notificationId_;

    qint32 s;
    datastream >> s;
    notification.type_ = static_cast<NotificationManagerInterface::NotificationType>(s);

    datastream >> notification.groupId_;
    datastream >> notification.userId_;
    datastream >> notification.timeout_;
    datastream >> notification.parameters_;
    return datastream;
}
