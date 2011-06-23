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

#ifndef APPLICATIONPACKAGEMONITOR_H
#define APPLICATIONPACKAGEMONITOR_H

#include <QObject>
#include <QDBusConnection>
#include <QSharedPointer>

#include "extradirwatcher.h"

/*!
 * ApplicationPackageMonitor listens dbus signals from Package Manager and watches installer-extra
 * folder to receive info on package installation states and signaling relevant changes.
 *
 * Some events may be communicated through both dbus signal and changes in desktop entries.
 * This can lead to same package having multiple signals for same event (like progress error).
 * Signal listener is responsible for handling such cases.
 */
class ApplicationPackageMonitor : public QObject
{
    Q_OBJECT
public:
    /*!
     * Initializes ApplicationPackageMonitor object. Connects dbus signals to slots.
     */
    ApplicationPackageMonitor();
    /*!
     * Destroys ApplicationPackageMonitor.
     */
    virtual ~ApplicationPackageMonitor();

    /*!
     * Emit's state signals for packages in relevat states.
     *
     * (eg. emits operationError() for all the packages in 'broken' state)
     */
    void updatePackageStates();

    //! Installer extra folder path
    static const QString INSTALLER_EXTRA_FOLDER;

    // Package states
    static const QString PACKAGE_STATE_INSTALLED;
    static const QString PACKAGE_STATE_INSTALLABLE;
    static const QString PACKAGE_STATE_BROKEN;
    static const QString PACKAGE_STATE_UPDATEABLE;
    static const QString PACKAGE_STATE_INSTALLING;
    static const QString PACKAGE_STATE_DOWNLOADING;
    static const QString PACKAGE_STATE_UNINSTALLING;

    /*!
     * Returns package name from desktop entry. Returns empty QString if desktop entry is not found.
     *
     *\param desktopEntryPath is the path of the desktop entry
     *\return package name of desktop entry
     */
    QString packageName(const QString &dekstopEntryPath);

    //! Checks whether given desktop entry is located in installer extra folder
    static bool isInstallerExtraEntry(const QString &desktopEntryPath);

    //! Returns given entry file in installer extra path.
    static QString toInstallerExtraEntryPath(const QString &desktopEntryPath);

    //! Returns given entry file in applictions path.
    static QString toApplicationsEntryPath(const QString &desktopEntryPath);

signals:
    /*!
     * Status of download progress of package being installed.
     *
     * \param desktopEntryName is name of desktop entry file.
     * \param bytesLoaded is current amount of bytes downloaded.
     * \param bytesTotal is size of downloading package in bytes.
     */
    void downloadProgressUpdated(const QString &desktopEntryPath, int bytesLoaded, int bytesTotal);

    /*!
     * Notifes about removal of install extra desktop entry.
     *
     * \param desktopEntryPath path of removed install extra file.
     */
    void installExtraEntryRemoved(const QString &desktopEntryPath);

    /*!
     * Notifies that package state has changed
     *
     * \param desktopEntryPath path that specifies the package.
     * \param packageName name of the package
     * \param state Package state
     * \param packageRemovable Whether package is removable from launcher
     */
    void packageStateUpdated(const QSharedPointer<MDesktopEntry> &, const QString &packageName, const QString &state, bool packageRemovable);

private slots:
    /*!
     * Slot to handle PackageManagers download_progress signal.
     */
    void packageDownloadProgress(const QString& operation, const QString& packageName, const QString& packageVersion, int already, int total);
    /*!
     * Slot to handle PackageManagers operation_started signal.
     */
    void packageOperationStarted(const QString& operation, const QString& packageName, const QString& packageVersion);
    /*!
     * Slot to handle PackageManagers operation_complete signal.
     */
    void packageOperationComplete(const QString& operation, const QString& packageName, const QString& packageVersion, const QString& error, bool need_reboot);

    /*!
     * This is only needed to workaround bug #268255.
     */
    void packageOperationProgress(const QString &operation, const QString &packageName, const QString &packageVersion, int percentage);

    /*!
     * Update the cached package information from the given desktop file.
     */
    void updatePackageState(const QSharedPointer<MDesktopEntry> &entry);

    /*!
     * Slot is called when entry from dataStore is removed.
     * Removes activePackages entry if package has one.
     */
    void packageRemoved(const QString &desktopEntryPath);

private:

    /*!
     * Checks that package has desktop entry and operation is install or upgrade.
     *
     *\param desktopEntryPath Desktop entry path.
     *\param operation Type of the current operation.
     */
    bool isValidOperation(const QString &desktopEntryPath, const QString &operation);

    //! DBus connection to system bus
    QDBusConnection con;

    /*!
     * Cache for the state of packages and the mapping from package names to .desktop files.
     * Not owned.
     */
    ExtraDirWatcherData *dataStore;

    /*!
     * Keeps track of the .desktop files under installer-extra directory
     */
    QSharedPointer<ExtraDirWatcher> extraDirWatcher;

    /*!
     * Checks whether the package related to desktop entry is removable.
     *
     *\param entry Desktop entry to be checked for removability
     */
    bool isPackageRemovable(const MDesktopEntry *entry);

#ifdef UNIT_TEST
    friend class Ut_ApplicationPackageMonitor;
#endif

};

#endif
