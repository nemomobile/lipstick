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

#ifndef APPLICATIONPACKAGEMONITORLISTENER_H
#define APPLICATIONPACKAGEMONITORLISTENER_H

#include <QObject>
#include "launcherbuttonmodel.h"

class ApplicationPackageMonitor;

/*!
 * ApplicationPackageMonitorListener listens signals from Application Package Manager to be used
 * for updating Launcher Button and Quick Launcg Bar Button statuses.
 */
class ApplicationPackageMonitorListener : public QObject
{
    Q_OBJECT

public:
    /*!
     * Initializes ApplicationPackageMonitorListener object.
     */
    ApplicationPackageMonitorListener();
    /*!
     * Destroys ApplicationPackageMonitorListener.
     */
    virtual ~ApplicationPackageMonitorListener();

    /*!
     * Updates package states
     */
    void updatePackageStates();

    //! Checks whether given desktop entry is located in installer extra folder
    static bool isInstallerExtraEntry(const QString &desktopEntryPath);

signals:

    /*!
     * Signal sent when we have received and handled a signal from ApplicationPackageMonitor
     */
    void packageStateChanged(const QString &desktopEntryPath, const QString &packageName, LauncherButtonModel::State state, int progress);

    /*!
     * Signal sent when we have received a installExtraEntryRemoved signal from ApplicationPackageMonitor
     */
    void installExtraEntryRemoved(const QString &desktopEntryPath);

    /*!
     * Notifies about a need to update package name.
     *
     * \param desktopEntryPath path that specifies the package.
     * \param packageName name of the package
     */
    void updatePackageName(const QString &desktopEntryPath, const QString &packageName);

public slots:

    /*!
     * Set button state to "downloading", and calculate and set it's progress
     *
     * \param desktopEntryName Desktop entry of the application button represents
     * \param packageName name of the package
     * \param bytesLoaded Amount of bytes loaded
     * \param bytesTotal Total amount of bytes to download
     */
    void setDownloadProgress(const QString& desktopEntryPath, const QString &packageName, int bytesLoaded, int bytesTotal);

    /*!
     * Set button state to "installing", and set it's progress
     *
     * \param desktopEntryName Desktop entry of the application button represents
     * \param packageName name of the package
     * \param percentage Percentage of installation completed
     */
    void setInstallProgress(const QString& desktopEntryPath, const QString &packageName, int percentage);

    /*!
     * Set button state to "installed"
     *
     * \param desktopEntryName Desktop entry of the application button represents
     * \param packageName name of the package
     */
    void setOperationSuccess(const QString& desktopEntryPath, const QString &packageName);

    /*!
     * Set button state to "broken"
     *
     * \param desktopEntryName Desktop entry of the application button represents
     * \param packageName name of the package
     * \param error Error message
     */
    void setOperationError(const QString& desktopEntryPath, const QString &packageName, const QString& error);


private:
    //! Application package monitor
    QSharedPointer<ApplicationPackageMonitor> packageMonitor;

#ifdef UNIT_TEST
    friend class Ut_ApplicationPackageMonitorListener;
#endif

};

#endif
