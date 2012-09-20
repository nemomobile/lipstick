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

#ifndef NOTIFICATIONHINTS_H
#define NOTIFICATIONHINTS_H

#include <QHash>
#include <QVariant>

class QDataStream;
class QDBusArgument;

/*!
 * Notification hints is a class for providing hints for a notification.
 * Hints are a way to provide extra data to a notification server that the server
 * may be able to make use of.
 */
class NotificationHints
{
public:
    /*!
     * Creates a notification hints object.
     */
    NotificationHints();

    friend QDataStream &operator<<(QDataStream &, const NotificationHints &);
    friend QDataStream &operator>>(QDataStream &, NotificationHints &);

    friend QDBusArgument &operator<<(QDBusArgument &, const NotificationHints &);
    friend const QDBusArgument &operator>>(const QDBusArgument &, NotificationHints &);

private:
    QHash<QString, QVariant> hintValues;
};

/*!
 * Serializes the given NotificationHints to a QDataStream
 *
 * \param datastream QDataStream to write to
 * \param parameters NotificationHints object to serialize
 */
QDataStream &operator<<(QDataStream &datastream, const NotificationHints &hints);

/*!
 * Deserializes NotificationHints from a QDataStream
 *
 * \param datastream QDataStream to read from
 * \param parameters the target NotificationHints object, its previous state will be overwritten
 */
QDataStream &operator>>(QDataStream &datastream, NotificationHints &hints);

Q_DECLARE_METATYPE(NotificationHints)

#endif // NOTIFICATIONHINTS_H
