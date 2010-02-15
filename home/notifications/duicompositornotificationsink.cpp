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

#include <QGraphicsScene>
#include <QTimer>
#include <DuiInfoBanner>
#include <DuiWindow>
#include <DuiScene>
#include <DuiSceneManager>
#include <DuiOrientationChangeEvent>
#include "homeapplication.h"
#include "mainwindow.h"
#include "notificationmanager.h"
#include "duicompositornotificationsink.h"
#include "notificationwidgetparameterfactory.h"
#include "x11wrapper.h"

DuiCompositorNotificationSink::DuiCompositorNotificationSink() :
    orientationChangeSignalConnected(false)
{
    // Get X11 Atoms for different window types
    Display *dpy = QX11Info::display();
    windowTypeAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    windowTypeDesktopAtom = X11Wrapper::XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    clientListStackingAtom = X11Wrapper::XInternAtom(dpy, "_NET_CLIENT_LIST_STACKING", False);
}

DuiCompositorNotificationSink::~DuiCompositorNotificationSink()
{
    // Destroy the remaining notifications
    foreach(uint id, idToNotification.keys()) {
        notificationDone(id, false);
    }
}

void DuiCompositorNotificationSink::addNotification(const Notification &notification)
{
    if (!canAddNotification(notification)) return;

    if (!orientationChangeSignalConnected) {
        // Get informed about orientation changes
        connect(MainWindow::instance(), SIGNAL(orientationChangeFinished(const Dui::Orientation &)), this, SLOT(rotateInfoBanners(const Dui::Orientation &)));
        orientationChangeSignalConnected = true;
    }

    if (idToNotification.contains(notification.notificationId())) {
        // The notification already exists so update it
        updateNotification(notification);
    } else if (!isDesktopWindowOnTop()) {
        // If the desktop window is not on top show an info banner
        // Create a new toplevel window for the info banner
        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsView *view = new QGraphicsView(scene);
        view->setFrameStyle(0);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setAttribute(Qt::WA_X11NetWmWindowTypeNotification);

        // Create a timer for the info banner; make it a child of the window so it is destroyed automatically
        QTimer *timer = new QTimer(view);
        timer->setSingleShot(true);
        connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
        timer->setProperty("notificationId", notification.notificationId());

        // Create info banner widget
        DuiInfoBanner *infoBanner = createInfoBanner(notification);
        infoBanner->setManagedManually(true);

        // Rotate the view to the current orientation and make the view as big as the info banner and
        setViewSizeAndRotation(*view, *infoBanner);
        view->show();

        // Add the info banner widget to the scene
        scene->addItem(infoBanner);

        // Keep track of the mapping between IDs and private notification information classes
        idToNotification.insert(notification.notificationId(), new DuiCompositorNotificationSinkNotification(view, timer, infoBanner));

        // Make the info banner disappear after the timeout
        timer->start(notification.timeout());
    }
}

void DuiCompositorNotificationSink::updateNotification(const Notification &notification)
{
    DuiCompositorNotificationSinkNotification *sinkNotification = idToNotification.value(notification.notificationId());

    if (sinkNotification != NULL && sinkNotification->infoBanner != NULL) {
        // Update the info banner widget
        sinkNotification->infoBanner->setImageID(notification.parameters().value(NotificationWidgetParameterFactory::imageIdKey()).toString());
        sinkNotification->infoBanner->setBodyText(notification.parameters().value(NotificationWidgetParameterFactory::bodyKey()).toString());
        sinkNotification->infoBanner->setIconID(determineIconId(notification.parameters()));

        // Update the info banner's actions
        updateActions(sinkNotification->infoBanner, notification.parameters());
    }
}

void DuiCompositorNotificationSink::removeNotification(uint notificationId)
{
    notificationDone(notificationId, false);
}

void DuiCompositorNotificationSink::notificationDone(uint notificationId, bool notificationIdInUse)
{
    DuiCompositorNotificationSinkNotification *sinkNotification = idToNotification.take(notificationId);
    if (sinkNotification != NULL) {
        // Destroy the notification window immediately; this also destroys the timer
        QGraphicsScene *scene = sinkNotification->view->scene();
        delete sinkNotification->view;

        // Remove the info banner from the scene and destroy it
        scene->removeItem(sinkNotification->infoBanner);
        delete sinkNotification->infoBanner;

        // Destroy the scene and the notification information
        delete scene;
        delete sinkNotification;
    }

    if (notificationIdInUse) {
        idToNotification.insert(notificationId, NULL);
    }
}

void DuiCompositorNotificationSink::timeout()
{
    QTimer *timer = qobject_cast<QTimer *>(sender());

    if (timer != NULL) {
        // Get the notification ID from the timer
        bool ok = false;
        uint notificationId = timer->property("notificationId").toUInt(&ok);

        if (ok) {
            // Remove the notification
            notificationDone(notificationId, true);
        }
    }
}

DuiCompositorNotificationSink::DuiCompositorNotificationSinkNotification::DuiCompositorNotificationSinkNotification(QGraphicsView *view, QTimer *timer, DuiInfoBanner *infoBanner) :
    view(view),
    timer(timer),
    infoBanner(infoBanner)
{
}

DuiCompositorNotificationSink::DuiCompositorNotificationSinkNotification::~DuiCompositorNotificationSinkNotification()
{
}

bool DuiCompositorNotificationSink::isDesktopWindowOnTop()
{
    bool desktopOnTop = false;

    // Get a list of all windows in their stacking order
    Display *dpy = QX11Info::display();
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;
    int result = X11Wrapper::XGetWindowProperty(dpy, DefaultRootWindow(dpy), clientListStackingAtom, 0, 0x7fffffff, False, XA_WINDOW,
                 &actualType, &actualFormat, &numWindowItems, &bytesLeft, &windowData);

    if (result == Success && windowData != None && numWindowItems > 0) {
        Window *wins = (Window *)windowData;
        unsigned char *typeData = NULL;
        unsigned long numTypeItems;

        // Get the window type
        result = X11Wrapper::XGetWindowProperty(dpy, wins[numWindowItems - 1], windowTypeAtom, 0L, 16L, False, XA_ATOM,
                                                &actualType, &actualFormat, &numTypeItems, &bytesLeft, &typeData);

        if (result == Success) {
            Atom *type = (Atom *)typeData;

            // Check if the types list includes desktop
            for (unsigned int n = 0; n < numTypeItems; n++) {
                if (type[n] == windowTypeDesktopAtom) {
                    desktopOnTop = true;
                    break;
                }
            }

            X11Wrapper::XFree(typeData);
        }

        X11Wrapper::XFree(wins);
    }

    return desktopOnTop;
}

void DuiCompositorNotificationSink::rotateInfoBanners(const Dui::Orientation &orientation)
{
    foreach(DuiCompositorNotificationSinkNotification * notification, idToNotification.values()) {
        // Rotate the views of all existing notifications to the current orientation and resize their sizes accordingly
        if (notification != NULL && notification->view != NULL && notification->infoBanner != NULL) {
            QGraphicsScene *scene = notification->view->scene();
            if (scene != NULL) {
                // Notify each item on the notification view's scene about the new orientation
                DuiOrientationChangeEvent event(orientation);
                Q_FOREACH(QGraphicsItem * item, scene->items()) {
                    scene->sendEvent(item, &event);
                }
            }

            setViewSizeAndRotation(*notification->view, *notification->infoBanner);
        }
    }
}

void DuiCompositorNotificationSink::setViewSizeAndRotation(QGraphicsView &view, const DuiInfoBanner &infoBanner)
{
    QTransform transform;
    transform.rotate(MainWindow::instance()->orientationAngle());
    view.setTransform(transform);
    if (MainWindow::instance()->orientation() == Dui::Landscape) {
        view.setFixedSize(infoBanner.preferredSize().width(), infoBanner.preferredSize().height());
    } else {
        view.setFixedSize(infoBanner.preferredSize().height(), infoBanner.preferredSize().width());
    }
}
