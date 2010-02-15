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

#include <QPluginLoader>
#include "systemnotificationsource.h"
#include "duihomesystemnotificationplugininterface.h"

SystemNotificationSource::SystemNotificationSource(NotificationManagerInterface &interface) : NotificationSource(interface)
{
    loadSystemNotificationPlugins();
}

SystemNotificationSource::~SystemNotificationSource()
{
    foreach(QObject * obj, pluginList) {
        delete obj;
    }
}
uint SystemNotificationSource::addNotification(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.addNotification(notificationUserId, notificationParameters(eventType, summary, body, action, imageURI, count), 0, false, NotificationManagerInterface::SystemEvent);
}

uint SystemNotificationSource::notificationUserId()
{
    return manager.notificationUserId();
}

void SystemNotificationSource::loadSystemNotificationPlugins()
{
    addPlugin("/usr/lib/dui/notifications/systemnotificationplugins/libpowermanagement.so");
    addPlugin("/usr/lib/dui/notifications/systemnotificationplugins/libconnectivity.so");

#ifdef TESTABILITY_ON
    addPlugin("/usr/lib/dui/notifications/systemnotificationplugins/libfakesystemnotificationplugin.so");
#endif
}

void SystemNotificationSource::addPlugin(const QString &path)
{
    QPluginLoader loader(path);
    QObject *object = loader.instance();
    if (object == NULL) {
        QString error = loader.errorString().toAscii().data();
        Q_UNUSED(error);
    }
    DuiHomeSystemNotificationPluginInterface *plugin = qobject_cast<DuiHomeSystemNotificationPluginInterface *>(object);

    if (plugin != NULL) {
        plugin->initialize(*this);
    }
    pluginList.append(object);
}
