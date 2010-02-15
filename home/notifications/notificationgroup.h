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


#ifndef NOTIFICATIONGROUP_H_
#define NOTIFICATIONGROUP_H_

#include "notificationmanagerinterface.h"
#include "notificationparameters.h"

#include <QVariant>

class QDataStream;

/*!
 * \brief A class for storing notification group information.
 *
 * The information can also be serialized in and out of a QDataStream.
 */
class NotificationGroup
{
public:
    /*!
     * Empty constructor. Initializes the values to defaults.
     */
    NotificationGroup();

    /*!
     * Constructor.
     *
     * \param groupId group ID of this notification group
     * \param userId user ID of this notification group
     * \param parameters The parameters for the notification group to be presented
     */
    NotificationGroup(uint groupId, uint userId, const NotificationParameters &parameters);

    /*!
     * Destructor.
     */
    ~NotificationGroup();

    /*!
     * Returns the group ID of this notification group.
     *
     * \return group ID of this notification group
     */
    uint groupId() const;

    /*!
     * Returns the user ID associated with this notification group.
     *
     * \return user ID associated with this notification group
     */
    uint userId() const;

    /*!
     * Returns the parameters list for this notification group.
     *
     * \return parameters list for this notification group
     */
    const NotificationParameters &parameters() const;

    /*!
     * Sets the parameters list for this notification group.
     *
     * \param parameters parameters for this notification group
     */
    void setParameters(const NotificationParameters &parameters);


    friend QDataStream &operator<<(QDataStream &, const NotificationGroup &);
    friend QDataStream &operator>>(QDataStream &, NotificationGroup &);

private:
    //! The ID of the notification group
    uint groupId_;
    //! The user ID associated with this notification group
    uint userId_;
    //! The parameters for the notification group to be presented
    NotificationParameters parameters_;
};


/*!
 * Serializes the given NotificationGroup to a QDataStream
 *
 * \param datastream QDataStream to write to
 * \param notificationGroup NotificationGroup object to serialize
 */
QDataStream &operator<<(QDataStream &datastream, const NotificationGroup &notificationGroup);

/*!
 * Deserializes a NotificationGroup from a QDataStream
 *
 * \param datastream QDataStream to read from
 * \param notificationGroup the target NotificationGroup object, its previous state will be reset
 */
QDataStream &operator>>(QDataStream &datastream, NotificationGroup &notificationGroup);


#endif // NOTIFICATIONGROUP_H_
