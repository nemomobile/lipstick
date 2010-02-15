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

#include "duifeedbacknotificationsink.h"
#include <DuiApplication>
#include <DuiFeedbackPlayer>
#include "feedbackparameterfactory.h"
#include "homeapplication.h"
#include "eventtypestore.h"
#include <QSettings>
#include "genericnotificationparameterfactory.h"

DuiFeedbackNotificationSink::DuiFeedbackNotificationSink()
{
}

DuiFeedbackNotificationSink::~DuiFeedbackNotificationSink()
{
}

QString DuiFeedbackNotificationSink::determineFeedbackId(const NotificationParameters &parameters)
{
    QString feedbackId = parameters.value(FeedbackParameterFactory::feedbackIdKey()).toString();
    if (feedbackId.isEmpty()) {
        const EventTypeStore &store = notificationManager().eventTypeStore();
        const QSettings *settings = store.settingsForEventType(parameters.value(GenericNotificationParameterFactory::eventTypeKey()).toString());
        if (settings) {
            feedbackId = settings->value(FeedbackParameterFactory::feedbackIdKey()).toString();
        }
    }
    return feedbackId;
}

void DuiFeedbackNotificationSink::addNotification(const Notification &notification)
{
    if (!canAddNotification(notification)) return;

    QString feedbackId = determineFeedbackId(notification.parameters());
    if (!feedbackId.isEmpty() && DuiApplication::feedbackPlayer() != NULL) {
        DuiApplication::feedbackPlayer()->play(feedbackId);
    }
}

void DuiFeedbackNotificationSink::removeNotification(uint notificationId)
{
    Q_UNUSED(notificationId)
}

NotificationManager &DuiFeedbackNotificationSink::notificationManager()
{
    HomeApplication *application = dynamic_cast<HomeApplication *>(qApp);
    return application->notificationManager();
}
