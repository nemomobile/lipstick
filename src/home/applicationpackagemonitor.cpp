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
#include <QDBusConnection>

static const QString PACKAGE_MANAGER_DBUS_SERVICE="com.nokia.package_manager";
static const QString PACKAGE_MANAGER_DBUS_PATH="/com/nokia/package_manager";
static const QString PACKAGE_MANAGER_DBUS_INTERFACE="com.nokia.package_manager";

static const QString OPERATION_INSTALL = "Install";
static const QString OPERATION_UNINSTALL = "Uninstall";
static const QString OPERATION_REFRESH = "Refresh";
static const QString OPERATION_UPGRADE = "Upgrade";

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

}

ApplicationPackageMonitor::~ApplicationPackageMonitor()
{
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
        emit downloadProgress(packageName, properties.desktopEntryName, already, total);
    }
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
        emit installProgress(packageName, properties.desktopEntryName, percentage);
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
    Q_UNUSED(operation)

    PackageProperties &properties = activePackageProperties(packageName);

    if (isValidOperation(properties, operation)) {
        if (!error.isEmpty()) {
            emit operationError(packageName, properties.desktopEntryName, error);
        } else if (activePackages[packageName].installing) {
            emit operationSuccess(packageName, properties.desktopEntryName);
        }
    }

    activePackages.remove(packageName);
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

QString ApplicationPackageMonitor::desktopEntryName(const QString &packageName)
{
    //TODO: acually return desktop entry name
    return QString(packageName);
}
