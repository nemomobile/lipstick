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

#include "ut_quicklaunchbar.h"
#include "quicklaunchbar.h"
#include "launcher_stub.h"
#include "launcherbutton_stub.h"

#include <QtTest/QtTest>
#include <QDir>
#include <MDesktopEntry>

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

// MDesktopEntry stubs
QList<QString> gValidDesktopFiles;
bool MDesktopEntry::isValid() const
{
    return gValidDesktopFiles.contains(fileName());
}

QVariant TestDataStore::value(const QString &key) const
{
    return values[key];
}

bool TestDataStore::setValue(const QString &key, const QVariant &value)
{
    values[key] = value;
    return true;
}

QStringList TestDataStore::allKeys() const
{
    return QStringList();
}

bool TestDataStore::contains(const QString &key) const
{
    return values.contains(key);
}

bool TestDataStore::createValue(const QString&, const QVariant&)
{
    return true;
}

void TestDataStore::remove(const QString &key)
{
    if (contains(key)) {
        QVariant value = values.take(key);
        emit valueChanged(key, value);
    }
}

void TestDataStore::clear()
{

}

bool Ut_QuickLaunchBar::mkpathCalled = false;

void Ut_QuickLaunchBar::initTestCase()
{
    m_configuration = NULL;
    m_subject = NULL;
}

void Ut_QuickLaunchBar::cleanupTestCase()
{
}

void Ut_QuickLaunchBar::init()
{
    mkpathCalled = false;

    gValidDesktopFiles.clear();
    gValidDesktopFiles << "/tmp/existing.desktop";

    MDataStore *configuration = new TestDataStore;
    configuration->setValue("2/desktopFile", "/tmp/existing.desktop");
    configuration->setValue("4/desktopFile", "/tmp/inexisting.desktop");
    setupTestSubject(configuration);
}

void Ut_QuickLaunchBar::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_QuickLaunchBar::setupTestSubject(MDataStore *configuration)
{
    delete m_subject;
    m_configuration = configuration;
    m_subject = new QuickLaunchBar(configuration);
    connect(this, SIGNAL(updateWidgetList()), m_subject, SLOT(updateWidgetList()));
    connect(this, SIGNAL(applicationLaunched(const QString &)), m_subject, SLOT(launchApplication(const QString &)));
    connect(this, SIGNAL(mApplicationLaunched(const QString &)), m_subject, SLOT(launchMApplication(const QString &)));
}

void Ut_QuickLaunchBar::testInitialization()
{
    // There should be 4 widgets; second should be a LauncherButton
    QCOMPARE(m_subject->model()->widgets().count(), 4);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(0)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(1)) != NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(2)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(3)) == NULL);
}

void Ut_QuickLaunchBar::testInitializationWithoutDataStore()
{
    setupTestSubject(NULL);
    // QDir::exists returns false so mkpath should be called
    QVERIFY(mkpathCalled);
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
    QCOMPARE(m_configuration->contains("2/desktopFile"), false);
}

void Ut_QuickLaunchBar::testLaunchApplication()
{
    emit applicationLaunched("testApplication");
    QCOMPARE(gLauncherStub->stubCallCount("startApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startApplication").parameter<QString>(0), QString("testApplication"));
}

void Ut_QuickLaunchBar::testLaunchMApplication()
{
    emit mApplicationLaunched("testService");
    QCOMPARE(gLauncherStub->stubCallCount("startMApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startMApplication").parameter<QString>(0), QString("testService"));
}

void Ut_QuickLaunchBar::testExternalConfigurationChangeIsNoticed()
{
    m_configuration->remove("2/desktopFile");
    QCOMPARE(m_subject->model()->widgets().count(), 4);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(0)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(1)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(2)) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->widgets().at(3)) == NULL);
}

QTEST_MAIN(Ut_QuickLaunchBar)
