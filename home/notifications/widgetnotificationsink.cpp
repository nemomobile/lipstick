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

#include "widgetnotificationsink.h"
#include "notificationwidgetparameterfactory.h"
#include "genericnotificationparameterfactory.h"
#include <DuiRemoteAction>
#include "homeapplication.h"
#include "eventtypestore.h"
#include <QSettings>
#include <QImageReader>
#include <QPixmap>

const char *WidgetNotificationSink::NOTIFICATION_ID_PROPERTY = "notificationId";
const char *WidgetNotificationSink::GROUP_ID_PROPERTY = "groupId";
static const int PIXMAP_WIDTH_MAX = 100;
static const int PIXMAP_HEIGHT_MAX = 100;
static const int IMAGE_DEFAULT_SIZE = 64;


/*
    Creates QPixmap from absolute path. Images smaller than maximum size 100x100
    are scaled to default size 64x64.
    Takes path to image as an argument, imagePath.
    Returns QPixmap object constructed from the given path.
    In case of error null QPixmap is returned.
*/
QPixmap loadAndScalePixmap(const QString &imagePath)
{
    QImageReader imageReader(imagePath);
    QPixmap pixmap = QPixmap();

    if (imageReader.canRead()) {
        QSize size = imageReader.size();
        if (size.height() <= PIXMAP_HEIGHT_MAX && size.width() <= PIXMAP_WIDTH_MAX) {
            size.scale(IMAGE_DEFAULT_SIZE, IMAGE_DEFAULT_SIZE, Qt::KeepAspectRatio);
            imageReader.setScaledSize(size);

            QImage image = imageReader.read();
            if (!image.isNull()) {
                pixmap = QPixmap::fromImage(image);
            }
        }
    }
    return pixmap;
}

QString WidgetNotificationSink::determineIconIdFromEventType(const QString &eventType)
{
    QString iconId;
    if (!eventType.isEmpty()) {
        HomeApplication *app = dynamic_cast<HomeApplication *>(qApp);
        if (app != NULL) {
            NotificationManager &notificationManager = app->notificationManager();
            const EventTypeStore &store = notificationManager.eventTypeStore();
            const QSettings *settings = store.settingsForEventType(eventType);
            if (settings != NULL) {
                iconId = settings->value(NotificationWidgetParameterFactory::iconIdKey()).toString();
            }
        }
    }
    return iconId;
}

QString WidgetNotificationSink::determineIconId(const NotificationParameters &parameters)
{
    QString iconId = parameters.value(NotificationWidgetParameterFactory::iconIdKey()).toString();
    if (iconId.isEmpty()) {
        QString eventType = parameters.value(GenericNotificationParameterFactory::eventTypeKey()).toString();
        iconId = determineIconIdFromEventType(eventType);
        if (iconId.isEmpty()) {
            iconId = "default";
        }
    }
    return iconId;
}

DuiInfoBanner *WidgetNotificationSink::createInfoBanner(const Notification &notification)
{
    DuiInfoBanner *infoBanner = createInfoBanner(notification.type() == NotificationManagerInterface::ApplicationEvent ? DuiInfoBanner::Event : DuiInfoBanner::Information,
                                notification.groupId(), notification.parameters());
    infoBanner->setProperty(NOTIFICATION_ID_PROPERTY, notification.notificationId());

    return infoBanner;
}

DuiInfoBanner *WidgetNotificationSink::createInfoBanner(DuiInfoBanner::BannerType type, uint groupId, const NotificationParameters &parameters)
{
    QString imageId = parameters.value(NotificationWidgetParameterFactory::imageIdKey()).toString();
    QString body = parameters.value(NotificationWidgetParameterFactory::bodyKey()).toString();
    QString iconId = determineIconId(parameters);
    DuiInfoBanner *infoBanner = new DuiInfoBanner(type);

    if (QFileInfo(imageId).isAbsolute()) {
        infoBanner->setPixmap(loadAndScalePixmap(imageId));
    } else {
        infoBanner->setImageID(imageId);
    }

    infoBanner->setBodyText(body);
    infoBanner->setIconID(iconId);
    infoBanner->setProperty(GROUP_ID_PROPERTY, groupId);

    updateActions(infoBanner, parameters);

    // Catch clicks from the info banner
    connect(infoBanner, SIGNAL(clicked()), this, SLOT(infoBannerClicked()), Qt::QueuedConnection);

    return infoBanner;
}

void WidgetNotificationSink::updateActions(DuiInfoBanner *infoBanner, const NotificationParameters &parameters)
{
    // Remove the old actions
    foreach(QAction * qAction, infoBanner->actions()) {
        infoBanner->removeAction(qAction);
        delete qAction;
    }

    // Add the action if it exists
    QString action = parameters.value(NotificationWidgetParameterFactory::actionKey()).toString();
    if (!action.isEmpty()) {
        DuiRemoteAction *remoteAction = new DuiRemoteAction(action, infoBanner);
        remoteAction->setVisible(false);
        infoBanner->addAction(remoteAction);
    }
}

void WidgetNotificationSink::infoBannerClicked()
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

        // Get the notification ID from the info banner
        bool ok = false;
        uint notificationId = infoBanner->property(NOTIFICATION_ID_PROPERTY).toUInt(&ok);
        if (ok) {
            // Request notification removal
            emit notificationRemovalRequested(notificationId);
        } else {
            uint groupId = infoBanner->property(GROUP_ID_PROPERTY).toUInt(&ok);
            if (ok) {
                // Request notification group clearing
                emit notificationGroupClearingRequested(groupId);
            }
        }
    }
}
