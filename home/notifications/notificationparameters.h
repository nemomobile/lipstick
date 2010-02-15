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

#ifndef NOTIFICATIONPARAMETERS_H
#define NOTIFICATIONPARAMETERS_H

#include <QHash>
#include <QVariant>

class QDataStream;
class NotificationParameter;

/*!
 * Notification parameters is a class for providing parameters for classes that
 * can present notifications.
 */
class NotificationParameters
{
public:
    /*!
     * Constructs a new NotificationParameters object.
     */
    NotificationParameters();

    /*!
     * Destroys the NotificationParameters.
     */
    virtual ~NotificationParameters();

    /*!
     * Adds a new parameter to the parameters with a given value.
     *
     * \param parameter the name of the parameter
     * \param value the value of the parameter
     */
    void add(const QString &parameter, const QVariant &value);

    /*!
     * Adds a new parameter to the parameters.
     *
     * \param parameter the parameter to be added
     */
    void add(const NotificationParameter &parameter);

    /*!
     * Returns the value of a parameter.
     *
     * \param parameter the name of the parameter
     * \return the value of the parameter
     */
    QVariant value(const QString &parameter) const;


    friend QDataStream &operator<<(QDataStream &, const NotificationParameters &);
    friend QDataStream &operator>>(QDataStream &, NotificationParameters &);

private:
    //! The mapping between the name of each parameter and its value
    QHash<QString, QVariant> parameterValues;
};


/*!
 * Serializes the given NotificationParameters to a QDataStream
 *
 * \param datastream QDataStream to write to
 * \param parameters NotificationParameters object to serialize
 */
QDataStream &operator<<(QDataStream &datastream, const NotificationParameters &parameters);

/*!
 * Deserializes a NotificationParameters from a QDataStream
 *
 * \param datastream QDataStream to read from
 * \param parameters the target NotificationParameters object, its previous state will be reset
 */
QDataStream &operator>>(QDataStream &datastream, NotificationParameters &parameters);


#endif
