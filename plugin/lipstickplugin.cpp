
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

#include <QtQml>
#include <components/launcheritem.h>
#include <components/launchermodel.h>
#include <components/launcherwatchermodel.h>
#include <components/launcherfoldermodel.h>
#include <notifications/notificationpreviewpresenter.h>
#include <notifications/notificationfeedbackplayer.h>
#include <notifications/notificationlistmodel.h>
#include <notifications/lipsticknotification.h>
#include <volume/volumecontrol.h>
#include <devicelock/devicelock.h>
#include <usbmodeselector.h>
#include <shutdownscreen.h>
#include <compositor/lipstickcompositor.h>
#include <compositor/windowmodel.h>
#include <compositor/windowpixmapitem.h>
#include <compositor/windowproperty.h>
#include <lipstickapi.h>
#include <hwcimage.h>

static QObject *lipstickApi_callback(QQmlEngine *e, QJSEngine *)
{
    return new LipstickApi(e);
}

LipstickPlugin::LipstickPlugin(QObject *parent) :
    QQmlExtensionPlugin(parent)
{
}

void LipstickPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);

    qmlRegisterType<LauncherModel>("org.nemomobile.lipstick", 0, 1, "LauncherModel");
    qmlRegisterType<LauncherWatcherModel>("org.nemomobile.lipstick", 0, 1, "LauncherWatcherModel");
    qmlRegisterType<NotificationListModel>("org.nemomobile.lipstick", 0, 1, "NotificationListModel");
    qmlRegisterType<LipstickNotification>("org.nemomobile.lipstick", 0, 1, "Notification");
    qmlRegisterType<LauncherItem>("org.nemomobile.lipstick", 0, 1, "LauncherItem");
    qmlRegisterType<LauncherFolderModel>("org.nemomobile.lipstick", 0, 1, "LauncherFolderModel");
    qmlRegisterType<LauncherFolderItem>("org.nemomobile.lipstick", 0, 1, "LauncherFolderItem");
    qmlRegisterType<HwcImage>("org.nemomobile.lipstick", 0, 1, "HwcImage");

    qmlRegisterUncreatableType<NotificationPreviewPresenter>("org.nemomobile.lipstick", 0, 1, "NotificationPreviewPresenter", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<NotificationFeedbackPlayer>("org.nemomobile.lipstick", 0, 1, "NotificationFeedbackPlayer", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<VolumeControl>("org.nemomobile.lipstick", 0, 1, "VolumeControl", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<USBModeSelector>("org.nemomobile.lipstick", 0, 1, "USBModeSelector", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<ShutdownScreen>("org.nemomobile.lipstick", 0, 1, "ShutdownScreen", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<DeviceLock>("org.nemomobile.lipstick", 0, 1, "DeviceLock", "This type is initialized by HomeApplication");

    qmlRegisterType<LipstickCompositor>("org.nemomobile.lipstick", 0, 1, "Compositor");
    qmlRegisterUncreatableType<QWaylandSurface>("org.nemomobile.lipstick", 0, 1, "WaylandSurface", "This type is created by the compositor");
    qmlRegisterType<WindowModel>("org.nemomobile.lipstick", 0, 1, "WindowModel");
    qmlRegisterType<WindowPixmapItem>("org.nemomobile.lipstick", 0, 1, "WindowPixmapItem");
    qmlRegisterType<WindowProperty>("org.nemomobile.lipstick", 0, 1, "WindowProperty");
    qmlRegisterSingletonType<LipstickApi>("org.nemomobile.lipstick", 0, 1, "Lipstick", lipstickApi_callback);

    qmlRegisterRevision<QQuickWindow,1>("org.nemomobile.lipstick", 0, 1);
}
