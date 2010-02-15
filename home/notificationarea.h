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

#ifndef NOTIFICATIONAREA_H_
#define NOTIFICATIONAREA_H_

#include <DuiWidgetController>
#include "notificationsink.h"
#include "notificationareamodel.h"

class NotificationManager;
class NotificationAreaSink;
class DuiInfoBanner;
class DuiFlowLayoutPolicy;

/*!
 * The NotificationArea is a widget that shows notifications.
 */
class NotificationArea : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(NotificationArea)

public:
    /*!
     * Constructs a NotificationArea widget.
     *
     * \param parent the parent widget of the NotificationArea, defaults to NULL
     */
    NotificationArea(DuiWidget *parent = NULL);

    /*!
     * Destroys the NotificationArea.
     */
    virtual ~NotificationArea();

private slots:
    /*!
     * Adds a notification to the notification area.
     *
     * \param notification the DuiInfoBanner to be added
     */
    void addNotification(DuiInfoBanner &notification);

    /*!
     * Removes a notification from the notification area.
     *
     * \param notification the DuiInfoBanner to be removed
     */
    void removeNotification(DuiInfoBanner &notification);

signals:
    /*!
     * \brief A signal for notifying about changes in the notification count.
     */
    void notificationCountChanged(int notificationCount);

private:
    //! Home notification sink for visualizing the notification on the notification area
    NotificationAreaSink *notificationAreaSink;
};

#endif /* NOTIFICATIONAREA_H_ */
