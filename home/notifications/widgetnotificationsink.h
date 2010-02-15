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

#ifndef WIDGETNOTIFICATIONSINK_H
#define WIDGETNOTIFICATIONSINK_H

#include "notificationsink.h"
#include <DuiInfoBanner>

/*!
 * WidgetNotificationSink is a common base class for all notification sinks that trigger
 * visual feedback using a graphics widget.
 *
 * WidgetNotificationSink creates DuiInfoBanner widgets for notifications. When DuiInfoBanner
 * is clicked the action bound to the notification is triggered and notification is removed
 * from the notification system signalling notificationRemovalRequested().
 */
class WidgetNotificationSink : public NotificationSink
{
    Q_OBJECT

signals:
    /*!
     * Requests removal of a notification from the notification system.
     * This can be connected to the associated NotificationManagerInterface::removeNotification().
     * \param notificationId Id of the notification to be removed from the system.
     */
    void notificationRemovalRequested(uint notificationId);

    /*!
     * Requests removal of notifications in a group.
     * \param groupId Id of the group to be cleared.
     */
    void notificationGroupClearingRequested(uint groupId);

protected:
    /*!
     * Determines icon id of a notification based on the given notification parameters.
     * \param parameters Notification parameters to determine the icon id from.
     * \return Logical icon id as a string
     */
    QString determineIconId(const NotificationParameters &parameters);

    /*!
     * Creates a DuiInfoBanner widget to represent a notification object.
     * Ownership of the constructed object is passed to the caller.
     * \param notification The notification object to represent with the DuiInfoBanner.
     * \return Constructed DuiInfoBanner that represents the notification.
     */
    DuiInfoBanner *createInfoBanner(const Notification &notification);

    /*!
     * Creates a DuiInfoBanner widget from the given notification parameters.
     * Ownership of the constructed object is passed to the caller.
     * \param type Type of the info banner to be constructed.
     * \param groupId The group ID to be associated with the info banner.
     * \param params NotificationParameters according to which configure the DuiInfoBanner.
     */
    DuiInfoBanner *createInfoBanner(DuiInfoBanner::BannerType type, uint groupId, const NotificationParameters &parameters);

    /*!
     * Removes old actions from the given info banner and adds the action
     * specified in the notification (if any)
     *
     * \param infoBanner the DuiInfoBanner to update
     * \param parameters the NotificationParameters to get the action from
     */
    void updateActions(DuiInfoBanner *infoBanner, const NotificationParameters &parameters);

private:
    /*!
     * Determines icon id of a notification based on the given event type.
     * \param eventType Notification event type.
     * \return Logical icon id as a string or an empty string if icon has not been defined for the
     *         given event type.
     */
    QString determineIconIdFromEventType(const QString &eventType);

    //! DuiInfoBanner property to store the notification ID into
    static const char *NOTIFICATION_ID_PROPERTY;
    //! DuiInfoBanner property to store the group ID into
    static const char *GROUP_ID_PROPERTY;

private slots:
    /*!
     * A slot for catching info banner clicks
     */
    void infoBannerClicked();
};

#endif // WIDGETNOTIFICATIONSINK_H
