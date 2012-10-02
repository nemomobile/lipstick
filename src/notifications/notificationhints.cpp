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
#include <QDebug>
#include "notificationhints.h"

const char *NotificationHints::HINT_URGENCY = "urgency";
const char *NotificationHints::HINT_CATEGORY = "category";
const char *NotificationHints::HINT_DESKTOP_ENTRY = "desktop-entry";
const char *NotificationHints::HINT_IMAGE_DATA = "image_data";
const char *NotificationHints::HINT_SOUND_FILE = "sound-file";
const char *NotificationHints::HINT_SUPPRESS_SOUND = "suppress-sound";
const char *NotificationHints::HINT_X = "x";
const char *NotificationHints::HINT_Y = "y";
const char *NotificationHints::HINT_CLASS = "x-nemo-class";
const char *NotificationHints::HINT_ICON = "x-nemo-icon";
const char *NotificationHints::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationHints::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationHints::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationHints::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationHints::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationHints::HINT_USER_REMOVABLE = "x-nemo-user-removable";
const char *NotificationHints::HINT_GENERIC_TEXT_TRANSLATION_ID = "x-nemo-generic-text-translation-id";
const char *NotificationHints::HINT_GENERIC_TEXT_TRANSLATION_CATALOGUE = "x-nemo-generic-text-translation-catalogue";

NotificationHints::NotificationHints()
{
}

void NotificationHints::setHint(const QString &hint, const QVariant &value)
{
    hintValues.insert(hint, value);
}

QVariant NotificationHints::hintValue(const QString &hint) const
{
    return hintValues.value(hint);
}

QStringList NotificationHints::hints() const
{
    return hintValues.keys();
}

QDBusArgument &operator<<(QDBusArgument &argument, const NotificationHints &hints)
{
    QHashIterator<QString, QVariant> i(hints.hintValues);

    argument.beginMap(QMetaType::QString, qMetaTypeId<QDBusVariant>());
    while (i.hasNext()) {
        i.next();

        argument.beginMapEntry();
        argument << i.key();
        argument << QDBusVariant(i.value());
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, NotificationHints &hints)
{
    argument.beginMap();
    while (!argument.atEnd()) {
        QString key;
        QDBusVariant value;

        argument.beginMapEntry();
        argument >> key;
        argument >> value;
        argument.endMapEntry();

        hints.hintValues[key] = value.variant();
    }
    argument.endMap();
    return argument;
}

QDebug operator<<(QDebug debug, const NotificationHints &hints)
{
    debug.space() << "(";

    foreach (const QString &hint, hints.hints()) {
        debug << "(" << hint << "," << hints.hintValue(hint) << ")";
    }

    debug << ")";

    return debug;
}
