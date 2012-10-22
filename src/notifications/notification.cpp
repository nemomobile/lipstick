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

#include <QDBusArgument>
#include "notificationmanager.h"
#include "notification.h"

Notification::Notification(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout, QObject *parent) :
    QObject(parent),
    appName_(appName),
    replacesId_(replacesId),
    appIcon_(appIcon),
    summary_(summary),
    body_(body),
    actions_(actions),
    hints_(hints),
    expireTimeout_(expireTimeout)
{
}

Notification::Notification() :
    replacesId_(0),
    expireTimeout_(-1)
{
}

Notification::Notification(const Notification &notification) :
    QObject(notification.parent()),
    appName_(notification.appName_),
    replacesId_(notification.replacesId_),
    appIcon_(notification.appIcon_),
    summary_(notification.summary_),
    body_(notification.body_),
    actions_(notification.actions_),
    hints_(notification.hints_),
    expireTimeout_(notification.expireTimeout_)
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

uint Notification::replacesId() const
{
    return replacesId_;
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

QDBusArgument &operator<<(QDBusArgument &argument, const Notification &notification)
{
    argument.beginStructure();
    argument << notification.appName_;
    argument << notification.replacesId_;
    argument << notification.appIcon_;
    argument << notification.summary_;
    argument << notification.body_;
    argument << notification.actions_;
    argument << notification.hints_;
    argument << notification.expireTimeout_;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Notification &notification)
{
    argument.beginStructure();
    argument >> notification.appName_;
    argument >> notification.replacesId_;
    argument >> notification.appIcon_;
    argument >> notification.summary_;
    argument >> notification.body_;
    argument >> notification.actions_;
    argument >> notification.hints_;
    argument >> notification.expireTimeout_;
    argument.endStructure();
    return argument;
}
