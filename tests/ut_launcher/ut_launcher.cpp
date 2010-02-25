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
#include <DuiApplication>
#include <DuiDesktopEntry>
#include <duidesktopentry_p.h>
#include <DuiApplicationIfProxy>
#include "ut_launcher.h"
#include "launcher.h"
#include "launcherbutton.h"
#include "homeapplication.h"

QFileInfoList Ut_Launcher::desktopFileInfoList;
QFileInfoList Ut_Launcher::directoryFileInfoList;
bool          Ut_Launcher::duiApplicationIfProxyLaunchCalled;
QString       Ut_Launcher::applicationStarted;

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// DuiDesktopEntry stubs (used by Launcher)
QMap<const DuiDesktopEntry *, QString> desktopEntryFileName;
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

DuiDesktopEntry::DuiDesktopEntry(const QString &fileName) :
    d_ptr(NULL)
{
    desktopEntryFileName.insert(this, fileName);
}

QString DuiDesktopEntry::fileName() const
{
    return desktopEntryFileName[this];
}

QStringList DuiDesktopEntry::categories() const
{
    return desktopEntryCategories.value(desktopEntryFileName.value(this));
}

bool DuiDesktopEntry::isValid() const
{
    return true;
}

QStringList DuiDesktopEntry::onlyShowIn() const
{
    return desktopEntryOnlyShowIn.value(desktopEntryFileName.value(this));
}

QStringList DuiDesktopEntry::notShowIn() const
{
    return desktopEntryNotShowIn.value(desktopEntryFileName.value(this));
}

QString DuiDesktopEntry::type() const
{
    return desktopEntryType.value(desktopEntryFileName.value(this));
}

QString DuiDesktopEntry::xMaemoService() const
{
    return desktopEntryXMaemoService.value(desktopEntryFileName.value(this));
}

QString DuiDesktopEntry::name() const
{
    return desktopEntryName.value(desktopEntryFileName.value(this));
}

QString DuiDesktopEntry::icon() const
{
    return desktopEntryIcon.value(desktopEntryFileName.value(this));
}

QString DuiDesktopEntry::exec() const
{
    return desktopEntryExec.value(desktopEntryFileName.value(this));
}

QString DuiDesktopEntry::url() const
{
    return desktopEntryUrl.value(desktopEntryFileName.value(this));
}

QString DuiDesktopEntry::nameUnlocalized() const
{
    return desktopEntryNameUnlocalized.value(desktopEntryFileName.value(this));
}

// QDir stubs
QFileInfoList QDir::entryInfoList(Filters filters, SortFlags sort) const
{
    Q_UNUSED(filters);
    Q_UNUSED(sort);

    // Return either a list of desktop entries or a list of directory entries based on the name filters given to QDir
    if (nameFilters().at(0) == "*.desktop")
        return Ut_Launcher::desktopFileInfoList;
    else
        return Ut_Launcher::directoryFileInfoList;
}

// QFileSystemWatcher stubs
void QFileSystemWatcher::addPath(const QString &)
{
}

void QDBusPendingReplyData::setMetaTypes(int, int const *)
{
}

QDBusPendingReply<> DuiApplicationIfProxy::launch()
{
    Ut_Launcher::duiApplicationIfProxyLaunchCalled = true;

    return QDBusPendingReply<>();
}

bool QProcess::startDetached(const QString &program)
{
    Ut_Launcher::applicationStarted = program;

    return true;
}

bool QDBusConnection::isConnected() const
{
    return true;
}

// Tests
void Ut_Launcher::initTestCase()
{
    // Create a DuiAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_launcher";
    app = new DuiApplication(argc, &app_name);

    // Test applications for root category
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop", "Test0");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop", "test0");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop", "Test1");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop", "test1");
    desktopEntryXMaemoService.insert(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop", "com.nokia.test1");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "link.desktop", "Test2");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "link.desktop", "Link");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "link.desktop", "Icon-camera");
    desktopEntryUrl.insert(QString(APPLICATIONS_DIRECTORY) + "link.desktop", "test2");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop", "Test3");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop", "Directory");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop", "Icon-camera");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "Only show in DUI");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "Icon-dui-application");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "dui-application");
    desktopEntryOnlyShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", QStringList() << "X-DUI" << "KDE" << "GNOME");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "Only show in desktops");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "desktop-application");
    desktopEntryOnlyShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", QStringList() << "KDE" << "GNOME");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDUI.desktop", "Not show in DUI");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDUI.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDUI.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDUI.desktop", "noshow-application");
    desktopEntryNotShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDUI.desktop", QStringList() << "X-DUI" << "KDE" << "GNOME");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "Not show in desktops");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "no-desktop-application");
    desktopEntryNotShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", QStringList() << "KDE" << "GNOME");

    // TODO: remove this desktop file when the Category DUI feature is removed
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryDUI.desktop", "Category DUI application");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryDUI.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryDUI.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryDUI.desktop", "dui-category-application");
    desktopEntryCategories.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryDUI.desktop", QStringList() << "DUI");

    // Test applications that belong to categories
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "Test4");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "Icon-music");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "test4");
    desktopEntryCategories.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", QStringList() << "somecategory" << "TestCat1");

    // Test categories
    desktopEntryName.insert(QString(CATEGORIES_DIRECTORY) + "directoryEntry1.directory", "TestCat1");
    desktopEntryType.insert(QString(CATEGORIES_DIRECTORY) + "directoryEntry1.directory", "Directory");
    desktopEntryIcon.insert(QString(CATEGORIES_DIRECTORY) + "directoryEntry1.directory", "Icon-camera");
    desktopEntryName.insert(QString(CATEGORIES_DIRECTORY) + "directoryEntry2.directory", "TestCat2");
    desktopEntryType.insert(QString(CATEGORIES_DIRECTORY) + "directoryEntry2.directory", "Directory");
    desktopEntryIcon.insert(QString(CATEGORIES_DIRECTORY) + "directoryEntry2.directory", "Icon-camera");

    // Incomplete files
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "test5.desktop", "Icon-music");
}

void Ut_Launcher::cleanupTestCase()
{
    // Destroy DuiApplication
    delete app;
}

void Ut_Launcher::init()
{
    // Create a launcher and connect the directory changed signal
    launcher = new Launcher;
    connect(this, SIGNAL(applicationLaunched(const QString)), launcher, SLOT(launchApplication(const QString)));
    connect(this, SIGNAL(duiApplicationLaunched(const QString)), launcher, SLOT(launchDuiApplication(const QString)));
    connect(this, SIGNAL(linkLaunched(const QString)), launcher, SLOT(launchLink(const QString)));
    connect(this, SIGNAL(directoryChanged(const QString)), launcher, SLOT(readDirectory(const QString)));
    connect(this, SIGNAL(directoryLaunched(QString, QString, QString)), launcher, SLOT(launchDirectory(QString, QString, QString)));

    // No files by default
    desktopFileInfoList.clear();
    directoryFileInfoList.clear();
    applicationStarted.clear();
    duiApplicationIfProxyLaunchCalled = false;
}

void Ut_Launcher::cleanup()
{
    // Destroy launcher
    delete launcher;
}

void Ut_Launcher::testInitialization()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop"));
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "link.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->openRootCategory();

    // There should be three widgets in the view
    QCOMPARE(launcher->model()->widgets().count(), 3);

    // The widgets should be LauncherButtons
    for (int i = 0; i < 3; i++) {
        LauncherButton *b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(i));

        QVERIFY(b != NULL);

        // Check name and icon for all entries
        QCOMPARE(b->text(), QString().sprintf("Test%d", i));
        QCOMPARE(b->iconID(), QString("Icon-camera"));

        // Check target type - the first two are Applications, then a Link
        switch (i) {
        case 0:
            QCOMPARE(b->targetType(), QString("Application"));
            break;
        case 1: {
            QCOMPARE(b->targetType(), QString("Service"));
            QCOMPARE(b->target(), QString("com.nokia.test1"));
        }
        break;
        case 2:
            QCOMPARE(b->targetType(), QString("Link"));
            break;
        default:
            break;
        }
    }
}

void Ut_Launcher::testOnlyShowInDui()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->openRootCategory();

    // There should be three widgets in the view
    QCOMPARE(launcher->model()->widgets().count(), 1);

    LauncherButton *b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(0));
    QVERIFY(b != NULL);

    // Check name and icon for all entries
    QCOMPARE(b->text(), QString("Only show in DUI"));
    QCOMPARE(b->iconID(), QString("Icon-dui-application"));
    QCOMPARE(b->targetType(), QString("Application"));
    QCOMPARE(b->target(), QString("dui-application"));
}

void Ut_Launcher::testOnlyShowInNotDui()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktop.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->openRootCategory();

    // There should be no widgets
    QCOMPARE(launcher->model()->widgets().count(), 0);
}

void Ut_Launcher::testNotShowInDui()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDUI.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->openRootCategory();

    // There should be no widgets
    QCOMPARE(launcher->model()->widgets().count(), 0);
}

void Ut_Launcher::testNotShowInNotDui()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->openRootCategory();

    // There should be one widget
    QCOMPARE(launcher->model()->widgets().count(), 1);

    LauncherButton *b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(0));
    QVERIFY(b != NULL);

    // Check name and icon for all entries
    QCOMPARE(b->text(), QString("Not show in desktops"));
    QCOMPARE(b->iconID(), QString("Icon-camera"));
    QCOMPARE(b->targetType(), QString("Application"));
    QCOMPARE(b->target(), QString("no-desktop-application"));
}

// TODO: remove this test when the Category DUI feature is removed
void Ut_Launcher::testCategoryDui()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryDUI.desktop"));
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->openRootCategory();

    // There should be one widget
    QCOMPARE(launcher->model()->widgets().count(), 1);

    LauncherButton *b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(0));
    QVERIFY(b != NULL);

    // Check name and icon for all entries
    QCOMPARE(b->text(), QString("Category DUI application"));
    QCOMPARE(b->iconID(), QString("Icon-camera"));
    QCOMPARE(b->targetType(), QString("Application"));
    QCOMPARE(b->target(), QString("dui-category-application"));
}

void Ut_Launcher::testDesktopEntryAddToRoot()
{
    // Initalize launcher
    launcher->openRootCategory();

    // There should be no widgets in the view
    QCOMPARE(launcher->model()->widgets().count(), 0);

    // Add a desktop file in the "desktop file directory"
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be one widget in the view
    QCOMPARE(launcher->model()->widgets().count(), 1);

    // The widget should be a LauncherButton
    LauncherButton *b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(0));

    QVERIFY(b != NULL);

    // Check name, icon, target and type
    QCOMPARE(b->text(), QString("Test0"));
    QCOMPARE(b->iconID(), QString("Icon-camera"));
    QCOMPARE(b->target(), QString("test0"));
    QCOMPARE(b->targetType(), QString("Application"));
}

void Ut_Launcher::testDesktopEntryAddToNamedCategory()
{
    // Adds two items, one to root category, another to "TestCat1" category. Checks that only first one is added to root category, then changes category to TestCat1, and tests that only second item is there.

    // Initalize launcher
    launcher->openRootCategory();

    // There should be no widgets in the view
    QCOMPARE(launcher->model()->widgets().count(), 0);

    // Add a desktop file in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    // Application defines category different from root, shouldn't be added to root
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop"));

    // Check root category

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be one widget in the view
    QCOMPARE(launcher->model()->widgets().count(), 1);

    // The widget should be a LauncherButton
    LauncherButton *b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(0));

    QVERIFY(b != NULL);

    // Check name, icon, target and type
    QCOMPARE(b->text(), QString("Test0"));
    QCOMPARE(b->iconID(), QString("Icon-camera"));
    QCOMPARE(b->target(), QString("test0"));
    QCOMPARE(b->targetType(), QString("Application"));

    // Check "TestCat1" category

    // Change category. No items from root should be present
    emit directoryLaunched(QString("TestCat1"));

    // There should be one widget in the category (the other one is in root)
    QCOMPARE(launcher->model()->widgets().count(), 1);

    // The widget should be a LauncherButton
    b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(0));

    QVERIFY(b != NULL);

    // Check that item from TestCat1 is there
    QCOMPARE(b->text(), QString("Test4"));
    QCOMPARE(b->iconID(), QString("Icon-music"));
    QCOMPARE(b->target(), QString("test4"));
    QCOMPARE(b->targetType(), QString("Application"));
}

void Ut_Launcher::testCategoryEntryAdd()
{
    launcher->openRootCategory();

    // There should be no widgets in the view
    QCOMPARE(launcher->model()->widgets().count(), 0);

    directoryFileInfoList.append(QFileInfo(QString(CATEGORIES_DIRECTORY) + "directoryEntry1.directory"));

    emit directoryChanged(CATEGORIES_DIRECTORY);

    // There should be one widget in the root
    QCOMPARE(launcher->model()->widgets().count(), 1);

    // The widget should be a LauncherButton
    LauncherButton *b = dynamic_cast<LauncherButton *>(launcher->model()->widgets().at(0));

    QVERIFY(b != NULL);

    // Check name, icon, target and type
    QCOMPARE(b->text(), QString("TestCat1"));
    QCOMPARE(b->iconID(), QString("Icon-camera"));
}

void Ut_Launcher::testCategoryEntryAddToAnotherCategory()
{
    launcher->openRootCategory();
    directoryFileInfoList.append(QFileInfo(QString(CATEGORIES_DIRECTORY) + "directoryEntry1.directory"));
    emit directoryChanged(CATEGORIES_DIRECTORY);

    //Change to previously added category
    emit directoryLaunched(QString("TestCat1"));

    // There should be no widgets in this category
    QCOMPARE(launcher->model()->widgets().count(), 0);

    // We don't support subcategories. Try adding a category to TestCat1
    directoryFileInfoList.append(QFileInfo(QString(CATEGORIES_DIRECTORY) + "directoryEntry2.directory"));

    // As we are not in root category, the item should not be added.
    QCOMPARE(launcher->model()->widgets().count(), 0);
}

void Ut_Launcher::testInvalidFiles()
{
    // Test adding files which should be considered invalid for some reason
    launcher->openRootCategory();

    // There should be no widgets in the root
    QCOMPARE(launcher->model()->widgets().count(), 0);

    // Entry is of type Directory, which shouldn't be added
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop"));

    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be no widgets in the root
    QCOMPARE(launcher->model()->widgets().count(), 0);

    // Entry is missing name, shouldn't be added
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "test5.desktop"));

    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be no widgets in the root
    QCOMPARE(launcher->model()->widgets().count(), 0);
}

void Ut_Launcher::testDesktopEntryRemove()
{
    // Add a desktop file in the "desktop file directory"
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    // Initalize launcher
    launcher->openRootCategory();

    // There should be one widget in the root
    QCOMPARE(launcher->model()->widgets().count(), 1);

    // "Delete" the desktop file
    desktopFileInfoList.clear();

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be no widgets in the root
    QCOMPARE(launcher->model()->widgets().count(), 0);
}

void Ut_Launcher::testApplicationLaunched()
{
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    launcher->openRootCategory();

    emit applicationLaunched("test0");

    QCOMPARE(applicationStarted, QString("test0"));
}

void Ut_Launcher::testDuiApplicationLaunched()
{
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop"));

    launcher->openRootCategory();

    emit duiApplicationLaunched("com.nokia.test1");

    QCOMPARE(duiApplicationIfProxyLaunchCalled, true);
}

void Ut_Launcher::testLaunchingApplicationFromCategory()
{
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    launcher->openRootCategory();

    // Set new category for the launcher and verify that category has changed.
    emit directoryLaunched(QString("directory"), QString("directoryTitle"), QString("directoryIcon"));
    QCOMPARE(launcher->model()->category(), LauncherModel::SubCategory);

    // Launch application.
    emit applicationLaunched("test0");

    // Verify that application is started and that launcher returns to root category
    QCOMPARE(applicationStarted, QString("test0"));
    QCOMPARE(launcher->model()->category(), LauncherModel::RootCategory);
}

void Ut_Launcher::testLaunchingDuiApplicationFromCategory()
{
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop"));

    launcher->openRootCategory();

    // Set new category for the launcher and verify that category has changed.
    emit directoryLaunched(QString("directory"), QString("directoryTitle"), QString("directoryIcon"));
    QCOMPARE(launcher->model()->category(), LauncherModel::SubCategory);

    // Launch dui application
    emit duiApplicationLaunched("com.nokia.test1");

    // Verify that application is started and that launcher returns to root category
    QCOMPARE(duiApplicationIfProxyLaunchCalled, true);
    QCOMPARE(launcher->model()->category(), LauncherModel::RootCategory);
}

void Ut_Launcher::testLaunchingLinkFromCategory()
{
    launcher->openRootCategory();

    // Launch link
    emit linkLaunched("testLink");

    // Verify that launcher returns to root category
    QCOMPARE(launcher->model()->category(), LauncherModel::RootCategory);
}

QTEST_APPLESS_MAIN(Ut_Launcher)
