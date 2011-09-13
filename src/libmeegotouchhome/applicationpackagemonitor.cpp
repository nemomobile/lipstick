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
static const QString OPERATION_UPGRADE_ALL = "UpgradeAll";

const QString ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER = "installer-extra/";

static const QString CONFIG_PATH = "/.config/meegotouchhome";

/* The possible package state strings in the extra .desktop files as defined by apt-desktop spec */
const QString PACKAGE_STATE_INSTALLED = "installed";
const QString PACKAGE_STATE_INSTALLABLE = "installable";
const QString PACKAGE_STATE_BROKEN = "broken";
const QString PACKAGE_STATE_MISSING = "missing";

ApplicationPackageMonitor::ApplicationPackageMonitor()
: con(QDBusConnection::systemBus())
{
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "download_progress",
                    this, SLOT(packageDownloadProgress(const QString&, const QString&, const QString&, int, int)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_started",
                    this, SLOT(packageOperationStarted(const QString&, const QString&, const QString&)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_complete",
                    this, SLOT(packageOperationComplete(const QString&, const QString&, const QString&, const QString&, bool)));
    con.connect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_progress",
                    this, SLOT(packageOperationProgress(const QString&, const QString &, const QString&, int)));
    QString configPath = QDir::homePath() + CONFIG_PATH;

    if (!QDir::root().exists(configPath)) {
        QDir::root().mkpath(configPath);
    }

    // ExtraDirWatcher takes ownership of dataStore
    extraDirWatcher = QSharedPointer<ExtraDirWatcher>(new ExtraDirWatcher(new ExtraDirWatcherData(), QStringList() << (APPLICATIONS_DIRECTORY+INSTALLER_EXTRA_FOLDER)));

    connect(extraDirWatcher.data(), SIGNAL(desktopEntryAdded(QSharedPointer<MDesktopEntry>)), this, SLOT(updatePackageState(QSharedPointer<MDesktopEntry>)), Qt::UniqueConnection);
    connect(extraDirWatcher.data(), SIGNAL(desktopEntryChanged(QSharedPointer<MDesktopEntry>)), this, SLOT(updatePackageState(QSharedPointer<MDesktopEntry>)), Qt::UniqueConnection);
    connect(extraDirWatcher.data(), SIGNAL(desktopEntryRemoved(QString)), this, SLOT(packageRemoved(QString)), Qt::UniqueConnection);
}

ApplicationPackageMonitor::~ApplicationPackageMonitor()
{
}

void ApplicationPackageMonitor::packageRemoved(const QString &desktopEntryPath)
{
    QString packageName;

    for (QHash<QString, PackageInfo>::const_iterator i = knownPackages.constBegin(); packageName.isEmpty() && i != knownPackages.constEnd(); i++) {
        if (i.value().desktopEntryPath == desktopEntryPath) {
            packageName = i.key();
        }
    }

    knownPackages.remove(packageName);

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

    QString desktopEntryPath = knownPackages.value(packageName).desktopEntryPath;

    if (isValidOperation(desktopEntryPath, operation)) {

        // No need to emit the package state update signal if the state hasn't changed
        if (knownPackages[packageName].state != Downloading) {
            const QSharedPointer<MDesktopEntry> entry(new MDesktopEntry(desktopEntryPath));
            emit packageStateUpdated(entry, packageName, Downloading, isPackageRemovable(entry.data()));

            knownPackages[packageName].state = Downloading;
        }

        emit downloadProgressUpdated(desktopEntryPath, already, total);
    }
}

void ApplicationPackageMonitor::packageOperationProgress(const QString &operation,
                                const QString &packageName,
                                const QString &packageVersion,
                                int percentage)
{
    Q_UNUSED(packageVersion)
    Q_UNUSED(percentage)

    QString desktopEntryPath = knownPackages.value(packageName).desktopEntryPath;

    if (isValidOperation(desktopEntryPath, operation)) {

        // No need to emit the package state update signal if the state hasn't changed
        if (knownPackages.value(packageName).state != Installing) {
            const QSharedPointer<MDesktopEntry> entry(new MDesktopEntry(desktopEntryPath));
            emit packageStateUpdated(entry, packageName, Installing, isPackageRemovable(entry.data()));

            knownPackages[packageName].state = Installing;
        }
    }
}

void ApplicationPackageMonitor::packageOperationStarted(const QString &operation,
                                const QString &packageName,
                                const QString &version)
{
    Q_UNUSED(version)

    QString desktopEntryPath = knownPackages.value(packageName).desktopEntryPath;

    if (!desktopEntryPath.isEmpty() && operation.compare(OPERATION_UNINSTALL, Qt::CaseInsensitive) == 0) {
        const QSharedPointer<MDesktopEntry> entry(new MDesktopEntry(desktopEntryPath));
        emit packageStateUpdated(entry, packageName, Uninstalling, true);

        knownPackages[packageName].state = Uninstalling;
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

    QString desktopEntryPath = knownPackages.value(packageName).desktopEntryPath;

    if (!isValidOperation(desktopEntryPath, operation)) {
        return;
    }

    const QSharedPointer<MDesktopEntry> entry(new MDesktopEntry(desktopEntryPath));

    PackageState packageState;
    if (!error.isEmpty()) {
        packageState = Broken;
    } else {
        packageState = Installed;
    }
    emit packageStateUpdated(entry, packageName, packageState, isPackageRemovable(entry.data()));
    knownPackages[packageName].state = packageState;
}

bool ApplicationPackageMonitor::isValidOperation(const QString &desktopEntryPath, const QString &operation)
{
    bool operationValid = (operation.compare(OPERATION_INSTALL, Qt::CaseInsensitive) == 0 ||
                           operation.compare(OPERATION_UPGRADE, Qt::CaseInsensitive) == 0 ||
                           operation.compare(OPERATION_UPGRADE_ALL, Qt::CaseInsensitive) == 0);

    if (operationValid && !desktopEntryPath.isEmpty()) {
         return true;
    } else {
         return false;
    }
}

bool ApplicationPackageMonitor::isPackageRemovable(const MDesktopEntry *entry)
{
    QString removable = entry->value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_REMOVABLE);
    return removable == "true" || removable.isEmpty();
}

void ApplicationPackageMonitor::updatePackageState(const QSharedPointer<MDesktopEntry> &entry)
{
    QString packageName = entry->value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_NAME);
    QString packageStateString = entry->value(ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO, ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_STATE);

    if (!packageName.isEmpty()) {
        PackageState packageState = Installable;
        if (packageStateString == PACKAGE_STATE_INSTALLED) {
            packageState = Installed;
        } else if (packageStateString == PACKAGE_STATE_BROKEN) {
            // NB#264571: Not reacting to desktop entry changes when currently installing package changes to broken
            if (knownPackages[packageName].state == Installing) {
                packageState = Installing;
            } else {
                packageState = Broken;
            }
        }

        emit packageStateUpdated(entry, packageName, packageState, isPackageRemovable(entry.data()));

        knownPackages.insert(packageName, PackageInfo(entry->fileName(), packageState));
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
