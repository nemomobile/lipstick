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

#include "launcher.h"
#include "launcherbutton.h"
#include "launcherdatastore.h"
#include "applicationpackagemonitor.h"

const QString Launcher::LOCATION_IDENTIFIER = "launcher";
const char Launcher::SECTION_SEPARATOR = '/';
const QString Launcher::PLACEMENT_TEMPLATE = LOCATION_IDENTIFIER + SECTION_SEPARATOR + "%1" + SECTION_SEPARATOR + "%2";

Launcher::Launcher(LauncherDataStore *dataStore, ApplicationPackageMonitor *packageMonitor, QGraphicsItem *parent) :
    MWidgetController(new LauncherModel, parent),
    dataStore(dataStore),
    packageMonitor(packageMonitor)
{
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));

    connect(packageMonitor, SIGNAL(downloadProgress(const QString&, const QString &, int, int)),
            this, SLOT(setDownloadProgress(const QString&, const QString &, int, int)));
    connect(packageMonitor, SIGNAL(installProgress(const QString&, const QString &, int)),
            this, SLOT(setInstallProgress(const QString&, const QString &, int)));
    connect(packageMonitor, SIGNAL(operationSuccess(const QString&, const QString&)),
            this, SLOT(setOperationSuccess(const QString&, const QString&)));
    connect(packageMonitor, SIGNAL(operationError(const QString&, const QString&, const QString&)),
            this, SLOT(setOperationError(const QString&, const QString&, const QString&)));
}

Launcher::~Launcher()
{
}

void Launcher::setDownloadProgress(const QString& packageName, const QString &desktopEntryPath, int bytesLoaded, int bytesTotal)
{
    int percentage = -1;
    if (bytesTotal > 0 && bytesLoaded <= bytesTotal) {
        percentage = ((double)bytesLoaded / (double)bytesTotal) * 100;
    }
    updateButtonState(packageName, desktopEntryPath, LauncherButtonModel::Downloading, percentage);
}

void Launcher::setInstallProgress(const QString& packageName, const QString &desktopEntryPath, int percentage)
{
    updateButtonState(packageName, desktopEntryPath, LauncherButtonModel::Installing, percentage);
}

void Launcher::setOperationSuccess(const QString& packageName, const QString& desktopEntryPath)
{
    if (launcherButtonMap.contains(packageName)) {
        launcherButtonMap.value(packageName)->updateFromDesktopEntry(desktopEntryPath);
        launcherButtonMap.value(packageName)->setState(LauncherButtonModel::Installed, 0);
        launcherButtonMap.remove(packageName);
    }
}

void Launcher::setOperationError(const QString& packageName, const QString& desktopEntryPath, const QString& error)
{
    Q_UNUSED(error)
    int progress = 0;
    updateButtonState(packageName, desktopEntryPath, LauncherButtonModel::Broken, progress);
}

void Launcher::updateButtonState(const QString &packageName, const QString &desktopEntryPath, LauncherButtonModel::State state, int progress)
{
    if (!launcherButtonMap.contains(packageName)) {
        QSharedPointer<LauncherButton> button = createLauncherButton(desktopEntryPath);
        launcherButtonMap.insert(packageName, button);
        addPlaceholderButtonToLauncher(button);
    }
    launcherButtonMap.value(packageName)->setState(state, progress);
}

void Launcher::addPlaceholderButtonToLauncher(QSharedPointer<LauncherButton> button)
{
    QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();

    QSharedPointer<LauncherPage> page;

    bool added = false;
    if (!pages.isEmpty()) {
        page = pages.last();
        added = page->appendButton(button);
    }

    if (!added) {
        page = QSharedPointer<LauncherPage>(new LauncherPage());
        pages.append(page);
        page->appendButton(button);
    }

    model()->setLauncherPages(pages);
}

void Launcher::updatePagesFromDataStore()
{
    // Stop listening to dataStoreChanged() and start listening to individual updates instead.
    disconnect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
    connect(dataStore, SIGNAL(desktopEntryAdded(QString)), this, SLOT(addLauncherButton(QString)), Qt::UniqueConnection);
    connect(dataStore, SIGNAL(desktopEntryRemoved(QString)), this, SLOT(removeLauncherButton(QString)), Qt::UniqueConnection);
    connect(dataStore, SIGNAL(desktopEntryChanged(QString)), this, SLOT(updateLauncherButton(QString)), Qt::UniqueConnection);

    // Update the pages
    QList<QSharedPointer<LauncherPage> > pages;
    addDesktopEntriesWithKnownPlacements(pages);
    addDesktopEntriesWithUnknownPlacements(pages);
    removeEmptyPages(pages);
    model()->setLauncherPages(pages);
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
            int insertedIndex = pages.at(placement.page)->insertButton(createLauncherButton(desktopEntryPath), placement.position);

            // If real button position is different than in store, update the new position to store
            if (insertedIndex != placement.position) {
                dataStore->updateDataForDesktopEntry(desktopEntryPath, PLACEMENT_TEMPLATE.arg(placement.page).arg(insertedIndex));
            }
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
            addLauncherButtonToPages(desktopEntryPath, pages);
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
    QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton> (new LauncherButton(desktopEntryPath));
    button->setObjectName("LauncherButton");
    connect(button.data(), SIGNAL(clicked()), this, SIGNAL(launcherButtonClicked()));
    return button;
}

void Launcher::addLauncherButton(const QString &desktopEntryPath)
{
    QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();

    addLauncherButtonToPages(desktopEntryPath, pages);

    model()->setLauncherPages(pages);
}

void Launcher::addLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages)
{
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

void Launcher::removeLauncherButton(const QString &desktopEntryPath)
{
    QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();

    int pageIndex = 0;
    foreach (QSharedPointer<LauncherPage> page, pages) {
        if (page->removeButton(desktopEntryPath)) {

            QList<QSharedPointer<LauncherButton> > buttons = page->model()->launcherButtons();
            if (buttons.count() == 0) {
                // remove empty page
                pages.removeOne(page);
                model()->setLauncherPages(pages);
            } else {
                // Update new locations for other launcher buttons on page (when button is removed other buttons get shifted)
                int buttonIndex = 0;
                foreach (QSharedPointer<LauncherButton> button, buttons) {
                    dataStore->updateDataForDesktopEntry(button->desktopEntry(), PLACEMENT_TEMPLATE.arg(pageIndex).arg(buttonIndex++));
                }
            }
            break;
        }
        pageIndex++;
    }
}

void Launcher::updateLauncherButton(const QString &desktopEntryPath)
{
    foreach (QSharedPointer<LauncherPage> page, model()->launcherPages()) {
        if (page->updateButton(desktopEntryPath)) break;
    }
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

int Launcher::panToPage(const QString &desktopFileEntry)
{

    Placement placement = buttonPlacement(desktopFileEntry);
    int page = placement.page;
    if (page >= 0) {
        emit panningRequested((uint)page);
    }
    return page;
}

void Launcher::setFirstPage()
{
    emit focusToFirstPageRequested();
}

Launcher::Placement Launcher::buttonPlacement(const QString &desktopFileEntry)
{
    int pageNum = -1;
    int position = -1;

    if (!desktopFileEntry.isEmpty()) {
        QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();
        int pageIndex = 0;
        foreach (QSharedPointer<LauncherPage> page, pages) {
            position = page->launcherButtonPosition(desktopFileEntry);
            if (position > -1 ) {
                pageNum = pageIndex;
                break;
            }
            pageIndex++;
        }
    }

    Placement placement(PLACEMENT_TEMPLATE.arg(pageNum).arg(position));

    return placement;
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
