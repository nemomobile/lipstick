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
#include <MDesktopEntry>
#include <MApplicationIfProxy>
#include "ut_launcher.h"
#include "launcher.h"
#include "launcherpage.h"
#include "launcherbutton.h"
#include "launcherdatastore_stub.h"
#include "launchermodel.h"
#include "launcherpagemodel.h"
#include "mockdatastore.h"



// LauncherButton stubs
LauncherButton::LauncherButton(MWidget *parent) : MButton(parent, new LauncherButtonModel) {}
LauncherButton::LauncherButton(const QString&, MWidget*parent) : MButton(parent, new LauncherButtonModel) {}
LauncherButton::~LauncherButton(){}

const static int BUTTONS_PER_PAGE = 12;


QString LauncherButton::desktopEntry() const
{
    return objectName();
}

int updateFromDesktopEntryCallCount = 0;
void LauncherButton::updateFromDesktopEntry(const QString &)
{
    updateFromDesktopEntryCallCount++;
}

void LauncherButton::launch()
{
}

void LauncherButton::retranslateUi()
{
}

bool LauncherButton::isInProgress() const
{
    return false;
}

void LauncherButton::setProgressIndicatorTimeout(int)
{
}

void LauncherButton::hideProgressIndicator()
{
}

void LauncherButton::hideProgressIndicatorIfObscured(const QList<WindowInfo> &)
{
}

void LauncherButton::init()
{
}

void QDBusPendingReplyData::setMetaTypes(int, int const *)
{
}

QString qProcessProgramStarted;
bool QProcess::startDetached(const QString &program)
{
    qProcessProgramStarted = program;

    return true;
}

bool QDBusConnection::isConnected() const
{
    return true;
}

void Ut_Launcher::addButtonsToLauncher(int amount)
{
    for (int i = 0; i < amount; i++) {
        launcher->addLauncherButton(QString("testApp%1.desktop").arg(i));
    }
}

// Tests
void Ut_Launcher::initTestCase()
{
}

void Ut_Launcher::cleanupTestCase()
{
}

void Ut_Launcher::init()
{
    // Set up the launcher data store
    launcherDataStore = new LauncherDataStore(new MockDataStore);

    // Create a launcher and connect the signals
    launcher = new Launcher(launcherDataStore);
    connect(this, SIGNAL(directoryChanged(const QString)), launcher, SLOT(updatePagesFromDataStore()));
    connect(this, SIGNAL(testPanToPageSignal(const QString &)), launcher, SLOT(panToPage(const QString &)));

    qProcessProgramStarted.clear();

    updateFromDesktopEntryCallCount = 0;
}

void Ut_Launcher::cleanup()
{
    delete launcher;
    delete launcherDataStore;
}

int Ut_Launcher::buttonsCount()
{
    int count = 0;
    foreach (QSharedPointer<LauncherPage> page, launcher->model()->launcherPages()) {
        count += page.data()->model()->launcherButtons().count();
    }
    return count;
}

void Ut_Launcher::createdDefaultSetOfDesktopEntries()
{
    QHash<QString, QVariant> dataForAllDesktopEntries;
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp00.desktop", QVariant("launcher/0/0"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp10.desktop", QVariant("launcher/1/0"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp20.desktop", QVariant("launcher/2/0"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp21.desktop", QVariant("launcher/2/1"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp22.desktop", QVariant("launcher/2/2"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp30.desktop", QVariant("launcher/3/0"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp40.desktop", QVariant("launcher/4/0"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp41.desktop", QVariant("launcher/4/1"));
    dataForAllDesktopEntries.insert(QString(APPLICATIONS_DIRECTORY) + "testApp50.desktop", QVariant("launcher/5/0"));
    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);
}

void Ut_Launcher::comparePageNumberArgument(QSignalSpy &spy, int page)
{
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), page);
}

void Ut_Launcher::testPaging()
{
    launcher->setEnabled(true);

    // Initially there should be no buttons
    QCOMPARE(buttonsCount(), 0);

    // Add some desktop files in the "desktop file directory"
    QHash<QString, QVariant> dataForAllDesktopEntries;
    dataForAllDesktopEntries.insert("noPlacement", QVariant());
    dataForAllDesktopEntries.insert("unknownPlacement", QVariant("unknown"));
    dataForAllDesktopEntries.insert("validPlacement1", QVariant("launcher/0/0"));
    dataForAllDesktopEntries.insert("validPlacement2", QVariant("launcher/1/1"));
    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be one button on the first page and 2 buttons on the second
    QCOMPARE(buttonsCount(), 3);
    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0).data()->model()->launcherButtons().count(), 1);
    QCOMPARE(launcher->model()->launcherPages().at(1).data()->model()->launcherButtons().count(), 2);

}

void Ut_Launcher::testEmptyPage()
{
    launcher->setEnabled(true);

    // Add 25 desktop files in the "desktop file directory"
    QHash<QString, QVariant> dataForAllDesktopEntries;
    for (int i = 0; i < 25; i++) {
        dataForAllDesktopEntries.insert(QString("noPlacement%1").arg(i), QVariant());
    }
    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be three launcher pages with 12, 12 and 1 items on them
    QCOMPARE(launcher->model()->launcherPages().count(), 3);
    QCOMPARE(launcher->model()->launcherPages().at(0)->model()->launcherButtons().count(), 12);
    QCOMPARE(launcher->model()->launcherPages().at(1)->model()->launcherButtons().count(), 12);
    QCOMPARE(launcher->model()->launcherPages().at(2)->model()->launcherButtons().count(), 1);

    // Don't put desktop files 12-23 in the "desktop file directory"
    dataForAllDesktopEntries.clear();
    for (int i = 0; i < 25; i++) {
        if (i < 12 || i > 23) {
            dataForAllDesktopEntries.insert(QString("noPlacement%1").arg(i), QVariant());
        }
    }
    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be two launcher pages with 12 and 1 buttons on them
    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0)->model()->launcherButtons().count(), 12);
    QCOMPARE(launcher->model()->launcherPages().at(1)->model()->launcherButtons().count(), 1);
}

void Ut_Launcher::testPanToPageWithAbsoluteFilePathEntry()
{
    QSignalSpy spyPanToReqPage(launcher, SIGNAL(panningRequested(uint)));

    createdDefaultSetOfDesktopEntries();

    emit testPanToPageSignal(QString(APPLICATIONS_DIRECTORY) + "testApp20.desktop");

    comparePageNumberArgument(spyPanToReqPage, 2);
}

void Ut_Launcher::testPanToPageWithOnlyDesktopFileName()
{
    QSignalSpy spyPanToReqPage(launcher, SIGNAL(panningRequested(uint)));

    createdDefaultSetOfDesktopEntries();

    emit testPanToPageSignal("testApp30.desktop");

    comparePageNumberArgument(spyPanToReqPage, 3);
}

void Ut_Launcher::testPanToPageMultipleTimes()
{
    QSignalSpy spyPanToReqPage(launcher, SIGNAL(panningRequested(uint)));

    createdDefaultSetOfDesktopEntries();

    emit testPanToPageSignal("testApp00.desktop");

    comparePageNumberArgument(spyPanToReqPage, 0);

    emit testPanToPageSignal("testApp21.desktop");

    comparePageNumberArgument(spyPanToReqPage, 2);

    emit testPanToPageSignal("testApp50.desktop");

    comparePageNumberArgument(spyPanToReqPage, 5);

    emit testPanToPageSignal("testApp22.desktop");

    comparePageNumberArgument(spyPanToReqPage, 2);
}

void Ut_Launcher::testPanToPageWithNonExistentFileName()
{
    QSignalSpy spyPanToReqPage(launcher, SIGNAL(panningRequested(uint)));

    createdDefaultSetOfDesktopEntries();

    emit testPanToPageSignal("testApp30");

    QCOMPARE(spyPanToReqPage.count(), 0);
}

void Ut_Launcher::testPanToPageWithEmptyFileName()
{
    QSignalSpy spyPanToReqPage(launcher, SIGNAL(panningRequested(uint)));

    createdDefaultSetOfDesktopEntries();

    emit testPanToPageSignal("");

    QCOMPARE(spyPanToReqPage.count(), 0);
}

void Ut_Launcher::testUpdatingLauncherButton()
{
    createdDefaultSetOfDesktopEntries();
    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    connect(this, SIGNAL(updateButton(QString)), launcher, SLOT(updateLauncherButton(QString)));

    QString updateButtonEntry = QString(APPLICATIONS_DIRECTORY) + "testApp20.desktop";
    // Make one specific button to "simulate" updated button
    // (Update is checked from button so that we don't have to stub LauncherPage just for this)
    launcher->model()->launcherPages().at(2).data()->model()->launcherButtons().at(0)->setObjectName(updateButtonEntry);
    emit updateButton(updateButtonEntry);

    QCOMPARE(updateFromDesktopEntryCallCount, 1);
}

void Ut_Launcher::testAddingButtons()
{
    launcher->setEnabled(true);

    addButtonsToLauncher(BUTTONS_PER_PAGE);

    QCOMPARE(launcher->model()->launcherPages().count(), 1);
    QCOMPARE(launcher->model()->launcherPages().at(0)->model()->launcherButtons().count(), BUTTONS_PER_PAGE);
}

void Ut_Launcher::testAddingButtonsOnMultiplePages()
{
    launcher->setEnabled(true);

    int buttonCountOnSecondPage = 3;
    int addedButtons = BUTTONS_PER_PAGE + buttonCountOnSecondPage;
    addButtonsToLauncher(addedButtons);

    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0)->model()->launcherButtons().count(), BUTTONS_PER_PAGE);
    QCOMPARE(launcher->model()->launcherPages().at(1)->model()->launcherButtons().count(), buttonCountOnSecondPage);
}

void Ut_Launcher::testAddingButtonsWithExistingButtons()
{
    launcher->setEnabled(true);

    // Add some existing buttons
    int initialButtons = BUTTONS_PER_PAGE/2;
    QHash<QString, QVariant> dataForAllDesktopEntries;
    for (int i = 0; i < initialButtons; i++) {
        dataForAllDesktopEntries.insert(QString("noPlacement%1").arg(i), QVariant());
    }

    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // adding more buttons
    int addedButtons = BUTTONS_PER_PAGE;
    int buttonCountOnSecondPage = initialButtons;
    addButtonsToLauncher(addedButtons);

    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0)->model()->launcherButtons().count(), BUTTONS_PER_PAGE);
    QCOMPARE(launcher->model()->launcherPages().at(1)->model()->launcherButtons().count(), buttonCountOnSecondPage);
}

void Ut_Launcher::testRemovingButtons()
{
    launcher->setEnabled(true);

    // adding more than one page
    int buttonCountOnFirstPage = BUTTONS_PER_PAGE;
    int buttonCountOnSecondPage = 2;
    int buttonCount = BUTTONS_PER_PAGE + buttonCountOnSecondPage;
    addButtonsToLauncher(buttonCount);

    // Make specific buttons to "simulate" removed buttons
    launcher->model()->launcherPages().at(0).data()->model()->launcherButtons().at(3)->setObjectName("testApp2.desktop");
    launcher->model()->launcherPages().at(0).data()->model()->launcherButtons().at(4)->setObjectName("testApp3.desktop");

    // remove 2 buttons from 1st page
    buttonCountOnFirstPage = BUTTONS_PER_PAGE - 2;
    launcher->removeLauncherButton("testApp2.desktop");
    launcher->removeLauncherButton("testApp3.desktop");

    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0)->model()->launcherButtons().count(), buttonCountOnFirstPage);
    QCOMPARE(launcher->model()->launcherPages().at(1)->model()->launcherButtons().count(), buttonCountOnSecondPage);
}

void Ut_Launcher::testSettingLauncherToFirstPage()
{
    QSignalSpy spy(launcher, SIGNAL(focusToFirstPageRequested()));
    launcher->setFirstPage();
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(Ut_Launcher)
