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
#include "ut_launcherpage.h"
#include "launcherpage.h"
#include "launcherbutton_stub.h"


static const QSharedPointer<LauncherButton> createLauncherButton(QString desktopFileName = QString());

void Ut_LauncherPage::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherpage" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherPage::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherPage::init()
{
    m_subject = new LauncherPage();
}

void Ut_LauncherPage::cleanup()
{
    delete m_subject;
}

void Ut_LauncherPage::testAddButtonWhenPageNotFull()
{
    m_subject->model()->setMaxButtons(2);
    QSharedPointer<LauncherButton> button1 = createLauncherButton();
    bool added = m_subject->appendButton(button1);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);

    QSharedPointer<LauncherButton> button2 = createLauncherButton();
    added = m_subject->appendButton(button2);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 2);
}

void Ut_LauncherPage::testAddButtonWhenPageIsFull()
{
    m_subject->model()->setMaxButtons(1);
    QSharedPointer<LauncherButton> button1 = createLauncherButton();
    bool added = m_subject->appendButton(button1);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);

    QSharedPointer<LauncherButton> button2 = createLauncherButton();
    added = m_subject->appendButton(button2);
    QVERIFY(!added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);
}

void Ut_LauncherPage::testContainsDesktopEntry()
{
    QSharedPointer<LauncherButton> button = createLauncherButton("my-entry-name");
    bool added = m_subject->appendButton(button);
    QVERIFY(added);

    bool contains = m_subject->contains("my-entry-name");
    QVERIFY(contains);

    contains = m_subject->contains("bad-entry-name");
    QVERIFY(!contains);

}

void Ut_LauncherPage::testRemoveButton()
{
    m_subject->model()->setMaxButtons(1);
    QSharedPointer<LauncherButton> button = createLauncherButton("my-entry-name");
    bool added = m_subject->appendButton(button);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);

    m_subject->removeButton(button);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 0);

    // Try to remove the same button again
    m_subject->removeButton(button);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 0);
}

void Ut_LauncherPage::testPruning()
{
    QStringList entryList;
    entryList << "/my/dir/my-entry-name"
              << "/my/dir/my-entry-name1"
              << "/my/dir/my-entry-name2"
              << "/another-my/dir/my-entry-name3"
              << "/my/dir/my-entry-name4"
              << "/not-my/dir/my-entry-name5";
    m_subject->model()->setMaxButtons(6);
    foreach (QString entry, entryList) {
	QSharedPointer<LauncherButton> button = createLauncherButton(entry);
	bool added = m_subject->appendButton(button);
	QVERIFY(added);
    }

    entryList.takeAt(2);
    const QString dir("/my/dir");
    const QString dir2("/another-my/dir");
    QVERIFY(m_subject->prune(entryList, QStringList() << dir << dir2));

    /*
      Check that the directory /another-my/dir is not pruned away
      from this page, but /not-my/dir is
     */
    QCOMPARE(m_subject->model()->launcherButtons().count(), 4);
    foreach (QSharedPointer<LauncherButton> button, m_subject->model()->launcherButtons()) {
	QVERIFY(entryList.contains(button.data()->model()->desktopEntryFile()));
    }

    entryList.clear();
    entryList << "/another-my/dir/my-entry-name3";
    QVERIFY(m_subject->prune(entryList, QStringList() << dir << dir2));

    /*
      Check that the "/another-my/dir" named directory is not pruned away from this page
     */
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);
    foreach (QSharedPointer<LauncherButton> button, m_subject->model()->launcherButtons()) {
	QVERIFY(entryList.contains(button.data()->model()->desktopEntryFile()));
    }

    entryList.clear();
    entryList << "/my/dir/does-not-matter";

    const QString lastDir("/not-my/dir");
    // verify that empty page returns false on pruning
    QVERIFY(!m_subject->prune(entryList, QStringList() << lastDir));
    QCOMPARE(m_subject->model()->launcherButtons().count(), 0);
}

static const QSharedPointer<LauncherButton> createLauncherButton(QString desktopFileName)
{
    QSharedPointer<LauncherButton> button(new LauncherButton);
    LauncherButtonModel *buttonModel = new LauncherButtonModel;
    button.data()->setModel(buttonModel);
    button.data()->model()->setDesktopEntryFile(desktopFileName);
    return button;
}

QTEST_APPLESS_MAIN(Ut_LauncherPage)
