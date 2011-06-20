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

#include "applicationpackagemonitor.h"
#include "launcherdatastore.h"
#include <QDir>
#include <QDBusConnection>
#include <mdesktopentry.h>

static const QString PACKAGE_MANAGER_DBUS_SERVICE="com.nokia.package_manager";
static const QString PACKAGE_MANAGER_DBUS_PATH="/com/nokia/package_manager";
static const QString PACKAGE_MANAGER_DBUS_INTERFACE="com.nokia.package_manager";

static const QString OPERATION_INSTALL = "Install";
static const QString OPERATION_UNINSTALL = "Uninstall";
static const QString OPERATION_REFRESH = "Refresh";
static const QString OPERATION_UPGRADE = "Upgrade";

const QString ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER = "installer-extra/";

static const QString CONFIG_PATH = "/.config/meegotouchhome";

const QString ApplicationPackageMonitor::PACKAGE_STATE_INSTALLED = "installed";
const QString ApplicationPackageMonitor::PACKAGE_STATE_INSTALLABLE = "installable";
const QString ApplicationPackageMonitor::PACKAGE_STATE_BROKEN = "broken";
const QString ApplicationPackageMonitor::PACKAGE_STATE_UPDATEABLE = "updateable";
const QString ApplicationPackageMonitor::PACKAGE_STATE_INSTALLING ="installing";
const QString ApplicationPackageMonitor::PACKAGE_STATE_DOWNLOADING ="downloading";
const QString ApplicationPackageMonitor::PACKAGE_STATE_UNINSTALLING ="uninstalling";

ApplicationPackageMonitor::ApplicationPackageMonitor()
: con(QDBusConnection::systemBus())
{
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "download_progress",
                    this, SLOT(packageDownloadProgress(const QString&, const QString&, const QString&, int, int)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_started",
                    this, SLOT(packageOperationStarted(const QString&, const QString&, const QString&)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_complete",
                    this, SLOT(packageOperationComplete(const QString&, const QString&, const QString&, const QString&, bool)));

    QString configPath = QDir::homePath() + CONFIG_PATH;

    if (!QDir::root().exists(configPath)) {
        QDir::root().mkpath(configPath);
    }

    dataStore = new ExtraDirWatcherData();

    // ExtraDirWatcher takes ownership of dataStore
    extraDirWatcher = QSharedPointer<ExtraDirWatcher>(new ExtraDirWatcher(dataStore, QStringList() << (APPLICATIONS_DIRECTORY+INSTALLER_EXTRA_FOLDER)));

    connect(extraDirWatcher.data(), SIGNAL(desktopEntryAdded(QSharedPointer<MDesktopEntry>)), this, SLOT(updatePackageState(QSharedPointer<MDesktopEntry>)), Qt::UniqueConnection);
    connect(extraDirWatcher.data(), SIGNAL(desktopEntryChanged(QSharedPointer<MDesktopEntry>)), this, SLOT(updatePackageState(QSharedPointer<MDesktopEntry>)), Qt::UniqueConnection);
    connect(extraDirWatcher.data(), SIGNAL(desktopEntryRemoved(QString)), this, SLOT(packageRemoved(QString)), Qt::UniqueConnection);
}

ApplicationPackageMonitor::~ApplicationPackageMonitor()
{
}

void ApplicationPackageMonitor::packageRemoved(const QString &desktopEntryPath)
{
    dataStore->remove(LauncherDataStore::entryPathToKey(desktopEntryPath));
    emit installExtraEntryRemoved(desktopEntryPath);
}

void ApplicationPackageMonitor::updatePackageStates()
{
    extraDirWatcher->updateDesktopEntryFiles();
}

void ApplicationPackageMonitor::packageDownloadProgress(const QString &operation,
                                    const QString &packageName,
                                    const QString &packageVersion,
                                    int already, int total)
{
    Q_UNUSED(operation)
    Q_UNUSED(packageVersion)

    QString desktopEntryPath = LauncherDataStore::keyToEntryPath(dataStore->key(packageName));
    if (isValidOperation(desktopEntryPath, operation)) {
        emit downloadProgressUpdated(desktopEntryPath, already, total);
    }
}

void ApplicationPackageMonitor::packageOperationStarted(const QString &operation,
                                const QString &packageName,
                                const QString &version)
{
    Q_UNUSED(version)
    QString desktopEntryPath = LauncherDataStore::keyToEntryPath(dataStore->key(packageName));
    if (!desktopEntryPath.isEmpty() && operation.compare(OPERATION_UNINSTALL, Qt::CaseInsensitive) == 0) {
        const QSharedPointer<MDesktopEntry> entry(new MDesktopEntry(desktopEntryPath));
        emit packageStateUpdated(entry, packageName, PACKAGE_STATE_UNINSTALLING, true);
    }
}

void ApplicationPackageMonitor::packageOperationComplete(const QString &operation,
                                const QString &packageName,
                                const QString &packageVersion,
                                const QString &error,
                                bool need_reboot)
{
    Q_UNUSED(packageVersion)
    Q_UNUSED(need_reboot)

    QString desktopEntryPath = LauncherDataStore::keyToEntryPath(dataStore->key(packageName));

    if (!isValidOperation(desktopEntryPath, operation)) {
        return;
    }

    const QSharedPointer<MDesktopEntry> entry(new MDesktopEntry(desktopEntryPath));
    if (!error.isEmpty()) {
        updatePackageState(entry);
    } else {
        emit packageStateUpdated(entry, packageName, PACKAGE_STATE_INSTALLED, isPackageRemovable(entry.data()));
    }
}

bool ApplicationPackageMonitor::isValidOperation(const QString &desktopEntryPath, const QString &operation)
{
    bool operationValid = (operation.compare(OPERATION_INSTALL, Qt::CaseInsensitive) == 0 ||
           operation.compare(OPERATION_UPGRADE, Qt::CaseInsensitive) == 0);

    if (operationValid && !desktopEntryPath.isEmpty()) {
         return true;
    } else {
         return false;
    }
}

bool ApplicationPackageMonitor::isPackageRemovable(const MDesktopEntry *entry)
{
    QString removable= entry->value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_REMOVABLE);
    return removable == "true" || removable.isEmpty();
}

void ApplicationPackageMonitor::updatePackageState(const QSharedPointer<MDesktopEntry> &entry)
{
    QString packageName = entry->value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_NAME);
    QString packageState = entry->value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_STATE);

    if (!packageName.isEmpty()) {
        emit packageStateUpdated(entry, packageName, packageState, isPackageRemovable(entry.data()));

        extraDirWatcher->updateDataForDesktopEntry(entry->fileName(), packageName);
    }
}

bool ApplicationPackageMonitor::isInstallerExtraEntry(const QString &desktopEntryPath)
{
    return desktopEntryPath.contains(INSTALLER_EXTRA_FOLDER);
}

QString ApplicationPackageMonitor::toInstallerExtraEntryPath(const QString &desktopEntryPath)
{
    return APPLICATIONS_DIRECTORY + INSTALLER_EXTRA_FOLDER + QFileInfo(desktopEntryPath).fileName();
}

QString ApplicationPackageMonitor::toApplicationsEntryPath(const QString &desktopEntryPath)
{
    return APPLICATIONS_DIRECTORY + QFileInfo(desktopEntryPath).fileName();
}
