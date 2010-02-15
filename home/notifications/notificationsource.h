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

#ifndef NOTIFICATIONSOURCE_H
#define NOTIFICATIONSOURCE_H

#include "notificationparameters.h"
class NotificationManagerInterface;

/*!
 * Notification source is an abstract interface for classes that can generate new notifications.
 * Concrete notification sources are responsible for manipulating notifications using the
 * NotificationManagerInterface object provided to them.
 */
class NotificationSource
{
public:
    /*!
     * Default constructor
     * \param manager The NotificationManagerInterface object that is to be used to control the notifications
     * from this source.
     */
    NotificationSource(NotificationManagerInterface &manager);

    //! Creates NotificationParameters from a set of simple API parameters
    NotificationParameters notificationParameters(const QString &eventType, const QString &summary = QString(), const QString &body = QString(), const QString &action = QString(), const QString &imageURI = QString(), uint count = 1);

protected:
    //! NotificationManagerInterface object used with this NotificationSource
    NotificationManagerInterface &manager;
};

#endif // NOTIFICATIONSOURCE_H
