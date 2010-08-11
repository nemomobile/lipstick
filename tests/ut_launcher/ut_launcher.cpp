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
#include "windowinfo_stub.h"
#include "launcherbutton_stub.h"
#include "launcherdatastore_stub.h"
#include "applicationpackagemonitor_stub.h"
#include "launchermodel.h"
#include "launcherpagemodel.h"
#include "mockdatastore.h"


const static int BUTTONS_PER_PAGE = 12;

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
    gLauncherButtonStub->stubReset();
}

void Ut_Launcher::cleanupTestCase()
{
}

void Ut_Launcher::init()
{
    // Set up the launcher data store
    launcherDataStore = new LauncherDataStore(new MockDataStore);
    packageMonitor = new ApplicationPackageMonitor();
    // Create a launcher and connect the signals
    launcher = new Launcher(launcherDataStore, packageMonitor);
    connect(this, SIGNAL(directoryChanged(const QString)), launcher, SLOT(updatePagesFromDataStore()));
    connect(this, SIGNAL(testPanToPageSignal(const QString &)), launcher, SLOT(panToPage(const QString &)));

    qProcessProgramStarted.clear();
}

void Ut_Launcher::cleanup()
{
    delete launcher;
    delete launcherDataStore;
    delete packageMonitor;
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

    gLauncherButtonStub->stubSetReturnValue("desktopEntry", updateButtonEntry);

    emit updateButton(updateButtonEntry);

    QCOMPARE(gLauncherButtonStub->stubCallCount("updateFromDesktopEntry"), 1);
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

    // remove 2 buttons from 1st page
    buttonCountOnFirstPage = BUTTONS_PER_PAGE - 2;

    gLauncherButtonStub->stubSetReturnValue("desktopEntry", QString("testApp3.desktop"));
    launcher->removeLauncherButton("testApp3.desktop");

    gLauncherButtonStub->stubSetReturnValue("desktopEntry", QString("testApp4.desktop"));
    launcher->removeLauncherButton("testApp4.desktop");

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

void Ut_Launcher::testPackageMonitorSignalConnections()
{
    QVERIFY(disconnect(packageMonitor, SIGNAL(downloadProgress(const QString&, const QString &, int, int)),
            launcher, SLOT(setDownloadProgress(const QString&, const QString &, int, int))));
    QVERIFY(disconnect(packageMonitor, SIGNAL(installProgress(const QString&, const QString &, int)),
            launcher, SLOT(setInstallProgress(const QString&, const QString &, int))));
    QVERIFY(disconnect(packageMonitor, SIGNAL(operationSuccess(const QString&, const QString&)),
            launcher, SLOT(setOperationSuccess(const QString&, const QString&))));
    QVERIFY(disconnect(packageMonitor, SIGNAL(operationError(const QString&, const QString&, const QString&)),
            launcher, SLOT(setOperationError(const QString&, const QString&, const QString&))));
}

void Ut_Launcher::testUpdateButtonState()
{
    gLauncherButtonStub->stubReset();
    const QString packageName = "testPackage";
    const QString desktopEntry = "test.desktop";
    int progress = 50;

    //First update state for button that doesn't exist in the launcherButtonMap
    QVERIFY(launcher->launcherButtonMap.isEmpty());
    launcher->updateButtonState(packageName, desktopEntry, LauncherButtonModel::Downloading, progress);
    QCOMPARE(launcher->launcherButtonMap.count(), 1);
    QCOMPARE(gLauncherButtonStub->stubCallCount("setState"), 1);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), progress);

    //Update button's state and progress
    progress = 10;
    launcher->updateButtonState(packageName, desktopEntry, LauncherButtonModel::Installing, progress);
    QCOMPARE(launcher->launcherButtonMap.count(), 1);
    QCOMPARE(gLauncherButtonStub->stubCallCount("setState"), 2);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Installing);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), progress);
}

void Ut_Launcher::testAddPlaceholderButtonToLauncher()
{
    QSharedPointer<LauncherButton> button;
    QVERIFY(launcher->model()->launcherPages().isEmpty());
    launcher->addPlaceholderButtonToLauncher(button);
    QCOMPARE(launcher->model()->launcherPages().count(), 1);
    QVERIFY(launcher->model()->launcherPages().last()->model()->launcherButtons().contains(button));

    QSharedPointer<LauncherButton> button2;
    launcher->addPlaceholderButtonToLauncher(button2);
    QVERIFY(launcher->model()->launcherPages().last()->model()->launcherButtons().contains(button2));
}

void Ut_Launcher::testSetDownloadProgress()
{
    int loaded = 50;
    int total = 100;
    int percentage = ((double)loaded / (double)total) * 100;
    launcher->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), percentage);

    //test with invalid values
    total = 0;
    launcher->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), -1);

    loaded = 100;
    total = 50;
    launcher->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), -1);
}

void Ut_Launcher::testSetDownloadProgressWithInvalidValues()
{
    int loaded = 50;
    int total = 0;
    launcher->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), -1);

    loaded = 100;
    total = 50;
    launcher->setDownloadProgress("testPackage", "test.desktop", loaded, total);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Downloading);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), -1);
}

void Ut_Launcher::testSetInstallProgress()
{
    int percentage = 25;
    launcher->setInstallProgress("testPackage", "test.desktop", percentage);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Installing);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), percentage);
}

void Ut_Launcher::testSetOperationSuccess()
{
    //First call setInstallProgress to get package added to map
    launcher->setInstallProgress("testPackage", "test.desktop", 99);
    QCOMPARE(launcher->launcherButtonMap.count(), 1);

    //After calling setOperationSuccess state should be "Installed" and progress 0
    launcher->setOperationSuccess("testPackage", "test.desktop");
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Installed);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), 0);
    QCOMPARE(gLauncherButtonStub->stubCallCount("updateFromDesktopEntry"), 1);
    QCOMPARE(launcher->launcherButtonMap.count(), 0);
}

void Ut_Launcher::testSetOperationError()
{
    launcher->setOperationError("testPackage", "test.desktop", "error_message");
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<LauncherButtonModel::State>(0), LauncherButtonModel::Broken);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setState").parameter<int>(1), 0);
}

QTEST_MAIN(Ut_Launcher)
