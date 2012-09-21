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

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "lipstickglobal.h"
#include "notificationhints.h"
#include <QStringList>

/*!
 * An object for storing information about a single notification.
 */
class LIPSTICK_EXPORT Notification
{
public:
    /*!
     * Creates an object for storing information about a single notification.
     *
     * \param appName name of the application sending the notification
     * \param appIcon icon ID of the application sending the notification
     * \param summary summary text for the notification
     * \param body body text for the notification
     * \param actions actions for the notification as a list of identifier/string pairs
     * \param hints hints for the notification
     * \param expireTimeout expiration timeout for the notification
     */
    Notification(const QString &appName, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const NotificationHints &hints, int expireTimeout);

    //! Returns the name of the application sending the notification
    QString appName() const;

    //! Returns the icon ID of the application sending the notification
    QString appIcon() const;

    //! Returns the summary text for the notification
    QString summary() const;

    //! Returns the body text for the notification
    QString body() const;

    //! Returns the actions for the notification as a list of identifier/string pairs
    QStringList actions() const;

    //! Returns the hints for the notification
    NotificationHints hints() const;

    //! Returns the expiration timeout for the notification
    int expireTimeout() const;

private:
    //! Name of the application sending the notification
    QString appName_;

    //! Icon ID of the application sending the notification
    QString appIcon_;

    //! Summary text for the notification
    QString summary_;

    //! Body text for the notification
    QString body_;

    //! Actions for the notification as a list of identifier/string pairs
    QStringList actions_;

    //! Hints for the notification
    NotificationHints hints_;

    //! Expiration timeout for the notification
    int expireTimeout_;
};

#endif // NOTIFICATION_H
