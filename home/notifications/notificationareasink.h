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

#ifndef NOTIFICATIONAREASINK_H_
#define NOTIFICATIONAREASINK_H_

#include "widgetnotificationsink.h"

class DuiInfoBanner;

/*!
 * A notification sink for the NotificationArea.
 */
class NotificationAreaSink : public WidgetNotificationSink
{
    Q_OBJECT

public:
    /*!
     * Creates a notification sink for the NotificationArea.
     */
    NotificationAreaSink();

    /*!
     * Destroys the NotificationAreaSink.
     */
    virtual ~NotificationAreaSink();

private slots:
    //! \reimp
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
     * A slot for catching notification clicks
     */
    void notificationClicked();

signals:
    /*!
     * Adds a notification to a notification area.
     *
     * \param notification the DuiInfoBanner to be added
     */
    virtual void addNotification(DuiInfoBanner &notification);

    /*!
     * Removes a notification from a notification area.
     *
     * \param notification the DuiInfoBanner to be removed
     */
    void removeNotification(DuiInfoBanner &notification);

private:
    //! Sets up the info banner can connects its signals
    void setupInfoBanner(DuiInfoBanner *infoBanner);
    //! Updates a notification widget
    DuiInfoBanner *updateNotification(DuiInfoBanner *dn, const NotificationParameters &parameters);

    //! A mapping between notification IDs and DuiInfoBanner widgets
    QHash<uint, DuiInfoBanner *> notificationIdToDuiInfoBanner;

    //! A mapping between group IDs and DuiInfoBanner widgets
    QHash<uint, DuiInfoBanner *> groupIdToDuiInfoBanner;
};

#endif /* NOTIFICATIONAREASINK_H_ */
