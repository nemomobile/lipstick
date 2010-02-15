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

#ifndef NOTIFIERNOTIFICATIONSINK_H_
#define NOTIFIERNOTIFICATIONSINK_H_

#include "notificationsink.h"

/*!
 * A notification sink for the Notifier.
 */
class NotifierNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    /*!
     * Creates a notifier notification sink.
     */
    NotifierNotificationSink();

    /*!
     * Destroys the NotifierNotificationSink.
     */
    virtual ~NotifierNotificationSink();

signals:
    /*!
     * Signals information about notification count changes.
     *
     * \param count the number of notifications
     */
    void notificationCountChanged(uint count);

private slots:
    //! \reimp
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

private:
    //! The group for each notification
    QHash<uint, uint> groupForNotification;

    //! The notification count of each notification
    QHash<uint, uint> notificationCountForNotification;

    //! The notification count of each group
    QHash<uint, uint> notificationCountForGroup;

    //! Current notification count
    uint notificationCount;

    //! Updates the notification count and signals notificationCountChanged() if it changed
    void updateNotificationCount();
};

#endif /* NOTIFIERNOTIFICATIONSINK_H_ */
