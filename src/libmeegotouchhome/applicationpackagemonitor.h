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

class MFileDataStore;

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

signals:
    /*!
     * Status of download progress of package being installed.
     *
     *\param desktopEntryName is name of desktop entry file.
     *\param bytesLoaded is current amount of bytes downloaded.
     *\param bytesTotal is size of downloading package in bytes.
     */
    void downloadProgress(const QString &packageExtraPath, int bytesLoaded, int bytesTotal);
    /*!
     * Status of install progress of package being installed.
     *
     *\param desktopEntryName is name of preliminary desktop entry file under installer-extra/.
     *\param percentage is install completion level.
     */
    void installProgress(const QString &packageExtraPath, int percentage);
    /*!
     * Notifies about success in installing a package.
     *
     *\param desktopEntryName is name of the installed desktop entry file.
     */
    void operationSuccess(const QString &packageExtraPath);
    /*!
     * Notifies about error in installing and downloading package.
     *
     *\param desktopEntryName is name of preliminary desktop entry file under installer-extra/.
     *\param error is string format description of error occured.
     */
    void operationError(const QString &packageExtraPath, const QString& error);

    /*!
     * Notifes about removal of install extra desktop entry.
     *
     * \param desktopEntryPath path of removed install extra file.
     */
    void installExtraEntryRemoved(const QString &desktopEntryPath);

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
     * Slot to handle PackageManagers operation_progress signal.
     */
    void packageOperationProgress(const QString& operation, const QString& packageame, const QString& packageVersion, int percentage);
    /*!
     * Slot to handle PackageManagers operation_complete signal.
     */
    void packageOperationComplete(const QString& operation, const QString& packageName, const QString& packageVersion, const QString& error, bool need_reboot);

    /*!
     * Update the cached package information from the given desktop file.
     */
    void updatePackageState(const QString &desktopEntryPath);

    /*!
     * Slot is called when entry from dataStore is removed.
     * Removes activePackages entry if package has one.
     */
    void packageRemoved(const QString &desktopEntryPath);

private:

    class ExtraDirWatcher;

    //! Stores package properties
    class PackageProperties {
    public:
        PackageProperties() :
            desktopEntryName(QString()),
            installing(false) {};

        QString desktopEntryName;
        bool installing;
    };

    /*!
     * Returns PackageProperties for package name and sets desktop entry path if one is found.
     *
     *\param name of package being installed
     *\return PackageProperties object.
     */
    PackageProperties & activePackageProperties(const QString packageName);

    /*!
     * Checks that package has desktop entry name property and operation is install or upgrade.
     *
     *\param properties of package being installed.
     *\param type of the current operation.
     */
    bool isValidOperation(const PackageProperties &properties, const QString &operation);

    /*!
     * Returns desktop entry path from APPLICATIONS_DIR/installer-extra/ or if it doesn't
     * exist empty QString.
     *
     *\param name of the package being installed
     *\return path to desktop file in installer-extra directory.
     */
    QString desktopEntryName(const QString &packageName);

    /*!
     * Stores package's current state to the dataStore.
     *
     *\param name of the package.
     *\param state of the package.
     */
    void storePackageState(const QString &packageName, const QString &state);


    //! Mapping of installing package names and installing phase properties
    QMap<QString, PackageProperties> activePackages;

    //! DBus connection to system bus
    QDBusConnection con;

    /*!
     * Cache for the state of packages and the mapping from package names to .desktop files.
     * Not owned.
     */
    MFileDataStore *dataStore;

    /*!
     * Keeps track of the .desktop files under installer-extra directory
     */
    QSharedPointer<ExtraDirWatcher> extraDirWatcher;

#ifdef UNIT_TEST
    friend class Ut_ApplicationPackageMonitor;
#endif

};

#endif
