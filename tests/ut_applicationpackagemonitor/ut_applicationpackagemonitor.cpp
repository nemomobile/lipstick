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

static const QString PACKAGE_MANAGER_DBUS_SERVICE="com.nokia.package_manager";
static const QString PACKAGE_MANAGER_DBUS_PATH="/com/nokia/package_manager";
static const QString PACKAGE_MANAGER_DBUS_INTERFACE="com.nokia.package_manager";

static const QString OPERATION_INSTALL = "Install";
static const QString OPERATION_UNINSTALL = "Uninstall";
static const QString OPERATION_REFRESH = "Refresh";
static const QString OPERATION_UPGRADE = "Upgrade";

void Ut_ApplicationPackageMonitor::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_applicationpackagemonitor" };
    app = new MApplication(argc, app_name);
}

void Ut_ApplicationPackageMonitor::cleanupTestCase()
{
    delete app;
}

void Ut_ApplicationPackageMonitor::init()
{
    m_subject = new ApplicationPackageMonitor();
}

void Ut_ApplicationPackageMonitor::cleanup()
{
    delete m_subject;
}

void Ut_ApplicationPackageMonitor::installUnsuccessfully(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    m_subject->packageDownloadProgress("Install", name, "version", 12, 24);

    QCOMPARE(spyDownload.count(), 1);
    QList<QVariant> arguments = spyDownload.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
    QVERIFY(arguments.at(1).toInt() == 12);
    QVERIFY(arguments.at(2).toInt() == 24);

    arguments.clear();
    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress("Install", name, "version", 50);

    QCOMPARE(spyInstall.count(), 1);
    arguments = spyInstall.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
    QVERIFY(arguments.at(1).toInt() == 50);

    arguments.clear();
    QSignalSpy spyError(m_subject, SIGNAL(operationError(const QString&, const QString&)));

    m_subject->packageOperationComplete("Install", name, "version", "Error", 0);

    QCOMPARE(spyError.count(), 1);
    arguments = spyError.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
    QVERIFY(arguments.at(1).toString() == "Error");
}

void Ut_ApplicationPackageMonitor::installSuccessfully(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    m_subject->packageDownloadProgress("Install", name, "version", 12, 24);

    QCOMPARE(spyDownload.count(), 1);
    QList<QVariant> arguments = spyDownload.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
    QVERIFY(arguments.at(1).toInt() == 12);
    QVERIFY(arguments.at(2).toInt() == 24);

    arguments.clear();
    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress("Install", name, "version", 50);

    QCOMPARE(spyInstall.count(), 1);
    arguments = spyInstall.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
    QVERIFY(arguments.at(1).toInt() == 50);

    arguments.clear();
    QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&, const QString&)));

    m_subject->packageOperationComplete("Install", name, "version", "", 0);

    QCOMPARE(spySuccess.count(), 1);
    arguments = spySuccess.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
}

void Ut_ApplicationPackageMonitor::installSuccessfullyWithOperationCompleteAfterDownload(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    m_subject->packageDownloadProgress("Install", name, "version", 12, 24);

    QCOMPARE(spyDownload.count(), 1);
    QList<QVariant> arguments = spyDownload.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
    QVERIFY(arguments.at(1).toInt() == 12);
    QVERIFY(arguments.at(2).toInt() == 24);

    arguments.clear();
    QSignalSpy spyComplete(m_subject, SIGNAL(operationSuccess(const QString&, const QString&)));

    m_subject->packageOperationComplete("Install", name, "version", "", 0);
    QCOMPARE(spyComplete.count(), 0);

    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress("Install", name, "version", 50);

    QCOMPARE(spyInstall.count(), 1);
    arguments = spyInstall.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
    QVERIFY(arguments.at(1).toInt() == 50);

    arguments.clear();
    QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&, const QString&)));

    m_subject->packageOperationComplete("Install", name, "version", "", 0);

    QCOMPARE(spySuccess.count(), 1);
    arguments = spySuccess.takeFirst();
    QVERIFY(arguments.at(0).toString() == name);
}


void Ut_ApplicationPackageMonitor::uninstall(const QString &name)
{
    QSignalSpy spyDownload(m_subject, SIGNAL(downloadProgress(const QString&, int, int)));

    m_subject->packageDownloadProgress("Uninstall", name, "version", 12, 24);

    QCOMPARE(spyDownload.count(), 0);

    QSignalSpy spyInstall(m_subject, SIGNAL(installProgress(const QString&, int)));

    m_subject->packageOperationProgress("Uninstall", name, "version", 50);

    QCOMPARE(spyInstall.count(), 0);

    QSignalSpy spySuccess(m_subject, SIGNAL(operationSuccess(const QString&, const QString&)));

    m_subject->packageOperationComplete("Uninstall", name, "version", "", 0);

    QCOMPARE(spySuccess.count(), 0);
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


QTEST_APPLESS_MAIN(Ut_ApplicationPackageMonitor)
