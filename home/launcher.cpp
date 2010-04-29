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

#include <MApplicationIfProxy>
#include <MDesktopEntry>
#include "launcher.h"
#include "launcherbutton.h"
#include "launcherdatastore.h"

const QString Launcher::LOCATION_IDENTIFIER = "launcher";
const char Launcher::SECTION_SEPARATOR = '/';
const QString Launcher::PLACEMENT_TEMPLATE = LOCATION_IDENTIFIER + SECTION_SEPARATOR + "%1" + SECTION_SEPARATOR + "%2";

Launcher::Launcher(LauncherDataStore *dataStore, QGraphicsItem *parent) :
    MWidgetController(new LauncherModel, parent),
    dataStore(dataStore)
{
    // Listen to changes in data store contents
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
}

Launcher::~Launcher()
{
}

bool Launcher::startApplication(const QString &application)
{
    if (!QProcess::startDetached(application)) {
        qWarning() << "Failed to start application:" << application;
        return false;
    } else {
        return true;
    }
}

bool Launcher::startMApplication(const QString &serviceName)
{
    MApplicationIfProxy mApplicationIfProxy(serviceName, NULL);

    if (mApplicationIfProxy.connection().isConnected()) {
        mApplicationIfProxy.launch();
        return true;
    } else {
        qWarning() << "Could not launch" << serviceName;
        qWarning() << "DBus not connected?";
        return false;
    }
}

void Launcher::updatePagesFromDataStore()
{
    // Temporarily disable the listening of the change signals from the configuration to prevent a recursive call to this method
    dataStore->disconnect(this);

    // Update the pages
    QList<QSharedPointer<LauncherPage> > pages;
    addDesktopEntriesWithKnownPlacements(pages);
    addDesktopEntriesWithUnknownPlacements(pages);
    removeEmptyPages(pages);
    model()->setLauncherPages(pages);

    // Reconnect signals
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
}

void Launcher::addDesktopEntriesWithKnownPlacements(QList<QSharedPointer<LauncherPage> > &pages)
{
    // Put the desktop entries with known placements in the desired pages
    QMapIterator<Placement, QString> iterator(createPlacementMap(dataStore->dataForAllDesktopEntries()));
    while (iterator.hasNext()) {
        iterator.next();
        Placement placement(iterator.key());
        QString desktopEntryPath(iterator.value());

        if (placement.page >= 0) {
            while (placement.page >= pages.count()) {
                // Create pages until the desired page exists. This may create empty pages which must be removed in the end
                pages.append(QSharedPointer<LauncherPage>(new LauncherPage));
            }

            // Create a launcher button for the desktop entry
            pages.at(placement.page)->insertButton(createLauncherButton(desktopEntryPath), placement.position);
        }
    }
}

void Launcher::addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages)
{
    // Put the desktop entries with no known placement on the last page
    QHash<QString, QVariant> allDesktopEntryPlacements = dataStore->dataForAllDesktopEntries();
    foreach (const QString &desktopEntryPath, allDesktopEntryPlacements.keys()) {
        Placement placement(allDesktopEntryPlacements.value(desktopEntryPath).toString());
        if ((placement.location.isEmpty() || placement.location == LOCATION_IDENTIFIER) && placement.page < 0) {
            // Create a launcher button for the desktop entry
            QSharedPointer<LauncherButton> launcherButton = createLauncherButton(desktopEntryPath);
            QSharedPointer<LauncherPage> page;

            bool added = false;
            if (!pages.isEmpty()) {
                page = pages.last();
                added = page->appendButton(launcherButton);
            }

            if (!added) {
                page = QSharedPointer<LauncherPage>(new LauncherPage());
                pages.append(page);
                page->appendButton(launcherButton);
            }

            // Set the launcher button location in the data store
            int pageIndex = pages.count() - 1;
            int buttonIndex = page->model()->launcherButtons().count() - 1;
            dataStore->updateDataForDesktopEntry(desktopEntryPath, PLACEMENT_TEMPLATE.arg(pageIndex).arg(buttonIndex));
        }
    }
}

void Launcher::removeEmptyPages(QList<QSharedPointer<LauncherPage> > &pages)
{
    // Remove empty pages if they exist
    int firstRemovedPage = -1;
    for (int page = 0; page < pages.count();) {
        if (pages.at(page)->model()->launcherButtons().count() == 0) {
            // The page is empty: remove it
            pages.removeAt(page);
            if (firstRemovedPage < 0) {
                // Keep track of the first removed page
                firstRemovedPage = page;
            }
        } else {
            page++;
        }
    }

    // Locations of switcher buttons on subsequent pages have changed so update all of them
    if (firstRemovedPage >= 0) {
        for (int page = firstRemovedPage; page < pages.count(); page++) {
            const QList<QSharedPointer<LauncherButton> > &launcherButtons = pages.at(page)->model()->launcherButtons();
            for (int position = 0; position < launcherButtons.count(); position++) {
                dataStore->updateDataForDesktopEntry(launcherButtons.at(position)->desktopEntry(), PLACEMENT_TEMPLATE.arg(page).arg(position));
            }
        }
    }
}

QSharedPointer<LauncherButton> Launcher::createLauncherButton(const QString &desktopEntryPath)
{
    MDesktopEntry desktopEntry(desktopEntryPath);
    QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton> (new LauncherButton(desktopEntry));
    button->setObjectName("LauncherButton");
    connect(button.data(), SIGNAL(clicked()), this, SIGNAL(launcherButtonClicked()));
    return button;
}

QMap<Launcher::Placement, QString> Launcher::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements)
{
    QMap<Launcher::Placement, QString> placementMap;

    foreach (const QString &desktopEntryPath, desktopEntryPlacements.keys()) {
        Placement placement(desktopEntryPlacements.value(desktopEntryPath).toString());
        if (placement.location == LOCATION_IDENTIFIER) {
            placementMap.insert(placement, desktopEntryPath);
        }
    }

    return placementMap;
}

Launcher::Placement::Placement(const QString &placement) : page(-1), position(-1) {
    location = placement.section(SECTION_SEPARATOR, 0, 0);
    if (location == LOCATION_IDENTIFIER) {
        page = placement.section(SECTION_SEPARATOR, 1, 1).toInt();
        position = placement.section(SECTION_SEPARATOR, 2, 2).toInt();
    }
}

inline bool operator<(const Launcher::Placement &p1, const Launcher::Placement &p2)
{
    if (p1.page != p2.page) {
        return p1.page < p2.page;
    }

    return p1.position < p2.position;
}
