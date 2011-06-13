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
#include "homewindowmonitor.h"
#include "launcherbutton_stub.h"
#include "launcherbuttonmodel.h"
#include "launcheraction_stub.h"
#include "launcherdatastore_stub.h"
#include "applicationpackagemonitor_stub.h"
#include "mockdatastore.h"
#include "windowinfo_stub.h"
#include "launcher_stub.h"

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
    launcherDataStore = new LauncherDataStore(new MockDataStore, QStringList());
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
    QVERIFY(disconnect(m_subject->model()->buttons().value(3).data(), SIGNAL(clicked()), m_subject, SIGNAL(launcherButtonClicked())));
}

void Ut_QuickLaunchBar::testSettingApplicationPackageMonitor()
{
    QVERIFY(disconnect(packageMonitor, SIGNAL(packageStateChanged(const QString&, QString, LauncherButtonModel::State, int, bool)),
            m_subject, SLOT(updateButtonState(const QString, QString, QString, bool))));
}

void Ut_QuickLaunchBar::testUpdateButtonState()
{
    addButton("/dev/null/test.desktop", 1);

    // With invalid desktop entry updateButtonState() shouldn't call setState()
    m_subject->updateButtonState("invalid.desktop", QString(), "downloading", true);
    QCOMPARE(gLauncherButtonStub->stubCallCount("setState"), 0);

    //Valid desktop entry
    m_subject->updateButtonState("test.desktop", QString(), "installing", true);
    QCOMPARE(gLauncherButtonStub->stubCallCount("setState"), 1);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Installing);
}

QTEST_MAIN(Ut_QuickLaunchBar)
