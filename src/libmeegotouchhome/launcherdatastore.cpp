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
#include "homefiledatastore.h"

static const QString KEY_PREFIX = "DesktopEntries";
static const char* const FILE_FILTER = "*.desktop";

static const QString DESKTOP_ENTRY_TYPE_KEY = "Desktop Entry/Type";
static const QString DESKTOP_ONLY_SHOW_IN_KEY = "Desktop Entry/OnlyShowIn";
static const QString DESKTOP_NOT_SHOW_IN_KEY = "Desktop Entry/NotShowIn";

LauncherDataStore::LauncherDataStore(MDataStore* dataStore, const QStringList &directories) :
        store(dataStore)
{
    // Set up the supported desktop entry types
    supportedDesktopEntryFileTypes << "Application" << "Link";

    // Take only valid directories that exist into account
    foreach (const QString &directory, directories) {
        QFileInfo fileInfo(directory);
        if (fileInfo.exists() && fileInfo.isDir()) {
            this->directories.append(fileInfo.canonicalFilePath());
        }
    }

    // Start watching the applications directory for changes
    connect(&watcher, SIGNAL(directoryChanged(const QString)), this, SLOT(updateDesktopEntryFiles()));
    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateDesktopEntry(QString)));
    foreach (const QString &directoryPath, this->directories) {
        watcher.addPath(directoryPath);
    }
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

void LauncherDataStore::updateDataForDesktopEntry(const QString &entryPath, const QVariant &data)
{
    // Disconnect listening store changes during the store is updated.
    disconnect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));

    // Update the data store
    store->createValue(entryPathToKey(entryPath), data);

    // Emit a dataStoreChanged() signal if something changes in the data store
    connect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));
}

void LauncherDataStore::updateDataForDesktopEntries(const QHash<QString, QString> &newValues)
{
    // Disconnect listening store changes during the store is updated.
    disconnect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));

    // If data store is HomeFileDataStore then store values with createValues() method which does only one sync
    HomeFileDataStore *fileDataStore = dynamic_cast<HomeFileDataStore *>(store);
    if (fileDataStore) {
        QHash<QString, QVariant> newStoreValues;
        foreach (const QString &entryPath, newValues.keys()) {
            newStoreValues.insert(entryPathToKey(entryPath), newValues.value(entryPath));
        }
        fileDataStore->createValues(newStoreValues);
    } else {
        foreach (const QString &entryPath, newValues.keys()) {
            store->createValue(entryPathToKey(entryPath), newValues.value(entryPath));
        }
    }

    // Emit a dataStoreChanged() signal if something changes in the data store
    connect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));
}

void LauncherDataStore::removeDataForDesktopEntry(const QString &entryPath)
{
    // Disconnect listening store changes during the store is updated.
    disconnect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));

    // Remove value from the data store
    store->remove(entryPathToKey(entryPath));

    // Emit a dataStoreChanged() signal if something changes in the data store
    connect(store, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(dataStoreChanged()));
}

void LauncherDataStore::updateDesktopEntryFiles()
{
    QFileInfoList updateQueue;
    foreach (const QString &directoryPath, directories) {
        updateQueue.append(QDir(directoryPath, FILE_FILTER).entryInfoList(QDir::Files));
    }

    QSet<QSharedPointer<MDesktopEntry> > addedEntries;
    QHash<QString, QString> addedEntriesData;
    while (!updateQueue.isEmpty()) {
        QFileInfo fileInfo = updateQueue.takeFirst();
        QString desktopEntryPath(fileInfo.absoluteFilePath());
        QString key = entryPathToKey(desktopEntryPath);

        // Add an entry if it is not yet invalidated or added to store
        if (!invalidEntries.contains(desktopEntryPath)) {
            if (!store->contains(key)) {
                if (isDesktopEntryValid(desktopEntryPath, supportedDesktopEntryFileTypes)) {
                    QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(desktopEntryPath));
                    // Add the new entry with an unknown location
                    addedEntriesData.insert(desktopEntryPath, QString());
                    addedEntries.insert(desktopEntry);
                } else {
                    invalidEntries.append(desktopEntryPath);
                }
            } else {
                // Check that entry found already from store is still valid, if not then remove
                if (!isDesktopEntryValid(desktopEntryPath, supportedDesktopEntryFileTypes)) {
                    removeDataForDesktopEntry(desktopEntryPath);
                    emit desktopEntryRemoved(desktopEntryPath);
                    invalidEntries.append(desktopEntryPath);
                }
            }

            addFilePathToWatcher(desktopEntryPath);
        }
    }

    // Add new entries to store and emit signals about the additions
    updateDataForDesktopEntries(addedEntriesData);
    foreach (const QSharedPointer<MDesktopEntry> &desktopEntry, addedEntries) {
        emit desktopEntryAdded(desktopEntry);
    }

    // When the update queue has been processed remove all non-existent desktop entries
    QStringList allKeys(store->allKeys());
    foreach (const QString &key, allKeys) {
        QString desktopEntryPath = keyToEntryPath(key);
        if (!key.startsWith(KEY_PREFIX) || !QFileInfo(desktopEntryPath).exists()) {
            removeDataForDesktopEntry(desktopEntryPath);
            emit desktopEntryRemoved(desktopEntryPath);
        }
    }

    // Emit a dataStoreChanged() signal since the contents have changed
    emit dataStoreChanged();
}

bool LauncherDataStore::isDesktopEntryValid(const QString &entryPath, const QStringList &acceptedTypes)
{
    bool valid = false;
    QFile file(entryPath);

    // Checks if the file exists and opens in readonly mode
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QMap<QString, QString> desktopEntryData;
        if (MDesktopEntry::readDesktopFile(file, desktopEntryData)) {
            valid = acceptedTypes.contains(desktopEntryData.value(DESKTOP_ENTRY_TYPE_KEY))
                    && (!desktopEntryData.contains(DESKTOP_ONLY_SHOW_IN_KEY)
                        || desktopEntryData.value(DESKTOP_ONLY_SHOW_IN_KEY).contains("X-MeeGo"))
                    && (!desktopEntryData.contains(DESKTOP_NOT_SHOW_IN_KEY)
                        || !desktopEntryData.value(DESKTOP_NOT_SHOW_IN_KEY).contains("X-MeeGo"));
        }
    }
    file.close();

    return valid;
}

QString LauncherDataStore::entryPathToKey(const QString &entryPath)
{
    // add key prefix to the entry path
    return QString(KEY_PREFIX + entryPath);
}

QString LauncherDataStore::keyToEntryPath(const QString &key)
{
    // remove key prefix from the key if it is found
    return QString(key).remove(KEY_PREFIX);
}

void LauncherDataStore::addFilePathToWatcher(const QString &filePath)
{
    if (!watcher.files().contains(filePath)) {
        watcher.addPath(filePath);
    }
}

void LauncherDataStore::updateDesktopEntry(const QString &desktopEntryPath)
{
    QString fullPath;
    if (desktopEntryPath.startsWith('/')) {
        // The path is an absolute path: use the path as is
        fullPath = desktopEntryPath;
    } else {
        // The path is not an absolute path: try to find the desktop entry from the watched directories
        foreach (const QString &directory, directories) {
            QFileInfo fileInfo(directory, desktopEntryPath);
            if (fileInfo.exists()) {
                // Use the first existing file
                fullPath = fileInfo.canonicalFilePath();
                break;
            }
        }
    }

    QFileInfo fileInfo(fullPath);
    if (fileInfo.exists() && directories.contains(fileInfo.canonicalPath())) {
        QString key = entryPathToKey(fullPath);

        if (store->contains(key)) {
            if (isDesktopEntryValid(fullPath, supportedDesktopEntryFileTypes)) {
                QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(fullPath));
                // update valid & existing entry
                emit desktopEntryChanged(desktopEntry);
            } else {
                // remove existing but now invalid entry
                store->remove(key);
                emit desktopEntryRemoved(fullPath);
                invalidEntries.append(fullPath);
            }
        } else {
            if (isDesktopEntryValid(fullPath, supportedDesktopEntryFileTypes)) {
                QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(fullPath));
                // If entry has been invalid before, but is now valid, add entry as a new entry
                store->createValue(key, QVariant());
                emit desktopEntryAdded(desktopEntry);
                invalidEntries.removeOne(fullPath);
            }
        }
    }
}
