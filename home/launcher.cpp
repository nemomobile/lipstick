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

#include <QFlags>
#include <QDir>
#include <QMutexLocker>
#include <QtConcurrentRun>
#include <MApplicationIfProxy>
#include <MDesktopEntry>
#include "mfiledatastore.h"
#include "launcherdatastore.h"

#include "launcher.h"
#include "launcherbutton.h"

static const char* const FILE_FILTER = "*.desktop";
static const int DATASTORE_WRITE_TIMEOUT = 500;

Launcher::Launcher(MWidget *parent) :
    MWidgetController(new LauncherModel, parent),
    dataStore_(NULL),
    initialized(false),
    updateButtonsMutex(QMutex::Recursive)
{
    supportedDesktopEntryFileTypes << "Application";
}

Launcher::~Launcher()
{
    delete dataStore_;
}

void Launcher::activateLauncher()
{
    if (!initialized) {
        // restore previous button order from datastore
        restoreButtonsFromDataStore();

        // Update the button list according to watched directories
        updateButtonList();


        // Start watching the applications directory for changes
        connect(&watcher, SIGNAL(directoryChanged(const QString)), this, SLOT(updateButtonList()));
        watcher.addPath(APPLICATIONS_DIRECTORY);
        connect(&updateButtonsTimer, SIGNAL(timeout()),
                this, SLOT(startButtonStoreThread()));
        // The launcher has now been initialized
        initialized = true;
    }
}

LauncherDataStore *Launcher::dataStore()
{
    if(dataStore_ == NULL) {
        if (!QDir::root().exists(QDir::homePath() + "/.config/duihome")) {
            QDir::root().mkpath(QDir::homePath() + "/.config/duihome");
        }

        QString dataStoreFileName = QDir::homePath() + "/.config/duihome/launcherbuttons.data";

        if (!QFile::exists(dataStoreFileName)) {
            QString defaultDataStoreFileName = M_XDG_DIR "/duihome/launcherbuttons.data";
            // Copy the default datastore only if it exists
            if (QFile::exists(defaultDataStoreFileName)) {
                QFile::copy(defaultDataStoreFileName, dataStoreFileName);
            }
        }

        MFileDataStore* backendStore = new MFileDataStore(dataStoreFileName);

        dataStore_ = new LauncherDataStore(backendStore);
    }

    return dataStore_;
}


void Launcher::updateButtonList()
{
    QStringList desktopEntryFiles;
    // Update buttons according to the new desktop entries
    foreach(QFileInfo fileInfo, QDir(APPLICATIONS_DIRECTORY, FILE_FILTER).entryInfoList(QDir::Files)) {
        QString filePath(fileInfo.absoluteFilePath());
        // If the entry is not in the launcher we might need to add it
        if (!contains(filePath)) {
            MDesktopEntry e(filePath);
            if (isDesktopEntryValid(e, supportedDesktopEntryFileTypes)) {
                desktopEntryFiles.append(filePath);

                // If the data store does not know the item, we put it in the laucher grid as the items
                // go by default into the laucher grid or
                // if the data store already says that it goes into the grid, then lets put it there
                LauncherDataStore::EntryLocation entryLocationInDataStore =
                    dataStore()->location(e);
                if (entryLocationInDataStore == LauncherDataStore::Unknown
                    || entryLocationInDataStore ==
                    LauncherDataStore::LauncherGrid) {
                    addNewLauncherButton(e);
                }
            }
        } else {
            desktopEntryFiles.append(filePath);
        }
    }

    QList< QSharedPointer<LauncherPage> > pages(model()->launcherPages());
    foreach (QSharedPointer<LauncherPage> page, pages) {
        // prune pages and remove empty pages
        if (!page->prune(desktopEntryFiles)) {
            pages.removeOne(page);
        }
    }
    model()->setLauncherPages(pages);
    updateButtonsTimer.setSingleShot(true);
    updateButtonsTimer.start(DATASTORE_WRITE_TIMEOUT);
}

bool Launcher::contains(const QString &desktopEntryFile)
{
    QList< QSharedPointer<LauncherPage> > pages(model()->launcherPages());

    bool containsButton = false;
    foreach (QSharedPointer<LauncherPage> page, pages) {
        containsButton = page->contains(desktopEntryFile);
        if(containsButton) {
            break;
        }
    }
    return containsButton;
}

void Launcher::addNewLauncherButton(const MDesktopEntry &entry)
{
    QList< QSharedPointer<LauncherPage> > pages(model()->launcherPages());
    QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton>(createLauncherButton(entry));

    bool added = false;
    if (!pages.isEmpty()) {
	QSharedPointer<LauncherPage> page = pages.last();
	added = page->appendButton(button);
    }

    if (!added) {
	QList< QSharedPointer<LauncherPage> > newPages(pages);
        QSharedPointer<LauncherPage> newPage = QSharedPointer<LauncherPage>(new LauncherPage());
        newPage->appendButton(button);
        newPages.append(newPage);
        model()->setLauncherPages(newPages);
    }
}

bool Launcher::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes)
{
    return (entry.isValid() && acceptedTypes.contains(entry.type())) &&
	(entry.onlyShowIn().count() == 0 || entry.onlyShowIn().contains("X-DUI") || entry.onlyShowIn().contains("X-MeeGo")) &&
	(entry.notShowIn().count() == 0 || !(entry.notShowIn().contains("X-DUI") || entry.notShowIn().contains("X-MeeGo")));
}

LauncherButton *Launcher::createLauncherButton(const MDesktopEntry &entry)
{
    LauncherButton *launcherButton = new LauncherButton(entry);
    connectLauncherButton(launcherButton);
    return launcherButton;
}

void Launcher::launchApplication(const QString &application)
{
    startApplication(application);
}

void Launcher::launchMApplication(const QString &serviceName)
{
    startMApplication(serviceName);
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

bool Launcher::startMApplication(const QString &serviceName)
{
    qDebug() << "Attempting to launch " << serviceName;

    MApplicationIfProxy mApplicationIfProxy(serviceName, NULL);

    if (mApplicationIfProxy.connection().isConnected()) {
        qDebug() << "Launching " << serviceName;
        mApplicationIfProxy.launch();
        return true;
    } else {
        qWarning() << "Could not launch" << serviceName;
        qWarning() << "DBus not connected?";
        return false;
    }
}

void Launcher::updateButtonsInDataStore()
{
    QMutexLocker guard(&updateButtonsMutex);
    // disconnect the signal first so that change notifications don't
    // happen when updating
    disconnect(dataStore_, SIGNAL(dataStoreChanged()),
               this, SLOT(restoreButtonsFromDataStore()));
    dataStore()->updateLauncherButtons(model()->launcherPages());
    // Listen for changes in ordering data
    connect(dataStore_, SIGNAL(dataStoreChanged()),
            this, SLOT(restoreButtonsFromDataStore()));
}

void Launcher::restoreButtonsFromDataStore()
{
    QList< QSharedPointer<LauncherPage> > restoredPages(dataStore()->launcherButtons());

    foreach (QSharedPointer<LauncherPage> page, restoredPages) {
	foreach (QSharedPointer<LauncherButton> button, page->model()->launcherButtons()) {
	    connectLauncherButton(button.data());
	}
    }
    model()->setLauncherPages(restoredPages);
}

void Launcher::startButtonStoreThread()
{
    QtConcurrent::run(this, &Launcher::updateButtonsInDataStore);
}

void Launcher::connectLauncherButton(LauncherButton* launcherButton)
{
    launcherButton->setObjectName("LauncherButton");
    connect(launcherButton, SIGNAL(applicationLaunched(const QString &)), this, SLOT(launchApplication(const QString &)), Qt::QueuedConnection);
    connect(launcherButton, SIGNAL(mApplicationLaunched(const QString &)), this, SLOT(launchMApplication(const QString &)), Qt::QueuedConnection);
    connect(launcherButton, SIGNAL(clicked()), this, SIGNAL(launcherButtonClicked()));
}
