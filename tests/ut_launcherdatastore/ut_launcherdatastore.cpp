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
#include "homefiledatastore_stub.h"

Q_DECLARE_METATYPE(QSharedPointer<MDesktopEntry>)

static const QString DESKTOP_ENTRY_TYPE_KEY = "Desktop Entry/Type";
static const QString DESKTOP_ONLY_SHOW_IN_KEY = "Desktop Entry/OnlyShowIn";
static const QString DESKTOP_NOT_SHOW_IN_KEY = "Desktop Entry/NotShowIn";

QMap<QString, QMap<QString, QString> > desktopEntryDataMap;

QMap<const MDesktopEntry *, QString> desktopEntryFileName;
QMap<QString, QStringList> desktopEntryOnlyShowIn;
QMap<QString, QStringList> desktopEntryNotShowIn;
QMap<QString, QString> desktopEntryType;
QSet<QString> desktopEntryHasInvalidNotation;

QStringList addedWatcherPathCalls;

bool MDesktopEntry::readDesktopFile(QIODevice &device, QMap<QString, QString> &desktopEntriesMap)
{
    QString filePath = QFileInfo(*dynamic_cast<QFile *>(&device)).absoluteFilePath();
    desktopEntriesMap.insert(DESKTOP_ENTRY_TYPE_KEY, desktopEntryType.value(filePath));
    if (desktopEntryOnlyShowIn.contains(filePath)) {
        desktopEntriesMap.insert(DESKTOP_ONLY_SHOW_IN_KEY, desktopEntryOnlyShowIn.value(filePath).join(""));
    }
    if (desktopEntryNotShowIn.contains(filePath)) {
        desktopEntriesMap.insert(DESKTOP_NOT_SHOW_IN_KEY, desktopEntryNotShowIn.value(filePath).join(""));
    }
    return !desktopEntryHasInvalidNotation.contains(filePath);
}

// QDir stubs
QMap<QString, QFileInfoList> desktopFileInfoList;
QFileInfoList QDir::entryInfoList(Filters, SortFlags) const
{
    return desktopFileInfoList.value(path());
}

// QFileInfo stubs
QStringList qFileInfoDirs;
bool QFileInfo::exists() const
{
    bool contains = qFileInfoDirs.contains(canonicalFilePath());

    if (!contains) {
        foreach (const QFileInfoList &fileInfoList, desktopFileInfoList.values()) {
            foreach (const QFileInfo &file, fileInfoList) {
                if (file.absoluteFilePath() == absoluteFilePath()) {
                    contains = true;
                    break;
                }
            }
        }
    }

    return contains;
}

bool QFileInfo::isDir() const
{
    return qFileInfoDirs.contains(canonicalFilePath());
}

QString QFileInfo::canonicalPath() const
{
    QString path = absolutePath();
    while(path.endsWith('/')) {
        path.chop(1);
    }
    return path;
}


QString QFileInfo::canonicalFilePath() const
{
    QString path = absoluteFilePath();
    while(path.endsWith('/')) {
        path.chop(1);
    }
    return path;
}

// QFile stubs
bool QFile::exists() const
{
    bool contains = false;
    foreach (const QFileInfoList &fileInfoList, desktopFileInfoList.values()) {
        foreach (const QFileInfo &fileInfo, fileInfoList) {
            if (fileInfo.absoluteFilePath() == QFileInfo(*this).absoluteFilePath()) {
                contains = true;
                break;
            }
        }
    }
    return contains;
}

bool QFile::open(OpenMode flags)
{
    // Return value needs to be stubbed but opening must still work for MApplication style files
    // (QFile::exists() is a stubbed method)
    bool opened = exists();
    if (!opened) {
        if (fileEngine()->open(flags)) {
            QIODevice::open(flags);
            opened = true;
        }
    }
    return opened;
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

void Ut_LauncherDataStore::initTestCase()
{
    qRegisterMetaType<QSharedPointer<MDesktopEntry> >();

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
    desktopEntryFileName.clear();
    desktopEntryOnlyShowIn.clear();
    desktopEntryNotShowIn.clear();
    desktopEntryType.clear();
    desktopEntryHasInvalidNotation.clear();
    desktopFileInfoList.clear();
    addedWatcherPathCalls.clear();
    qFileInfoDirs.clear();
    qFileInfoDirs.append(QFileInfo(APPLICATIONS_DIRECTORY).canonicalFilePath());
}

void Ut_LauncherDataStore::cleanup()
{    
}

QString fileNameWithPath(const QString &fileName, const QString &directoryName = QString(APPLICATIONS_DIRECTORY))
{
    return directoryName + fileName;
}

void addDesktopEntry(const QString &fileName, const QString &type = QString("Application"), const QString &directoryName = QString(APPLICATIONS_DIRECTORY))
{
    QString fullFileName = fileNameWithPath(fileName, directoryName);
    desktopEntryType.insert(fullFileName, type);
    desktopFileInfoList[QFileInfo(directoryName).path()].append(fullFileName);
}

void removeDesktopEntries()
{
    desktopEntryType.clear();
    desktopEntryOnlyShowIn.clear();
    desktopEntryNotShowIn.clear();
    desktopFileInfoList.clear();
    desktopEntryHasInvalidNotation.clear();
}

void Ut_LauncherDataStore::testAddingDesktopEntryFiles()
{
    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY);

    QStringList addedEntries;
    for (int i = 0; i < 4; i++) {
        QString entry = QString("testApplication%1.desktop").arg(i);
        addedEntries.append(fileNameWithPath(entry));
        addDesktopEntry(entry, "Application");
    }

    QSignalSpy spyDesktopEntryAdded(&dataStore, SIGNAL(desktopEntryAdded(QSharedPointer<MDesktopEntry>)));
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));

    dataStore.updateDesktopEntryFiles();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 4);
    QCOMPARE(spyDesktopEntryAdded.count(), 4);
    while (spyDesktopEntryAdded.count() > 0) {
        QString expectedEntry = spyDesktopEntryAdded.takeFirst().at(0).value<QSharedPointer<MDesktopEntry> >()->fileName();
        QCOMPARE(addedEntries.removeOne(expectedEntry), true);
    }
}

void Ut_LauncherDataStore::testRemovingDesktopEntryFiles()
{
    addDesktopEntry("testApplication1.desktop", "Application");
    addDesktopEntry("testApplication2.desktop", "Application");
    addDesktopEntry("testApplication3.desktop", "Application");
    addDesktopEntry("testApplication4.desktop", "Application");
    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY);
    QSignalSpy spyDesktopEntryRemoved(&dataStore, SIGNAL(desktopEntryRemoved(QString)));

    dataStore.updateDesktopEntryFiles();

    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 4);

    // Updating the desktop entry directory should be reflected in the data
    removeDesktopEntries();
    addDesktopEntry("testApplication.desktop", "Application");

    dataStore.updateDesktopEntryFiles();

    data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 1);
    QCOMPARE(data.contains(fileNameWithPath("testApplication.desktop")), true);
    QCOMPARE(spyDesktopEntryRemoved.count(), 4);
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication1.desktop")));
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication2.desktop")));
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication3.desktop")));
    QCOMPARE(spyDesktopEntryRemoved.takeFirst().at(0).toString(), QString(fileNameWithPath("testApplication4.desktop")));
}

void Ut_LauncherDataStore::testUpdatingDesktopFilesFiltersDesktopEntries()
{
    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY);

    // Add valid entries:
    QStringList expectedEntries;
    QString regularEntry("regularApplication.desktop");
    expectedEntries.append(fileNameWithPath(regularEntry));
    addDesktopEntry(regularEntry, "Application");

    QString meegoApp("onlyShowInMeeGoApplication.desktop");
    expectedEntries.append(fileNameWithPath(meegoApp));
    addDesktopEntry(meegoApp, "Link");
    desktopEntryOnlyShowIn.insert(fileNameWithPath("onlyShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");

    // Add invalid entries:
    addDesktopEntry("onlyShowInInvalidApplication.desktop", "Application");
    addDesktopEntry("notShowInMeeGoApplication.desktop", "Application");
    addDesktopEntry("invalidApplication.desktop", "Invalid");
    desktopEntryOnlyShowIn.insert(fileNameWithPath("onlyShowInInvalidApplication.desktop"), QStringList() << "X-Invalid");
    desktopEntryNotShowIn.insert(fileNameWithPath("notShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");

    QSignalSpy spyDesktopEntryAdded(&dataStore, SIGNAL(desktopEntryAdded(QSharedPointer<MDesktopEntry>)));
    dataStore.updateDesktopEntryFiles();

    QCOMPARE(spyDesktopEntryAdded.count(), 2);
    while (spyDesktopEntryAdded.count()) {
        QVERIFY(expectedEntries.removeOne(spyDesktopEntryAdded.takeFirst().at(0).value<QSharedPointer<MDesktopEntry> >()->fileName()));
    }
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("regularApplication.desktop")), false);
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("onlyShowInMeeGoApplication.desktop")), false);
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("onlyShowInInvalidApplication.desktop")), true);
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("notShowInMeeGoApplication.desktop")), true);
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("invalidApplication.desktop")), true);
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("unknownApplication.desktop")), false);

    // The data store should contain two entries and the default value should be empty
    QHash<QString, QVariant> data = dataStore.dataForAllDesktopEntries();
    QCOMPARE(data.count(), 2);
    QCOMPARE(data.contains(fileNameWithPath("regularApplication.desktop")), true);
    QCOMPARE(data.contains(fileNameWithPath("invalidApplication.desktop")), false);
    QCOMPARE(data.contains(fileNameWithPath("onlyShowInMeeGoApplication.desktop")), true);
    QCOMPARE(data.value(fileNameWithPath("onlyShowInMeeGoApplication.desktop")), QVariant(QString()));
    QCOMPARE(data.value(fileNameWithPath("regularApplication.desktop")), QVariant(QString()));
}

void Ut_LauncherDataStore::testUpdatingDataForDesktopEntry()
{
    // Test application
    addDesktopEntry("regularApplication.desktop", "Application");
    MockDataStore *mockStore = new MockDataStore;
    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    dataStore.updateDesktopEntryFiles();

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
    addDesktopEntry("test.desktop", "Application");

    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY);
    dataStore.updateDesktopEntryFiles();
    QSignalSpy spy(&dataStore, SIGNAL(dataStoreChanged()));

    dataStore.removeDataForDesktopEntry(fileNameWithPath("test.desktop"));
    QCOMPARE(dataStore.dataForAllDesktopEntries().count(), 0);
    QCOMPARE(spy.count(), 0);
}

void Ut_LauncherDataStore::testRemovingNonPrefixedKeys()
{
    MockDataStore *mockStore = new MockDataStore;

    // Test applications
    QString regularDesktopEntry(fileNameWithPath("regularApplication.desktop"));
    mockStore->createValue("DesktopEntries" + regularDesktopEntry, QVariant("data0"));
    desktopFileInfoList[QFileInfo(QString(APPLICATIONS_DIRECTORY)).path()].append(regularDesktopEntry);
    QString otherApplication1(fileNameWithPath("otherApplication1.desktop"));
    mockStore->createValue("OtherPrefix" + fileNameWithPath("otherApplication1.desktop"), QVariant("data1"));
    desktopFileInfoList[QFileInfo(QString(APPLICATIONS_DIRECTORY)).path()].append(otherApplication1);
    QString otherApplication2(fileNameWithPath("otherApplication2.desktop"));
    mockStore->createValue(fileNameWithPath("otherApplication2.desktop"), QVariant("data2"));
    desktopFileInfoList[QFileInfo(QString(APPLICATIONS_DIRECTORY)).path()].append(otherApplication2);

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

    QString dirPath("/test/directory1");
    qFileInfoDirs.append(dirPath);

    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY << dirPath << "/test/directory2/");

    // Test applications
    QStringList expectedEntryPaths;
    for (int i = 0; i < 2; i++) {
        QString entry = QString("testApplication%1.desktop").arg(i);
        expectedEntryPaths.append(fileNameWithPath(entry));
        addDesktopEntry(entry, "Application");
    }

    dataStore.updateDesktopEntryFiles();

    QCOMPARE(addedWatcherPathCalls.count(), 4);

    // The directories are added first
    QCOMPARE(addedWatcherPathCalls.at(0), fileNameWithPath("", QFileInfo(APPLICATIONS_DIRECTORY).canonicalFilePath()));
    QCOMPARE(addedWatcherPathCalls.at(1), fileNameWithPath("", "/test/directory1"));
    foreach (QString expectedPath, expectedEntryPaths) {
        QVERIFY(addedWatcherPathCalls.contains(expectedPath));
    }

    addDesktopEntry("testApplication3.desktop", "Application", "/test/directory1/");

    dataStore.updateDesktopEntryFiles();

    QCOMPARE(addedWatcherPathCalls.count(), 5);
    QCOMPARE(addedWatcherPathCalls.at(4), fileNameWithPath("testApplication3.desktop", "/test/directory1/"));
}

void Ut_LauncherDataStore::testUpdatingDesktopEntry()
{
    qFileInfoDirs.append("/test/directory1");
    qFileInfoDirs.append("/test/directory2");

    addDesktopEntry("testApplication0.desktop", "Application");
    addDesktopEntry("testApplication1.desktop", "Application", "/test/directory1/");
    addDesktopEntry("testApplication2.desktop", "Application", "/test/directory2/");
    addDesktopEntry("testApplication3.desktop", "Application", "/test/directory2/");
    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY << "/test/directory1" << "/test/directory2/");
    dataStore.updateDesktopEntryFiles();

    QSignalSpy spyDesktopEntryChanged(&dataStore, SIGNAL(desktopEntryChanged(QSharedPointer<MDesktopEntry>)));
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));
    dataStore.updateDesktopEntry(fileNameWithPath("testApplication0.desktop"));
    dataStore.updateDesktopEntry(fileNameWithPath("testApplication1.desktop", "/test/directory1/"));
    dataStore.updateDesktopEntry(fileNameWithPath("testApplication2.desktop", "/test/directory2/"));
    dataStore.updateDesktopEntry("testApplication3.desktop");
    dataStore.updateDesktopEntry("testApplication4.desktop");

    // No data store change when entry is just updated
    QCOMPARE(spyDataStoreChanged.count(), 0);
    QCOMPARE(spyDesktopEntryChanged.count(), 4);
    QCOMPARE(spyDesktopEntryChanged.at(0).at(0).value<QSharedPointer<MDesktopEntry> >()->fileName(), QString(fileNameWithPath("testApplication0.desktop")));
    QCOMPARE(spyDesktopEntryChanged.at(1).at(0).value<QSharedPointer<MDesktopEntry> >()->fileName(), QString(fileNameWithPath("testApplication1.desktop", "/test/directory1/")));
    QCOMPARE(spyDesktopEntryChanged.at(2).at(0).value<QSharedPointer<MDesktopEntry> >()->fileName(), QString(fileNameWithPath("testApplication2.desktop", "/test/directory2/")));
    QCOMPARE(spyDesktopEntryChanged.at(3).at(0).value<QSharedPointer<MDesktopEntry> >()->fileName(), QString(fileNameWithPath("testApplication3.desktop", "/test/directory2/")));
}

void Ut_LauncherDataStore::testUpdatingInvalidEntry()
{
    addDesktopEntry("invalidAndValidApplication.desktop", "invalid");
    addDesktopEntry("testApplication2.desktop", "Application");
    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY);

    dataStore.updateDesktopEntryFiles();

    // connect signals only after entries have been added first time
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));
    QSignalSpy spyDesktopEntryAdded(&dataStore, SIGNAL(desktopEntryAdded(QSharedPointer<MDesktopEntry>)));
    QSignalSpy spyDesktopEntryRemoved(&dataStore, SIGNAL(desktopEntryRemoved(QString)));

    // change to valid and send changed
    desktopEntryType.insert(fileNameWithPath("invalidAndValidApplication.desktop"), QString("Application"));
    dataStore.updateDesktopEntry(fileNameWithPath("invalidAndValidApplication.desktop"));

    QCOMPARE(spyDesktopEntryAdded.count(), 1);
    QCOMPARE(spyDesktopEntryAdded.takeFirst().at(0).value<QSharedPointer<MDesktopEntry> >()->fileName(), QString(fileNameWithPath("invalidAndValidApplication.desktop")));
    QCOMPARE(spyDataStoreChanged.count(), 1);
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("invalidAndValidApplication.desktop")), false);

    // change back to invalid and send changed
    desktopEntryType.insert(fileNameWithPath("invalidAndValidApplication.desktop"), QString("invalid_again"));
    dataStore.updateDesktopEntry(fileNameWithPath("invalidAndValidApplication.desktop"));

    QCOMPARE(spyDesktopEntryRemoved.count(), 1);
    QCOMPARE(spyDataStoreChanged.count(), 2);
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("invalidAndValidApplication.desktop")), true);
}

void Ut_LauncherDataStore::testRemovingEntriesWhenApplicationsDirectoryGetsEmpty()
{
    // Add desktop entries
    addDesktopEntry("testApplication1.desktop", "Application");
    addDesktopEntry("testApplication2.desktop", "Application");
    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY);
    dataStore.updateDesktopEntryFiles();

    QSignalSpy spyDesktopEntryRemoved(&dataStore, SIGNAL(desktopEntryRemoved(QString)));
    QSignalSpy spyDataStoreChanged(&dataStore, SIGNAL(dataStoreChanged()));

    removeDesktopEntries();
    dataStore.updateDesktopEntryFiles();

    QCOMPARE(spyDesktopEntryRemoved.count(), 2);
    QCOMPARE(spyDataStoreChanged.count(), 1);
}

void Ut_LauncherDataStore::testNotReprocessingInvalidEntry()
{
    addDesktopEntry("notShowInMeeGoApplication.desktop", "Application");
    desktopEntryNotShowIn.insert(fileNameWithPath("notShowInMeeGoApplication.desktop"), QStringList() << "X-MeeGo");
    LauncherDataStore dataStore(new MockDataStore, QStringList() << APPLICATIONS_DIRECTORY);
    dataStore.updateDesktopEntryFiles();
    QCOMPARE(dataStore.isDesktopEntryKnownToBeInvalid(fileNameWithPath("notShowInMeeGoApplication.desktop")), true);
    desktopEntryFileName.clear();

    dataStore.updateDesktopEntryFiles();

    // Entry should not be found from list of created MDesktopEntry's
    QVERIFY(!desktopEntryFileName.key(fileNameWithPath("notShowInMeeGoApplication.desktop"), NULL));
}

void Ut_LauncherDataStore::testUpdatingMultipleEntries()
{
    LauncherDataStore dataStore(new HomeFileDataStore("path"), QStringList() << APPLICATIONS_DIRECTORY);

    // Add one existing value that should be updated too
    addDesktopEntry("testApplication1.desktop", "Application");

    QHash<QString, QString> updatedData;
    const int AMOUNT_OF_BUTTONS = 3;
    for(int i = 0; i < AMOUNT_OF_BUTTONS; i++) {
        updatedData.insert(QString("testApplication%1.desktop").arg(i), QString("launcher/0/%1").arg(i));
    }
    dataStore.updateDataForDesktopEntries(updatedData);

    QHash<QString, QVariant> storedEntries = gHomeFileDataStoreStub->stubLastCallTo("createValues").parameter<QHash<QString, QVariant> >(0);
    QCOMPARE(storedEntries.count(), 3);
    foreach (const QString &key, storedEntries.keys()) {
        QString entryPath = LauncherDataStore::keyToEntryPath(key);
        QCOMPARE(storedEntries.value(key).toString(), updatedData.value(entryPath));
    }
}

void Ut_LauncherDataStore::testValidatingPreviouslyAddedDesktopEntiesOnStartUp()
{
    MockDataStore *mockStore = new MockDataStore;

    // Add two valid values to data store
    QString invalidDesktopEntryPath = fileNameWithPath("testApplication1.desktop", APPLICATIONS_DIRECTORY);
    desktopEntryHasInvalidNotation.insert(invalidDesktopEntryPath);
    QString validDesktopEntryPath = fileNameWithPath("testApplication2.desktop", APPLICATIONS_DIRECTORY);
    addDesktopEntry("testApplication1.desktop");
    addDesktopEntry("testApplication2.desktop");
    mockStore->createValue(LauncherDataStore::entryPathToKey(invalidDesktopEntryPath), QVariant("data1"));
    mockStore->createValue(LauncherDataStore::entryPathToKey(validDesktopEntryPath), QVariant("validData"));

    LauncherDataStore dataStore(mockStore, QStringList() << APPLICATIONS_DIRECTORY);
    dataStore.updateDesktopEntryFiles();

    // Verify that invalid entry was removed when datastore when desktop entries were processed
    QCOMPARE(mockStore->allKeys().count(), 1);
    QCOMPARE(mockStore->value(LauncherDataStore::entryPathToKey(invalidDesktopEntryPath)), QVariant());
    QCOMPARE(mockStore->value(LauncherDataStore::entryPathToKey(validDesktopEntryPath)).toString(), QString("validData"));
}

QTEST_APPLESS_MAIN(Ut_LauncherDataStore)
