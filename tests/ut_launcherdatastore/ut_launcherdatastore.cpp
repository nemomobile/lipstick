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
QMap<QString, QFileInfoList> desktopFileInfoList;
QFileInfoList QDir::entryInfoList(Filters, SortFlags) const
{
    return desktopFileInfoList.value(path());
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    bool contains = false;
    foreach (const QFileInfoList &fileInfoList, desktopFileInfoList.values()) {
        foreach (const QFileInfo &file, fileInfoList) {
            if (file.absoluteFilePath() == absoluteFilePath()) {
                contains = true;
                break;
            }
        }
    }
    return contains;
}

// QFile stubs
bool QFile::exists(const QString &file)
{
    bool contains = false;
    foreach (const QFileInfoList &fileInfoList, desktopFileInfoList.values()) {
        foreach (const QFileInfo &fileInfo, fileInfoList) {
            if (fileInfo.absoluteFilePath() == file) {
                contains = true;
                break;
            }
        }
    }
    return contains;
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

QString fileNameWithPath(const QString &fileName, const QString &directoryName = QString(APPLICATIONS_DIRECTORY))
{
    return directoryName + fileName;
}

void addDesktopEntry(const QString &fileName, const QString &name, const QString &type, const QString &icon, const QString &exec, const QString &directoryName = QString(APPLICATIONS_DIRECTORY))
{
    QString fullFileName = fileNameWithPath(fileName, directoryName);
    desktopEntryName.insert(fullFileName, name);
    desktopEntryType.insert(fullFileName, type);
    desktopEntryIcon.insert(fullFileName, icon);
    desktopEntryExec.insert(fullFileName, exec);
    desktopFileInfoList[QFileInfo(directoryName).path()].append(fullFileName);
}

void removeDesktopEntries()
{
    desktopEntryName.clear();
    desktopEntryType.clear();
    desktopEntryIcon.clear();
    desktopEntryExec.clear();
    desktopFileInfoList.clear();
}

void Ut_LauncherDataStore::testUpdateStartedDuringInitialization()
{
    addDesktopEntry("testApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    QVERIFY(qTimerStarted);
}

void Ut_LauncherDataStore::testUpdateStartedWhenApplicationsDirectoryChanges()
{
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    addDesktopEntry("testApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    emit directoryChanged();
    QVERIFY(qTimerStarted);
}

void Ut_LauncherDataStore::testUpdateNotStartedWhenInProgress()
{
    addDesktopEntry("testApplication.desktop", "Test0", "Application", "Icon-camera", "test0");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    qTimerStarted = false;

    // Requesting an update while one is in progress should not restart it
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    emit directoryChanged();
    QCOMPARE(qTimerStarted, false);
}

void Ut_LauncherDataStore::testProcessUpdateQueueDoesNothingWhenQueueEmpty()
{
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
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
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    QSignalSpy spyDesktopEntryAdded(&dataStore, SIGNAL(desktopEntryAdded(QString)));
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    // The amount of files processed (N) is currently 3
    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 3);
    // Verify that we have had an added signal for every processed file
    QCOMPARE(spyDesktopEntryAdded.count(), 3);

    // Since there are items remaining the timer should be started and dataStoreChanged() should not be emitted
    QVERIFY(qTimerStarted);
    QCOMPARE(spyDataStoreChanged.count(), 0);
}

void Ut_LauncherDataStore::testProcessUpdateQueueFinishesProcessingCorrectly()
{
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");
    addDesktopEntry("testApplication3.desktop", "Test3", "Application", "Icon-camera", "test3");
    addDesktopEntry("testApplication4.desktop", "Test4", "Application", "Icon-camera", "test4");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    QSignalSpy spyDesktopEntryAdded(&dataStore, SIGNAL(desktopEntryAdded(QString)));
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    // The second batch should finish the job
    qTimerStarted = false;
    emit timeout();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 4);
    QCOMPARE(spyDesktopEntryAdded.count(), 4);
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication1.desktop")));
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication2.desktop")));
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication3.desktop")));
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication4.desktop")));

    // Since there are no items remaining the timer should not be started and dataStoreChanged() should be emitted
    QVERIFY(!qTimerStarted);
    QCOMPARE(spyDataStoreChanged.count(), 1);
}

void Ut_LauncherDataStore::testProcessUpdateQueueRestartsIfRequested()
{
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
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
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    QSignalSpy spyDesktopEntryRemoved(&dataStore, SIGNAL(desktopEntryRemoved(QString)));
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
    QCOMPARE(spyDesktopEntryRemoved.count(), 4);
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication1.desktop")));
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication2.desktop")));
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication3.desktop")));
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication4.desktop")));
}

void Ut_LauncherDataStore::testProcessUpdateQueueFiltersDesktopFiles()
{
    // Add valid entries:
    addDesktopEntry("regularApplication.desktop", "Test4", "Application", "Icon-camera", "test4");
    addDesktopEntry("onlyShowInMeeGoApplication.desktop", "Test0", "Link", "Icon-camera", "test0");
    desktopEntryOnlyShowIn.insert(fileNameWithPath("onlyShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");

    // Add invalid entries:
    addDesktopEntry("onlyShowInInvalidApplication.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("notShowInMeeGoApplication.desktop", "Test2", "Application", "Icon-camera", "test2");
    addDesktopEntry("invalidApplication.desktop", "Test3", "Invalid", "Icon-camera", "test3");
    desktopEntryOnlyShowIn.insert(fileNameWithPath("onlyShowInInvalidApplication.desktop"), QStringList() << "X-Invalid");
    desktopEntryNotShowIn.insert(fileNameWithPath("notShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");

    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    QSignalSpy spyDesktopEntryAdded(&dataStore, SIGNAL(desktopEntryAdded(QString)));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();
    emit timeout();

    QCOMPARE(spyDesktopEntryAdded.count(), 2);
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).toString(), QString(fileNameWithPath("regularApplication.desktop")));
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).toString(), QString(fileNameWithPath("onlyShowInMeeGoApplication.desktop")));
    QCOMPARE(dataStore.invalidEntries.count(), 3);

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

    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QSignalSpy spy(&dataStore, SIGNAL(dataStoreChanged()));

    // Updating an inexisting entry should add a new value but not send any signals
    dataStore.updateDataForDesktopEntry("/dev/null", "test");
    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 2);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(spy.count(), 0);

    // Updating an existing entry should succeed but not add new value or send any signals
    dataStore.updateDataForDesktopEntry(fileNameWithPath("regularApplication.desktop"), "test");
    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 2);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(data.value(fileNameWithPath("regularApplication.desktop")), QVariant("test"));
    QCOMPARE(spy.count(), 0);

    // Updating the data store directly should reflect in changed data
    mockStore->setValue("DesktopEntries" + fileNameWithPath("regularApplication.desktop"), QVariant("modifiedTest"));
    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 2);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(data.value(fileNameWithPath("regularApplication.desktop")), QVariant("modifiedTest"));
    QCOMPARE(spy.count(), 1);
}

void Ut_LauncherDataStore::testRemovingDataForDesktopEntry()
{
    addDesktopEntry("test.desktop", "Test0", "Application", "Icon-camera", "test0");

    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();
    QSignalSpy spy(&dataStore, SIGNAL(dataStoreChanged()));

    dataStore.removeDataForDesktopEntry(fileNameWithPath("test.desktop"));
    QCOMPARE(dataStore.dataForAllDesktopEntries().count(), 0);
    QCOMPARE(spy.count(), 0);
}

void Ut_LauncherDataStore::testOnlyPrefixedKeys()
{
    // Test applications
    mockStore->createValue("DesktopEntries" + fileNameWithPath("regularApplication.desktop"), QVariant("data0"));
    mockStore->createValue("OtherPrefix" + fileNameWithPath("otherApplication1.desktop"), QVariant("data1"));
    mockStore->createValue(fileNameWithPath("otherApplication2.desktop"), QVariant("data2"));

    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);

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

    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY << "/test/directory/");
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));

    emit timeout();

    QCOMPARE(addedWatcherPathCalls.count(), 4);
    // The directories are added first
    QCOMPARE(addedWatcherPathCalls.at(0), fileNameWithPath(""));
    QCOMPARE(addedWatcherPathCalls.at(1), fileNameWithPath("", "/test/directory/"));
    QCOMPARE(addedWatcherPathCalls.at(2), fileNameWithPath("testApplication1.desktop"));
    QCOMPARE(addedWatcherPathCalls.at(3), fileNameWithPath("testApplication2.desktop"));

    addDesktopEntry("testApplication3.desktop", "Test3", "Application", "Icon-camera", "test3", "/test/directory/");

    emit directoryChanged();
    emit timeout();

    QCOMPARE(addedWatcherPathCalls.count(), 5);
    QCOMPARE(addedWatcherPathCalls.at(0), fileNameWithPath(""));
    QCOMPARE(addedWatcherPathCalls.at(1), fileNameWithPath("", "/test/directory/"));
    QCOMPARE(addedWatcherPathCalls.at(2), fileNameWithPath("testApplication1.desktop"));
    QCOMPARE(addedWatcherPathCalls.at(3), fileNameWithPath("testApplication2.desktop"));
    QCOMPARE(addedWatcherPathCalls.at(4), fileNameWithPath("testApplication3.desktop", "/test/directory/"));
}

void Ut_LauncherDataStore::testUpdatingDesktopEntry()
{
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit directoryChanged();
    emit timeout();

    QSignalSpy spyDesktopEntryChanged(&dataStore, SIGNAL(desktopEntryChanged(QString)));
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));
    connect(this, SIGNAL(fileChanged(QString)), &dataStore, SLOT(updateDesktopEntry(QString)));
    emit fileChanged(fileNameWithPath("testApplication1.desktop"));
    // No data store change when entry is just updated
    QCOMPARE(spyDataStoreChanged.count(), 0);
    QCOMPARE(spyDesktopEntryChanged.count(), 1);
    QList<QVariant> arguments = spyDesktopEntryChanged.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString(fileNameWithPath("testApplication1.desktop")));
}

void Ut_LauncherDataStore::testUpdatingInvalidEntry()
{
    addDesktopEntry("invalidAndValidApplication.desktop", "Test3", "invalid", "Icon-camera", "test3");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    connect(this, SIGNAL(fileChanged(QString)), &dataStore, SLOT(updateDesktopEntry(QString)));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    // connect signals only after entries have been added first time
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));
    QSignalSpy spyDesktopEntryAdded(&dataStore, SIGNAL(desktopEntryAdded(QString)));
    QSignalSpy spyDesktopEntryRemoved(&dataStore, SIGNAL(desktopEntryRemoved(QString)));

    // change to valid and send changed
    desktopEntryType.insert(fileNameWithPath("invalidAndValidApplication.desktop"), QString("Application"));
    emit fileChanged(fileNameWithPath("invalidAndValidApplication.desktop"));

    QCOMPARE(spyDesktopEntryAdded.count(), 1);
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).toString(), QString(fileNameWithPath("invalidAndValidApplication.desktop")));
    QCOMPARE(spyDataStoreChanged.count(), 1);
    QVERIFY(!dataStore.invalidEntries.contains(fileNameWithPath("invalidAndValidApplication.desktop")));

    // change back to invalid and send changed
    desktopEntryType.insert(fileNameWithPath("invalidAndValidApplication.desktop"), QString("invalid_again"));
    emit fileChanged(fileNameWithPath("invalidAndValidApplication.desktop"));

    QCOMPARE(spyDesktopEntryRemoved.count(), 1);
    QCOMPARE(spyDataStoreChanged.count(), 2);
    QVERIFY(dataStore.invalidEntries.contains(fileNameWithPath("invalidAndValidApplication.desktop")));
}

void Ut_LauncherDataStore::testRemovingEntriesWhenApplicationsDirectoryGetsEmpty()
{
    // Add desktop entries
    addDesktopEntry("testApplication1.desktop", "Test1", "Application", "Icon-camera", "test1");
    addDesktopEntry("testApplication2.desktop", "Test2", "Application", "Icon-camera", "test2");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    connect(this, SIGNAL(directoryChanged()), &dataStore, SLOT(updateDataFromDesktopEntryFiles()));
    connect(this, SIGNAL(timeout()), &dataStore, SLOT(processUpdateQueue()));
    emit timeout();

    QSignalSpy spyDesktopEntryRemoved(&dataStore, SIGNAL(desktopEntryRemoved(QString)));
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));

    removeDesktopEntries();
    emit directoryChanged();
    emit timeout();

    QCOMPARE(spyDesktopEntryRemoved.count(), 2);
    QCOMPARE(spyDataStoreChanged.count(), 1);
}

void Ut_LauncherDataStore::testNotReprocessingInvalidEntry()
{
    addDesktopEntry("notShowInMeeGoApplication.desktop", "Test2", "Application", "Icon-camera", "test2");
    desktopEntryNotShowIn.insert(fileNameWithPath("notShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    dataStore.processUpdateQueue();
    QVERIFY(dataStore.invalidEntries.contains(fileNameWithPath("notShowInMeeGoApplication.desktop")));
    desktopEntryFileName.clear();

    dataStore.processUpdateQueue();

    // Entry should not be found from list of created MDesktopEntry's
    QVERIFY(!desktopEntryFileName.key(fileNameWithPath("notShowInMeeGoApplication.desktop"), NULL));
}

QTEST_APPLESS_MAIN(Ut_LauncherDataStore)
