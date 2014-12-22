
// This file is part of lipstick, a QML desktop library
//
// Copyright (c) 2014 Jolla Ltd.
// Contact: Thomas Perl <thomas.perl@jolla.com>
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

#include "launcherdbus.h"

#include "launcheritem.h"
#include "launchermodel.h"

#include <QDBusConnection>
#include <QDBusMessage>


LauncherDBus::LauncherDBus(LauncherModel *model)
    : QObject(model)
    , QDBusContext()
    , m_model(model)
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(LIPSTICK_DBUS_LAUNCHER_MODEL_PATH,
            this, QDBusConnection::ExportAllSlots |
            QDBusConnection::ExportAllSignals);
}

LauncherDBus::~LauncherDBus()
{
}

void LauncherDBus::requestLaunch(QString packageName)
{
    emit showUpdatingProgress(packageName);
}

void LauncherDBus::updatingStarted(QString packageName, QString label, QString iconPath, QString desktopFile)
{
    m_model->updatingStarted(packageName, label, iconPath, desktopFile, message().service());
}

void LauncherDBus::updatingProgress(QString packageName, int progress)
{
    m_model->updatingProgress(packageName, progress, message().service());
}

void LauncherDBus::updatingFinished(QString packageName)
{
    m_model->updatingFinished(packageName, message().service());
}

void LauncherDBus::notifyLaunching(const QString &desktopFile)
{
    m_model->notifyLaunching(desktopFile);
}
