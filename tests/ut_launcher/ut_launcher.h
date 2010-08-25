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
#ifndef _UT_LAUNCHER_
#define _UT_LAUNCHER_

#include <QObject>
#include <QVector>
#include <QSignalSpy>

class Launcher;
class LauncherDataStore;
class ApplicationPackageMonitor;
class MApplication;
class MWidget;

class Ut_Launcher : public QObject
{
    Q_OBJECT

private:
    Launcher *launcher;
    LauncherDataStore *launcherDataStore;
    ApplicationPackageMonitor *packageMonitor;
    MApplication *app;

    void writeDesktopFile(QString fileName, QString type, QString name, QString iconName, QString exec);
    int buttonsCount();
    // Convience function for pantToPage tests to check that signal has correct page as an argument
    void comparePageNumberArgument(QSignalSpy &spy, int page);
    // adds buttons to launcher
    void addButtonsToLauncher(int amount);
    // Create default set of desktop file entries for panToPage tests
    void createdDefaultSetOfDesktopEntries();

signals:
    void directoryLaunched(const QString &directory, const QString &title = QString(), const QString &iconId = QString());
    void directoryChanged(const QString path);
    void buttonClicked();
    void testPanToPageSignal(const QString &filePath);
    void updateButton(const QString &filePath);

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test that launcher buttons are paged to multiple pages
    void testPaging();
    // Test that empty page is removed from launcher
    void testEmptyPage();
    // Test buttonPlacement() with full desktopEntryPath, with name of the file and faulty name.
    void testButtonPlacement();
    // Test panToPage() with successfull and unsuccessfull case.
    void testPanToPage();
    // Test that launcher button is updated when signal received
    void testUpdatingLauncherButton();
    // Test adding buttons
    void testAddingButtons();
    // Test adding buttons on multiple pages
    void testAddingButtonsOnMultiplePages();
    // Test adding buttons test adding button when there are existing buttons
    void testAddingButtonsWithExistingButtons();
    // Test removing buttons
    void testRemovingButtons();
    // Test setting launcher to show first page
    void testSettingLauncherToFirstPage();
    // Test signal connections
    void testPackageMonitorSignalConnections();
    // Test updating button state
    void testUpdateButtonState();
    // Test adding palceholder button to launcher
    void testAddPlaceholderButtonToLauncher();
    // Test checking if desktop entry exists in datastore
    void testEntryExistsInDatastore();
    // Test handling of downloadProgress signal
    void testSetDownloadProgress();
    // Test handling of downloadProgress signal with invalid values
    void testSetDownloadProgressWithInvalidValues();
    // Test handling of installProgress signal
    void testSetInstallProgress();
    // Test handling of operationSuccess signal
    void testSetOperationSuccess();
    // Test handling of operationError signal
    void testSetOperationError();
    // Test setting the maximum page size
    void testSetMaximumPageSize();
    // Test handling of operation error when button is already found in launcher
    void testSetOperationErrorWhenButtonFoundFromLauncher();
    // Test handling of operation error when entry has a placeholder launcher
    void testSetOperationErrorWhenButtonHasPlaceholder();
};
#endif //_UT_LAUNCHER_
