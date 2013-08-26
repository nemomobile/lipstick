/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QGuiApplication>
#include <QScreen>
#include "homewindow.h"
#include <QQmlContext>
#include "utilities/closeeventeater.h"
#include "notifications/notificationmanager.h"
#include "notificationpreviewpresenter.h"
#include "compositor/lipstickcompositor.h"

#include <qmdisplaystate.h>
#include <qmlocks.h>

enum PreviewMode {
    AllNotificationsEnabled = 0,
    ApplicationNotificationsDisabled,
    SystemNotificationsDisabled,
    AllNotificationsDisabled
};

NotificationPreviewPresenter::NotificationPreviewPresenter(QObject *parent) :
    QObject(parent),
    window(0),
    currentNotification(0)
    ,locks(new MeeGo::QmLocks(this)),
    displayState(new MeeGo::QmDisplayState(this))
{
    connect(NotificationManager::instance(), SIGNAL(notificationModified(uint)), this, SLOT(updateNotification(uint)));
    connect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), this, SLOT(removeNotification(uint)));
}

NotificationPreviewPresenter::~NotificationPreviewPresenter()
{
    delete window;
}

void NotificationPreviewPresenter::showNextNotification()
{
    if (!LipstickCompositor::instance() && !notificationQueue.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(showNextNotification()));
        return;
    }

    if (notificationQueue.isEmpty()) {
        // No more notifications to show: hide the notification window if it's visible
        if (window != 0 && window->isVisible()) {
            window->hide();
        }

        setCurrentNotification(0);
    } else {
        LipstickNotification *notification = notificationQueue.takeFirst();

        if (locks->getState(MeeGo::QmLocks::TouchAndKeyboard) == MeeGo::QmLocks::Locked && displayState->get() == MeeGo::QmDisplayState::Off) {
            // Screen locked and off: don't show the notification but just remove it from the queue
            emit notificationPresented(notification->property("id").toUInt());

            setCurrentNotification(0);

            showNextNotification();
        } else {
            // Show the notification window and the first queued notification in it
            createWindowIfNecessary();
            if (!window->isVisible()) {
                window->show();
            }

            emit notificationPresented(notification->property("id").toUInt());

            setCurrentNotification(notification);
        }
    }
}

LipstickNotification *NotificationPreviewPresenter::notification() const
{
    return currentNotification;
}

void NotificationPreviewPresenter::updateNotification(uint id)
{
    LipstickNotification *notification = NotificationManager::instance()->notification(id);

    if (notification != 0) {
        notification->setProperty("id", id);
        if (notificationShouldBeShown(notification)) {
            // Add the notification to the queue if not already there or the current notification
            if (currentNotification != notification && !notificationQueue.contains(notification)) {
                notificationQueue.append(notification);

                // Show the notification if no notification currently being shown
                if (currentNotification == 0) {
                    showNextNotification();
                }
            }
        } else {
            // Remove updated notification only from the queue so that a currently visible notification won't suddenly disappear
            emit notificationPresented(id);

            removeNotification(id, true);

            if (currentNotification != notification && notification->hints().value(NotificationManager::HINT_URGENCY).toInt() >= 2) {
                NotificationManager::instance()->CloseNotification(id);
            }
        }
    }
}

void NotificationPreviewPresenter::removeNotification(uint id, bool onlyFromQueue)
{
    // Remove the notification from the queue
    LipstickNotification *notification = NotificationManager::instance()->notification(id);

    if (notification != 0) {
        notificationQueue.removeAll(notification);

        // If the notification is currently being shown hide it - the next notification will be shown after the current one has been hidden
        if (!onlyFromQueue && currentNotification == notification) {
            currentNotification = 0;
            emit notificationChanged();
        }
    }
}

void NotificationPreviewPresenter::createWindowIfNecessary()
{
    if (window != 0) {
        return;
    }

    window = new HomeWindow();
    window->setGeometry(QRect(QPoint(), QGuiApplication::primaryScreen()->size()));
    window->setCategory(QLatin1String("notification"));
    window->setWindowTitle("Notification");
    window->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
    window->setContextProperty("notificationPreviewPresenter", this);
    window->setSource(QUrl("qrc:/qml/NotificationPreview.qml"));
    window->installEventFilter(new CloseEventEater(this));
}

bool NotificationPreviewPresenter::notificationShouldBeShown(LipstickNotification *notification)
{
    bool screenOrDeviceLocked = locks->getState(MeeGo::QmLocks::TouchAndKeyboard) == MeeGo::QmLocks::Locked || locks->getState(MeeGo::QmLocks::Device) == MeeGo::QmLocks::Locked;
    bool notificationHidden = notification->hints().value(NotificationManager::HINT_HIDDEN).toBool();
    bool notificationHasPreviewText = !(notification->previewBody().isEmpty() && notification->previewSummary().isEmpty());
    int notificationIsCritical = notification->hints().value(NotificationManager::HINT_URGENCY).toInt() >= 2;

    uint mode = AllNotificationsEnabled;
    QWaylandSurface *surface = LipstickCompositor::instance()->surfaceForId(LipstickCompositor::instance()->topmostWindowId());
    if (surface != 0) {
        mode = surface->windowProperties().value("NOTIFICATION_PREVIEWS_DISABLED", uint(AllNotificationsEnabled)).toUInt();
    }

    return !notificationHidden && notificationHasPreviewText && (!screenOrDeviceLocked || notificationIsCritical) &&
            (mode == AllNotificationsEnabled || (mode == ApplicationNotificationsDisabled && notificationIsCritical) || (mode == SystemNotificationsDisabled && !notificationIsCritical));
}

void NotificationPreviewPresenter::setCurrentNotification(LipstickNotification *notification)
{
    if (currentNotification != notification) {
        if (currentNotification != 0 && currentNotification->hints().value(NotificationManager::HINT_URGENCY).toInt() >= 2) {
            NotificationManager::instance()->CloseNotification(currentNotification->property("id").toUInt());
        }

        currentNotification = notification;
        emit notificationChanged();
    }
}
