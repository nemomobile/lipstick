
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include "lipstickplugin.h"

#include <QtDeclarative>
#include <components/launcheritem.h>
#include <components/launchermodel.h>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <components/switchermodel.h>
#include <components/switcherpixmapitem.h>
#include <components/windowmanager.h>
#include <components/windowinfo.h>
#endif
#include <notifications/notificationpreviewpresenter.h>
#include <notifications/notificationlistmodel.h>
#include <notifications/lipsticknotification.h>
#include <volume/volumecontrol.h>
#include <usbmodeselector.h>
#include <shutdownscreen.h>

LipstickPlugin::LipstickPlugin(QObject *parent) :
    QDeclarativeExtensionPlugin(parent)
{
}

void LipstickPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);

    qmlRegisterType<LauncherModel>("org.nemomobile.lipstick", 0, 1, "LauncherModel");
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qmlRegisterType<SwitcherModel>("org.nemomobile.lipstick", 0, 1, "SwitcherModel");
    qmlRegisterType<SwitcherPixmapItem>("org.nemomobile.lipstick", 0, 1, "SwitcherPixmapItem");
#endif
    qmlRegisterType<NotificationListModel>("org.nemomobile.lipstick", 0, 1, "NotificationListModel");
    qmlRegisterType<LipstickNotification>("org.nemomobile.lipstick", 0, 1, "Notification");
    qmlRegisterType<LauncherItem>("org.nemomobile.lipstick", 0, 1, "LauncherItem");
    qmlRegisterUncreatableType<NotificationPreviewPresenter>("org.nemomobile.lipstick", 0, 1, "NotificationPreviewPresenter", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<VolumeControl>("org.nemomobile.lipstick", 0, 1, "VolumeControl", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<USBModeSelector>("org.nemomobile.lipstick", 0, 1, "USBModeSelector", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<ShutdownScreen>("org.nemomobile.lipstick", 0, 1, "ShutdownScreen", "This type is initialized by HomeApplication");
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qmlRegisterUncreatableType<WindowInfo>("org.nemomobile.lipstick", 0, 1, "WindowInfo", "This type is initialized by SwitcherModel");
    qmlRegisterUncreatableType<WindowManager>("org.nemomobile.lipstick", 0, 1, "WindowManager", "This type should be accessed through a context property.");
#endif
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(lipstick, LipstickPlugin)
#endif
