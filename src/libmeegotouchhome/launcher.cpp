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

#include <MWidgetCreator>
M_REGISTER_WIDGET(Launcher)

const QString Launcher::LOCATION_IDENTIFIER = "launcher";
const char Launcher::SECTION_SEPARATOR = '/';
const QString Launcher::PLACEMENT_TEMPLATE = LOCATION_IDENTIFIER + SECTION_SEPARATOR + "%1" + SECTION_SEPARATOR + "%2";

Launcher::Launcher(QGraphicsItem *parent) :
    MWidgetController(new LauncherModel, parent),
    dataStore(NULL),
    packageMonitor(NULL),
    maximumPageSize(-1)
{
}

Launcher::~Launcher()
{
}

void Launcher::setLauncherDataStore(LauncherDataStore *dataStore)
{
    if (this->dataStore != NULL) {
        disconnect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
    }
    this->dataStore = dataStore;
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
}

void Launcher::setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor)
{
    this->packageMonitor = packageMonitor;
    connect(packageMonitor, SIGNAL(downloadProgress(const QString &, int, int)),
            this, SLOT(setDownloadProgress(const QString &, int, int)));
    connect(packageMonitor, SIGNAL(installProgress(const QString &, int)),
            this, SLOT(setInstallProgress(const QString &, int)));
    connect(packageMonitor, SIGNAL(operationSuccess(const QString&)),
            this, SLOT(setOperationSuccess(const QString&)));
    connect(packageMonitor, SIGNAL(operationError(const QString&, const QString&)),
            this, SLOT(setOperationError(const QString&, const QString&)));
}

void Launcher::setMaximumPageSize(int maximumPageSize)
{
    this->maximumPageSize = maximumPageSize;

    if (maximumPageSize >= 0) {
        foreach (QSharedPointer<LauncherPage> page, model()->launcherPages()) {
            page->setMaximumButtonCount(maximumPageSize);
        }
    }
}

void Launcher::setDownloadProgress(const QString &desktopEntryPath, int bytesLoaded, int bytesTotal)
{
    int percentage = -1;
    if (bytesTotal > 0 && bytesLoaded <= bytesTotal) {
        percentage = ((double)bytesLoaded / (double)bytesTotal) * 100;
    }
    updateButtonState(desktopEntryPath, LauncherButtonModel::Downloading, percentage);
}

void Launcher::setInstallProgress(const QString &desktopEntryPath, int percentage)
{
    updateButtonState(desktopEntryPath, LauncherButtonModel::Installing, percentage);
}

void Launcher::setOperationSuccess(const QString& desktopEntryPath)
{
    QString desktopEntryFilename = QFileInfo(desktopEntryPath).fileName();
    if (placeholderMap.contains(desktopEntryFilename)) {
        placeholderMap.value(desktopEntryFilename)->updateFromDesktopEntry(desktopEntryPath);
        placeholderMap.value(desktopEntryFilename)->setState(LauncherButtonModel::Installed, 0);
        placeholderMap.remove(desktopEntryFilename);

        // Update placement in data store when application is fully installed
        updateButtonPlacementInStore(desktopEntryPath);
    }
}

void Launcher::setOperationError(const QString& desktopEntryPath, const QString& error)
{
    Q_UNUSED(error)
    updateButtonState(desktopEntryPath, LauncherButtonModel::Broken, 0);
}

void Launcher::updateButtonState(const QString &desktopEntryPath, LauncherButtonModel::State state, int progress)
{
    QString desktopEntryFilename = QFileInfo(desktopEntryPath).fileName();
    if (!placeholderMap.contains(desktopEntryFilename) && !entryExistsInDatastore(desktopEntryPath)) {
        addPlaceholderButton(desktopEntryPath);
    }

    placeholderMap.value(desktopEntryFilename)->setState(state, progress);
}

bool Launcher::entryExistsInDatastore(const QString &desktopEntryPath)
{
    bool exists = false;
    QString entryFileName = QFileInfo(desktopEntryPath).fileName();
    QHash<QString, QVariant> allDesktopEntryPlacements = dataStore->dataForAllDesktopEntries();
    foreach (const QString &desktopEntryPath, allDesktopEntryPlacements.keys()) {
        if (QFileInfo(desktopEntryPath).fileName() == entryFileName) {
            exists = true;
        }
    }
    return exists;
}

void Launcher::addPlaceholderButton(const QString& desktopEntryPath)
{
    QSharedPointer<LauncherButton> button;
    Launcher::Placement placement(buttonPlacement(QFileInfo(desktopEntryPath).fileName()));
    if (!placement.isNull()) {
        // If button for package is already found in launcher, then just update/add the button to placeholders.
        QSharedPointer<LauncherPage> page = model()->launcherPages().at(placement.page);
        button = page->model()->launcherButtons().at(placement.position);
    } else {
        // If there is no button yet for the application, then we need to create a placeholder
        button = createLauncherButton(desktopEntryPath);
        QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();
        appendButtonToPages(button, pages);
        model()->setLauncherPages(pages);
    }

    placeholderMap.insert(QFileInfo(desktopEntryPath).fileName(), button);
}

void Launcher::updatePagesFromDataStore()
{
    if (dataStore != NULL) {
        // Stop listening to dataStoreChanged() and start listening to individual updates instead.
        disconnect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
        connect(dataStore, SIGNAL(desktopEntryAdded(QString)), this, SLOT(addLauncherButton(QString)), Qt::UniqueConnection);
        connect(dataStore, SIGNAL(desktopEntryRemoved(QString)), this, SLOT(removeLauncherButton(QString)), Qt::UniqueConnection);
        connect(dataStore, SIGNAL(desktopEntryChanged(QString)), this, SLOT(updateLauncherButton(QString)), Qt::UniqueConnection);
    }

    // Update the pages
    QList<QSharedPointer<LauncherPage> > pages;
    addDesktopEntriesWithKnownPlacements(pages);
    addDesktopEntriesWithUnknownPlacements(pages);
    removeEmptyPages(pages);
    model()->setLauncherPages(pages);

    // After initializing launcher we can update the states from package monitor
    packageMonitor->updatePackageStates();
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
                QSharedPointer<LauncherPage> page = QSharedPointer<LauncherPage>(new LauncherPage);
                setMaximumPageSizeIfNecessary(page);
                pages.append(page);
            }

            // Create a launcher button for the desktop entry
            int insertedIndex = pages.at(placement.page)->insertButton(createLauncherButton(desktopEntryPath), placement.position);

            // If real button position is different than in store, update the new position to store
            if (insertedIndex != placement.position && dataStore != NULL) {
                dataStore->updateDataForDesktopEntry(desktopEntryPath, PLACEMENT_TEMPLATE.arg(placement.page).arg(insertedIndex));
            }
        }
    }
}

void Launcher::addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages)
{
    if (dataStore != NULL) {
        // Put the desktop entries with no known placement on the last page
        QHash<QString, QVariant> allDesktopEntryPlacements = dataStore->dataForAllDesktopEntries();
        foreach (const QString &desktopEntryPath, allDesktopEntryPlacements.keys()) {
            Placement placement(allDesktopEntryPlacements.value(desktopEntryPath).toString());
            if ((placement.location.isEmpty() || placement.location == LOCATION_IDENTIFIER) && placement.page < 0) {
                addNewLauncherButtonToPages(desktopEntryPath, pages);
            }
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

    if (dataStore != NULL) {
        // Locations of launcher buttons on subsequent pages have changed so update all of them
        if (firstRemovedPage >= 0) {
            for (int page = firstRemovedPage; page < pages.count(); page++) {
                const QList<QSharedPointer<LauncherButton> > &launcherButtons = pages.at(page)->model()->launcherButtons();
                for (int position = 0; position < launcherButtons.count(); position++) {
                    dataStore->updateDataForDesktopEntry(launcherButtons.at(position)->desktopEntry(), PLACEMENT_TEMPLATE.arg(page).arg(position));
                }
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
    // Don't add a button if we already have a placeholder button
    if (!placeholderMap.contains(QFileInfo(desktopEntryPath).fileName())) {
        // First try to update button if it already exists in launcher
        if (!updateLauncherButton(desktopEntryPath)) {
            QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();
            addNewLauncherButtonToPages(desktopEntryPath, pages);
            model()->setLauncherPages(pages);
        }
    }
}

void Launcher::addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages)
{
    // Create a launcher button for the desktop entry
    QSharedPointer<LauncherButton> launcherButton = createLauncherButton(desktopEntryPath);

    appendButtonToPages(launcherButton, pages);

    updateButtonPlacementInStore(desktopEntryPath);
}

void Launcher::appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages)
{
    QSharedPointer<LauncherPage> page;

    bool added = false;
    if (!pages.isEmpty()) {
        page = pages.last();
        added = page->appendButton(button);
    }

    if (!added) {
        page = QSharedPointer<LauncherPage>(new LauncherPage());
        setMaximumPageSizeIfNecessary(page);
        pages.append(page);
        page->appendButton(button);
    }
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
            } else if (dataStore != NULL) {
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

bool Launcher::updateLauncherButton(const QString &desktopEntryPath)
{
    bool found = false;
    foreach (QSharedPointer<LauncherPage> page, model()->launcherPages()) {
        if (page->updateButton(desktopEntryPath)) {
            found = true;
            break;
        }
    }
    return found;
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

void Launcher::setMaximumPageSizeIfNecessary(QSharedPointer<LauncherPage> &page)
{
    if (maximumPageSize >= 0) {
        page->setMaximumButtonCount(maximumPageSize);
    }
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

void Launcher::updateButtonPlacementInStore(const QString &desktopEntryPath)
{
    Placement placement = buttonPlacement(desktopEntryPath);

    dataStore->updateDataForDesktopEntry(desktopEntryPath, PLACEMENT_TEMPLATE.arg(placement.page).arg(placement.position));
}

Launcher::Placement::Placement() : page(-1), position(-1) {
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
