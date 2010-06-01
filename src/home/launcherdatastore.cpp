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

#include <QDir>
#include <QFile>
#include <MDesktopEntry>
#include <MDataStore>
#include "launcherpage.h"
#include "launcherbutton.h"
#include "launcherdatastore.h"

static const QString KEY_PREFIX = "DesktopEntries";
static const char* const FILE_FILTER = "*.desktop";
static const int FILES_PROCESSED_AT_ONCE = 3;

LauncherDataStore::LauncherDataStore(MDataStore* dataStore) :
        store(dataStore),
        updatePending(false)
{
    connect(&processUpdateQueueTimer, SIGNAL(timeout()), this, SLOT(processUpdateQueue()));
    processUpdateQueueTimer.setSingleShot(true);
    processUpdateQueueTimer.setInterval(0);

    supportedDesktopEntryFileTypes << "Application";
    updateDataFromDesktopEntryFiles();

    // Start watching the applications directory for changes
    connect(&watcher, SIGNAL(directoryChanged(const QString)), this, SLOT(updateDataFromDesktopEntryFiles()));
    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateDesktopEntry(QString)));
    watcher.addPath(APPLICATIONS_DIRECTORY);
}

LauncherDataStore::~LauncherDataStore()
{
    delete store;
}

QHash<QString, QVariant> LauncherDataStore::dataForAllDesktopEntries()
{
    QHash<QString, QVariant> data;
    foreach (const QString &key, store->allKeys()) {
        // Ignore any entries not withing the "DesktopEntries" section
        if (key.indexOf(KEY_PREFIX) != 0)
            continue;
        data.insert(keyToEntryPath(key), store->value(key));
    }
    return data;
}

bool LauncherDataStore::updateDataForDesktopEntry(const QString &entryPath, const QVariant &data)
{
    QString key = entryPathToKey(entryPath);
    if (store->contains(key)) {
        // Disconnect the dataStoreChanged() signal connection during updates
        store->disconnect(this);

        // Update the data store
        store->createValue(key, data);

        // Emit a dataStoreChanged() signal if something changes in the data store
        connect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));
        return true;
    } else {
        return false;
    }
}

void LauncherDataStore::updateDataFromDesktopEntryFiles()
{
    if (updateQueue.isEmpty()) {
        // Only start an update if one isn't already in progress
        startProcessingUpdateQueue();
    } else {
        // An update is already in progress but a new one has been requested: raise the update pending flag
        updatePending = true;
    }
}

void LauncherDataStore::startProcessingUpdateQueue()
{
    updatePending = false;
    updateQueue = QDir(APPLICATIONS_DIRECTORY, FILE_FILTER).entryInfoList(QDir::Files);
    updateValidKeys.clear();
    if (!updateQueue.isEmpty()) {
        processUpdateQueueTimer.start();
    }
}

void LauncherDataStore::processUpdateQueue()
{
    if (updateQueue.isEmpty()) {
        // If the update queue is empty do nothing
        return;
    }

    // Disconnect the dataStoreChanged() signal connection during updates
    store->disconnect(this);

    for (int i = 0; i < FILES_PROCESSED_AT_ONCE && !updateQueue.isEmpty(); i++) {
        QFileInfo fileInfo = updateQueue.takeFirst();
        QString desktopEntryPath(fileInfo.absoluteFilePath());
        QString key = entryPathToKey(desktopEntryPath);

        if (!store->contains(key)) {
            MDesktopEntry desktopEntry(desktopEntryPath);
            if (isDesktopEntryValid(desktopEntry, supportedDesktopEntryFileTypes)) {
                // Add the entry with an unknown location
                store->createValue(key, QVariant());
                // Keep track of all valid desktop entries
                updateValidKeys.insert(key);

                emit desktopEntryAdded(desktopEntryPath);
            }
        } else {
            // Keep track of all valid desktop entries (consider already added desktop entries to be valid)
            updateValidKeys.insert(key);
        }

        addFilePathToWatcher(desktopEntryPath);
    }

    if (updateQueue.isEmpty()) {
        // When the update queue has been processed remove all desktop entries that are not valid
        QStringList allKeys(store->allKeys());
        foreach (const QString &key, allKeys) {
            if (!updateValidKeys.contains(key)) {
                store->remove(key);
                emit desktopEntryRemoved(keyToEntryPath(key));
            }
        }

        // Emit a dataStoreChanged() signal since the contents have changed
        emit dataStoreChanged();

        if (updatePending) {
            // If another update is pending start processing the queue again
            startProcessingUpdateQueue();
        }
    } else {
        // There are still files in the queue: restart the timer
        processUpdateQueueTimer.start();
    }

    // Emit a dataStoreChanged() signal if something changes in the data store during runtime
    connect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));
}

bool LauncherDataStore::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes)
{
    return (entry.isValid() && acceptedTypes.contains(entry.type())) &&
    (entry.onlyShowIn().count() == 0 || entry.onlyShowIn().contains("X-DUI") || entry.onlyShowIn().contains("X-MeeGo")) &&
    (entry.notShowIn().count() == 0 || !(entry.notShowIn().contains("X-DUI") || entry.notShowIn().contains("X-MeeGo")));
}

QString LauncherDataStore::entryPathToKey(QString entryPath)
{
    // add key prefix to the entry path
    return QString(KEY_PREFIX + entryPath);
}

QString LauncherDataStore::keyToEntryPath(QString key)
{
    // remove key prefix from the key if if is found
    if (key.indexOf(KEY_PREFIX) == 0) {
        return key.replace(0, KEY_PREFIX.length(), "");
    }

    return key;
}

void LauncherDataStore::addFilePathToWatcher(const QString &filePath)
{
    if (!watcher.files().contains(filePath)) {
        watcher.addPath(filePath);
    }
}

void LauncherDataStore::updateDesktopEntry(const QString &desktopEntryPath)
{
    if (QFile::exists(desktopEntryPath)) {
        QString key = entryPathToKey(desktopEntryPath);

        if (store->contains(key)) {
            MDesktopEntry desktopEntry(desktopEntryPath);
            if (isDesktopEntryValid(desktopEntry, supportedDesktopEntryFileTypes)) {
                // update valid & existing entry
                emit desktopEntryChanged(desktopEntryPath);
            } else {
                // remove existing but now invalid entry
                updateValidKeys.remove(key);
                store->remove(key);
                emit desktopEntryRemoved(keyToEntryPath(key));
            }
        } else if (!isInQueue(key)) {
            MDesktopEntry desktopEntry(desktopEntryPath);
            if (isDesktopEntryValid(desktopEntry, supportedDesktopEntryFileTypes)) {
                // if entry has been invalid before, but is now valid, add entry as a new entry
                store->createValue(key, QVariant());
                updateValidKeys.insert(key);

		emit desktopEntryAdded(desktopEntryPath);
            }
        }
    }
}

bool LauncherDataStore::isInQueue(const QString &key)
{
    bool contains = false;
    foreach (const QFileInfo &fileInfo, updateQueue) {
        contains = (fileInfo.absolutePath() == keyToEntryPath(key));
        if (contains)
            break;
    }
    return contains;
}
