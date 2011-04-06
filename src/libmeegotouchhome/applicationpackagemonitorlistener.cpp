/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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
#include "launcherbuttonmodel.h"
#include "applicationpackagemonitor.h"
#include "applicationpackagemonitorlistener.h"

ApplicationPackageMonitorListener::ApplicationPackageMonitorListener()
    : packageMonitor(new ApplicationPackageMonitor)
{
    connect(packageMonitor.data(), SIGNAL(downloadProgress(QString, QString, int, int)),
            this, SLOT(setDownloadProgress(QString, QString, int, int)));
    connect(packageMonitor.data(), SIGNAL(installProgress(QString, QString, int)),
            this, SLOT(setInstallProgress(QString, QString, int)));
    connect(packageMonitor.data(), SIGNAL(operationSuccess(QString, QString)),
            this, SLOT(setOperationSuccess(QString, QString)));
    connect(packageMonitor.data(), SIGNAL(operationError(QString, QString, QString)),
            this, SLOT(setOperationError(QString, QString, QString)));
    connect(packageMonitor.data(), SIGNAL(packageUninstall(QString, QString)),
            this, SLOT(setPackageUninstall(QString, QString)));
    connect(packageMonitor.data(), SIGNAL(installExtraEntryRemoved(QString)),
            this, SIGNAL(installExtraEntryRemoved(QString)));
    connect(packageMonitor.data(), SIGNAL(updatePackageName(QString, QString)),
            this, SIGNAL(updatePackageName(QString, QString)));
}

ApplicationPackageMonitorListener::~ApplicationPackageMonitorListener()
{
}

void ApplicationPackageMonitorListener::setDownloadProgress(const QString &desktopEntryPath, const QString &packageName, int bytesLoaded, int bytesTotal)
{
    int percentage = -1;
    if (bytesTotal > 0 && bytesLoaded <= bytesTotal) {
        percentage = ((double)bytesLoaded / (double)bytesTotal) * 100;
    }
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Downloading, percentage);
}

void ApplicationPackageMonitorListener::setInstallProgress(const QString &desktopEntryPath, const QString &packageName, int percentage)
{
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Installing, percentage);
}

void ApplicationPackageMonitorListener::setOperationSuccess(const QString& desktopEntryPath, const QString &packageName)
{
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Installed, 0);
}

void ApplicationPackageMonitorListener::setOperationError(const QString& desktopEntryPath, const QString &packageName, const QString& error)
{
    Q_UNUSED(error)
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Broken, 0);
}

void ApplicationPackageMonitorListener::updatePackageStates()
{
    packageMonitor->updatePackageStates();
}

void ApplicationPackageMonitorListener::setPackageUninstall(const QString &desktopEntryPath, const QString &packageName)
{
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Uninstall, 0);
}

bool ApplicationPackageMonitorListener::isInstallerExtraEntry(const QString &desktopEntryPath)
{
    return desktopEntryPath.contains(ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER);
}
