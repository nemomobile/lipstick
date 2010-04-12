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

#include "launcherdatastore.h"
#include "mdatastore.h"

#include <MDesktopEntry>
#include "launcherpage.h"
#include "launcherbutton.h"

static const QString LAUNCHER_PLACEMENT = "launcher";
static const QString QUICKLAUNCHBAR_PLACEMENT = "quicklaunchbar";
static const char SECTION_SEPARATOR = '/';
static const QString KEY_PREFIX = "DesktopEntries";

LauncherDataStore::LauncherDataStore(MDataStore* dataStore)
{
    store = dataStore;
}

LauncherDataStore::~LauncherDataStore()
{
    delete store;
}


void LauncherDataStore::updateLauncherButtons(const QList< QSharedPointer<LauncherPage> > &pages)
{
    QStringList oldListOfAllKeys(store->allKeys());

    // Collect entries of buttons in launcher from the old list
    // This is used to prune non-existent entries from store
    QMap<QString, QString> oldLauncherEntries;
    // Collect entries of buttons in launcher in the new list
    // This is used to prune non-existent entries from store
    QSet<QString> newLauncherEntries;


    foreach(QString key, oldListOfAllKeys){
        QString place(store->value(key).toString());
        oldLauncherEntries.insert(keyToEntryPath(key), place);
    }

    QString launcherPlacement(LAUNCHER_PLACEMENT + SECTION_SEPARATOR + "%1" + SECTION_SEPARATOR + "%2");
    int pageIndex = 0;
    int buttonPosition;
    foreach(QSharedPointer<LauncherPage> page, pages){
        buttonPosition = 0;
        foreach(QSharedPointer<LauncherButton> button, page->model()->launcherButtons()){
            QString desktopFile = button->model()->desktopEntryFile();
            QString placement(launcherPlacement.arg(pageIndex).arg(buttonPosition));
            newLauncherEntries.insert(desktopFile);
            if (!oldLauncherEntries.contains(desktopFile)
                || oldLauncherEntries[desktopFile] != placement) {
                store->createValue(entryPathToKey(desktopFile),
                                   QVariant(placement));
            }
            buttonPosition++;
        }
        pageIndex++;
    }

    // Remove old non-existent entries from data store
    for (QMap<QString, QString>::const_iterator entry =
             oldLauncherEntries.constBegin(),
             end = oldLauncherEntries.constEnd();
         entry != end; ++entry){
        if (entry.value().section(SECTION_SEPARATOR, 0, 0) == LAUNCHER_PLACEMENT) {
            if (!newLauncherEntries.contains(entry.key())) {
                store->remove(entryPathToKey(entry.key()));
            }
        }
    }
}

QList< QSharedPointer<LauncherPage> > LauncherDataStore::launcherButtons()
{
    QStringList entryKeys(store->allKeys());
    QList< QSharedPointer<LauncherPage> > pages;

    // Collect a map of the entries and locations so that entries can be sorted by location before adding to pages.
    // This is done as buttons doesn't know/maintain their positions and empty spaces in pages are not supported.
    // So when adding multiple buttons to pages we need to do it in order.
    QMap<QString, QString> entryLocationMap;
    foreach(QString key, entryKeys) {
        QString placement(store->value(key).toString());

        QString place(placement.section(SECTION_SEPARATOR, 0, 0));
        if(place == LAUNCHER_PLACEMENT){
            entryLocationMap.insert(placement, keyToEntryPath(key));
        }
    }

    QMapIterator<QString, QString> iterator(entryLocationMap);
    while (iterator.hasNext()) {
        iterator.next();
        QString placement(iterator.key());
        QString entryFile(iterator.value());
        int pageIndex = placement.section(SECTION_SEPARATOR, 1, 1).toInt();
        int buttonPositionOnPage = placement.section(SECTION_SEPARATOR, 2, 2).toInt();
        QSharedPointer<LauncherPage> newPage;
        if (pageIndex < pages.count()) {
            newPage = pages.at(pageIndex);
        } else {
            newPage = QSharedPointer<LauncherPage>(new LauncherPage());
            pages.append(newPage);
        }
        MDesktopEntry entry(entryFile);
        QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton> (new LauncherButton(entry));
        newPage->insertButton(button, buttonPositionOnPage);
    }

    return pages;
}

LauncherDataStore::EntryLocation LauncherDataStore::location(const MDesktopEntry &entry)
{
    EntryLocation location = Unknown;
    QString key(entryPathToKey(entry.fileName()));
    if (store->contains(key)) {
        QString placement = store->value(key).toString();
        if (placement.section(SECTION_SEPARATOR, 0, 0) == LAUNCHER_PLACEMENT) {
            location = LauncherGrid;
        } else if (placement.section(SECTION_SEPARATOR, 0, 0) == QUICKLAUNCHBAR_PLACEMENT) {
            location = QuickLaunchBar;
	}
    }
    return location;
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
