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

#include <MApplication>
#include <MDesktopEntry>
#include "launcherdatastore.h"
#include "launcherpage.h"
#include "ut_launcherdatastore.h"

static const QString KEY_PREFIX = "DesktopEntries";

static QSharedPointer<LauncherButton> createLauncherButton(const QString path);

static void comparePageLocations(QList< QSharedPointer<LauncherPage> > pages);

static void compareEntryPageLocations(QSharedPointer<LauncherPage> pageToCompare, int page);

static void createSimpleTestData(QList< QSharedPointer<LauncherPage> > &pages);

static QSharedPointer<LauncherPage> createLauncherPage(const QStringList entries);

static QString pathFromKey(QString key);
static QString keyFromPath(QString path);

static QHash<QString, QVariant> testData;

static QString formatter("launcher/%1/%2");

bool TestDataStore::createValue (const QString &key, const QVariant &value)
{
    testData.insert(key, value);
    return true;
}

void TestDataStore::remove (const QString &key)
{
    testData.take(key);
}

void TestDataStore::clear()
{
    testData.clear();
}


QVariant TestDataStore::value(const QString& key) const
{
    return testData.value(key);
}

bool TestDataStore::setValue(const QString& key, const QVariant& value)
{
    bool ok = testData.contains(key);
    if (ok) {
	testData[key] = value;
    }
    return ok;
}

QStringList TestDataStore::allKeys() const
{
    const QStringList list(testData.keys());
    return list;
}

bool TestDataStore::contains(const QString& key) const
{
    return testData.contains(key);
}

// MDesktopEntry stubs
QList<QString> gValidDesktopFiles;
bool MDesktopEntry::isValid() const
{
    return gValidDesktopFiles.contains(fileName());
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
    testStore = new TestDataStore;
    m_subject = new LauncherDataStore(testStore);
    testData.clear();
    gValidDesktopFiles.clear();
}

void Ut_LauncherDataStore::cleanup()
{    
    delete m_subject;
}

void Ut_LauncherDataStore::testAddingLauncherItemsToAnEmptyStore()
{
    QCOMPARE(testData.count(), 0);
    QList< QSharedPointer<LauncherPage> > pages;
    createSimpleTestData(pages);

    m_subject->updateLauncherButtons(pages);

    QCOMPARE(testData.count(), 5);

    comparePageLocations(pages);
}

void Ut_LauncherDataStore::testAddingLauncherItemsToNonEmptyStore()
{
    QCOMPARE(testData.count(), 0);

    QStringList existingEntryList;
    existingEntryList << QString(KEY_PREFIX + "/path/old-entry.desktop") << QString(KEY_PREFIX + "/path/old-entry-1.desktop");
    existingEntryList << QString(KEY_PREFIX + "/path/old-entry-2.desktop")<< QString(KEY_PREFIX + "/path/old-entry-3.desktop");

    for (int i = 0; i < existingEntryList.count(); i++) {
        QString value(formatter.arg(i).arg(0));
	testData.insert(existingEntryList[i], value);
    }

    QCOMPARE(testData.count(), existingEntryList.count());

    QStringList pageOneEntries;
    pageOneEntries << QString("/path/new-entry.desktop") << QString("/path/new-entry-1.desktop");

    QStringList pageTwoEntries;
    pageTwoEntries << QString("/path/new-entry-2.desktop") << QString("/path/new-entry-3.desktop") << QString("/path/new-entry-4.desktop");

    QSharedPointer<LauncherPage> pageOne = createLauncherPage(pageOneEntries);
    QSharedPointer<LauncherPage> pageTwo = createLauncherPage(pageTwoEntries);

    QList< QSharedPointer<LauncherPage> > pages;
    pages.append(pageOne);
    pages.append(pageTwo);

    m_subject->updateLauncherButtons(pages);

    QCOMPARE(testData.count(), 5);

    comparePageLocations(pages);
}

void Ut_LauncherDataStore::testGettingLauncherButtonsFromAnEmptyDataStore()
{
    QList< QSharedPointer<LauncherPage> > pages(m_subject->launcherButtons());
    QCOMPARE(pages.count(), 0);
}

void Ut_LauncherDataStore::testGettingLauncherButtonsFromDataStore()
{
    QCOMPARE(testData.count(), 0);

    QStringList existingEntryList;
    existingEntryList << QString(KEY_PREFIX + "/path/old-entry.desktop") << QString(KEY_PREFIX + "/path/old-entry-1.desktop");
    existingEntryList << QString(KEY_PREFIX + "/path/old-entry-2.desktop")<< QString(KEY_PREFIX + "/path/old-entry-3.desktop");

    for (int i = 0; i < existingEntryList.count(); i++) {
        QString value(formatter.arg(i).arg(0));
	// Every button goes into its own page.
	testData.insert(existingEntryList[i], value);
    }

    //Right now we have four pages in the data store, lets add some more

    existingEntryList << QString(KEY_PREFIX + "/path/old-entry-4.desktop") << QString(KEY_PREFIX + "/path/old-entry-5.desktop");
    existingEntryList << QString(KEY_PREFIX + "/path/old-entry-6.desktop") << QString(KEY_PREFIX + "/path/old-entry-7.desktop");
    // First, second and third pages get some extra data
    testData.insert(existingEntryList[4], QString("launcher/0/1"));
    testData.insert(existingEntryList[5], QString("launcher/1/1"));
    testData.insert(existingEntryList[6], QString("launcher/3/1"));
    testData.insert(existingEntryList[7], QString("launcher/3/2"));

    // Check that the test data count is ok
    QCOMPARE(testData.count(), existingEntryList.count());

    // Now start checking that the result read from the data store is correct

    // We should have four pages in total
    QList< QSharedPointer<LauncherPage> > pages(m_subject->launcherButtons());
    QCOMPARE(pages.count(), 4);

    // the first page should have 2 items
    QSharedPointer<LauncherPage> page = pages.at(0);
    QList<QSharedPointer<LauncherButton> > pageButtons = page.data()->model()->launcherButtons();
    QCOMPARE(pageButtons.count(), 2);
    QSharedPointer<LauncherButton> button = pageButtons.at(0);
    QCOMPARE(button.data()->desktopEntry(), pathFromKey(existingEntryList[0]));
    button = pageButtons.at(1);
    QCOMPARE(button.data()->desktopEntry(), pathFromKey(existingEntryList[4]));

    // the second page should have 2 items
    page = pages.at(1);
    pageButtons = page.data()->model()->launcherButtons();
    QCOMPARE(pageButtons.count(), 2);
    button = pageButtons.at(0);
    QCOMPARE(button.data()->desktopEntry(), pathFromKey(existingEntryList[1]));
    button = pageButtons.at(1);
    QCOMPARE(button.data()->desktopEntry(), pathFromKey(existingEntryList[5]));

    // The third page should have only one item
    page = pages.at(2);
    QCOMPARE(page.data()->model()->launcherButtons().count(), 1);

    // The last page should have three items
    page = pages.at(3);
    pageButtons = page.data()->model()->launcherButtons();
    QCOMPARE(pageButtons.count(), 3);
    button = pageButtons.at(0);
    QCOMPARE(button.data()->desktopEntry(), pathFromKey(existingEntryList[3]));
    button = pageButtons.at(1);
    QCOMPARE(button.data()->desktopEntry(), pathFromKey(existingEntryList[6]));
    button = pageButtons.at(2);
    QCOMPARE(button.data()->desktopEntry(), pathFromKey(existingEntryList[7]));
}

void Ut_LauncherDataStore::testLauncherButtonLocation()
{
    QList< QSharedPointer<LauncherPage> > pages;
    createSimpleTestData(pages);
    m_subject->updateLauncherButtons(pages);
    const MDesktopEntry entry("/path/entry.desktop");
    LauncherDataStore::EntryLocation location = m_subject->location(entry);
    QCOMPARE(location, LauncherDataStore::LauncherGrid);

    const MDesktopEntry unknownEntry("/path/to-an-unknown-entry.desktop");
    location = m_subject->location(unknownEntry);
    QCOMPARE(location, LauncherDataStore::Unknown);
}

void Ut_LauncherDataStore::testGettingQuickLaunchBarButtonsWithInvalidDesktopEntries()
{
    testData.insert(QString(KEY_PREFIX + "/path/entry1.desktop"), QString("quicklaunchbar/0"));
    testData.insert(QString(KEY_PREFIX + "/path/entry2.desktop"), QString("quicklaunchbar/1"));

    // none of the desktop entries are valid:
    QList<LauncherButton*> buttons = m_subject->quickLaunchBarButtons();
    QCOMPARE(buttons.count(), 0);
    // invalid entries were removed:
    QCOMPARE(testData.count(), 0);
}

void Ut_LauncherDataStore::testGettingQuickLaunchBarButtonsWithOneValidDesktopEntry()
{
    testData.insert(QString(KEY_PREFIX + "/path/entry1.desktop"), QString("quicklaunchbar/0"));
    testData.insert(QString(KEY_PREFIX + "/path/entry2.desktop"), QString("quicklaunchbar/1"));

    // make one desktop entry valid:
    gValidDesktopFiles.append("/path/entry1.desktop");
    QList<LauncherButton*> buttons = m_subject->quickLaunchBarButtons();
    QCOMPARE(buttons.count(), 1);
    QVERIFY(dynamic_cast<LauncherButton *>(buttons[0]) != NULL);
    // invalid entries were removed:
    QCOMPARE(testData.count(), 1);
}

void Ut_LauncherDataStore::testGettingQuickLaunchBarButtonsWithEmptyPlaces()
{
    // there can be empty places in the quick launch bar list
    testData.insert(QString(KEY_PREFIX + "/path/entry1.desktop"), QString("quicklaunchbar/3"));
    gValidDesktopFiles.append("/path/entry1.desktop");
    QList<LauncherButton*> buttons = m_subject->quickLaunchBarButtons();
    QCOMPARE(buttons.count(), 4);
    QVERIFY(buttons[0] == NULL);
    QVERIFY(buttons[1] == NULL);
    QVERIFY(buttons[2] == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(buttons[3]) != NULL);
}

static void createSimpleTestData(QList< QSharedPointer<LauncherPage> > &pages)
{
    QStringList pageOneEntries;
    pageOneEntries << QString("/path/entry.desktop") << QString("/path/entry-1.desktop");

    QStringList pageTwoEntries;
    pageTwoEntries << QString("/path/entry-2.desktop") << QString("/path/entry-3.desktop") << QString("/path/entry-4.desktop");

    QSharedPointer<LauncherPage> pageOne = createLauncherPage(pageOneEntries);
    QSharedPointer<LauncherPage> pageTwo = createLauncherPage(pageTwoEntries);
    pages.append(pageOne);
    pages.append(pageTwo);
}

static void comparePageLocations(QList< QSharedPointer<LauncherPage> > pages)
{
    foreach (QSharedPointer<LauncherPage> page, pages) {
	compareEntryPageLocations(page, pages.indexOf(page));
    }
}

static void compareEntryPageLocations(QSharedPointer<LauncherPage> pageToCompare, int page)
{
    int positionOnPage = 0;
    foreach (QSharedPointer<LauncherButton> button, pageToCompare.data()->model()->launcherButtons()) {
        QString position(formatter.arg(page).arg(positionOnPage));
        QCOMPARE(testData.value(keyFromPath(button.data()->desktopEntry())).toString(), position);
        positionOnPage++;
    }
}

static QSharedPointer<LauncherPage> createLauncherPage(const QStringList entries)
{
    const QSharedPointer<LauncherPage> page = QSharedPointer<LauncherPage>(new LauncherPage());
    foreach (QString entry, entries) {
	QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton>(createLauncherButton(entry));
	page.data()->appendButton(button);
    }
    return page;
}

static QSharedPointer<LauncherButton> createLauncherButton(const QString path)
{
    const MDesktopEntry de(path);
    QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton>(new LauncherButton(de));
    return button;
}

static QString pathFromKey(QString key)
{
    key.replace(0, KEY_PREFIX.length(), "");
    return key;
}

static QString keyFromPath(QString path)
{
    path.insert(0, KEY_PREFIX);
    return path;
}

QTEST_APPLESS_MAIN(Ut_LauncherDataStore)
