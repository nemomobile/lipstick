
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

#include <components/launcheritem.h>
#include <components/launchermodel.h>
#include <components/switchermodel.h>
#include <components/switcherpixmapitem.h>
#include <components/statusbar.h>
#include <components/windowmanager.h>
#include <components/windowinfo.h>
#include <notifications/notificationpreviewpresenter.h>
#include <notifications/notificationlistmodel.h>
#include <notifications/notification.h>
#include <volume/volumecontrol.h>

LipstickPlugin::LipstickPlugin(QObject *parent) :
    QDeclarativeExtensionPlugin(parent)
{
}

void LipstickPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);

    qmlRegisterType<LauncherModel>("org.nemomobile.lipstick", 0, 1, "LauncherModel");
    qmlRegisterType<SwitcherModel>("org.nemomobile.lipstick", 0, 1, "SwitcherModel");
    qmlRegisterType<SwitcherPixmapItem>("org.nemomobile.lipstick", 0, 1, "SwitcherPixmapItem");
    qmlRegisterType<StatusBar>("org.nemomobile.lipstick", 0, 1, "StatusBar");
    qmlRegisterType<NotificationListModel>("org.nemomobile.lipstick", 0, 1, "NotificationListModel");
    qmlRegisterType<Notification>("org.nemomobile.lipstick", 0, 1, "Notification");
    qmlRegisterType<LauncherItem>("org.nemomobile.lipstick", 0, 1, "LauncherItem");
    qmlRegisterUncreatableType<NotificationPreviewPresenter>("org.nemomobile.lipstick", 0, 1, "NotificationPreviewPresenter", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<VolumeControl>("org.nemomobile.lipstick", 0, 1, "VolumeControl", "This type is initialized by HomeApplication");
    qmlRegisterUncreatableType<WindowInfo>("org.nemomobile.lipstick", 0, 1, "WindowInfo", "This type is initialized by SwitcherModel");
    qmlRegisterUncreatableType<WindowManager>("org.nemomobile.lipstick", 0, 1, "WindowManager", "This type should be accessed through a context property.");
}

Q_EXPORT_PLUGIN2(lipstick, LipstickPlugin)
