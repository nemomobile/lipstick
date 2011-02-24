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
    connect(packageMonitor.data(), SIGNAL(downloadProgress(QString, int, int)),
            this, SLOT(setDownloadProgress(QString, int, int)));
    connect(packageMonitor.data(), SIGNAL(installProgress(QString, int)),
            this, SLOT(setInstallProgress(QString, int)));
    connect(packageMonitor.data(), SIGNAL(operationSuccess(QString)),
            this, SLOT(setOperationSuccess(QString)));
    connect(packageMonitor.data(), SIGNAL(operationError(QString, QString)),
            this, SLOT(setOperationError(QString, QString)));
    connect(packageMonitor.data(), SIGNAL(installExtraEntryRemoved(QString)),
            this, SIGNAL(installExtraEntryRemoved(QString)));
}

ApplicationPackageMonitorListener::~ApplicationPackageMonitorListener()
{
}

void ApplicationPackageMonitorListener::setDownloadProgress(const QString &desktopEntryPath, int bytesLoaded, int bytesTotal)
{
    int percentage = -1;
    if (bytesTotal > 0 && bytesLoaded <= bytesTotal) {
        percentage = ((double)bytesLoaded / (double)bytesTotal) * 100;
    }
    emit packageStateChanged(desktopEntryPath, LauncherButtonModel::Downloading, percentage);
}

void ApplicationPackageMonitorListener::setInstallProgress(const QString &desktopEntryPath, int percentage)
{
    emit packageStateChanged(desktopEntryPath, LauncherButtonModel::Installing, percentage);
}

void ApplicationPackageMonitorListener::setOperationSuccess(const QString& desktopEntryPath)
{
    emit packageStateChanged(desktopEntryPath, LauncherButtonModel::Installed, 0);
}

void ApplicationPackageMonitorListener::setOperationError(const QString& desktopEntryPath, const QString& error)
{
    Q_UNUSED(error)
    emit packageStateChanged(desktopEntryPath, LauncherButtonModel::Broken, 0);
}

void ApplicationPackageMonitorListener::updatePackageStates()
{
    packageMonitor->updatePackageStates();
}

bool ApplicationPackageMonitorListener::isInstallerExtraEntry(const QString &desktopEntryPath)
{
    return desktopEntryPath.contains(ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER);
}
