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

#ifndef UT_LAUNCHERDATASTORE_H
#define UT_LAUNCHERDATASTORE_H

#include <QObject>

class MApplication;
class LauncherDataStore;
class MockDataStore;

class Ut_LauncherDataStore : public QObject
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
    void testAddingDesktopEntryFiles();
    void testRemovingDesktopEntryFiles();
    void testUpdatingDesktopFilesFiltersDesktopEntries();
    void testUpdatingDataForDesktopEntry();
    void testRemovingDataForDesktopEntry();
    void testRemovingNonPrefixedKeys();
    void testAddingWatcherDesktopEntryPaths();
    void testUpdatingDesktopEntry();
    void testUpdatingInvalidEntry();
    void testRemovingEntriesWhenApplicationsDirectoryGetsEmpty();
    void testNotReprocessingInvalidEntry();
    void testUpdatingMultipleEntries();

private:
    // MApplication
    MApplication *app;
    // Data store for testing
    MockDataStore* mockStore;
};

#endif
