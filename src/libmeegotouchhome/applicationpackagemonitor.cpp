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

static const QString PACKAGE_STATE_INSTALLED = "installed";
static const QString PACKAGE_STATE_INSTALLABLE = "installable";
static const QString PACKAGE_STATE_BROKEN = "broken";
static const QString PACKAGE_STATE_UPDATEABLE = "updateable";
static const QString PACKAGE_STATE_INSTALLING ="installing";
static const QString PACKAGE_STATE_DOWNLOADING ="downloading";

ApplicationPackageMonitor::ApplicationPackageMonitor()
: con(QDBusConnection::systemBus())
{
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "download_progress",
                    this, SLOT(packageDownloadProgress(const QString&, const QString&, const QString&, int, int)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_started",
                    this, SLOT(packageOperationStarted(const QString&, const QString&, const QString&)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_progress",
                    this, SLOT(packageOperationProgress(const QString&, const QString &, const QString&, int)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_complete",
                    this, SLOT(packageOperationComplete(const QString&, const QString&, const QString&, const QString&, bool)));

    QString configPath = QDir::homePath() + CONFIG_PATH;

    if (!QDir::root().exists(configPath)) {
        QDir::root().mkpath(configPath);
    }

    dataStore = new ExtraDirWatcherData();

    // ExtraDirWatcher takes ownership of dataStore
    extraDirWatcher = QSharedPointer<ExtraDirWatcher>(new ExtraDirWatcher(dataStore, QStringList() << (APPLICATIONS_DIRECTORY+INSTALLER_EXTRA_FOLDER)));

    connect(extraDirWatcher.data(), SIGNAL(desktopEntryAdded(QString)), this, SLOT(updatePackageState(QString)), Qt::UniqueConnection);
    connect(extraDirWatcher.data(), SIGNAL(desktopEntryChanged(QString)), this, SLOT(updatePackageState(QString)), Qt::UniqueConnection);
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
    foreach (const QString &key, dataStore->allKeys()) {
        updatePackageState(LauncherDataStore::keyToEntryPath(key));
    }
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
        emit downloadProgress(desktopEntryPath, packageName, already, total, isPackageRemovable(desktopEntryPath));
    }
}

void ApplicationPackageMonitor::packageOperationStarted(const QString &operation,
                                const QString &packageName,
                                const QString &version)
{
    Q_UNUSED(version)
    QString desktopEntryPath = LauncherDataStore::keyToEntryPath(dataStore->key(packageName));
    if (!desktopEntryPath.isEmpty() && operation.compare(OPERATION_UNINSTALL, Qt::CaseInsensitive) == 0) {
        emit packageUninstall(desktopEntryPath, packageName, true);
    }
}

void ApplicationPackageMonitor::packageOperationProgress(const QString &operation,
                                const QString &packageName,
                                const QString &packageVersion,
                                int percentage)
{
    Q_UNUSED(packageVersion)

    QString desktopEntryPath = LauncherDataStore::keyToEntryPath(dataStore->key(packageName));
    if (isValidOperation(desktopEntryPath, operation)) {
        emit installProgress(desktopEntryPath, packageName, percentage, isPackageRemovable(desktopEntryPath));
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

    if (!error.isEmpty()) {
        emit operationError(desktopEntryPath, packageName, QString(), isPackageRemovable(desktopEntryPath));
    } else {
        emit operationSuccess(desktopEntryPath.replace(INSTALLER_EXTRA_FOLDER, QString()), packageName, isPackageRemovable(desktopEntryPath));
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

bool ApplicationPackageMonitor::isPackageRemovable(const QString &desktopEntryPath)
{
    MDesktopEntry entry(desktopEntryPath);
    QString removable= entry.value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_REMOVABLE);
    return removable == "true" || removable.isEmpty();
}

void ApplicationPackageMonitor::updatePackageState(const QString &desktopEntryPath)
{
    MDesktopEntry entry(desktopEntryPath);

    QString packageName = entry.value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_NAME);
    QString packageState = entry.value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_STATE);
    bool packageHadError = entry.value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_HAD_ERROR) == "true";

    bool packageRemovable;
    QString removable= entry.value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_REMOVABLE);
    packageRemovable = removable == "true" || removable.isEmpty();

    if (packageHadError)
      packageState = PACKAGE_STATE_BROKEN;

    if (!packageName.isEmpty()) {
        if (packageState == PACKAGE_STATE_BROKEN) {
            emit operationError(desktopEntryPath, packageName, QString(), packageRemovable);
        } else if (packageState == PACKAGE_STATE_INSTALLED || packageState == PACKAGE_STATE_UPDATEABLE) {
            QString applicationsFolderPath(desktopEntryPath);
            applicationsFolderPath.replace(INSTALLER_EXTRA_FOLDER, QString());
            emit operationSuccess(applicationsFolderPath, packageName, packageRemovable);
        } else if(packageState == PACKAGE_STATE_DOWNLOADING) {
                emit downloadProgress(desktopEntryPath, packageName, 0, 0, packageRemovable);
        } else if(packageState == PACKAGE_STATE_INSTALLING) {
            emit installProgress(desktopEntryPath, packageName, 0, packageRemovable);
        }

        extraDirWatcher->updateDataForDesktopEntry(desktopEntryPath, packageName);
    }
}
