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

#include "homenotificationsink.h"
#include "notificationmanager.h"
#include "notificationwidgetparameterfactory.h"
#include "mainwindow.h"
#include "homeapplication.h"
#include <DuiInfoBanner>
#include <DuiSceneManager>
#include <QTimer>

HomeNotificationSink::HomeNotificationSink() : notificationAreaVisible(false)
{
}

HomeNotificationSink::~HomeNotificationSink()
{
    // Destroy the remaining notifications
    foreach(uint id, idToInfoBanner.keys()) {
        notificationDone(id);
    }
}

void HomeNotificationSink::setNotificationAreaVisible(bool visible)
{
    notificationAreaVisible = visible;
}

bool HomeNotificationSink::canAddNotification(const Notification &notification)
{
    return !notificationAreaVisible && NotificationSink::canAddNotification(notification);
}

void HomeNotificationSink::addNotification(const Notification &notification)
{
    if (idToInfoBanner.contains(notification.notificationId())) {
        // The notification already exists so update it
        updateNotification(notification);
    } else {
        if (canAddNotification(notification)) {
            // Get notification parameter values
            // Create a new info banner for the notification
            DuiInfoBanner *infoBanner = createInfoBanner(notification);
            connect(infoBanner, SIGNAL(windowHidden()), this, SLOT(notificationAnimationDone()), Qt::QueuedConnection);

            // Create a timer for the notification; make it a child of the info banner so it is destroyed automatically
            QTimer *timer = new QTimer(infoBanner);
            timer->setSingleShot(true);
            connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
            timer->setProperty("notificationId", notification.notificationId());

            // Keep track of the mapping between IDs and info banners, between info banner widgets and notification information
            idToInfoBanner.insert(notification.notificationId(), infoBanner);
            infoBannerToNotification.insert(infoBanner, new HomeNotificationSinkNotification(notification.notificationId(), notification.groupId(), notification.userId(), notification.parameters(), notification.type(), notification.timeout()));

            // Keep the ownership in this class, destroy here based on animation done signals
            MainWindow::instance()->sceneManager()->showWindow(infoBanner);

            // Make the info banner disappear after the timeout
            timer->start(notification.timeout());
        } else {
            // Previews are disabled: Transfer the notification onwards immediately
            emit transferNotification(notification);
        }
    }
}

void HomeNotificationSink::updateNotification(const Notification &notification)
{
    DuiInfoBanner *infoBanner = idToInfoBanner.value(notification.notificationId());
    if (infoBanner != NULL) {
        // Is the info banner being displayed?
        HomeNotificationSinkNotification *sinkNotification = infoBannerToNotification.value(infoBanner);
        if (sinkNotification->state == HomeNotificationSinkNotification::Visible) {
            sinkNotification->setParameters(notification.parameters());

            // Update the info banner widget
            infoBanner->setImageID(notification.parameters().value(NotificationWidgetParameterFactory::imageIdKey()).toString());
            infoBanner->setBodyText(notification.parameters().value(NotificationWidgetParameterFactory::bodyKey()).toString());
            infoBanner->setIconID(determineIconId(notification.parameters()));

            // Update the info banner's actions
            updateActions(infoBanner, notification.parameters());
        } else if (sinkNotification->state == HomeNotificationSinkNotification::Transferred) {
            // The notification has been transferred onwards so transfer the update as well
            emit transferNotification(notification);
        }
    }
}

void HomeNotificationSink::removeNotification(uint notificationId)
{
    removeNotification(idToInfoBanner.value(notificationId));
}

void HomeNotificationSink::removeNotification(DuiInfoBanner *infoBanner)
{
    if (infoBanner != NULL) {
        // Mark the notification as removed so it won't get transferred on the notification area
        HomeNotificationSinkNotification *sinkNotification = infoBannerToNotification.value(infoBanner);
        sinkNotification->state = HomeNotificationSinkNotification::Removed;

        if (infoBanner->isVisible()) {
            // Info banner is visible: don't destroy it, let the scene animator animate it and destroy when animation is complete
            MainWindow::instance()->sceneManager()->hideWindow(infoBanner);
        } else {
            // Info banner is not visible: destroy it right away
            notificationDone(sinkNotification->notificationId());
        }
    }
}

void HomeNotificationSink::notificationAnimationDone()
{
    DuiInfoBanner *infoBanner = qobject_cast<DuiInfoBanner *>(sender());

    if (infoBanner != NULL) {
        HomeNotificationSinkNotification *sinkNotification = infoBannerToNotification.value(infoBanner);

        if (sinkNotification != NULL) {
            if (sinkNotification->type() == NotificationManagerInterface::ApplicationEvent && sinkNotification->state == HomeNotificationSinkNotification::Visible) {
                // Application event notifications are shown in the notification area so transfer them there
                emit transferNotification(*static_cast<Notification *>(sinkNotification));
                sinkNotification->state = HomeNotificationSinkNotification::Transferred;
            }

            // Remove from the maps and destroy the info banner
            if (sinkNotification->state == HomeNotificationSinkNotification::Removed) {
                notificationDone(sinkNotification->notificationId());
            }
        } else {
            // The info banner is not known, destroy it
            delete infoBanner;
        }
    }
}

void HomeNotificationSink::notificationDone(uint notificationId)
{
    DuiInfoBanner *infoBanner = idToInfoBanner.take(notificationId);
    if (infoBanner != NULL) {
        delete infoBannerToNotification.take(infoBanner);
        delete infoBanner;
    }
}

void HomeNotificationSink::timeout()
{
    QTimer *timer = qobject_cast<QTimer *>(sender());

    if (timer != NULL) {
        // Get the notification ID from the timer
        bool ok = false;
        uint notificationId = timer->property("notificationId").toUInt(&ok);

        if (ok) {
            DuiInfoBanner *infoBanner = idToInfoBanner.value(notificationId);
            if (infoBanner != NULL) {
                // Make the info banner disappear
                MainWindow::instance()->sceneManager()->hideWindow(infoBanner);
            }
        }
    }
}

HomeNotificationSink::HomeNotificationSinkNotification::HomeNotificationSinkNotification(uint notificationId, uint groupId, uint userId, const NotificationParameters &parameters,
        NotificationManagerInterface::NotificationType type, int timeout) :
    Notification(notificationId, groupId, userId, parameters, type, timeout),
    state(HomeNotificationSinkNotification::Visible)
{
}
