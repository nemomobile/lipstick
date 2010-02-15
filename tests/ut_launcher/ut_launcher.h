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
#ifndef _UT_LAUNCHER_
#define _UT_LAUNCHER_

#include <QtTest/QtTest>
#include <QObject>
#include <QVector>

class Launcher;
class DuiApplication;
class DuiWidget;

class Ut_Launcher : public QObject
{
    Q_OBJECT

public:
    static QFileInfoList desktopFileInfoList;
    static QFileInfoList directoryFileInfoList;
    static QString       applicationStarted;
    static bool          duiApplicationIfProxyLaunchCalled;

private:
    Launcher *launcher;
    DuiApplication *app;

    void writeDesktopFile(QString fileName, QString type, QString name, QString iconName, QString exec);

signals:
    void directoryLaunched(const QString &directory, const QString &title = QString(), const QString &iconId = QString());
    void directoryChanged(const QString path);
    void applicationLaunched(const QString &service);
    void duiApplicationLaunched(const QString &service);
    void linkLaunched(const QString &link);
    void buttonClicked();

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test that the launcher initialization creates items for all desktop entries
    void testInitialization();
    // Test that the launcher includes an entry that is supposed to be shown in DUI
    void testOnlyShowInDui();
    // Test that the launcher doesn't include an entry that isn't supposed to be shown in DUI
    void testOnlyShowInNotDui();
    // Test that the launcher doesn't include an entry that is not supposed to be shown in DUI
    void testNotShowInDui();
    // Test that the launcher includes an entry that is not supposed to be shown in some other environment than DUI
    void testNotShowInNotDui();
    // Test that the launcher includes an entry that has a Category set to DUI
    // TODO: remove this when the Category DUI feature is removed
    void testCategoryDui();
    // Test that adding a new desktop entry to root adds a new widget
    void testDesktopEntryAddToRoot();
    // Test that adding a new desktop entry to a named category adds a new widget only to the correct category
    void testDesktopEntryAddToNamedCategory();
    // Test adding category to root
    void testCategoryEntryAdd();
    // Test adding category to another category
    void testCategoryEntryAddToAnotherCategory();
    // Test that invalid files are not added
    void testInvalidFiles();
    // Test that removing a new desktop entry removes the widget
    void testDesktopEntryRemove();
    // Test that launching an Application is attempted
    void testApplicationLaunched();
    // Test that launching a DuiApplication is attempted
    void testDuiApplicationLaunched();
    // Test launching an application from a subcategory
    void testLaunchingApplicationFromCategory();
    // Test launching a DUI application from a subcategory
    void testLaunchingDuiApplicationFromCategory();
    // Test launching a link from a subcategory
    void testLaunchingLinkFromCategory();
};
#endif //_UT_LAUNCHER_
