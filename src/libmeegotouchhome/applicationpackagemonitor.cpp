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
#include <mfiledatastore.h>
#include <msubdatastore.h>

static const QString PACKAGE_MANAGER_DBUS_SERVICE="com.nokia.package_manager";
static const QString PACKAGE_MANAGER_DBUS_PATH="/com/nokia/package_manager";
static const QString PACKAGE_MANAGER_DBUS_INTERFACE="com.nokia.package_manager";

static const QString OPERATION_INSTALL = "Install";
static const QString OPERATION_UNINSTALL = "Uninstall";
static const QString OPERATION_REFRESH = "Refresh";
static const QString OPERATION_UPGRADE = "Upgrade";

static const QString DESKTOPENTRY_PREFIX = "DesktopEntries";
static const QString PACKAGE_PREFIX = "Packages/";
const QString ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER = "installer-extra/";

static const QString CONFIG_PATH = "/.config/meegotouchhome";

static const QString PACKAGE_STATE_INSTALLED = "installed";
static const QString PACKAGE_STATE_INSTALLABLE = "installable";
static const QString PACKAGE_STATE_BROKEN = "broken";
static const QString PACKAGE_STATE_UPDATEABLE = "updateable";
static const QString PACKAGE_STATE_INSTALLING ="installing";
static const QString PACKAGE_STATE_DOWNLOADING ="downloading";

static const QString DESKTOP_ENTRY_KEY_PACKAGE_STATE = "PackageState";
static const QString DESKTOP_ENTRY_KEY_PACKAGE_HAD_ERROR = "PackageHadError";
static const QString DESKTOP_ENTRY_KEY_PACKAGE_NAME = "Package";
static const QString DESKTOP_ENTRY_GROUP_MEEGO = "X-MeeGo";

class ApplicationPackageMonitor::ExtraDirWatcher : public LauncherDataStore
{
public:
    ExtraDirWatcher(MDataStore *dataStore, const QStringList &directories);
    ~ExtraDirWatcher();

protected:
    virtual bool isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes);
};

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

    QString dataStoreFileName = configPath + "/applicationpackage.data";

    dataStore = new MFileDataStore(dataStoreFileName);

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
    QString packageKey;

    foreach(const QString &pkgKey, dataStore->allKeys()) {
        if(pkgKey.contains(PACKAGE_PREFIX)) {
            if(dataStore->value(pkgKey).toString() == desktopEntryPath) {
                packageKey = pkgKey;
                break;
            }
        }
    }

    dataStore->remove(DESKTOPENTRY_PREFIX+desktopEntryPath);
    dataStore->remove(packageKey);
    activePackages.remove(packageKey.remove(PACKAGE_PREFIX));

    emit installExtraEntryRemoved(desktopEntryPath);
}

void ApplicationPackageMonitor::updatePackageStates()
{
    QStringList keyList(dataStore->allKeys());

    foreach (const QString &key, keyList) {
        if (key.contains(PACKAGE_PREFIX)) {
            QString desktopEntryPath = dataStore->value(key).toString();
            QString state = dataStore->value(DESKTOPENTRY_PREFIX + desktopEntryPath).toString();
            if (state == PACKAGE_STATE_BROKEN) {
                // emit operation error for a broken package
                emit operationError(desktopEntryPath, QString());
            } else if(state == PACKAGE_STATE_DOWNLOADING) {
                emit downloadProgress(desktopEntryPath, 0, 0);
            } else if(state == PACKAGE_STATE_INSTALLING) {
                emit installProgress(desktopEntryPath, 0);
            }
        }
    }
}

ApplicationPackageMonitor::PackageProperties &ApplicationPackageMonitor::activePackageProperties(const QString packageName)
{
    if (!activePackages.contains(packageName)) {
        // Set the desktopEntryName if already known
        activePackages[packageName].desktopEntryName = desktopEntryName(packageName);
    }

    return activePackages[packageName];
}

void ApplicationPackageMonitor::packageDownloadProgress(const QString &operation,
                                    const QString &packageName,
                                    const QString &packageVersion,
                                    int already, int total)
{
    Q_UNUSED(operation)
    Q_UNUSED(packageVersion)

    PackageProperties &properties = activePackageProperties(packageName);

    if (isValidOperation(properties, operation)) {
        emit downloadProgress(properties.desktopEntryName, already, total);
    }

    storePackageState(packageName, PACKAGE_STATE_DOWNLOADING);
}

void ApplicationPackageMonitor::packageOperationStarted(const QString &operation,
                                const QString &packageName,
                                const QString &version)
{
    Q_UNUSED(operation)
    Q_UNUSED(packageName)
    Q_UNUSED(version)
}

void ApplicationPackageMonitor::packageOperationProgress(const QString &operation,
                                const QString &packageName,
                                const QString &packageVersion,
                                int percentage)
{
    Q_UNUSED(packageVersion)

    PackageProperties &properties = activePackageProperties(packageName);

    if (isValidOperation(properties, operation)) {
        properties.installing = true;
        emit installProgress(properties.desktopEntryName, percentage);
        storePackageState(packageName, PACKAGE_STATE_INSTALLING);
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

    PackageProperties &properties = activePackageProperties(packageName);

    if (!isValidOperation(properties, operation)) {
        return;
    }

    if (!error.isEmpty()) {
        if (properties.installing) {
            emit operationError(properties.desktopEntryName, error);
            storePackageState(packageName, PACKAGE_STATE_BROKEN);
        }
        else {
            // Downloading
            MDesktopEntry entry(properties.desktopEntryName);
            QString packageState = entry.value(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_STATE);
            if (packageState == PACKAGE_STATE_INSTALLED || packageState == PACKAGE_STATE_UPDATEABLE) {
                // Application is previously installed. Downloading update failed but application still usable.
                emit operationSuccess(properties.desktopEntryName.replace(INSTALLER_EXTRA_FOLDER, QString()));
                storePackageState(packageName, packageState);
            } else {
                emit operationError(properties.desktopEntryName, error);
                storePackageState(packageName, PACKAGE_STATE_BROKEN);
            }
        }
    } else {
        emit operationSuccess(properties.desktopEntryName.replace(INSTALLER_EXTRA_FOLDER, QString()));
        storePackageState(packageName, PACKAGE_STATE_INSTALLED);
    }

    activePackages.remove(packageName);
}

void ApplicationPackageMonitor::storePackageState(const QString& packageName, const QString& state)
{
    QString path = dataStore->value(PACKAGE_PREFIX + packageName).toString();
    extraDirWatcher->updateDataForDesktopEntry(path, state);
}

bool ApplicationPackageMonitor::isValidOperation(const PackageProperties &properties, const QString &operation)
{
    if ((operation.compare(OPERATION_INSTALL, Qt::CaseInsensitive) == 0 ||
           operation.compare(OPERATION_UPGRADE, Qt::CaseInsensitive) == 0 ) &&
        !properties.desktopEntryName.isEmpty() ) {
         return true;
    } else {
         return false;
    }
}

void ApplicationPackageMonitor::updatePackageState(const QString &desktopEntryPath)
{
    MDesktopEntry entry(desktopEntryPath);

    QString packageName = entry.value(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_NAME);
    QString packageState = entry.value(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_STATE);
    bool packageHadError = entry.value(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_HAD_ERROR) == "true";

    if (packageHadError)
      packageState = PACKAGE_STATE_BROKEN;

    if (!packageName.isEmpty()) {
        QString pkgKey = PACKAGE_PREFIX+packageName;

        if (packageState == PACKAGE_STATE_BROKEN) {
            // emit operation error for a broken package
            emit operationError(desktopEntryPath, QString());
        }

        dataStore->createValue(pkgKey, desktopEntryPath);

        if (activePackages.contains(packageName)) {
            // Package is being installed, add the desktop entry path directly
            activePackages[packageName].desktopEntryName = desktopEntryPath;
        }
    }

    extraDirWatcher->updateDataForDesktopEntry(desktopEntryPath, packageState);
}

QString ApplicationPackageMonitor::desktopEntryName(const QString &packageName)
{
    QString pkgKey = PACKAGE_PREFIX+packageName;

    if (!dataStore->contains(pkgKey)) {
        // Package not installed
        return QString();
    }

    if (!QFile::exists(dataStore->value(pkgKey).toString())) {
        // The extra desktop file doesn't exist anymore, the package has been uninstalled
        dataStore->remove(pkgKey);
        return QString();
    }

    return dataStore->value(pkgKey).toString();
}

ApplicationPackageMonitor::ExtraDirWatcher::ExtraDirWatcher(MDataStore *dataStore, const QStringList &directories) :
    LauncherDataStore(dataStore, directories)
{
}

ApplicationPackageMonitor::ExtraDirWatcher::~ExtraDirWatcher()
{
}

bool ApplicationPackageMonitor::ExtraDirWatcher::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes)
{
    Q_UNUSED(acceptedTypes);

    return entry.contains(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_NAME)
        && entry.contains(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_STATE);
}
