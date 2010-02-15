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

#include <QtPlugin>
#include <QDir>
#include "fakesystemnotificationplugin.h"

FakeSystemNotificationPlugin::FakeSystemNotificationPlugin()
{
    connect(&watcher, SIGNAL(directoryChanged(QString)), this, SLOT(onDirectoryChanged(QString)));
    QDir dir;
    if (!dir.exists(WATCHED_PATH))
        dir.mkdir(WATCHED_PATH);
    watcher.addPath(WATCHED_PATH);
}

void FakeSystemNotificationPlugin::readPluginData()
{
    addNotification("batterylow", "Battery is running low", "Please charge", "10", "/usr/share/themes/base/dui/libdui/icons/icon-m-battery.svg", 1);
}

FakeSystemNotificationPlugin::~FakeSystemNotificationPlugin()
{
}

void FakeSystemNotificationPlugin::onDirectoryChanged(const QString &file)
{
    Q_UNUSED(file);
    readPluginData();
}

Q_EXPORT_PLUGIN2(systemnotificationplugin, FakeSystemNotificationPlugin);
