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

#include "lipstickglobal.h"
#include <QHash>
#include <QVariant>

class QDataStream;
class QDBusArgument;

/*!
 * Notification hints is a class for providing hints for a notification.
 * Hints are a way to provide extra data to a notification server that the server
 * may be able to make use of.
 */
class LIPSTICK_EXPORT NotificationHints
{
public:
    /*!
     * Creates a notification hints object.
     */
    NotificationHints();

    /*!
     * Sets the value of a hint.
     *
     * \param hint the name of the hint to set the value of
     * \param value a value of the hint
     */
    void setHint(const QString &hint, const QVariant &hintValue);

    /*!
     * Gets the value of a hint.
     *
     * \param hint the name of the hint to get the value of
     * \return the value of the hint
     */
    QVariant hintValue(const QString &hint) const;

    /*!
     * Returns a list of hints contained by the notification hints object.
     *
     * \return a list of hints
     */
    QStringList hints() const;

    friend QDataStream &operator<<(QDataStream &, const NotificationHints &);
    friend QDataStream &operator>>(QDataStream &, NotificationHints &);

    friend LIPSTICK_EXPORT QDBusArgument &operator<<(QDBusArgument &, const NotificationHints &);
    friend const LIPSTICK_EXPORT QDBusArgument &operator>>(const QDBusArgument &, NotificationHints &);

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

QDebug operator<<(QDebug debug, const NotificationHints &hints);

Q_DECLARE_METATYPE(NotificationHints)

#endif // NOTIFICATIONHINTS_H
