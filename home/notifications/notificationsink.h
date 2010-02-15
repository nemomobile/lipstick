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

#ifndef NOTIFICATIONSINK_H
#define NOTIFICATIONSINK_H

#include <QObject>
#include "notificationmanager.h"

/*!
 * Notification sink is an abstract interface for classes that can present notifications.
 * Concrete notification sinks are responsible for presenting notifications and possibly
 * notifying the NotificationManagerInterface if the notification was dismissed.
 */
class NotificationSink : public QObject
{
    Q_OBJECT

public:
    NotificationSink();

    //! Returns whether the the sink should process notifications with ApplicationEvent type
    bool applicationEventsEnabled();

    /*!
     * Sets whether the the sink should process notifications with ApplicationEvent type
     * \param enabled the new status
     */
    void setApplicationEventsEnabled(bool enabled);

protected:
    /*!
     * Returns whether the sink should add the given notification
     * \param notification the notification
     */
    bool canAddNotification(const Notification &notification);

protected slots:
    /*!
     * Adds a notification to be presented.
     * \param notification the data of the notification
     */
    virtual void addNotification(const Notification &notification) = 0;

    /*!
     * Removes a notification.
     *
     * \param notificationId the ID of the notification to be removed
     */
    virtual void removeNotification(uint notificationId) = 0;

    /*!
     * Creates a notification group.
     *
     * \param groupId the ID of the group
     * \param parameters NotificationParameters for the group
     */
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);

    /*!
     * Removes a notification group.
     *
     * \param groupId the ID of the group
     */
    virtual void removeGroup(uint groupId);

signals:
    /*!
     * A signal for notifying that the notification has been canceled by the user.
     */
    void cancelNotification(uint notificationId);

private:
    bool appEventsEnabled;
};

#endif
