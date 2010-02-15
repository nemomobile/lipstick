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

#ifndef NOTIFICATIONMANAGERINTERFACE_H
#define NOTIFICATIONMANAGERINTERFACE_H

#include <QString>
#include "notificationparameters.h"

/*!
 * Abstract notification manager interface. Declares interface for adding,
 * updating, removing individual notifications as well as adding a
 * notification to the group, updating and removing the group.
 */
class NotificationManagerInterface
{
public:
    /*!
     * Notification types. Applications can only send ApplicationEvent
     * notifications since the D-Bus API does not allow the application
     * to set the type.
     */
    enum NotificationType {
        ApplicationEvent,
        SystemEvent
    };

    /*!
     * Adds a notification. Optionally adds the notification to a
     * notification group. The default group number 0 means that the
     * notification is not added to any group. You can create groups
     * by calling \c addGroup.
     *
     * If an invalid group ID is given the notification is not added.
     * In this case 0 is returned.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param parameters Parameters for the notification
     * \param groupId A notification group where this notification is added.
     * \param persistent \c true if the notification group should be persistent, \c false otherwise
     * \param type the type of the notification. \see NotificationType.
     * \return a notification ID.
     * \see addGroup
     */
    virtual uint addNotification(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters(), uint groupId = 0, bool persistent = false, NotificationType type = ApplicationEvent) = 0;

    /*!
     * Updates a notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId The ID of the notification to be updated
     * \param parameters Parameters for the notification
     * \return true if the update succeeded, false otherwise
     */
    virtual bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters = NotificationParameters()) = 0;


    /*!
     * Adds a new notification group. Later on notifications can be added to
     * this group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param parameters Parameters for the notification group
     * \param persistent \c true if the notification group should be persistent, \c false otherwise
     *
     * \return the new group id.
     */
    virtual uint addGroup(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters(), bool persistent = false) = 0;

    /*!
     * Updates the contents of a notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId The ID of the notification group to be updated
     * \param parameters Parameters for the notification group
     * \return true if the update succeeded, false otherwise
     */
    virtual bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters = NotificationParameters()) = 0;

    /*!
     * Removes a notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId The ID of the notification group to be removed.
     * \return true if the removal succeeded, false otherwise
     */
    virtual bool removeGroup(uint notificationUserId, uint groupId) = 0;

    /*!
     * Returns a user ID for the notification system. The user ID has to
     * be supplied with every notification system call.
     *
     * \return a user ID for the notification system
     */
    virtual uint notificationUserId() = 0;

    /*!
     * Returns list of notification ids by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification ids that belong to notificationUserId
     */
    virtual QList<uint> notificationIdList(uint notificationUserId) = 0;

public slots:
    /*!
     * Removes a notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId The ID of the notification to be removed.
     * \return true if the removal succeeded, false otherwise
     */
    virtual bool removeNotification(uint notificationUserId, uint notificationId) = 0;
};

#endif // NOTIFICATIONMANAGERINTERFACE_H
