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

#include "notifier.h"
#include "homeapplication.h"
#include "notifiernotificationsink.h"
#include "homenotificationsink.h"

Notifier::Notifier(DuiWidget *parent) : DuiButton(parent),
    notifierSink(new NotifierNotificationSink)
{
    // Connect the notification signals
    HomeApplication *application = dynamic_cast<HomeApplication *>(qApp);
    if (application != NULL) {
        NotificationManager *notificationManager = &application->notificationManager();

        // Connect notification signals
        connect(&application->homeNotificationSink(), SIGNAL(transferNotification(const Notification &)), notifierSink, SLOT(addNotification(const Notification &)));
        connect(notificationManager, SIGNAL(notificationRemoved(uint)), notifierSink, SLOT(removeNotification(uint)));
        connect(notificationManager, SIGNAL(groupUpdated(uint, const NotificationParameters &)), notifierSink, SLOT(addGroup(uint, const NotificationParameters &)));
        connect(notificationManager, SIGNAL(groupRemoved(uint)), notifierSink, SLOT(removeGroup(uint)));
    }

    connect(notifierSink, SIGNAL(notificationCountChanged(uint)), this, SLOT(notificationCountChanged(uint)));
    hide();
}

Notifier::~Notifier()
{
    delete notifierSink;
}

void Notifier::notificationCountChanged(uint count)
{
    if (count > 0) {
        setText(QString().sprintf("%u", count));
        show();
    } else {
        hide();
        setText(QString().sprintf("%u", count));
    }
}
