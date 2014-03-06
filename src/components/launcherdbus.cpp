
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
#include <QDebug>


LauncherDBus::LauncherDBus(LauncherModel *model)
    : QObject(model)
    , m_model(model)
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/LauncherModel", this, QDBusConnection::ExportAllSlots);
}

LauncherDBus::~LauncherDBus()
{
}

void LauncherDBus::requestLaunch(QString packageName)
{
    emit showInstallProgress(packageName);
}

void LauncherDBus::installStarted(QString packageName, QString label, QString iconPath, QString desktopFile)
{
    qDebug("installStarted: %s, %s, %s, %s", qPrintable(packageName),
        qPrintable(label), qPrintable(iconPath), qPrintable(desktopFile));
    m_model->installStarted(packageName, label, iconPath, desktopFile);
}

void LauncherDBus::installProgress(QString packageName, int progress)
{
    qDebug("installProgress: %s, %d", qPrintable(packageName), progress);
    m_model->installProgress(packageName, progress);
}

void LauncherDBus::installFinished(QString packageName)
{
    qDebug("installFinished: %s", qPrintable(packageName));
    m_model->installFinished(packageName);
}
