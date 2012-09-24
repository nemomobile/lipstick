/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
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
    expireTimeout_(-1)
{
}

Notification::Notification(const QString &appName, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const NotificationHints &hints, int expireTimeout) :
    appName_(appName),
    appIcon_(appIcon),
    summary_(summary),
    body_(body),
    actions_(actions),
    hints_(hints),
    expireTimeout_(expireTimeout)
{
}

QString Notification::appName() const
{
    return appName_;
}

QString Notification::appIcon() const
{
    return appIcon_;
}

QString Notification::summary() const
{
    return summary_;
}

QString Notification::body() const
{
    return body_;
}

QStringList Notification::actions() const
{
    return actions_;
}

NotificationHints Notification::hints() const
{
    return hints_;
}

void Notification::setHints(const NotificationHints &hints)
{
    hints_ = hints;
}

int Notification::expireTimeout() const
{
    return expireTimeout_;
}

