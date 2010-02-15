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

#ifndef DUIHOMESYSTEMNOTIFICATIONSOURCEINTERFACE_H
#define DUIHOMESYSTEMNOTIFICATIONSOURCEINTERFACE_H

/*!
 * DuiHomeSystemNotificationSourceInterface is an Interface to the system notification source loaded by home. System notification
 * plugins send addNotifications against this interface. A concrete implementation of this interface is in duihome.
 */
class DuiHomeSystemNotificationSourceInterface
{
public:
    /*! This method is called by the system notification plugin(base) class to add a system notification to the notification manager
     * \param notificationUserId Notification user id for this notification
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \return Notification id for the new notification created.
     */
    virtual uint addNotification(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count) = 0;

    /*! Return the notification user id from the notification Manager
     * \return uint First available notification used id from the notification manager.
     */
    virtual uint notificationUserId() = 0;
};

#endif // DUIHOMESYSTEMNOTIFICATIONSOURCEINTERFACE_H
