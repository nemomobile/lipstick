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
