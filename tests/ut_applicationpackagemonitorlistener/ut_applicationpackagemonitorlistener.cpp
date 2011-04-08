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
#include "ut_applicationpackagemonitorlistener.h"
#include "applicationpackagemonitorlistener.h"
#include "applicationpackagemonitor_stub.h"

void Ut_ApplicationPackageMonitorListener::initTestCase()
{
    qRegisterMetaType<LauncherButtonModel::State>("State");

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_applicationpackagemonitorlistener" };
    app = new MApplication(argc, app_name);
}

void Ut_ApplicationPackageMonitorListener::cleanupTestCase()
{
    delete app;
}

void Ut_ApplicationPackageMonitorListener::init()
{
    m_subject = new ApplicationPackageMonitorListener();
}

void Ut_ApplicationPackageMonitorListener::cleanup()
{
    delete m_subject;
}

void Ut_ApplicationPackageMonitorListener::testSignalConnections()
{
    QVERIFY(disconnect(m_subject->packageMonitor.data(), SIGNAL(downloadProgress(QString, QString, int, int)),
            m_subject, SLOT(setDownloadProgress(QString, QString, int, int))));
    QVERIFY(disconnect(m_subject->packageMonitor.data(), SIGNAL(installProgress(QString, QString, int)),
            m_subject, SLOT(setInstallProgress(QString, QString, int))));
    QVERIFY(disconnect(m_subject->packageMonitor.data(), SIGNAL(operationSuccess(QString, QString)),
            m_subject, SLOT(setOperationSuccess(QString, QString))));
    QVERIFY(disconnect(m_subject->packageMonitor.data(), SIGNAL(operationError(QString, QString, QString)),
            m_subject, SLOT(setOperationError(QString, QString, QString))));
    QVERIFY(disconnect(m_subject->packageMonitor.data(), SIGNAL(installExtraEntryRemoved(QString)),
            m_subject, SIGNAL(installExtraEntryRemoved(QString))));
    QVERIFY(disconnect(m_subject->packageMonitor.data(), SIGNAL(updatePackageName(QString, QString)),
            m_subject, SIGNAL(updatePackageName(QString, QString))));

}

void Ut_ApplicationPackageMonitorListener::testSetDownloadProgress_data()
{
    QTest::addColumn<int>("loaded");
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("progress");
    QTest::newRow("Download progress") << 50 << 100 << 50;
    QTest::newRow("Total loaded zero") << 50 << 0 << -1;
    QTest::newRow("Loaded bigger then total") << 100 << 50 << -1;
}

void Ut_ApplicationPackageMonitorListener::testSetDownloadProgress()
{
    QFETCH(int, loaded);
    QFETCH(int, total);
    QFETCH(int, progress);

    QSignalSpy spyButtonState(m_subject, SIGNAL(packageStateChanged(QString, QString, LauncherButtonModel::State, int)));

    m_subject->setDownloadProgress("test.desktop", QString(), loaded, total);
    
    QCOMPARE(spyButtonState.count(), 1);
    QList<QVariant> arguments = spyButtonState.at(0);
    QCOMPARE(arguments.at(0).toString(), QString("test.desktop"));
    QCOMPARE(arguments.at(2).value<LauncherButtonModel::State>(), LauncherButtonModel::Downloading);
    QCOMPARE(arguments.at(3).toInt(), progress);
}

void Ut_ApplicationPackageMonitorListener::testSetInstallProgress()
{
    QSignalSpy spyButtonState(m_subject, SIGNAL(packageStateChanged(QString, QString, LauncherButtonModel::State, int)));
    int percentage = 50;
    m_subject->setInstallProgress("test.desktop", QString(), percentage);

    QCOMPARE(spyButtonState.count(), 1);
    QList<QVariant> arguments = spyButtonState.at(0);
    QCOMPARE(arguments.at(0).toString(), QString("test.desktop"));
    QCOMPARE(arguments.at(2).value<LauncherButtonModel::State>(), LauncherButtonModel::Installing);
    QCOMPARE(arguments.at(3).toInt(), percentage);
}

void Ut_ApplicationPackageMonitorListener::testSetOperationSuccess()
{
    QSignalSpy spyButtonState(m_subject, SIGNAL(packageStateChanged(QString, QString, LauncherButtonModel::State, int)));

    m_subject->setOperationSuccess("test.desktop", QString());

    QCOMPARE(spyButtonState.count(), 1);
    QList<QVariant> arguments = spyButtonState.at(0);
    QCOMPARE(arguments.at(0).toString(), QString("test.desktop"));
    QCOMPARE(arguments.at(2).value<LauncherButtonModel::State>(), LauncherButtonModel::Installed);
    QCOMPARE(arguments.at(3).toInt(), 0);
}

void Ut_ApplicationPackageMonitorListener::testSetOperationError()
{
    QSignalSpy spyButtonState(m_subject, SIGNAL(packageStateChanged(QString, QString, LauncherButtonModel::State, int)));

    m_subject->setOperationError("test.desktop", QString(), "error_message");

    QCOMPARE(spyButtonState.count(), 1);
    QList<QVariant> arguments = spyButtonState.at(0);
    QCOMPARE(arguments.at(0).toString(), QString("test.desktop"));
    QCOMPARE(arguments.at(2).value<LauncherButtonModel::State>(), LauncherButtonModel::Broken);
    QCOMPARE(arguments.at(3).toInt(), 0);
}

void Ut_ApplicationPackageMonitorListener::testUpdatePackageStates()
{
    m_subject->updatePackageStates();
    QCOMPARE(gApplicationPackageMonitorStub->stubCallCount("updatePackageStates"), 1);
}

void Ut_ApplicationPackageMonitorListener::testIsInstallerExtraEntry()
{
    QVERIFY(ApplicationPackageMonitorListener::isInstallerExtraEntry("/dev/null/installer-extra/entry.desktop"));
    QVERIFY(!ApplicationPackageMonitorListener::isInstallerExtraEntry("/dev/null/entry.desktop"));
}

QTEST_APPLESS_MAIN(Ut_ApplicationPackageMonitorListener)
