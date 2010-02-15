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

#ifndef HOMENOTIFICATIONSINK_H_
#define HOMENOTIFICATIONSINK_H_

#include <QHash>
#include "notification.h"
#include "widgetnotificationsink.h"

class DuiInfoBanner;

/*!
 * HomeNotificationSink implements the NotificationSink interface for
 * displaying notifications on the home screen as DuiInfoBanners.
 */
class HomeNotificationSink : public WidgetNotificationSink
{
    Q_OBJECT

public:
    HomeNotificationSink();
    virtual ~HomeNotificationSink();

    /*!
      * Sets the visibility status of the notification area
      *
      * \param visible the new state
      */
    void setNotificationAreaVisible(bool visible);

protected:
    /*!
     * Returns whether the sink should add the given notification
     * \param notification the notification
     */
    bool canAddNotification(const Notification &notification);

private slots:
    //! \reimp
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
     * \brief For getting information that the window animation is done.
     */
    void notificationAnimationDone();

    /*!
     * A slot for timing out the notification windows
     */
    void timeout();

signals:
    /*!
     * Transfers a notification to another sink.
     *
     * \param notification the notification to be presented
     */
    void transferNotification(const Notification &notification);

private:
    /*!
     * Updates an existing info banner with the given notification parameters.
     *
     * \param notification the notification to be updated
     */
    void updateNotification(const Notification &notification);

    /*!
     * Removes a notification with the given info banner.
     *
     * \param infoBanner a pointer to the info banner of the notification to be removed
     */
    void removeNotification(DuiInfoBanner *infoBanner);

    /*!
     * Destroys the notification objects.
     *
     * \param notificationId the ID of the notification to be removed
     */
    void notificationDone(uint notificationId);

    /*!
     * A private class for storing notification information
     */
    class HomeNotificationSinkNotification : public Notification
    {
    public:
        //! The notification states (visible, hidden, transferred, removed)
        enum State {
            Visible,
            Hidden,
            Transferred,
            Removed
        };

        HomeNotificationSinkNotification(uint notificationId, uint groupId, uint userId, const NotificationParameters &parameters,
                                         NotificationManagerInterface::NotificationType type, int timeout);

        //! The state of the notification (visible, hidden, transferred, removed)
        State state;
    };

    //! A mapping between notification IDs and DuiInfoBanner widgets
    QHash<uint, DuiInfoBanner *> idToInfoBanner;

    //! A mapping between DuiInfoBanner widgets and private notification information classes
    QHash<DuiInfoBanner *, HomeNotificationSinkNotification *> infoBannerToNotification;

    //! Whether the notification area is visible
    bool notificationAreaVisible;
};

#endif /* HOMENOTIFICATIONSINK_H_ */
