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

#include "notificationareasink.h"
#include "notificationwidgetparameterfactory.h"

#include <DuiInfoBanner>
#include <DuiRemoteAction>

NotificationAreaSink::NotificationAreaSink()
{
}

NotificationAreaSink::~NotificationAreaSink()
{
    // Destroy the remaining notifications
    foreach(DuiInfoBanner * n, notificationIdToDuiInfoBanner) {
        delete n;
    }

    // Destroy the remaining groups
    foreach(DuiInfoBanner * n, groupIdToDuiInfoBanner) {
        delete n;
    }
}

void NotificationAreaSink::setupInfoBanner(DuiInfoBanner *infoBanner)
{
    // Don't allow the scene manager to destroy the banner
    infoBanner->setManagedManually(true);

    // Catch clicks for removing the info banner
    connect(infoBanner, SIGNAL(clicked()), this, SLOT(notificationClicked()), Qt::QueuedConnection);
}

DuiInfoBanner *NotificationAreaSink::updateNotification(DuiInfoBanner *infoBanner, const NotificationParameters &parameters)
{
    // Update the info banner widget
    infoBanner->setImageID(parameters.value(NotificationWidgetParameterFactory::imageIdKey()).toString());
    infoBanner->setBodyText(parameters.value(NotificationWidgetParameterFactory::bodyKey()).toString());
    infoBanner->setIconID(determineIconId(parameters));

    // Update the info banner's actions
    updateActions(infoBanner, parameters);

    return infoBanner;
}

void NotificationAreaSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    DuiInfoBanner *infoBanner = groupIdToDuiInfoBanner.value(groupId);
    if (infoBanner != NULL) {
        // If the info banner is already in the map, only update it
        updateNotification(infoBanner, parameters);
    } else {
        // Keep track of the mapping between IDs and info banners
        DuiInfoBanner *infoBanner = createInfoBanner(DuiInfoBanner::Event, groupId, parameters);
        setupInfoBanner(infoBanner);
        groupIdToDuiInfoBanner.insert(groupId, infoBanner);
    }
}

void NotificationAreaSink::removeGroup(uint groupId)
{
    if (groupIdToDuiInfoBanner.contains(groupId)) {
        DuiInfoBanner *infoBanner = groupIdToDuiInfoBanner.take(groupId);

        // If the group is already visible, send signal to remove it
        if (infoBanner->parentItem()) {
            // Remove from the notification area
            emit removeNotification(*infoBanner);
        }

        // Destroy
        delete infoBanner;
    }
}

void NotificationAreaSink::addNotification(const Notification &notification)
{
    // Only application events are shown in the notification area
    if (notification.type() == NotificationManagerInterface::ApplicationEvent) {
        // If the notification has group id other than 0, don't add the info banner to
        // notification area, but only add the group if it exists and not already added
        if (notification.groupId() > 0) {
            DuiInfoBanner *infoBanner = groupIdToDuiInfoBanner.value(notification.groupId());
            if (infoBanner && infoBanner->parentItem() == NULL) {
                // Add the group to the notification area if this is the first notification to the group
                emit addNotification(*infoBanner);
            }
        } else {
            // The notification is not in a group, add it as such to notification area
            DuiInfoBanner *infoBanner = notificationIdToDuiInfoBanner.value(notification.notificationId());
            if (infoBanner != NULL) {
                // If the notification is already in the map, only update it
                updateNotification(infoBanner, notification.parameters());
            } else {
                DuiInfoBanner *infoBanner = createInfoBanner(notification);
                setupInfoBanner(infoBanner);
                notificationIdToDuiInfoBanner.insert(notification.notificationId(), infoBanner);

                // Add to the notification area
                emit addNotification(*infoBanner);
            }
        }
    }
}

void NotificationAreaSink::removeNotification(uint notificationId)
{
    if (notificationIdToDuiInfoBanner.contains(notificationId)) {
        DuiInfoBanner *infoBanner = notificationIdToDuiInfoBanner.take(notificationId);

        // Remove from the notification area
        emit removeNotification(*infoBanner);

        // Destroy
        delete infoBanner;
    }
}

void NotificationAreaSink::notificationClicked()
{
    DuiInfoBanner *infoBanner = qobject_cast<DuiInfoBanner *>(sender());

    if (infoBanner != NULL) {
        // Trigger each remote action associated with the clicked info banner
        foreach(QAction * qAction, infoBanner->actions()) {
            DuiRemoteAction *remoteAction = dynamic_cast<DuiRemoteAction *>(qAction);
            if (remoteAction) {
                remoteAction->trigger();
            }
        }

        if (!notificationIdToDuiInfoBanner.keys(infoBanner).isEmpty()) {
            // The info banner represents a single notification: Remove the notification ID mapping
            foreach(uint key, notificationIdToDuiInfoBanner.keys(infoBanner)) {
                notificationIdToDuiInfoBanner.remove(key);
            }

            // Remove from the notification area
            emit removeNotification(*infoBanner);

            // Destroy
            delete infoBanner;
        } else {
            // Remove from the notification area
            emit removeNotification(*infoBanner);
        }
    }
}
