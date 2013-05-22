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

#include <qmdisplaystate.h>
#include <qmlocks.h>

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
    if (notificationQueue.isEmpty()) {
        // No more notifications to show: hide the notification window if it's visible
        if (window != 0 && window->isVisible()) {
            window->hide();
        }

        setCurrentNotification(0);
    } else {
        if (locks->getState(MeeGo::QmLocks::TouchAndKeyboard) == MeeGo::QmLocks::Locked && displayState->get() == MeeGo::QmDisplayState::Off) {
            // Screen locked and off: don't show the notification but just remove it from the queue
            notificationQueue.removeFirst();

            setCurrentNotification(0);

            showNextNotification();
        } else {
            // Show the notification window and the first queued notification in it
            createWindowIfNecessary();
            if (!window->isVisible()) {
                window->show();
            }

            setCurrentNotification(notificationQueue.takeFirst());
        }
    }
}

Notification *NotificationPreviewPresenter::notification() const
{
    return currentNotification;
}

void NotificationPreviewPresenter::updateNotification(uint id)
{
    Notification *notification = NotificationManager::instance()->notification(id);
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
        removeNotification(id, true);
    }
}

void NotificationPreviewPresenter::removeNotification(uint id, bool onlyFromQueue)
{
    // Remove the notification from the queue
    Notification *notification = NotificationManager::instance()->notification(id);

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
    window->setWindowTitle("Notification");
    window->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
    window->setContextProperty("notificationPreviewPresenter", this);
    window->setSource(QUrl("qrc:/qml/NotificationPreview.qml"));
    window->installEventFilter(new CloseEventEater(this));
}

bool NotificationPreviewPresenter::notificationShouldBeShown(Notification *notification)
{
    bool screenOrDeviceLocked = locks->getState(MeeGo::QmLocks::TouchAndKeyboard) == MeeGo::QmLocks::Locked || locks->getState(MeeGo::QmLocks::Device) == MeeGo::QmLocks::Locked;
    bool notificationHidden = notification->hints().value(NotificationManager::HINT_HIDDEN).toBool();
    bool notificationHasPreviewText = !(notification->previewBody().isEmpty() && notification->previewSummary().isEmpty());
    int notificationIsCritical = notification->hints().value(NotificationManager::HINT_URGENCY).toInt() >= 2;
    return !notificationHidden && notificationHasPreviewText && (!screenOrDeviceLocked || notificationIsCritical);
}

void NotificationPreviewPresenter::setNotificationPreviewRect(qreal x1, qreal y1, qreal x2, qreal y2)
{
    // TODO set touch area
    Q_UNUSED(x1)
    Q_UNUSED(y1)
    Q_UNUSED(x2)
    Q_UNUSED(y2)
}

void NotificationPreviewPresenter::setCurrentNotification(Notification *notification)
{
    if (currentNotification != notification) {
        if (currentNotification != 0 && currentNotification->hints().value(NotificationManager::HINT_URGENCY).toInt() >= 2) {
            NotificationManager::instance()->CloseNotification(currentNotification->property("id").toUInt());
        }

        currentNotification = notification;
        emit notificationChanged();
    }
}
