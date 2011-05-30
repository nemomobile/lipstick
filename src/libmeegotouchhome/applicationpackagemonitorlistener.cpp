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
#include <QFileInfo>

ApplicationPackageMonitorListener::ApplicationPackageMonitorListener()
    : packageMonitor(new ApplicationPackageMonitor)
{
    connect(packageMonitor.data(), SIGNAL(downloadProgress(QString, QString, int, int, bool)),
            this, SLOT(setDownloadProgress(QString, QString, int, int, bool)));
    connect(packageMonitor.data(), SIGNAL(installProgress(QString, QString, int, bool)),
            this, SLOT(setInstallProgress(QString, QString, int, bool)));
    connect(packageMonitor.data(), SIGNAL(operationSuccess(QString, QString, bool)),
            this, SLOT(setOperationSuccess(QString, QString, bool)));
    connect(packageMonitor.data(), SIGNAL(operationError(QString, QString, QString, bool)),
            this, SLOT(setOperationError(QString, QString, QString, bool)));
    connect(packageMonitor.data(), SIGNAL(packageUninstall(QString, QString, bool)),
            this, SLOT(setPackageUninstall(QString, QString, bool)));
    connect(packageMonitor.data(), SIGNAL(installExtraEntryRemoved(QString)),
            this, SIGNAL(installExtraEntryRemoved(QString)));
    connect(packageMonitor.data(), SIGNAL(updatePackageName(QString, QString)),
            this, SIGNAL(updatePackageName(QString, QString)));
}

ApplicationPackageMonitorListener::~ApplicationPackageMonitorListener()
{
}

void ApplicationPackageMonitorListener::setDownloadProgress(const QString &desktopEntryPath, const QString &packageName, int bytesLoaded, int bytesTotal, bool packageRemovable)
{
    int percentage = -1;
    if (bytesTotal > 0 && bytesLoaded <= bytesTotal) {
        percentage = ((double)bytesLoaded / (double)bytesTotal) * 100;
    }
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Downloading, percentage, packageRemovable);
}

void ApplicationPackageMonitorListener::setInstallProgress(const QString &desktopEntryPath, const QString &packageName, int percentage, bool packageRemovable)
{
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Installing, percentage, packageRemovable);
}

void ApplicationPackageMonitorListener::setOperationSuccess(const QString& desktopEntryPath, const QString &packageName, bool packageRemovable)
{
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Installed, 0, packageRemovable);
}

void ApplicationPackageMonitorListener::setOperationError(const QString& desktopEntryPath, const QString &packageName, const QString& error, bool packageRemovable)
{
    Q_UNUSED(error)
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Broken, 0, packageRemovable);
}

void ApplicationPackageMonitorListener::updatePackageStates()
{
    packageMonitor->updatePackageStates();
}

void ApplicationPackageMonitorListener::setPackageUninstall(const QString &desktopEntryPath, const QString &packageName, bool packageRemovable)
{
    emit packageStateChanged(desktopEntryPath, packageName, LauncherButtonModel::Uninstall, 0, packageRemovable);
}

bool ApplicationPackageMonitorListener::isInstallerExtraEntry(const QString &desktopEntryPath)
{
    return desktopEntryPath.contains(ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER);
}

QString ApplicationPackageMonitorListener::toInstallerExtraEntryPath(const QString &desktopEntryPath)
{
    return APPLICATIONS_DIRECTORY + ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER + QFileInfo(desktopEntryPath).fileName();
}

QString ApplicationPackageMonitorListener::toApplicationsEntryPath(const QString &desktopEntryPath)
{
    return APPLICATIONS_DIRECTORY + QFileInfo(desktopEntryPath).fileName();
}
