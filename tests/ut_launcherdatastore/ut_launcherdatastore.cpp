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
#include <MDesktopEntry>
#include "launcherdatastore.h"
#include "launcherpage.h"
#include "mockdatastore.h"
#include "ut_launcherdatastore.h"

// MDesktopEntry stubs (used by Launcher)
QMap<const MDesktopEntry *, QString> desktopEntryFileName;
QMap<QString, QStringList> desktopEntryCategories;
QMap<QString, QStringList> desktopEntryOnlyShowIn;
QMap<QString, QStringList> desktopEntryNotShowIn;
QMap<QString, QString> desktopEntryType;
QMap<QString, QString> desktopEntryXMaemoService;
QMap<QString, QString> desktopEntryName;
QMap<QString, QString> desktopEntryIcon;
QMap<QString, QString> desktopEntryExec;
QMap<QString, QString> desktopEntryUrl;
QMap<QString, QString> desktopEntryNameUnlocalized;

QStringList addedWatcherPathCalls;

MDesktopEntry::MDesktopEntry(const QString &fileName) :
    d_ptr(NULL)
{
    desktopEntryFileName.insert(this, fileName);
}

QString MDesktopEntry::fileName() const
{
    return desktopEntryFileName[this];
}


bool MDesktopEntry::isValid() const
{
    return true;
}

QStringList MDesktopEntry::onlyShowIn() const
{
    return desktopEntryOnlyShowIn.value(desktopEntryFileName.value(this));
}

QStringList MDesktopEntry::notShowIn() const
{
    return desktopEntryNotShowIn.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::type() const
{
    return desktopEntryType.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::xMaemoService() const
{
    return desktopEntryXMaemoService.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::name() const
{
    return desktopEntryName.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::icon() const
{
    return desktopEntryIcon.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::exec() const
{
    return desktopEntryExec.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::url() const
{
    return desktopEntryUrl.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::nameUnlocalized() const
{
    return desktopEntryNameUnlocalized.value(desktopEntryFileName.value(this));
}

// QDir stubs
QFileInfoList desktopFileInfoList;
QFileInfoList QDir::entryInfoList(Filters, SortFlags) const
{
    return desktopFileInfoList;
}

// QFileSystemWatcher stubs
void QFileSystemWatcher::addPath(const QString &path)
{
    addedWatcherPathCalls.append(path);
}

QStringList QFileSystemWatcher::files() const
{
    return addedWatcherPathCalls;
}

// QTimer stubs
bool qTimerStarted;
void QTimer::start()
{
    qTimerStarted = true;
}

void Ut_LauncherDataStore::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherdatastore" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherDataStore::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherDataStore::init()
{
    mockStore = new MockDataStore;
    mockStore->clear();
    desktopEntryFileName.clear();
    desktopEntryCategories.clear();
    desktopEntryOnlyShowIn.clear();
    desktopEntryNotShowIn.clear();
    desktopEntryType.clear();
    desktopEntryXMaemoService.clear();
    desktopEntryName.clear();
    desktopEntryIcon.clear();
    desktopEntryExec.clear();
    desktopEntryUrl.clear();
    desktopEntryNameUnlocalized.clear();
    desktopFileInfoList.clear();
    addedWatcherPathCalls.clear();
    qTimerStarted = false;
}

void Ut_LauncherDataStore::cleanup()
{    
}

QString fileNameWithPath(const QString &fileName)
{
    return QString(APPLICATIONS_DIRECTORY) + fileName;
}

void addDesktopEntry(const QString &fileName, const QString &name, const QString &type, const QString &icon, const QString &exec)
{
    QString fullFileName = fileNameWithPath(fileName);
    desktopEntryName.insert(fullFileName, name);
    desktopEntryType.insert(fullFileName, type);
    desktopEntryIcon.insert(fullFileName, icon);
    desktopEntryExec.insert(fullFileName, exec);
    desktopFileInfoList.append(fullFileName);
}

void removeDesktopEntries()
{
    desktopEntryName.clear();
    desktopEntryType.clear();
    desktopEntryIcon.clear();
    desktopEntryExec.clear();
    desktopFileInfoList.clear();
}

void Ut_LauncherDataStore::testUpdateStartedDuringInitializationWhenApplicationsDirectoryNotEmpty()
{
    addDesktopEntry("testApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    LauncherDataStore dataStore(mockStore);
    QVERIFY(qTimerStarted);
}

void Ut_LauncherDataStore::testUpdateNotStartedDuringInitializationWhenApplicationsDirectoryEmpty()
{
    LauncherDataStore dataStore(mockStore);
    QVERIFY(!qTimerStarted);
}

void Ut_LauncherDataStore::testUpdateStartedWhenApplicationsDirectoryChanges()
{
    LauncherDataStore dataStore(mockStore);
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    addDesktopEntry("testApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    emit directoryChanged();
    QVERIFY(qTimerStarted);
}

void Ut_LauncherDataStore::testUpdateNotStartedWhenApplicationsDirectoryEmpty()
{
    LauncherDataStore dataStore(mockStore);
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    emit directoryChanged();
    QVERIFY(!qTimerStarted);
}

void Ut_LauncherDataStore::testUpdateNotStartedWhenInProgress()
{
    addDesktopEntry("testApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    LauncherDataStore dataStore(mockStore);
    qTimerStarted = false;

    // Requesting an update while one is in progress should not restart it
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    emit directoryChanged();
    QVERIFY(!qTimerStarted);
}

void Ut_LauncherDataStore::testProcessUpdateQueueDoesNothingWhenQueueEmpty()
{
    LauncherDataStore dataStore(mockStore);
    addDesktopEntry("testApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 0);
}

void Ut_LauncherDataStore::testProcessUpdateQueueProcessesNFilesAtATime()
{
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");
    addDesktopEntry("testApplication3.desktop", "Test3", "Application", "Icon-camera", "test3");
    addDesktopEntry("testApplication4.desktop", "Test4", "Application", "Icon-camera", "test4");
    LauncherDataStore dataStore(mockStore);
    QSignalSpy spy(&dataStore, SIGNAL(dataStoreChanged()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    // The amount of files processed (N) is currently 3
    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 3);

    // Since there are items remaining the timer should be started and dataStoreChanged() should not be emitted
    QVERIFY(qTimerStarted);
    QCOMPARE(spy.count(), 0);
}

void Ut_LauncherDataStore::testProcessUpdateQueueFinishesProcessingCorrectly()
{
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");
    addDesktopEntry("testApplication3.desktop", "Test3", "Application", "Icon-camera", "test3");
    addDesktopEntry("testApplication4.desktop", "Test4", "Application", "Icon-camera", "test4");
    LauncherDataStore dataStore(mockStore);
    QSignalSpy spy(&dataStore, SIGNAL(dataStoreChanged()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    // The second batch should finish the job
    qTimerStarted = false;
    emit timeout();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 4);

    // Since there are no items remaining the timer should not be started and dataStoreChanged() should be emitted
    QVERIFY(!qTimerStarted);
    QCOMPARE(spy.count(), 1);
}

void Ut_LauncherDataStore::testProcessUpdateQueueRestartsIfRequested()
{
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    LauncherDataStore dataStore(mockStore);
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    emit directoryChanged();

    qTimerStarted = false;
    emit timeout();

    // The processing should be finished but the timer should be restarted since a new update has been requested
    QVERIFY(qTimerStarted);
}

void Ut_LauncherDataStore::testProcessUpdateQueueRemovesDeletedFilesWhenDone()
{
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");
    addDesktopEntry("testApplication3.desktop", "Test3", "Application", "Icon-camera", "test3");
    addDesktopEntry("testApplication4.desktop", "Test4", "Application", "Icon-camera", "test4");
    LauncherDataStore dataStore(mockStore);
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();
    emit timeout();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 4);

    // Updating the desktop entry directory should be reflected in the data
    removeDesktopEntries();
    addDesktopEntry("testApplication.desktop", "Test", "Application", "Icon-camera", "test");
    emit directoryChanged();
    emit timeout();
    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QCOMPARE(data.contains(fileNameWithPath("testApplication.desktop")), true);
}

void Ut_LauncherDataStore::testProcessUpdateQueueFiltersDesktopFiles()
{
    // Test applications: two valid and one invalid
    addDesktopEntry("onlyShowInMeeGoApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    addDesktopEntry("onlyShowInInvalidApplication.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("notShowInMeeGoApplication.desktop", "Test2", "Application", "Icon-camera", "test2");
    addDesktopEntry("invalidApplication.desktop", "Test3", "Invalid", "Icon-camera", "test3");
    addDesktopEntry("regularApplication.desktop", "Test4", "Application", "Icon-camera", "test4");
    desktopEntryOnlyShowIn.insert(fileNameWithPath("onlyShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");
    desktopEntryOnlyShowIn.insert(fileNameWithPath("onlyShowInInvalidApplication.desktop"), QStringList() << "X-Invalid");
    desktopEntryNotShowIn.insert(fileNameWithPath("notShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");

    LauncherDataStore dataStore(mockStore);
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();
    emit timeout();

    // The data store should contain two entries and the default value should be empty
    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 2);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(data.contains(fileNameWithPath("invalidApplication.desktop")), false);
    QCOMPARE(data.contains(fileNameWithPath("onlyShowInMeeGoApplication.desktop")), true);
    QCOMPARE(data.value(fileNameWithPath("regularApplication.desktop")), QVariant());
    QCOMPARE(data.value(fileNameWithPath("onlyShowInMeeGoApplication.desktop")), QVariant());
}

void Ut_LauncherDataStore::testUpdatingDataForDesktopEntry()
{
    // Test application
    addDesktopEntry("regularApplication.desktop", "Test0", "Application", "Icon-camera", "test0");

    LauncherDataStore dataStore(mockStore);
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QSignalSpy spy(&dataStore, SIGNAL(dataStoreChanged()));

    // Updating an inexisting entry should fail
    QCOMPARE(dataStore.updateDataForDesktopEntry("/dev/null", "test"), false);
    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(spy.count(), 0);

    // Updating an existing entry should succeed but not send any signals
    QCOMPARE(dataStore.updateDataForDesktopEntry(fileNameWithPath("regularApplication.desktop"), "test"), true);
    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(data.value(fileNameWithPath("regularApplication.desktop")), QVariant("test"));
    QCOMPARE(spy.count(), 0);

    // Updating the data store directly should reflect in changed data
    mockStore->setValue("DesktopEntries" + fileNameWithPath("regularApplication.desktop"), QVariant("modifiedTest"));
    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(data.value(fileNameWithPath("regularApplication.desktop")), QVariant("modifiedTest"));
    QCOMPARE(spy.count(), 1);
}

void Ut_LauncherDataStore::testOnlyPrefixedKeys()
{
    // Test applications
    mockStore->createValue("DesktopEntries" + fileNameWithPath("regularApplication.desktop"), QVariant("data0"));
    mockStore->createValue("OtherPrefix" + fileNameWithPath("otherApplication1.desktop"), QVariant("data1"));
    mockStore->createValue(fileNameWithPath("otherApplication2.desktop"), QVariant("data2"));

    LauncherDataStore dataStore(mockStore);

    // Launcher data store should only list entries prefixed with the
    // "DesktopEntries" prefix and ignore all others
    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(data.value(fileNameWithPath("regularApplication.desktop")), QVariant("data0"));
}

void Ut_LauncherDataStore::testAddingWatcherDesktopEntryPaths()
{
    // Test applications
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");

    LauncherDataStore dataStore(mockStore);
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));

    emit timeout();

    QCOMPARE(addedWatcherPathCalls.count(), 3);
    // APPLICATIONS_DIRECTORY is 1st path
    QCOMPARE(addedWatcherPathCalls.at(0), fileNameWithPath(""));
    QCOMPARE(addedWatcherPathCalls.at(1), fileNameWithPath("testApplication1.desktop"));
    QCOMPARE(addedWatcherPathCalls.at(2), fileNameWithPath("testApplication2.desktop"));

    addDesktopEntry("testApplication3.desktop", "Test3", "Application", "Icon-camera", "test3");

    emit directoryChanged();
    emit timeout();

    QCOMPARE(addedWatcherPathCalls.count(), 4);
    // APPLICATIONS_DIRECTORY is 1st path
    QCOMPARE(addedWatcherPathCalls.at(0), fileNameWithPath(""));
    QCOMPARE(addedWatcherPathCalls.at(1), fileNameWithPath("testApplication1.desktop"));
    QCOMPARE(addedWatcherPathCalls.at(2), fileNameWithPath("testApplication2.desktop"));
    QCOMPARE(addedWatcherPathCalls.at(3), fileNameWithPath("testApplication3.desktop"));
}

QTEST_APPLESS_MAIN(Ut_LauncherDataStore)
