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
//#include <QtDBus>
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

void ApplicationPackageMonitor::packageDownloadProgress(const QString &operation,
                                    const QString &packagename,
                                    const QString &packageversion,
                                    int already, int total)
{
    Q_UNUSED(operation)
    Q_UNUSED(packageversion)

    if(operation.compare(OPERATION_INSTALL, Qt::CaseInsensitive) == 0 ||
           operation.compare(OPERATION_UPGRADE, Qt::CaseInsensitive) == 0)
    {
        emit downloadProgress(packagename, already, total);
    }
}

void ApplicationPackageMonitor::packageOperationStarted(const QString &operation,
                                const QString &packagename,
                                const QString &version)
{
    Q_UNUSED(operation)
    Q_UNUSED(packagename)
    Q_UNUSED(version)
}

void ApplicationPackageMonitor::packageOperationProgress(const QString &operation,
                                const QString &packagename,
                                const QString &packageversion,
                                int percentage)
{
    Q_UNUSED(packageversion)

    if(operation.compare(OPERATION_INSTALL, Qt::CaseInsensitive) == 0 ||
           operation.compare(OPERATION_UPGRADE, Qt::CaseInsensitive) == 0)
    {
        activePackages[packagename].downloadCompleted = true;
        emit installProgress(packagename, percentage);
    }
}

void ApplicationPackageMonitor::packageOperationComplete(const QString &operation,
                                const QString &packagename,
                                const QString &packageversion,
                                const QString &error,
                                bool need_reboot)
{
    Q_UNUSED(packageversion)
    Q_UNUSED(need_reboot)
    Q_UNUSED(operation)

    if(operation.compare(OPERATION_INSTALL, Qt::CaseInsensitive) == 0 ||
           operation.compare(OPERATION_UPGRADE, Qt::CaseInsensitive) == 0)
    {
        if (!error.isEmpty()) {
            activePackages.remove(packagename);
            emit operationError(packagename, error);
        } else if (activePackages[packagename].downloadCompleted) {
            activePackages[packagename].installCompleted = true;
            emitSuccessIfPackageFinished(packagename);
        }
    }
}

void ApplicationPackageMonitor::emitSuccessIfPackageFinished(const QString &packagename)
{
    const PackageProperties &properties = activePackages.value(packagename);
    if (properties.installCompleted && properties.downloadCompleted) {
        activePackages.remove(packagename);
        emit operationSuccess(packagename, properties.desktopEntryName);
    }
}
