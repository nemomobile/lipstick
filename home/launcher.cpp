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

#include <QFlags>
#include <QDir>
#include <DuiApplicationIfProxy>
#include <DuiDesktopEntry>
#include "duifiledatastore.h"
#include "launcherdatastore.h"

#include "launcher.h"
#include "launcherbutton.h"

static const char* const FILE_FILTER = "*.desktop";

Launcher::Launcher(DuiWidget *parent) :
    DuiWidgetController(new LauncherModel, parent),
    dataStore(NULL),
    initialized(false)
{
    pathsForDesktopEntries << APPLICATIONS_DIRECTORY;
#ifdef TESTABILITY_ON
    pathsForDesktopEntries << QDir::tempPath();
#endif
    supportedDesktopEntryFileTypes << "Application";
}

Launcher::~Launcher()
{
    delete dataStore;
}

void Launcher::activateLauncher()
{
    if (!initialized) {

        if (!QDir::root().exists(QDir::homePath() + "/.config/duihome")) {
            QDir::root().mkpath(QDir::homePath() + "/.config/duihome");
        }
	
	DuiFileDataStore* backendStore = new DuiFileDataStore(QDir::homePath() + "/.config/duihome/launcherbuttons.data");

        dataStore = new LauncherDataStore(backendStore);

        // restore previous button order from datastore
        restoreButtonsFromDataStore();

        // Update the button list according to watched directories
        updateButtonList();

        // Start watching the applications directory for changes
        connect(&watcher, SIGNAL(directoryChanged(const QString)), this, SLOT(updateButtonListFromDirectory(const QString)));
        foreach (const QString& dir, pathsForDesktopEntries) {
            watcher.addPath(dir);
        }
        // The launcher has now been initialized
        initialized = true;
    }
}

void Launcher::updateButtonList()
{
    // this updates for all paths
    updateButtonListFromEntries(pathsForDesktopEntries,
                                pathsForDesktopEntries,
                                FILE_FILTER,
                                supportedDesktopEntryFileTypes
                                );
}

void Launcher::updateButtonListFromDirectory(const QString &path)
{
    // this updates for path whose directory changed
    updateButtonListFromEntries(QStringList() << path,
                                pathsForDesktopEntries,
                                FILE_FILTER,
                                supportedDesktopEntryFileTypes);
}

void Launcher::updateButtonListFromEntries(const QStringList &modifiedPaths,
                                           const QStringList &allPaths,
                                           const QString &nameFilter,
                                           const QStringList &acceptedTypes)
{
    QStringList desktopEntryFiles;
    // Update buttons according to the new desktop entries
    foreach(const QString& path, modifiedPaths) {
        foreach(QFileInfo fileInfo, QDir(path, nameFilter).entryInfoList(QDir::Files)) {
            QString filePath(fileInfo.absoluteFilePath());
            DuiDesktopEntry e(filePath);
            if (isDesktopEntryValid(e, acceptedTypes)) {
                desktopEntryFiles.append(filePath);

                // If the entry is not in the launcher we might need to add it
                if (!contains(e)) {
                    // If the data store does not know the item, we put it in the laucher grid as the items
                    // go by default into the laucher grid or
                    // if the data store already says that it goes into the grid, then lets put it there
                    if (dataStore->location(e) == LauncherDataStore::Unknown ||
                        dataStore->location(e) == LauncherDataStore::LauncherGrid) {
                        addNewLauncherButton(e);
                    }
                }
            }
        }
    }

    QList< QSharedPointer<LauncherPage> > pages(model()->launcherPages());
    foreach (QSharedPointer<LauncherPage> page, pages) {
        // prune pages and remove empty pages
        if (!page->prune(desktopEntryFiles, allPaths)) {
            pages.removeOne(page);
        }
    }
    model()->setLauncherPages(pages);

    updateButtonsInDataStore();
}


bool Launcher::contains(const DuiDesktopEntry &entry)
{
    QList< QSharedPointer<LauncherPage> > pages(model()->launcherPages());

    bool containsButton = false;
    foreach (QSharedPointer<LauncherPage> page, pages) {
        containsButton = page.data()->contains(entry);
        if(containsButton) {
            break;
        }
    }
    return containsButton;
}

void Launcher::addNewLauncherButton(const DuiDesktopEntry &entry)
{
    QList< QSharedPointer<LauncherPage> > pages(model()->launcherPages());
    QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton>(createLauncherButton(entry));

    bool added = false;
    if (!pages.isEmpty()) {
	QSharedPointer<LauncherPage> page = pages.last();
	added = page.data()->appendButton(button);
    }

    if (!added) {
	QList< QSharedPointer<LauncherPage> > newPages(pages);
        QSharedPointer<LauncherPage> newPage = QSharedPointer<LauncherPage>(new LauncherPage());
        newPage.data()->appendButton(button);
        newPages.append(newPage);
        model()->setLauncherPages(newPages);
    }
}

bool Launcher::isDesktopEntryValid(const DuiDesktopEntry &entry, const QStringList &acceptedTypes)
{
    return (entry.isValid() && acceptedTypes.contains(entry.type())) &&
	(entry.onlyShowIn().count() == 0 || entry.onlyShowIn().contains("X-DUI")) &&
	(entry.notShowIn().count() == 0 || !entry.notShowIn().contains("X-DUI"));
}

LauncherButton *Launcher::createLauncherButton(const DuiDesktopEntry &entry)
{
    LauncherButton *launcherButton = new LauncherButton(entry);
    connectLauncherButton(launcherButton);
    return launcherButton;
}

void Launcher::launchApplication(const QString &application)
{
    startApplication(application);
}

void Launcher::launchDuiApplication(const QString &serviceName)
{
    startDuiApplication(serviceName);
}

void Launcher::setEnabled(bool enabled)
{
    QGraphicsItem::setEnabled(enabled);
    if (enabled) {
        activateLauncher();
    }
}

bool Launcher::startApplication(const QString &application)
{
    if (!QProcess::startDetached(application)) {
        qWarning() << "Failed to start application:" << application;
        return false;
    } else {
        qDebug() << "Started application succesfully:" << application;
        return true;
    }
}

bool Launcher::startDuiApplication(const QString &serviceName)
{
    qDebug() << "Attempting to launch " << serviceName;

    DuiApplicationIfProxy duiApplicationIfProxy(serviceName, NULL);

    if (duiApplicationIfProxy.connection().isConnected()) {
        qDebug() << "Launching " << serviceName;
        duiApplicationIfProxy.launch();
        return true;
    } else {
        qWarning() << "Could not launch" << serviceName;
        qWarning() << "DBus not connected?";
        return false;
    }
}

void Launcher::updateButtonsInDataStore()
{
    dataStore->updateLauncherButtons(model()->launcherPages());
}

void Launcher::restoreButtonsFromDataStore()
{
    QStringList acceptedTypes = (QStringList() << "Application");
    QList< QSharedPointer<LauncherPage> > restoredPages(dataStore->launcherButtons());

    foreach (QSharedPointer<LauncherPage> page, restoredPages) {
	foreach (QSharedPointer<LauncherButton> button, page.data()->model()->launcherButtons()) {
	    connectLauncherButton(button.data());
	}
    }
    model()->setLauncherPages(restoredPages);
}

void Launcher::connectLauncherButton(LauncherButton* launcherButton)
{
    launcherButton->setObjectName("LauncherButton");
    connect(launcherButton, SIGNAL(applicationLaunched(const QString &)), this, SLOT(launchApplication(const QString &)), Qt::QueuedConnection);
    connect(launcherButton, SIGNAL(duiApplicationLaunched(const QString &)), this, SLOT(launchDuiApplication(const QString &)), Qt::QueuedConnection);
    connect(launcherButton, SIGNAL(clicked()), this, SIGNAL(launcherButtonClicked()));
}
