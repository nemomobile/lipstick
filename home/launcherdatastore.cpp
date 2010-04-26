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

#include <QDebug>
#include <QDir>
#include <MDesktopEntry>
#include <MDataStore>
#include "launcherpage.h"
#include "launcherbutton.h"
#include "launcherdatastore.h"

static const QString KEY_PREFIX = "DesktopEntries";
static const char* const FILE_FILTER = "*.desktop";

LauncherDataStore::LauncherDataStore(MDataStore* dataStore) :
        store(dataStore)
{
    supportedDesktopEntryFileTypes << "Application";
    updateDataFromDesktopEntryFiles();

    // Start watching the applications directory for changes
    connect(&watcher, SIGNAL(directoryChanged(const QString)), this, SLOT(updateDataFromDesktopEntryFiles()));
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
    // Disconnect the dataStoreChanged() signal connection during updates
    store->disconnect(this);

    QStringList validKeys;
    foreach(QFileInfo fileInfo, QDir(APPLICATIONS_DIRECTORY, FILE_FILTER).entryInfoList(QDir::Files)) {
        QString desktopEntryPath(fileInfo.absoluteFilePath());
        QString key = entryPathToKey(desktopEntryPath);

        if (!store->contains(key)) {
            MDesktopEntry desktopEntry(desktopEntryPath);
            if (isDesktopEntryValid(desktopEntry, supportedDesktopEntryFileTypes)) {
                // Add the entry with an unknown location
                store->createValue(key, QVariant());

                // Keep track of all valid desktop entries
                validKeys.append(key);
            }
        } else {
            // Keep track of all valid desktop entries (consider already added desktop entries to be valid)
            validKeys.append(key);
        }
    }

    QStringList allKeys(store->allKeys());
    foreach (const QString &key, allKeys) {
        if (!validKeys.contains(key)) {
            // Remove all desktop entries that are not valid
            store->remove(key);
        }
    }

    // Emit a dataStoreChanged() signal since the contents have changed
    emit dataStoreChanged();

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
    // remove key prefix from the key
    return key.replace(0, KEY_PREFIX.length(), "");
}
