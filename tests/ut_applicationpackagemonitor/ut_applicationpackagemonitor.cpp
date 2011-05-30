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
#include "extradirwatcher.h"
#include "homefiledatastore_stub.h"
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

const static QString INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE("/null/dev/applications/installer-extra/test_entry%1.desktop");
const static QString APPLICATIONS_ENTRY_NAME_TEMPLATE("/null/dev/applications/test_entry%1.desktop");
const static QString PACKAGE_NAME_TEMPLATE("package%1");
void Ut_ApplicationPackageMonitor::initializeEntries(int count, const QStringList &states, bool packageRemovable)
{
    for (int i = 0; i < count; i++) {
        QString entry = INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(i);
        QString packageName = PACKAGE_NAME_TEMPLATE.arg(i);
        QString removable = packageRemovable ? "true" : "false";
        m_subject->dataStore->setValue("DesktopEntries" + entry, QVariant(packageName));

        QMap<QString, QString> value;
        value["X-MeeGo/Package"] = packageName;
        if (i < states.count()) {
            value["X-MeeGo/PackageState"] = states[i];
        } else {
            value["X-MeeGo/PackageState"] = "installed";
        }
        value["X-MeeGo/PackageRemovable"] = removable;
        g_desktopEntryValue[entry] = value;
    }
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
    m_subject = new ApplicationPackageMonitor();
}

void Ut_ApplicationPackageMonitor::cleanup()
{
    delete m_subject;
}

void Ut_ApplicationPackageMonitor::testUpdatingPackageStates()
{
    initializeEntries(4, QStringList() << "installing" << "downloading" << "broken" << "installed");

    QSignalSpy spyInstalling(m_subject, SIGNAL(installProgress(QString, QString, int, bool)));
    QSignalSpy spyDownloading(m_subject, SIGNAL(downloadProgress(QString, QString, int, int, bool)));
    QSignalSpy spyError(m_subject, SIGNAL(operationError(QString, QString, QString, bool)));
    QSignalSpy spyInstalled(m_subject, SIGNAL(operationSuccess(QString, QString, bool)));

    m_subject->updatePackageStates();

    QCOMPARE(spyInstalling.count(), 1);
    QCOMPARE(spyInstalling.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0)));
    QCOMPARE(spyInstalling.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(0)));

    QCOMPARE(spyDownloading.count(), 1);
    QCOMPARE(spyDownloading.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(1)));
    QCOMPARE(spyDownloading.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(1)));

    QCOMPARE(spyError.count(), 1);
    QCOMPARE(spyError.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(2)));
    QCOMPARE(spyError.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(2)));

    QCOMPARE(spyInstalled.count(), 1);
    QCOMPARE(spyInstalled.at(0).at(0).toString(), QString(APPLICATIONS_ENTRY_NAME_TEMPLATE.arg(3)));
    QCOMPARE(spyInstalled.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(3)));
}

void Ut_ApplicationPackageMonitor::testUpdatingPackageDataWhenDesktopEntryAdded()
{
    QString entryPath = INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0);
    QString packageName = PACKAGE_NAME_TEMPLATE.arg(0);

    QMap<QString, QString> value;
    value["X-MeeGo/Package"] = packageName;
    value["X-MeeGo/PackageState"] = "installed";
    g_desktopEntryValue[entryPath] = value;

    QVERIFY(disconnect(m_subject->extraDirWatcher.data(), SIGNAL(desktopEntryAdded(QString)), m_subject, SLOT(updatePackageState(QString))));
    m_subject->updatePackageState(entryPath);

    // Verify that data was added
    QCOMPARE(m_subject->dataStore->allKeys().at(0), LauncherDataStore::entryPathToKey(entryPath));
    QCOMPARE(m_subject->dataStore->value(LauncherDataStore::entryPathToKey(entryPath)).toString(), packageName);
}

void Ut_ApplicationPackageMonitor::testUpdatingPackageStateWhenDesktopEntryChanges_data()
{
    QTest::addColumn<QString>("state");
    QTest::addColumn<void *>("signal");

    QTest::newRow("Broken state") << QString("broken") << (void *)SIGNAL(operationError(QString, QString, QString, bool));
    QTest::newRow("Installing state") << QString("installing") << (void *)SIGNAL(installProgress(QString, QString, int, bool));
    QTest::newRow("Downloading state") << QString("downloading") << (void *)SIGNAL(downloadProgress(QString, QString, int, int, bool));
    QTest::newRow("Installed state") << QString("installed") << (void *)SIGNAL(operationSuccess(QString, QString, bool));
}

void Ut_ApplicationPackageMonitor::testUpdatingPackageStateWhenDesktopEntryChanges()
{
    QFETCH(QString, state);
    QFETCH(void *, signal);

    QSignalSpy signalSpy(m_subject, (char *)signal);
    initializeEntries(1, QStringList() << state);

    QVERIFY(disconnect(m_subject->extraDirWatcher.data(), SIGNAL(desktopEntryChanged(QString)), m_subject, SLOT(updatePackageState(QString))));
    m_subject->updatePackageState(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0));

    QCOMPARE(signalSpy.count(), 1);
    if (state == "installed") {
        QCOMPARE(signalSpy.at(0).at(0).toString(), QString(APPLICATIONS_ENTRY_NAME_TEMPLATE.arg(0)));
    } else {
        QCOMPARE(signalSpy.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0)));
    }
}

void Ut_ApplicationPackageMonitor::testRemovingPackageDataWhenDesktopEntryIsRemoved()
{
    QSignalSpy signalSpy(m_subject, SIGNAL(installExtraEntryRemoved(QString)));
    initializeEntries(1, QStringList() << "installed");
    QString entryPath = INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0);

    QVERIFY(disconnect(m_subject->extraDirWatcher.data(), SIGNAL(desktopEntryRemoved(QString)), m_subject, SLOT(packageRemoved(QString))));
    m_subject->packageRemoved(entryPath);

    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(signalSpy.at(0).at(0).toString(), QString(entryPath));
    // Verify that data was removed
    QCOMPARE(m_subject->dataStore->value(LauncherDataStore::entryPathToKey(entryPath)), QVariant(QString()));
}

void Ut_ApplicationPackageMonitor::testDownloadProgressSignal()
{
    QVERIFY(m_subject->con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "download_progress",
                    m_subject, SLOT(packageDownloadProgress(const QString&, const QString&, const QString&, int, int))));

    initializeEntries(1, QStringList() << "installed", false);

    QSignalSpy signalSpy(m_subject, SIGNAL(downloadProgress(QString, QString, int, int, bool)));

    m_subject->packageDownloadProgress("Install", PACKAGE_NAME_TEMPLATE.arg(0), "", 50, 100);
    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(signalSpy.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(2).toInt(), 50);
    QCOMPARE(signalSpy.at(0).at(3).toInt(), 100);
    QCOMPARE(signalSpy.at(0).at(4).toBool(), false);
}

void Ut_ApplicationPackageMonitor::testInstallProgressSignal()
{
    QVERIFY(m_subject->con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_progress",
                    m_subject, SLOT(packageOperationProgress(const QString&, const QString &, const QString&, int))));

    initializeEntries(1, QStringList() << "installed", true);

    QSignalSpy signalSpy(m_subject, SIGNAL(installProgress(QString, QString, int, bool)));

    m_subject->packageOperationProgress("Install", PACKAGE_NAME_TEMPLATE.arg(0), "", 50);
    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(signalSpy.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(2).toInt(), 50);
    QCOMPARE(signalSpy.at(0).at(3).toBool(), true);
}

void Ut_ApplicationPackageMonitor::testInstallSuccessSignal()
{
    QVERIFY(m_subject->con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_complete",
                    m_subject, SLOT(packageOperationComplete(const QString&, const QString&, const QString&, const QString&, bool))));

    initializeEntries(1, QStringList() << "installed");

    QSignalSpy signalSpy(m_subject, SIGNAL(operationSuccess(QString, QString, bool)));

    m_subject->packageOperationComplete("Install", PACKAGE_NAME_TEMPLATE.arg(0), "", "", false);
    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(signalSpy.at(0).at(0).toString(), QString(APPLICATIONS_ENTRY_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(2).toBool(), true);
}

void Ut_ApplicationPackageMonitor::testInstallSuccessSignalWithError()
{
    initializeEntries(1, QStringList() << "broken");

    QSignalSpy signalSpy(m_subject, SIGNAL(operationError(QString, QString, QString, bool)));

    m_subject->packageOperationComplete("Install", PACKAGE_NAME_TEMPLATE.arg(0), "", "BAR_ERROR", false);
    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(signalSpy.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(3).toBool(), true);
}

void Ut_ApplicationPackageMonitor::testUninstallSignal()
{
    QVERIFY(m_subject->con.disconnect(QString(),PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "operation_started",
                    m_subject, SLOT(packageOperationStarted(const QString&, const QString&, const QString&))));

    initializeEntries(1, QStringList() << "installed");

    QSignalSpy signalSpy(m_subject, SIGNAL(packageUninstall(QString, QString, bool)));

    m_subject->packageOperationStarted("Uninstall", PACKAGE_NAME_TEMPLATE.arg(0), "");
    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(signalSpy.at(0).at(0).toString(), QString(INSTALLER_EXTRA_ENTRY_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(1).toString(), QString(PACKAGE_NAME_TEMPLATE.arg(0)));
    QCOMPARE(signalSpy.at(0).at(2).toBool(), true);
}

QTEST_APPLESS_MAIN(Ut_ApplicationPackageMonitor)
