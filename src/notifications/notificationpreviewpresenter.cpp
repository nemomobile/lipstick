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

#include <QApplication>
#include <QDesktopWidget>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QX11Info>
#include <X11/extensions/shape.h>
#include "utilities/closeeventeater.h"
#include "xtools/x11wrapper.h"
#include "notifications/notificationmanager.h"
#include "notificationpreviewpresenter.h"

NotificationPreviewPresenter::NotificationPreviewPresenter(QObject *parent) :
    QObject(parent),
    window(0),
    currentNotification(0),
    presentOnlyCriticalNotifications(false)
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

        currentNotification = 0;
        emit notificationChanged();
    } else {
        // A notification to show: show the notification window and the first queued notification in it
        createWindowIfNecessary();
        if (!window->isVisible()) {
            window->show();
        }

        currentNotification = notificationQueue.takeFirst();
        emit notificationChanged();
    }
}

Notification *NotificationPreviewPresenter::notification() const
{
    return currentNotification;
}

void NotificationPreviewPresenter::updateNotification(uint id)
{
    Notification *notification = NotificationManager::instance()->notification(id);
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

    window = new QDeclarativeView();
    window->setAttribute(Qt::WA_TranslucentBackground);
    window->setAttribute(Qt::WA_X11DoNotAcceptFocus);
    window->setAttribute(Qt::WA_X11NetWmWindowTypeNotification);
    window->setWindowTitle("Notification");
    window->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    window->viewport()->setAutoFillBackground(false);
    window->rootContext()->setContextProperty("initialSize", QApplication::desktop()->screenGeometry(window).size());
    window->rootContext()->setContextProperty("notificationPreviewPresenter", this);
    window->setSource(QUrl("qrc:/qml/NotificationPreview.qml"));
    window->installEventFilter(new CloseEventEater(this));
}

bool NotificationPreviewPresenter::notificationShouldBeShown(Notification *notification)
{
    return !(notification->previewBody().isEmpty() && notification->previewSummary().isEmpty()) && (!presentOnlyCriticalNotifications || notification->hints().value(NotificationManager::HINT_URGENCY).toInt() >= 2);
}

void NotificationPreviewPresenter::setNotificationPreviewRect(qreal x1, qreal y1, qreal x2, qreal y2)
{
    Display *dpy = QX11Info::display();
    XRectangle rect;
    rect.x = x1 < x2 ? x1 : x2;
    rect.y = y1 < y2 ? y1 : y2;
    rect.width = x1 < x2 ? (x2 - x1) : (x1 - x2);
    rect.height = y1 < y2 ? (y2 - y1) : (y1 - y2);
    XserverRegion shapeRegion = X11Wrapper::XFixesCreateRegion(dpy, &rect, 1);
    X11Wrapper::XFixesSetWindowShapeRegion(dpy, window->winId(), ShapeInput, 0, 0, shapeRegion);
    X11Wrapper::XFixesDestroyRegion(dpy, shapeRegion);
    X11Wrapper::XSync(dpy, False);
}

void NotificationPreviewPresenter::setPresentOnlyCriticalNotifications(bool onlyCritical)
{
    presentOnlyCriticalNotifications = onlyCritical;
}
