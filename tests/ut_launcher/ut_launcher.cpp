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
#include <MApplicationIfProxy>
#include "ut_launcher.h"
#include "launcher.h"
#include "launcherpage.h"
#include "launcherbutton.h"
#include "launcherdatastore.h"
#include "launchermodel.h"
#include "launcherpagemodel.h"
#include "homeapplication.h"

QFileInfoList Ut_Launcher::desktopFileInfoList;
QFileInfoList Ut_Launcher::directoryFileInfoList;
bool          Ut_Launcher::mApplicationIfProxyLaunchCalled;
QString       Ut_Launcher::applicationStarted;
static bool gConcurrentRunCalled = false;
static const int DATASTORE_WRITE_TIMEOUT = 500;
static bool gTimerActive = false;
static int gTimerTimeout = 0;
static QTimer* gButtonTimer;
// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// QTimer stubs

void QTimer::start(int interval)
{
    if (this == gButtonTimer) {
        gTimerActive = true;
        gTimerTimeout = interval;
    }
}

// QtConcurrent stubs
namespace QtConcurrent {
    template<>
    QFuture<void> RunFunctionTaskBase<void>::start()
    {
        gConcurrentRunCalled = true;
        return QFuture<void>();
    }
}
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
QFileInfoList QDir::entryInfoList(Filters filters, SortFlags sort) const
{
    Q_UNUSED(filters);
    Q_UNUSED(sort);

    return Ut_Launcher::desktopFileInfoList;
}

// QFileSystemWatcher stubs
void QFileSystemWatcher::addPath(const QString &)
{
}

void QDBusPendingReplyData::setMetaTypes(int, int const *)
{
}

// LauncherDataStore stubs
LauncherDataStore::LauncherDataStore(MDataStore* dataStore)
{
    Q_UNUSED(dataStore);
}

LauncherDataStore::~LauncherDataStore() { }

void LauncherDataStore::updateLauncherButtons(const QList< QSharedPointer<LauncherPage> > &pages)
{
    Q_UNUSED(pages)
}

QList< QSharedPointer<LauncherPage> > LauncherDataStore::launcherButtons()
{
    return QList< QSharedPointer<LauncherPage> >();
}

LauncherDataStore::EntryLocation LauncherDataStore::location(const MDesktopEntry &entry)
{
    Q_UNUSED(entry);
    return LauncherDataStore::LauncherGrid;
}

QDBusPendingReply<> MApplicationIfProxy::launch()
{
    Ut_Launcher::mApplicationIfProxyLaunchCalled = true;

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
    // clear timer data
    gTimerActive = false;
    gTimerTimeout = 0;
    // clear concurrent run status
    gConcurrentRunCalled = false;
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_launcher";
    app = new MApplication(argc, &app_name);

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
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop", "Test3");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop", "Directory");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop", "Icon-camera");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "Only show in M");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "Icon-m-application");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", "m-application");
    desktopEntryOnlyShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop", QStringList() << "X-MeeGo" << "KDE" << "GNOME");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "Only show in desktops");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", "desktop-application");
    desktopEntryOnlyShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktops.desktop", QStringList() << "KDE" << "GNOME");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInM.desktop", "Not show in M");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInM.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInM.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInM.desktop", "noshow-application");
    desktopEntryNotShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInM.desktop", QStringList() << "X-MeeGo" << "KDE" << "GNOME");
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "Not show in desktops");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", "no-desktop-application");
    desktopEntryNotShowIn.insert(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop", QStringList() << "KDE" << "GNOME");

    // TODO: remove this desktop file when the Category M feature is removed
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryM.desktop", "Category M application");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryM.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryM.desktop", "Icon-camera");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryM.desktop", "m-category-application");
    desktopEntryCategories.insert(QString(APPLICATIONS_DIRECTORY) + "applicationWithOnlyCategoryM.desktop", QStringList() << "M");

    // Test applications that belong to categories
    desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "Test4");
    desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "Application");
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "Icon-music");
    desktopEntryExec.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", "test4");
    desktopEntryCategories.insert(QString(APPLICATIONS_DIRECTORY) + "applicationInCategory.desktop", QStringList() << "somecategory" << "TestCat1");

    // Incomplete files
    desktopEntryIcon.insert(QString(APPLICATIONS_DIRECTORY) + "test5.desktop", "Icon-music");

    // Filler app files
    for (int i = 0; i < 20; i++) {
        desktopEntryName.insert(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication%1.desktop").arg(i), QString("Fill%1").arg(i));
        desktopEntryType.insert(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication%1.desktop").arg(i), "Application");
    }
}

void Ut_Launcher::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_Launcher::init()
{
    // Create a launcher and connect the signals
    launcher = new Launcher();
    gButtonTimer = &(launcher->updateButtonsTimer);
    connect(this, SIGNAL(directoryChanged(const QString)), launcher, SLOT(updateButtonList()));
    connect(this, SIGNAL(applicationLaunched(const QString)), launcher, SLOT(launchApplication(const QString)));
    connect(this, SIGNAL(mApplicationLaunched(const QString)), launcher, SLOT(launchMApplication(const QString)));
    connect(this, SIGNAL(timerTimedOut()), launcher, SLOT(startButtonStoreThread()));

    // No files by default
    desktopFileInfoList.clear();
    directoryFileInfoList.clear();
    applicationStarted.clear();
    mApplicationIfProxyLaunchCalled = false;
}

void Ut_Launcher::cleanup()
{
    // Destroy launcher
    delete launcher;
}

int Ut_Launcher::buttonsCount()
{
    QList< QSharedPointer<LauncherPage> > pages(launcher->model()->launcherPages());
    int count = 0;
    foreach (QSharedPointer<LauncherPage> page, pages) {
        count += page.data()->model()->launcherButtons().count();
    }
return count;
}

void Ut_Launcher::testInitialization()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->setEnabled(true);

    QCOMPARE(buttonsCount(), desktopFileInfoList.count());

   // The launcherButtons should be LauncherButtons
    for (int i = 0; i < desktopFileInfoList.count(); i++) {
        QSharedPointer<LauncherButton> b = launcher->model()->launcherPages().at(0)->model()->launcherButtons().at(i);

        QVERIFY(b != NULL);

        // Check name and icon for all entries
        QCOMPARE(b->text(), QString().sprintf("Test%d", i));
        QCOMPARE(b->iconID(), QString("Icon-camera"));

        // Check target type - the first two are Applications
        switch (i) {
        case 0:
            QCOMPARE(b->targetType(), QString("Application"));
            break;
        case 1: {
            QCOMPARE(b->targetType(), QString("Service"));
            QCOMPARE(b->target(), QString("com.nokia.test1"));
        }
        break;
        default:
            break;
        }
    }
}

void Ut_Launcher::testButtonStoreTimerAndThread()
{
    // Initalize launcher
    launcher->setEnabled(true);
    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);
    QCOMPARE(gTimerActive, true);
    QCOMPARE(gTimerTimeout, DATASTORE_WRITE_TIMEOUT);
    emit timerTimedOut();
    QCOMPARE(gConcurrentRunCalled, true);
}

void Ut_Launcher::testOnlyShowInDUI()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDUI.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->setEnabled(true);

    // There should be three launcherButtons in the view
    QCOMPARE(buttonsCount(), desktopFileInfoList.count());

    QSharedPointer<LauncherButton> b = launcher->model()->launcherPages().at(0)->model()->launcherButtons().at(0);
    QVERIFY(b != NULL);

    // Check name and icon for all entries
    QCOMPARE(b->text(), QString("Only show in M"));
    QCOMPARE(b->iconID(), QString("Icon-m-application"));
    QCOMPARE(b->targetType(), QString("Application"));
    QCOMPARE(b->target(), QString("m-application"));
}

void Ut_Launcher::testOnlyShowInNotDUI()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationOnlyShowInDesktop.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->setEnabled(true);

    // There should be no launcherButtons
    QCOMPARE(buttonsCount(), 0);
}

void Ut_Launcher::testNotShowInDUI()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInM.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->setEnabled(true);

    // There should be no launcherButtons
    QCOMPARE(buttonsCount(), 0);
}

void Ut_Launcher::testNotShowInNotDUI()
{
    // Add some desktop files in the root category
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "applicationNotShowInDesktop.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Initalize launcher
    launcher->setEnabled(true);

    // There should be one widget
    QCOMPARE(buttonsCount(), desktopFileInfoList.count());

    QSharedPointer<LauncherButton> b = launcher->model()->launcherPages().at(0)->model()->launcherButtons().at(0);
    QVERIFY(b != NULL);

    // Check name and icon for all entries
    QCOMPARE(b->text(), QString("Not show in desktops"));
    QCOMPARE(b->iconID(), QString("Icon-camera"));
    QCOMPARE(b->targetType(), QString("Application"));
    QCOMPARE(b->target(), QString("no-desktop-application"));
}

void Ut_Launcher::testDesktopEntryAdd()
{
    // Initalize launcher
    launcher->setEnabled(true);

    // There should be no launcherButtons in the view
    QCOMPARE(buttonsCount(), 0);

    // Add a desktop file in the "desktop file directory"
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be one widget in the view
    QCOMPARE(buttonsCount(), 1);

    // The widget should be a LauncherButton
    QSharedPointer<LauncherButton> b = launcher->model()->launcherPages().at(0)->model()->launcherButtons().at(0);

    QVERIFY(b != NULL);

    // Check name, icon, target and type
    QCOMPARE(b->text(), QString("Test0"));
    QCOMPARE(b->iconID(), QString("Icon-camera"));
    QCOMPARE(b->target(), QString("test0"));
    QCOMPARE(b->targetType(), QString("Application"));
}

void Ut_Launcher::testInvalidFiles()
{
    // Test adding files which should be considered invalid for some reason

    launcher->setEnabled(true);

    // There should be no launcherButtons in the root
    QCOMPARE(buttonsCount(), 0);

    // Entry is of type Directory, which shouldn't be added
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "directoryInApplicationsDirectory.desktop"));

    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be no launcherButtons in the root
    QCOMPARE(buttonsCount(), 0);

    // Entry is missing name, shouldn't be added
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "test5.desktop"));

    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be no launcherButtons in the root
    QCOMPARE(buttonsCount(), 0);
}

void Ut_Launcher::testDesktopEntryRemove()
{
    // Add a desktop file in the "desktop file directory"
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    // Initalize launcher
    launcher->setEnabled(true);

    // There should be one widget in the root
    QCOMPARE(buttonsCount(), desktopFileInfoList.count());

    // "Delete" the desktop file
    desktopFileInfoList.clear();

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // There should be no launcherButtons in the root
    QCOMPARE(buttonsCount(), 0);
}

void Ut_Launcher::testApplicationLaunched()
{
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "regularApplication.desktop"));

    launcher->setEnabled(true);

    emit applicationLaunched("test0");

    QCOMPARE(applicationStarted, QString("test0"));
}

void Ut_Launcher::testMApplicationLaunched()
{
    desktopFileInfoList.append(QFileInfo(QString(APPLICATIONS_DIRECTORY) + "xMaemoApplication.desktop"));

    launcher->setEnabled(true);

    emit mApplicationLaunched("com.nokia.test1");

    QCOMPARE(mApplicationIfProxyLaunchCalled, true);
}

void Ut_Launcher::testPaging()
{
    launcher->setEnabled(true);

    for (int i = 0; i < 15; i++) {
        desktopFileInfoList.append(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication%1.desktop").arg(i));
    }

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    QCOMPARE(buttonsCount(), desktopFileInfoList.count());
    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0).data()->model()->launcherButtons().count(), 12);
    QCOMPARE(launcher->model()->launcherPages().at(1).data()->model()->launcherButtons().count(), 3);

}

void Ut_Launcher::testEmptyPage()
{
    launcher->setEnabled(true);

    for (int i = 0; i < 15; i++) {
        desktopFileInfoList.append(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication%1.desktop").arg(i));
    }

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    desktopFileInfoList.clear();
    for (int i = 0; i < 12; i++) {
        desktopFileInfoList.append(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication%1.desktop").arg(i));
    }

    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    QCOMPARE(launcher->model()->launcherPages().count(), 1);
}

void Ut_Launcher::testAddingAndRemovingButtonsWithMultiplePages()
{
    launcher->setEnabled(true);

    for (int i = 0; i < 15; i++) {
        desktopFileInfoList.append(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication%1.desktop").arg(i));
    }
    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    // make a space for 1st page
    desktopFileInfoList.removeOne(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication11.desktop"));
    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0).data()->model()->launcherButtons().count(), 11);
    QCOMPARE(launcher->model()->launcherPages().at(1).data()->model()->launcherButtons().count(), 3);

    // add new button and verify it was added to last page
    desktopFileInfoList.append(QString(APPLICATIONS_DIRECTORY) + QString("fillerApplication15.desktop"));
    // Process events to make sure all notifications are done
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    // Fake a directory change notification
    emit directoryChanged(APPLICATIONS_DIRECTORY);

    QCOMPARE(launcher->model()->launcherPages().count(), 2);
    QCOMPARE(launcher->model()->launcherPages().at(0).data()->model()->launcherButtons().count(), 11);
    QCOMPARE(launcher->model()->launcherPages().at(1).data()->model()->launcherButtons().count(), 4);
}

QTEST_APPLESS_MAIN(Ut_Launcher)
