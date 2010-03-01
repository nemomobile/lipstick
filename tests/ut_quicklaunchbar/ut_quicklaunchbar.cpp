/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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

#include "ut_quicklaunchbar.h"
#include "quicklaunchbar.h"
#include "launcher_stub.h"
#include "launcherbutton_stub.h"

#include <QDir>
#include <DuiDesktopEntry>
#include <duifiledatastore.h>

// QDir stubs
bool QDir::exists(const QString &) const
{
    return false;
}

bool QDir::mkpath(const QString &) const
{
    Ut_QuickLaunchBar::mkpathCalled = true;
    return true;
}

// DuiFileDataStore stubs
bool DuiFileDataStore::contains(const QString &key) const
{
    return key == "2/desktopFile" || key == "4/desktopFile";
}

QVariant DuiFileDataStore::value(const QString &key) const
{
    if (key == "2/desktopFile") {
        return QVariant("/tmp/existing.desktop");
    } else {
        return QVariant("/tmp/inexisting.desktop");
    }
}

// DuiDesktopEntry stubs
QList<QString> gValidDesktopFiles;
bool DuiDesktopEntry::isValid() const
{
    return gValidDesktopFiles.contains(fileName());
}

bool Ut_QuickLaunchBar::mkpathCalled;

void Ut_QuickLaunchBar::initTestCase()
{
}

void Ut_QuickLaunchBar::cleanupTestCase()
{
}

void Ut_QuickLaunchBar::init()
{
    mkpathCalled = false;

    gValidDesktopFiles.clear();
    gValidDesktopFiles << "/tmp/existing.desktop";

    m_subject = new QuickLaunchBar();
    connect(this, SIGNAL(updateWidgetList()), m_subject, SLOT(updateWidgetList()));
    connect(this, SIGNAL(applicationLaunched(const QString &)), m_subject, SLOT(launchApplication(const QString &)));
    connect(this, SIGNAL(duiApplicationLaunched(const QString &)), m_subject, SLOT(launchDuiApplication(const QString &)));
}

void Ut_QuickLaunchBar::cleanup()
{
    delete m_subject;
}

void Ut_QuickLaunchBar::testInitialization()
{
    // QDir::exists returns false so mkpath should be called
    QVERIFY(mkpathCalled);
}

void Ut_QuickLaunchBar::testUpdateWidgetList()
{
    // There should be 4 widgets; second should be a LauncherButton
    QCOMPARE(m_subject->model()->widgets().count(), 4);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(0)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(1)) != NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(2)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(3)) == NULL);
}

void Ut_QuickLaunchBar::testRemoveOneApplicationFromFileSystem()
{
    // "Remove" one application from file system
    gValidDesktopFiles.removeAll("/tmp/existing.desktop");
    emit updateWidgetList();
    QCOMPARE(m_subject->model()->widgets().count(), 4);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(0)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(1)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(2)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(3)) == NULL);
}

void Ut_QuickLaunchBar::testLaunchApplication()
{
    emit applicationLaunched("testApplication");
    QCOMPARE(gLauncherStub->stubCallCount("startApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startApplication").parameter<QString>(0), QString("testApplication"));
}

void Ut_QuickLaunchBar::testLaunchDuiApplication()
{
    emit duiApplicationLaunched("testService");
    QCOMPARE(gLauncherStub->stubCallCount("startDuiApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startDuiApplication").parameter<QString>(0), QString("testService"));
}

QTEST_MAIN(Ut_QuickLaunchBar)
