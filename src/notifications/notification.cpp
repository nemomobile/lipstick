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

#include "notificationmanager.h"
#include "notification.h"

Notification::Notification(const QString &appName, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout, QObject *parent) :
    QObject(parent),
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

void Notification::setAppName(const QString &appName)
{
    appName_ = appName;
}

QString Notification::appIcon() const
{
    return appIcon_;
}

void Notification::setAppIcon(const QString &appIcon)
{
    appIcon_ = appIcon;
}

QString Notification::summary() const
{
    return summary_;
}

void Notification::setSummary(const QString &summary)
{
    if (summary_ != summary) {
        summary_ = summary;
        emit summaryChanged();
    }
}

QString Notification::body() const
{
    return body_;
}

void Notification::setBody(const QString &body)
{
    if (body_ != body) {
        body_ = body;
        emit bodyChanged();
    }
}

QStringList Notification::actions() const
{
    return actions_;
}

void Notification::setActions(const QStringList &actions)
{
    actions_ = actions;
}

QVariantHash Notification::hints() const
{
    return hints_;
}

void Notification::setHints(const QVariantHash &hints)
{
    QString oldIcon = icon();
    QDateTime oldTimestamp = timestamp();

    hints_ = hints;

    if (oldIcon != icon()) {
        emit iconChanged();
    }

    if (oldTimestamp != timestamp()) {
        emit localizedTimestampChanged();
    }
}

int Notification::expireTimeout() const
{
    return expireTimeout_;
}

void Notification::setExpireTimeout(int expireTimeout)
{
    expireTimeout_ = expireTimeout;
}

QString Notification::icon() const
{
    return hints_.value(NotificationManager::HINT_ICON).toString();
}

QDateTime Notification::timestamp() const
{
    return hints_.value(NotificationManager::HINT_TIMESTAMP).toDateTime();
}

QString Notification::localizedTimestamp() const
{
    return timestamp().toString("hh:mm:ss");
}
