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

#ifndef UT_APPLICATIONPACKAGEMONITOR_H
#define UT_APPLICATIONPACKAGEMONITOR_H

#include <QObject>

class MApplication;
class ApplicationPackageMonitor;

class Ut_ApplicationPackageMonitor : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases

    //Tests created dbus connections.
    void testConstruction();
    //Tests that calling updatePackageStates() sends correct signals
    void testUpdatingPackageStates();
    //Tests successfull install with multiple packages.
    void testSuccessfullInstall();
    //Tests unsuccessfull install with multiple packages
    void testUnsuccessfullInstall();
    //Tests uninstall.
    void testUninstallSignal();
    //Tests installing package that first fails and then succeeds.
    void testUnsuccesfullAndSuccesfullInstall();
    //Tests installing multiple packages successfully with operation
    //complete signal received after download progress.
    void testSuccessfullInstallWithOperationCompletedAfterDownload();
    // Test that monitor sents correct signals when new broken desktop entry appears
    void testErrorSignalsForNewBrokenDesktopEntry();
    // Test that monitor sents correct signals when existing desktop entry changes state to broken
    void testErrorSignalsForDesktopEntryChangingToBrokenState();
    // Tests that correct signals are emitted for already installed application when it's upgraded.
    void testUpgradingPackage();
    // Test scenarios when downloading is cancelled and launcher button set to successfull state.
    void testCancelDownloadingSuccessfully();
    // Test scenarios when downloading is cancelled and launcher button set to broken state.
    void testCancelDownloadingUnsuccessfully();
    // Test that right signal is emitted when installer extra file is removed.
    void testRemovingInstallerExtraFile();
    // Test that right signals are emitted according to package states when update of package states is requested.
    void testEmitPackageStateWhenAllPackageStatesAreUpdated();
    // Test PackageHadError in entry
    void testPackageHadError();

signals:

    // Signals normally emitted by LauncherDataStore

    void desktopEntryChanged(const QString &);

    void desktopEntryAdded(const QString &);

    void desktopEntryRemoved(const QString &);

private:

    //Simulates slots called during unsuccessfull package install
    void installUnsuccessfully(const QString&);

    //Simulates slots called during succesfull package install
    void installSuccessfully(const QString&);

    //There is a possibility that operation complete signal is emitted when download
    //progress is finished. Simulates that possibility.
    void installSuccessfullyWithOperationCompleteAfterDownload(const QString&);

    //Simulates slots called when uninstalling
    void uninstall(const QString&);

    //Simulates upgrading package.
    void upgradePackageSuccessfully(const QString&, const QString&);

    //Simulates cancelling operation.
    void cancelOperation(const QString &name, const QString &operation, const QString &state);

    // Mimicks an installation of the extra desktop file for a package
    void installPackageExtra(const QString &packageName, const QString &state = "installable");

    // Mimicks the change in the extra desktop file for a package when it is uninstalled
    void uninstallPackageExtra(QString packageName);

    // Mimicks the change in the extra desktop file for a package when it is broken
    void breakPackageExtra(QString packageName);

    // Mimicks the change in the extra desktop file for a package when it has error
    void hadErrorPackageExtra(QString packageName, QString packageState, QString hadError);

    // Compares packages given 'state' to packages state in datastore.
    void comparePackageStateInDataStore(const QString &packageName, const QString &state);

    // MApplication
    MApplication *app;
    // The object being tested
    ApplicationPackageMonitor *m_subject;

};

#endif
