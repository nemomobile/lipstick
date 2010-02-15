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

#ifndef SYSTEMNOTIFICATIONSOURCE_H
#define SYSTEMNOTIFICATIONSOURCE_H

#include "notificationsource.h"
#include "notificationmanagerinterface.h"
#include "duihomesystemnotificationsourceinterface.h"

/*!
 * SystemNotificationSource loads a predefined number of trusted plugins which can spawn a system notication. During plugin initialization
 * context of this class is given to the plugin which can then call the addNotification for adding a new notification.
 */
class SystemNotificationSource : public DuiHomeSystemNotificationSourceInterface, public NotificationSource
{
public:
    /*!
     * Creates a new SystemNotificationSource.
     *
     * \param interface the NotificationManagerInterface to post the notifications to
     */
    SystemNotificationSource(NotificationManagerInterface &interface);

    /*!
     * Destroys a SystemNotificationSource.
     */
    ~SystemNotificationSource();

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
    uint addNotification(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);

    /*! This method returns the first available notification user id from the manager.
     * \return notification user id.
     */
    uint notificationUserId();

private:
    //! Loads a predefined set of trusted system notification plugins
    void loadSystemNotificationPlugins();

    /*! Loads a plugin using Qt plugin loader
     * \param path Path to the plugin to be loaded.
     */
    void addPlugin(const QString &path);

    //! List of plugins loaded
    QList<QObject *> pluginList;
};

#endif // SYSTEMNOTIFICATIONSOURCE_H
