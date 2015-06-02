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
#include "lipsticknotification.h"

#include <QDBusArgument>
#include <QtDebug>

LipstickNotification::LipstickNotification(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout, QObject *parent) :
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
    updateHintValues();
}

LipstickNotification::LipstickNotification(QObject *parent) :
    QObject(parent),
    replacesId_(0),
    expireTimeout_(-1)
{
}

LipstickNotification::LipstickNotification(const LipstickNotification &notification) :
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
    updateHintValues();
}

QString LipstickNotification::appName() const
{
    return appName_;
}

void LipstickNotification::setAppName(const QString &appName)
{
    appName_ = appName;
}

uint LipstickNotification::replacesId() const
{
    return replacesId_;
}

QString LipstickNotification::appIcon() const
{
    return appIcon_;
}

void LipstickNotification::setAppIcon(const QString &appIcon)
{
    appIcon_ = appIcon;
}

QString LipstickNotification::summary() const
{
    return summary_;
}

void LipstickNotification::setSummary(const QString &summary)
{
    if (summary_ != summary) {
        summary_ = summary;
        emit summaryChanged();
    }
}

QString LipstickNotification::body() const
{
    return body_;
}

void LipstickNotification::setBody(const QString &body)
{
    if (body_ != body) {
        body_ = body;
        emit bodyChanged();
    }
}

QStringList LipstickNotification::actions() const
{
    return actions_;
}

void LipstickNotification::setActions(const QStringList &actions)
{
    actions_ = actions;
}

QVariantHash LipstickNotification::hints() const
{
    return hints_;
}

QVariantMap LipstickNotification::hintValues() const
{
    return hintValues_;
}

void LipstickNotification::setHints(const QVariantHash &hints)
{
    QString oldIcon = icon();
    QDateTime oldTimestamp = timestamp();
    QString oldPreviewIcon = previewIcon();
    QString oldPreviewSummary = previewSummary();
    QString oldPreviewBody = previewBody();
    int oldUrgency = urgency();
    int oldItemCount = itemCount();
    int oldPriority = priority();
    QString oldCategory = category();

    hints_ = hints;
    updateHintValues();

    if (oldIcon != icon()) {
        emit iconChanged();
    }

    if (oldTimestamp != timestamp()) {
        emit timestampChanged();
    }

    if (oldPreviewIcon != previewIcon()) {
        emit previewIconChanged();
    }

    if (oldPreviewSummary != previewSummary()) {
        emit previewSummaryChanged();
    }

    if (oldPreviewBody != previewBody()) {
        emit previewBodyChanged();
    }

    if (oldUrgency != urgency()) {
        emit urgencyChanged();
    }

    if (oldItemCount != itemCount()) {
        emit itemCountChanged();
    }

    if (oldPriority != priority()) {
        emit priorityChanged();
    }

    if (oldCategory != category()) {
        emit categoryChanged();
    }

    emit hintsChanged();
}

int LipstickNotification::expireTimeout() const
{
    return expireTimeout_;
}

void LipstickNotification::setExpireTimeout(int expireTimeout)
{
    expireTimeout_ = expireTimeout;
}

QString LipstickNotification::icon() const
{
    return hints_.value(NotificationManager::HINT_ICON).toString();
}

QDateTime LipstickNotification::timestamp() const
{
    return hints_.value(NotificationManager::HINT_TIMESTAMP).toDateTime();
}

QString LipstickNotification::previewIcon() const
{
    return hints_.value(NotificationManager::HINT_PREVIEW_ICON).toString();
}

QString LipstickNotification::previewSummary() const
{
    return hints_.value(NotificationManager::HINT_PREVIEW_SUMMARY).toString();
}

QString LipstickNotification::previewBody() const
{
    return hints_.value(NotificationManager::HINT_PREVIEW_BODY).toString();
}

int LipstickNotification::urgency() const
{
    return hints_.value(NotificationManager::HINT_URGENCY).toInt();
}

int LipstickNotification::itemCount() const
{
    return hints_.value(NotificationManager::HINT_ITEM_COUNT).toInt();
}

int LipstickNotification::priority() const
{
    return hints_.value(NotificationManager::HINT_PRIORITY).toInt();
}

QString LipstickNotification::category() const
{
    return hints_.value(NotificationManager::HINT_CATEGORY).toString();
}

bool LipstickNotification::isUserRemovable() const
{
    return hints_.value(NotificationManager::HINT_USER_REMOVABLE, QVariant(true)).toBool();
}

bool LipstickNotification::hidden() const
{
    return hints_.value(NotificationManager::HINT_HIDDEN, QVariant(false)).toBool();
}

QVariantList LipstickNotification::remoteActions() const
{
    QVariantList rv;

    QStringList::const_iterator it = actions_.constBegin(), end = actions_.constEnd();
    while (it != end) {
        const QString name(*it);
        QString displayName;
        if (++it != end) {
            displayName = *it;
            ++it;
        }

        const QString hint(hints_.value(NotificationManager::HINT_REMOTE_ACTION_PREFIX + name).toString());
        if (!hint.isEmpty()) {
            const QString icon(hints_.value(NotificationManager::HINT_REMOTE_ACTION_ICON_PREFIX + name).toString());

            QVariantMap vm;
            vm.insert(QStringLiteral("name"), name);
            if (!displayName.isEmpty()) {
                vm.insert(QStringLiteral("displayName"), displayName);
            }
            if (!icon.isEmpty()) {
                vm.insert(QStringLiteral("icon"), icon);
            }

            // Extract the element of the DBus call
            QStringList elements(hint.split(' ', QString::SkipEmptyParts));
            if (elements.size() <= 3) {
                qWarning() << "Unable to decode invalid remote action:" << hint;
            } else {
                int index = 0;
                vm.insert(QStringLiteral("service"), elements.at(index++));
                vm.insert(QStringLiteral("path"), elements.at(index++));
                vm.insert(QStringLiteral("iface"), elements.at(index++));
                vm.insert(QStringLiteral("method"), elements.at(index++));

                QVariantList args;
                while (index < elements.size()) {
                    const QString &arg(elements.at(index++));
                    const QByteArray buffer(QByteArray::fromBase64(arg.toUtf8()));

                    QDataStream stream(buffer);
                    QVariant var;
                    stream >> var;
                    args.append(var);
                }
                vm.insert(QStringLiteral("arguments"), args);
            }

            rv.append(vm);
        }
    }

    return rv;
}

QString LipstickNotification::origin() const
{
    return hints_.value(NotificationManager::HINT_ORIGIN).toString();
}

QString LipstickNotification::owner() const
{
    return hints_.value(NotificationManager::HINT_OWNER).toString();
}

int LipstickNotification::maxContentLines() const
{
    return hints_.value(NotificationManager::HINT_MAX_CONTENT_LINES).toInt();
}

void LipstickNotification::updateHintValues()
{
    hintValues_.clear();

    QVariantHash::const_iterator it = hints_.constBegin(), end = hints_.constEnd();
    for ( ; it != end; ++it) {
        // Filter out the hints that are represented by other properties
        const QString &hint(it.key());
        if (hint.compare(NotificationManager::HINT_ICON, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_TIMESTAMP, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_PREVIEW_ICON, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_PREVIEW_SUMMARY, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_PREVIEW_BODY, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_URGENCY, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_ITEM_COUNT, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_PRIORITY, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_CATEGORY, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_USER_REMOVABLE, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_HIDDEN, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_ORIGIN, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_OWNER, Qt::CaseInsensitive) != 0 &&
            hint.compare(NotificationManager::HINT_MAX_CONTENT_LINES, Qt::CaseInsensitive) != 0 &&
            !hint.startsWith(NotificationManager::HINT_REMOTE_ACTION_PREFIX, Qt::CaseInsensitive) &&
            !hint.startsWith(NotificationManager::HINT_REMOTE_ACTION_ICON_PREFIX, Qt::CaseInsensitive)) {
            hintValues_.insert(hint, it.value());
        }
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const LipstickNotification &notification)
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

const QDBusArgument &operator>>(const QDBusArgument &argument, LipstickNotification &notification)
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

NotificationList::NotificationList()
{
}

NotificationList::NotificationList(const QList<LipstickNotification *> &notificationList) :
    notificationList(notificationList)
{
}

NotificationList::NotificationList(const NotificationList &notificationList) :
    notificationList(notificationList.notificationList)
{
}

QList<LipstickNotification *> NotificationList::notifications() const
{
    return notificationList;
}

QDBusArgument &operator<<(QDBusArgument &argument, const NotificationList &notificationList)
{
    argument.beginArray(qMetaTypeId<LipstickNotification>());
    foreach (LipstickNotification *notification, notificationList.notificationList) {
        argument << *notification;
    }
    argument.endArray();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, NotificationList &notificationList)
{
    argument.beginArray();
    notificationList.notificationList.clear();
    while (!argument.atEnd()) {
        LipstickNotification *notification = new LipstickNotification;
        argument >> *notification;
        notificationList.notificationList.append(notification);
    }
    argument.endArray();
    return argument;
}
