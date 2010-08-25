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
#include "ut_quicklaunchbar.h"
#include "quicklaunchbar.h"
#include "launcherbutton_stub.h"
#include "launcherbuttonmodel.h"
#include "launcheraction_stub.h"
#include "launcherdatastore_stub.h"
#include "applicationpackagemonitor_stub.h"
#include "mockdatastore.h"
#include "windowinfo_stub.h"

void Ut_QuickLaunchBar::addButton(QString entryPath, int position)
{
    QHash<QString, QVariant> dataForAllDesktopEntries;
    QString placement = QString("quicklaunchbar/%1").arg(position);
    dataForAllDesktopEntries.insert(entryPath, QVariant(placement));
    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);
    gLauncherButtonStub->stubSetReturnValue("desktopEntry", entryPath);
    emit launcherDataStoreChanged();
}

void Ut_QuickLaunchBar::initTestCase()
{
}

void Ut_QuickLaunchBar::cleanupTestCase()
{
}

void Ut_QuickLaunchBar::init()
{
    launcherDataStore = new LauncherDataStore(new MockDataStore);
    packageMonitor = new ApplicationPackageMonitor();

    m_subject = new QuickLaunchBar;
    m_subject->setLauncherDataStore(launcherDataStore);
    m_subject->setApplicationPackageMonitor(packageMonitor);
    connect(this, SIGNAL(updateButtons()), m_subject, SLOT(updateButtons()));
    connect(this, SIGNAL(launcherDataStoreChanged()), launcherDataStore, SIGNAL(dataStoreChanged()));
}

void Ut_QuickLaunchBar::cleanup()
{
    delete launcherDataStore;
    launcherDataStore = NULL;
    delete packageMonitor;
    packageMonitor = NULL;
    delete m_subject;
    m_subject = NULL;
}

void Ut_QuickLaunchBar::testLauncherDataStoreChanged()
{
    QHash<QString, QVariant> dataForAllDesktopEntries;
    dataForAllDesktopEntries.insert("validPlacement", QVariant("quicklaunchbar/3"));
    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);
    emit launcherDataStoreChanged();
    QCOMPARE(m_subject->model()->buttons().count(), 1);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(0).data()) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(1).data()) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(2).data()) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(3).data()) != NULL);
}

void Ut_QuickLaunchBar::testSettingApplicationPackageMonitor()
{
    QVERIFY(disconnect(packageMonitor, SIGNAL(downloadProgress(const QString&, const QString &, int, int)),
            m_subject, SLOT(setDownloadProgress(const QString&, const QString &, int, int))));
    QVERIFY(disconnect(packageMonitor, SIGNAL(installProgress(const QString&, const QString &, int)),
            m_subject, SLOT(setInstallProgress(const QString&, const QString &, int))));
    QVERIFY(disconnect(packageMonitor, SIGNAL(operationSuccess(const QString&, const QString&)),
            m_subject, SLOT(setOperationSuccess(const QString&, const QString&))));
    QVERIFY(disconnect(packageMonitor, SIGNAL(operationError(const QString&, const QString&, const QString&)),
            m_subject, SLOT(setOperationError(const QString&, const QString&, const QString&))));
}

void Ut_QuickLaunchBar::testUpdateButtonState()
{
    addButton("/dev/null/test.desktop", 1);

    int progress = 50;

    // With invalid desktop entry updateButtonState() shouldn't call setState()
    m_subject->updateButtonState("invalid.desktop", LauncherButtonModel::Downloading, progress);
    QCOMPARE(gLauncherButtonStub->stubCallCount("setState"), 0);

    //Valid desktop entry
    m_subject->updateButtonState("test.desktop", LauncherButtonModel::Installing, progress);
    QCOMPARE(gLauncherButtonStub->stubCallCount("setState"), 1);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Installing);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), progress);
}

void Ut_QuickLaunchBar::testSetDownloadProgress()
{
    addButton("/dev/null/test.desktop", 1);

    int loaded = 50;
    int total = 100;
    int percentage = ((double)loaded / (double)total) * 100;
    m_subject->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), percentage);

    //test with invalid values
    total = 0;
    m_subject->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), -1);

    loaded = 100;
    total = 50;
    m_subject->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), -1);
}

void Ut_QuickLaunchBar::testSetInstallProgress()
{
    addButton("/dev/null/test.desktop", 1);

    int percentage = 25;
    m_subject->setInstallProgress("testPackage", "test.desktop", percentage);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Installing);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), percentage);
}

void Ut_QuickLaunchBar::testSetOperationSuccess()
{
    addButton("/dev/null/test.desktop", 1);

    //After calling setOperationSuccess state should be "Installed" and progress 0
    m_subject->setOperationSuccess("testPackage", "test.desktop");
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Installed);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), 0);
}

void Ut_QuickLaunchBar::testSetOperationError()
{
    addButton("/dev/null/test.desktop", 1);

    m_subject->setOperationError("testPackage", "test.desktop", "error_message");
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Broken);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), 0);
}

QTEST_MAIN(Ut_QuickLaunchBar)
