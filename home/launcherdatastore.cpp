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

LauncherDataStore::Placement::Placement(const QString &placement) : location(LauncherDataStore::Unknown), page(0), position(0) {
    QString loc = placement.section(SECTION_SEPARATOR, 0, 0);
    if(loc == LAUNCHER_PLACEMENT) {
        location = LauncherDataStore::LauncherGrid;
        page     = placement.section(SECTION_SEPARATOR, 1, 1).toInt();
        position = placement.section(SECTION_SEPARATOR, 2, 2).toInt();
    } else if(loc == QUICKLAUNCHBAR_PLACEMENT) {
        location = LauncherDataStore::QuickLaunchBar;
        position = placement.section(SECTION_SEPARATOR, 1, 1).toInt();
    }
}

inline bool operator<(const LauncherDataStore::Placement &p1, const LauncherDataStore::Placement &p2)
{
    if(p1.page != p2.page) {
        return p1.page < p2.page;
    }

    return p1.position < p2.position;
}

LauncherDataStore::LauncherDataStore(MDataStore* dataStore)
{
    store = dataStore;
    connect(store, SIGNAL(valueChanged(QString,QVariant)), this, SIGNAL(dataStoreChanged()));
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


QMap<LauncherDataStore::Placement, QString> LauncherDataStore::entryPlacementMap(LauncherDataStore::EntryLocation location)
{
    QStringList entryKeys(store->allKeys());
    QMap<Placement, QString> map;

    foreach(QString key, entryKeys) {
        Placement p(store->value(key).toString());

        if(p.location == location) {
            map.insert(p, keyToEntryPath(key));
        }
    }

    return map;
}

QList< QSharedPointer<LauncherPage> > LauncherDataStore::launcherButtons()
{
    QList< QSharedPointer<LauncherPage> > pages;
    // Collect a map of the entries and locations so that entries can be sorted by location before adding to pages.
    // This is done as buttons doesn't know/maintain their positions and empty spaces in pages are not supported.
    // So when adding multiple buttons to pages we need to do it in order.
    QMap<Placement, QString> entryPlaces = entryPlacementMap(LauncherGrid);
    QMapIterator<Placement, QString> iterator(entryPlaces);

    while (iterator.hasNext()) {
        iterator.next();
        Placement placement(iterator.key());
        QString entryFile(iterator.value());
        QSharedPointer<LauncherPage> newPage;
        if (placement.page < pages.count()) {
            newPage = pages.at(placement.page);
        } else {
            newPage = QSharedPointer<LauncherPage>(new LauncherPage());
            pages.append(newPage);
        }
        MDesktopEntry entry(entryFile);
        QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton> (new LauncherButton(entry));
        newPage->insertButton(button, placement.position);
    }

    return pages;
}

QList<LauncherButton*> LauncherDataStore::quickLaunchBarButtons()
{
    QList<LauncherButton*> buttons;
    QMap<Placement, QString> entryPlaces = this->entryPlacementMap(QuickLaunchBar);
    QMapIterator<Placement, QString> iterator(entryPlaces);

    while (iterator.hasNext()) {
        iterator.next();
        Placement placement(iterator.key());
        QString entryFile(iterator.value());

        MDesktopEntry entry(entryFile);
        if(entry.isValid()) {
            LauncherButton *button = new LauncherButton(entry);
            // allow empty places
            while(placement.position >= buttons.size()) {
                buttons.append(NULL);
            }
            buttons.replace(placement.position, button);
        } else {
            // remove from datastore
            store->remove(entryPathToKey(entryFile));
        }
    }

    return buttons;
}

LauncherDataStore::EntryLocation LauncherDataStore::location(const MDesktopEntry &entry)
{
    EntryLocation location = Unknown;
    QString key(entryPathToKey(entry.fileName()));
    if (store->contains(key)) {
        Placement p(store->value(key).toString());
        location = p.location;
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
