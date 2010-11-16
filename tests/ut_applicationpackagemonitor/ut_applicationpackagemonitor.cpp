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

#include <QtTest/QtTest>
#include <MApplication>
#include "ut_applicationpackagemonitor.h"
#include "applicationpackagemonitor.h"
#include <QtDBus>

#include "launcherdatastore_stub.h"
#include <mfiledatastore.h>
#include <mdesktopentry.h>

static const QString PACKAGE_MANAGER_DBUS_SERVICE="com.nokia.package_manager";
static const QString PACKAGE_MANAGER_DBUS_PATH="/com/nokia/package_manager";
static const QString PACKAGE_MANAGER_DBUS_INTERFACE="com.nokia.package_manager";

static const QString OPERATION_INSTALL = "Install";
static const QString OPERATION_UNINSTALL = "Uninstall";
static const QString OPERATION_REFRESH = "Refresh";
static const QString OPERATION_UPGRADE = "Upgrade";

static const QString PACKAGE_STATE_INSTALLED = "installed";
static const QString PACKAGE_STATE_INSTALLABLE = "installable";
static const QString PACKAGE_STATE_BROKEN = "broken";
static const QString PACKAGE_STATE_UPDATEABLE = "updateable";
static const QString PACKAGE_STATE_INSTALLING ="installing";
static const QString PACKAGE_STATE_DOWNLOADING ="downloading";

static const QString INSTALLER_EXTRA = "installer-extra/";

QString QDir::homePath()
{
    return QString("/tmp/ut_applicationpackagemonitor/");
}

// MFileDataStore stubs
QMap<QString, QVariant> g_fileDataStoreData;

MFileDataStore::MFileDataStore(const QString &fileName) :
    d_ptr(NULL)
{
    Q_UNUSED(fileName);
}

bool MFileDataStore::createValue(const QString &key, const QVariant &value)
{
    g_fileDataStoreData[key] = value;
    return true;
}

void MFileDataStore::remove(const QString &key)
{
    g_fileDataStoreData.remove(key);
}

bool MFileDataStore::contains(const QString &key) const
{
    return g_fileDataStoreData.contains(key);
}


QVariant MFileDataStore::value(const QString &key) const
{
    return g_fileDataStoreData[key];
}

QStringList MFileDataStore::allKeys() const
{
    return g_fileDataStoreData.keys();
}

// MDesktopEntry stubs
QMap<const MDesktopEntry *, QString> g_desktopEntryFileName;
QMap<QString, QMap<QString, QString> > g_desktopEntryValue;

QStringList addedWatcherPathCalls;

MDesktopEntry::MDesktopEntry(const QString &fileName) :
    d_ptr(NULL)
{
    g_desktopEntryFileName.insert(this, fileName);
}

MDesktopEntry::~MDesktopEntry()
{
    g_desktopEntryFileName.remove(this);
}

QString MDesktopEntry::fileName() const
{
    return g_desktopEntryFileName[this];
}


bool MDesktopEntry::isValid() const
{
    return true;
}

QString MDesktopEntry::value(const QString &group, const QString &key) const
{
    return g_desktopEntryValue.value(g_desktopEntryFileName.value(this)).value(group+"/"+key);
}

bool QFile::exists(const QString &filename)
{
    bool fileExists = filename.startsWith(QDir::homePath());

    if (!fileExists) {
        FILE *file = fopen(filename.toUtf8().constData(), "r");
        bool fileExists = file != NULL;

        if (fileExists) {
            fclose(file);
        }
    }

    return fileExists;
}

void Ut_ApplicationPackageMonitor::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_applicationpackagemonitor";
    app = new MApplication(argc, &app_name);
}

void Ut_ApplicationPackageMonitor::cleanupTestCase()
{
    delete app;
}

void Ut_ApplicationPackageMonitor::init()
{
    g_desktopEntryFileName.clear();
    g_desktopEntryValue.clear();
    g_fileDataStoreData.clear();
    gLauncherDataStoreStub->stubReset();
    m_subject = new ApplicationPackageMonitor();

    connect(this, SIGNAL(desktopEntryAdded(QString)), m_subject, SLOT(updatePackageState(QString)));
    connect(this, SIGNAL(desktopEntryChanged(QString)), m_subject, SLOT(updatePackageState(QString)));
    connect(this, SIGNAL(desktopEntryRemoved(QString)), m_subject, SLOT(packageRemoved(QString)));
}

void Ut_ApplicationPackageMonitor::cleanup()
{
    delete m_subject;
}


void Ut_ApplicationPackageMonitor::testUpdatingPackageStates()
{
    g_fileDataStoreData.insert("DesktopEntries/test_entry1.desktop", QVariant("installable"));
    g_fileDataStoreData.insert("DesktopEntries/test_entry2.desktop", QVariant("broken"));
    g_fileDataStoreData.insert("DesktopEntries/test_entry3.desktop", QVariant("installed"));
    g_fileDataStoreData.insert("DesktopEntries/test_entry4.desktop", QVariant("broken"));
    g_fileDataStoreData.insert("Packages/pkg_1", QVariant("/test_entry1.desktop"));
    g_fileDataStoreData.insert("Packages/pkg_2", QVariant("/test_entry2.desktop"));
    g_fileDataStoreData.insert("Packages/pkg_3", QVariant("/test_entry3.desktop"));
    g_fileDataStoreData.insert("Packages/pkg_4", QVariant("/test_entry4.desktop"));

    QSignalSpy spyError(m_subject, SIGNAL(operationError(QString, QString)));

    m_subject->updatePackageStates();

    QCOMPARE(spyError.count(), 2);
    QList<QVariant> arguments = spyError.at(0);
    QCOMPARE(arguments.at(0).toString(), QString("/test_entry2.desktop"));

    arguments = spyError.at(1);
    QCOMPARE(arguments.at(0).toString(), QString("/test_entry4.desktop"));
}

void Ut_ApplicationPackageMonitor::installUnsuccessfully(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    QString extraDesktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+name+".desktop";

    installPackageExtra(name);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLABLE);

    m_subject->packageDownloadProgress("Install", name, "version", 12, 24);

    comparePackageStateInDataStore(name, PACKAGE_STATE_DOWNLOADING);

    QCOMPARE(spyDownload.count(), 1);
    QList<QVariant> arguments = spyDownload.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 12);
    QVERIFY(arguments.at(2).toInt() == 24);

    arguments.clear();
    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&,  int)));

    m_subject->packageOperationProgress("Install", name, "version", 50);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLING);

    QCOMPARE(spyInstall.count(), 1);
    arguments = spyInstall.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 50);

    arguments.clear();
    QSignalSpy spyError(m_subject, SIGNAL(operationError(const QString&, const QString&)));

    m_subject->packageOperationComplete("Install", name, "version", "Error", 0);

    breakPackageExtra(name);

    comparePackageStateInDataStore(name, PACKAGE_STATE_BROKEN);

    // We should receive two signals, one from dbus signal and one from desktop entry monitor
    QCOMPARE(spyError.count(), 2);
    arguments = spyError.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toString() == "Error");
}

void Ut_ApplicationPackageMonitor::installSuccessfully(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+name+".desktop";
    QString extraDesktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+name+".desktop";

    installPackageExtra(name);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLABLE);

    m_subject->packageDownloadProgress("Install", name, "version", 12, 24);

    comparePackageStateInDataStore(name, PACKAGE_STATE_DOWNLOADING);

    QCOMPARE(spyDownload.count(), 1);
    QList<QVariant> arguments = spyDownload.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 12);
    QVERIFY(arguments.at(2).toInt() == 24);

    arguments.clear();
    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress("Install", name, "version", 50);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLING);

    QCOMPARE(spyInstall.count(), 1);
    arguments = spyInstall.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 50);

    arguments.clear();
    QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&)));

    // Change package's installer-extra desktop entry to installed state
    installPackageExtra(name, PACKAGE_STATE_INSTALLED);

    m_subject->packageOperationComplete("Install", name, "version", "", 0);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLED);

    QCOMPARE(spySuccess.count(), 1);
    arguments = spySuccess.takeFirst();
    QCOMPARE(arguments.at(0).toString(), desktopEntryFilename);
}

void Ut_ApplicationPackageMonitor::installSuccessfullyWithOperationCompleteAfterDownload(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+name+".desktop";
    QString extraDesktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+name+".desktop";

    installPackageExtra(name);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLABLE);

    m_subject->packageDownloadProgress("Install", name, "version", 12, 24);

    comparePackageStateInDataStore(name, PACKAGE_STATE_DOWNLOADING);

    QCOMPARE(spyDownload.count(), 1);
    QList<QVariant> arguments = spyDownload.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 12);
    QVERIFY(arguments.at(2).toInt() == 24);

    arguments.clear();
    QSignalSpy spyComplete(m_subject, SIGNAL(operationSuccess(const QString&)));

    m_subject->packageOperationComplete("Install", name, "version", "", 0);
    QCOMPARE(spyComplete.count(), 1);

    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress("Install", name, "version", 50);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLING);

    QCOMPARE(spyInstall.count(), 1);
    arguments = spyInstall.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 50);

    arguments.clear();
    QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&)));

    // Change package's installer-extra desktop entry to installed state
    installPackageExtra(name, PACKAGE_STATE_INSTALLED);

    m_subject->packageOperationComplete("Install", name, "version", "", 0);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLED);

    QCOMPARE(spySuccess.count(), 1);
    arguments = spySuccess.takeFirst();
    QCOMPARE(arguments.at(0).toString(), desktopEntryFilename);
}

void Ut_ApplicationPackageMonitor::upgradePackageSuccessfully(const QString &name, const QString &state)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+name+".desktop";
    QString extraDesktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+name+".desktop";

    installPackageExtra(name, state);

    comparePackageStateInDataStore(name, state);

    m_subject->packageDownloadProgress(OPERATION_UPGRADE, name, "version", 12, 24);

    comparePackageStateInDataStore(name, PACKAGE_STATE_DOWNLOADING);

    QCOMPARE(spyDownload.count(), 1);
    QList<QVariant> arguments = spyDownload.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 12);
    QVERIFY(arguments.at(2).toInt() == 24);

    arguments.clear();
    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress(OPERATION_UPGRADE, name, "version", 50);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLING);

    QCOMPARE(spyInstall.count(), 1);
    arguments = spyInstall.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    QVERIFY(arguments.at(1).toInt() == 50);

    arguments.clear();
    QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&)));

    m_subject->packageOperationComplete(OPERATION_UPGRADE, name, "version", "", 0);

    // Change package's installer-extra desktop entry to installed state
    installPackageExtra(name, PACKAGE_STATE_INSTALLED);

    comparePackageStateInDataStore(name, PACKAGE_STATE_INSTALLED);

    QCOMPARE(spySuccess.count(), 1);
    arguments = spySuccess.takeFirst();
    QCOMPARE(arguments.at(0).toString(), desktopEntryFilename);
}

void Ut_ApplicationPackageMonitor::cancelOperation(const QString &name, const QString &operation, const QString &state)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+name+".desktop";
    QString extraDesktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+name+".desktop";

    installPackageExtra(name, state);

    m_subject->packageDownloadProgress(operation, name, "version", 12, 24);

    QCOMPARE(m_subject->activePackages.count(), 1);

    g_fileDataStoreData.insert("DesktopEntries"+extraDesktopEntryFilename, QVariant(state));
    g_fileDataStoreData.insert("Packages/"+name, QVariant(extraDesktopEntryFilename));

    if (state == PACKAGE_STATE_INSTALLABLE || state == PACKAGE_STATE_BROKEN) {

        QSignalSpy spyError(m_subject, SIGNAL(operationError(const QString&, const QString&)));

        m_subject->packageOperationComplete(operation, name, "version", "cancel_download", 0);

        comparePackageStateInDataStore(name, PACKAGE_STATE_BROKEN);

        QCOMPARE(spyError.count(), 1);
        QList<QVariant> arguments = spyError.takeFirst();
        QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);

    } else if (state == PACKAGE_STATE_INSTALLED || state == PACKAGE_STATE_UPDATEABLE) {

        QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&)));

        m_subject->packageOperationComplete(operation, name, "version", "cancel_upgrade", 0);

        comparePackageStateInDataStore(name, state);

        QCOMPARE(spySuccess.count(), 1);
        QList<QVariant> arguments = spySuccess.takeFirst();
        QCOMPARE(arguments.at(0).toString(), desktopEntryFilename);
    }

    QCOMPARE(m_subject->activePackages.count(), 0);
}

void Ut_ApplicationPackageMonitor::uninstall(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    uninstallPackageExtra(name);

    m_subject->packageDownloadProgress("Uninstall", name, "version", 12, 24);

    QCOMPARE(spyDownload.count(), 0);

    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress("Uninstall", name, "version", 50);

    QCOMPARE(spyInstall.count(), 0);

    QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&)));

    m_subject->packageOperationComplete("Uninstall", name, "version", "", 0);
    QCOMPARE(spySuccess.count(), 0);

}

void Ut_ApplicationPackageMonitor::comparePackageStateInDataStore(const QString &packageName, const QString &state)
{
    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+packageName+".desktop";
    QCOMPARE(gLauncherDataStoreStub->stubLastCallTo("updateDataForDesktopEntry").parameter<QString>(0), desktopEntryFilename);
    QCOMPARE(gLauncherDataStoreStub->stubLastCallTo("updateDataForDesktopEntry").parameter<QVariant>(1).toString(), state);

    gLauncherDataStoreStub->stubReset();
}


void Ut_ApplicationPackageMonitor::installPackageExtra(const QString &packageName, const QString &state)
{
    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+packageName+".desktop";

    QMap<QString, QString> value;

    value["X-MeeGo/Package"] = packageName;
    value["X-MeeGo/PackageState"] = state;

    if (g_desktopEntryValue.contains(desktopEntryFilename)) {
        g_desktopEntryValue[desktopEntryFilename] = value;
        emit desktopEntryChanged(desktopEntryFilename);
    } else {
        g_desktopEntryValue[desktopEntryFilename] = value;
        emit desktopEntryAdded(desktopEntryFilename);
    }

    QCOMPARE(g_fileDataStoreData["Packages/"+packageName].toString(),
             desktopEntryFilename);
}

void Ut_ApplicationPackageMonitor::uninstallPackageExtra(QString packageName)
{
    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+packageName+".desktop";
    g_desktopEntryValue[desktopEntryFilename]["X-MeeGo/Package"] = packageName;
    g_desktopEntryValue[desktopEntryFilename]["X-MeeGo/PackageState"] = "installable";

    emit desktopEntryChanged(desktopEntryFilename);
}

void Ut_ApplicationPackageMonitor::breakPackageExtra(QString packageName)
{
    QString desktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+packageName+".desktop";
    g_desktopEntryValue[desktopEntryFilename]["X-MeeGo/Package"] = packageName;
    g_desktopEntryValue[desktopEntryFilename]["X-MeeGo/PackageState"] = "broken";

    emit desktopEntryChanged(desktopEntryFilename);

    QVERIFY(g_fileDataStoreData.contains("Packages/"+packageName));
}

void Ut_ApplicationPackageMonitor::hadErrorPackageExtra(QString packageName, QString packageState, QString hadError)
{
    QString desktopEntryFilename = "/" + packageName + ".desktop";
    g_desktopEntryValue[desktopEntryFilename]["X-MeeGo/Package"] = packageName;
    g_desktopEntryValue[desktopEntryFilename]["X-MeeGo/PackageState"] = packageState;
    g_desktopEntryValue[desktopEntryFilename]["X-MeeGo/PackageHadError"] = hadError;

    emit desktopEntryChanged(desktopEntryFilename);
}

void Ut_ApplicationPackageMonitor::testConstruction()
{
    QDBusConnection con = m_subject->con;
    bool result = false;
    result = con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "download_progress",
                    m_subject, SLOT(packageDownloadProgress(const QString&, const QString&, const QString&, int, int)));
    QCOMPARE(result, true);

    result = con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_started",
                    m_subject, SLOT(packageOperationStarted(const QString&, const QString&, const QString&)));
    QCOMPARE(result, true);

    result = con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_progress",
                    m_subject, SLOT(packageOperationProgress(const QString&, const QString &, const QString&, int)));
    QCOMPARE(result, true);

    result = con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_complete",
                    m_subject, SLOT(packageOperationComplete(const QString&, const QString&, const QString&, const QString&, bool)));
    QCOMPARE(result, true);

    ApplicationPackageMonitor::ExtraDirWatcher *watcher = m_subject->extraDirWatcher.data();
    disconnect((QObject*)watcher, SIGNAL(desktopEntryAdded(QString)), m_subject, SLOT(updatePackageState(QString)));
    disconnect((QObject*)watcher, SIGNAL(desktopEntryChanged(QString)), m_subject, SLOT(updatePackageState(QString)));
    disconnect((QObject*)watcher, SIGNAL(desktopEntryRemoved(QString)), m_subject, SLOT(packageRemoved(QString)));
}


void Ut_ApplicationPackageMonitor::testSuccessfullInstall()
{
    installSuccessfully("TestPackage1");
    installSuccessfully("TestPackage2");
    installSuccessfully("TestPackage3");
}

void Ut_ApplicationPackageMonitor::testUnsuccessfullInstall()
{
    installUnsuccessfully("TestPackage1");
    installUnsuccessfully("TestPackage2");
    installUnsuccessfully("TestPackage3");
}

void Ut_ApplicationPackageMonitor::testUninstallSignal()
{
    installSuccessfully("TestPackage");
    uninstall("TestPackage");
}

void Ut_ApplicationPackageMonitor::testUnsuccesfullAndSuccesfullInstall()
{
    QString name = "TestPackage";
    installUnsuccessfully(name);
    installSuccessfully(name);
}

void Ut_ApplicationPackageMonitor::testSuccessfullInstallWithOperationCompletedAfterDownload()
{
   installSuccessfullyWithOperationCompleteAfterDownload("TestPackage1");
   installSuccessfullyWithOperationCompleteAfterDownload("TestPackage2");
   installSuccessfullyWithOperationCompleteAfterDownload("TestPackage3");
}

void Ut_ApplicationPackageMonitor::testErrorSignalsForNewBrokenDesktopEntry()
{
    QString name("BrokenTestPackage");
    QString extraDesktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+name+".desktop";
    QSignalSpy spyError(m_subject, SIGNAL(operationError(const QString&, const QString&)));

    installPackageExtra("BrokenTestPackage", "broken");

    QCOMPARE(spyError.count(), 1);
    QList<QVariant> arguments = spyError.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    // We don't know the error when desktop entry changes to broken
    QVERIFY(arguments.at(1).toString() == "");
}

void Ut_ApplicationPackageMonitor::testErrorSignalsForDesktopEntryChangingToBrokenState()
{
    QString name("TestPackage");
    QString extraDesktopEntryFilename = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+name+".desktop";
    installPackageExtra(name);

    QSignalSpy spyError(m_subject, SIGNAL(operationError(const QString&, const QString&)));

    breakPackageExtra(name);

    QCOMPARE(spyError.count(), 1);
    QList<QVariant> arguments = spyError.takeFirst();
    QCOMPARE(arguments.at(0).toString(), extraDesktopEntryFilename);
    // We don't know the error when desktop entry changes to broken
    QVERIFY(arguments.at(1).toString() == "");
}

void Ut_ApplicationPackageMonitor::testUpgradingPackage()
{
    upgradePackageSuccessfully("TestPackage", PACKAGE_STATE_UPDATEABLE);
    upgradePackageSuccessfully("TestPackage", PACKAGE_STATE_INSTALLED);
}

void Ut_ApplicationPackageMonitor::testCancelDownloadingUnsuccessfully()
{
    cancelOperation("TestPackage", OPERATION_INSTALL, PACKAGE_STATE_INSTALLABLE);
    cancelOperation("TestPackage", OPERATION_UPGRADE, PACKAGE_STATE_INSTALLABLE);
    cancelOperation("TestPackage", OPERATION_INSTALL, PACKAGE_STATE_BROKEN);
    cancelOperation("TestPackage", OPERATION_UPGRADE, PACKAGE_STATE_BROKEN);
}

void Ut_ApplicationPackageMonitor::testCancelDownloadingSuccessfully()
{
    cancelOperation("TestPackage", OPERATION_INSTALL, PACKAGE_STATE_INSTALLED);
    cancelOperation("TestPackage", OPERATION_UPGRADE, PACKAGE_STATE_INSTALLED);
    cancelOperation("TestPackage", OPERATION_INSTALL, PACKAGE_STATE_UPDATEABLE);
    cancelOperation("TestPackage", OPERATION_UPGRADE, PACKAGE_STATE_UPDATEABLE);
}

void Ut_ApplicationPackageMonitor::testRemovingInstallerExtraFile()
{
    QString desktopEntryName1 = "entryInActiveProperties";
    QString desktopEntryName2 = "entryNotInActiveProperties";
    QString desktopEntryPath1 = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+desktopEntryName1+".desktop";
    QString desktopEntryPath2 = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+desktopEntryName2+".desktop";
    QString packageName1 = "pkg_1";
    QString packageName2 = "pkg_2";


    ApplicationPackageMonitor::PackageProperties properties;
    properties.desktopEntryName = desktopEntryPath1;

    m_subject->activePackages[packageName1] = properties;

    g_fileDataStoreData.insert("DesktopEntries"+desktopEntryPath1, QVariant(PACKAGE_STATE_INSTALLED));
    g_fileDataStoreData.insert("DesktopEntries"+desktopEntryPath2, QVariant(PACKAGE_STATE_INSTALLED));
    g_fileDataStoreData.insert("Packages/"+packageName1, QVariant(desktopEntryPath1));
    g_fileDataStoreData.insert("Packages/"+packageName2, QVariant(desktopEntryPath2));

    QSignalSpy spy(m_subject, SIGNAL(installExtraEntryRemoved(const QString &)));

    emit desktopEntryRemoved(desktopEntryPath2);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), desktopEntryPath2);
    QCOMPARE(m_subject->activePackages.count(), 1);

    QVERIFY(!g_fileDataStoreData.contains("DesktopEntries"+desktopEntryPath2+".desktop"));
    QVERIFY(!g_fileDataStoreData.contains("Packages/"+packageName2));

    arguments.clear();
    emit desktopEntryRemoved(desktopEntryPath1);

    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), desktopEntryPath1);
    QCOMPARE(m_subject->activePackages.count(), 0);

    QVERIFY(!g_fileDataStoreData.contains("DesktopEntries"+desktopEntryPath1+".desktop"));
    QVERIFY(!g_fileDataStoreData.contains("Packages/"+packageName1));
}

void Ut_ApplicationPackageMonitor::testEmitPackageStateWhenAllPackageStatesAreUpdated()
{
    QString entryPathInstalled = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+"entryInstalled.desktop";
    QString entryPathInstalling = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+"entryInstalling.desktop";
    QString entryPathBroken = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+"entryBroken.desktop";
    QString entryPathDownloading = APPLICATIONS_DIRECTORY+INSTALLER_EXTRA+"entryDownloading.desktop";
    QString packageName1 = "pkg_1";
    QString packageName2 = "pkg_2";
    QString packageName3 = "pkg_3";
    QString packageName4 = "pkg_4";

    g_fileDataStoreData.insert("DesktopEntries"+entryPathInstalled, QVariant(PACKAGE_STATE_INSTALLED));
    g_fileDataStoreData.insert("DesktopEntries"+entryPathInstalling, QVariant(PACKAGE_STATE_INSTALLING));
    g_fileDataStoreData.insert("DesktopEntries"+entryPathBroken, QVariant(PACKAGE_STATE_BROKEN));
    g_fileDataStoreData.insert("DesktopEntries"+entryPathDownloading, QVariant(PACKAGE_STATE_DOWNLOADING));
    g_fileDataStoreData.insert("Packages/"+packageName1, QVariant(entryPathInstalled));
    g_fileDataStoreData.insert("Packages/"+packageName2, QVariant(entryPathInstalling));
    g_fileDataStoreData.insert("Packages/"+packageName3, QVariant(entryPathBroken));
    g_fileDataStoreData.insert("Packages/"+packageName4, QVariant(entryPathDownloading));

    QSignalSpy spyInstalling(m_subject, SIGNAL(installProgress(const QString&, int)));
    QSignalSpy spyBroken(m_subject, SIGNAL(operationError(const QString&, const QString&)));
    QSignalSpy spyDownloading(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    m_subject->updatePackageStates();

    QCOMPARE(spyInstalling.count(), 1);
    QList<QVariant> arguments = spyInstalling.takeFirst();
    QCOMPARE(arguments.at(0).toString(), entryPathInstalling);

    arguments.clear();
    QCOMPARE(spyBroken.count(), 1);
    arguments = spyBroken.takeFirst();
    QCOMPARE(arguments.at(0).toString(), entryPathBroken);

    arguments.clear();
    QCOMPARE(spyDownloading.count(), 1);
    arguments = spyDownloading.takeFirst();
    QCOMPARE(arguments.at(0).toString(), entryPathDownloading);
}

void Ut_ApplicationPackageMonitor::testPackageHadError()
{
    g_fileDataStoreData.insert("DesktopEntries/pkg_1.desktop", QVariant("installable"));
    g_fileDataStoreData.insert("DesktopEntries/pkg_2.desktop", QVariant("broken"));
    g_fileDataStoreData.insert("DesktopEntries/pkg_3.desktop", QVariant("installed"));
    g_fileDataStoreData.insert("Packages/pkg_1", QVariant("/pkg_1.desktop"));
    g_fileDataStoreData.insert("Packages/pkg_2", QVariant("/pkg_2.desktop"));
    g_fileDataStoreData.insert("Packages/pkg_3", QVariant("/pkg_3.desktop"));

    QSignalSpy spyError(m_subject, SIGNAL(operationError(QString, QString)));

    // verify PackageHadError 'false' does nothing
    hadErrorPackageExtra("pkg_1", "installable", "false");
    QCOMPARE(spyError.count(), 0);
    spyError.clear();

    // Verify PackageHadError 'true' leads to broken package in any state
    hadErrorPackageExtra("pkg_1", "installable", "true");
    hadErrorPackageExtra("pkg_2", "installed", "true");
    hadErrorPackageExtra("pkg_3", "broken", "true");

    QCOMPARE(spyError.count(), 3);
    QCOMPARE(spyError.at(0).at(0).toString(), QString("/pkg_1.desktop"));
    QCOMPARE(spyError.at(1).at(0).toString(), QString("/pkg_2.desktop"));
    QCOMPARE(spyError.at(2).at(0).toString(), QString("/pkg_3.desktop"));
}

QTEST_APPLESS_MAIN(Ut_ApplicationPackageMonitor)
