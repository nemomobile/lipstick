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

/*!
 * Returns button placement in given pages
 *
 * \param desktopFileEntry Entry for the button to be found
 * \param pages Pages from where the button placement is checked
 * \return Placement of the button
 */
static Launcher::Placement buttonPlacementInLauncherPages(const QString &desktopFileEntry, QList<QSharedPointer<LauncherPage> > &pages)
{
    int pageNum = -1;
    int position = -1;

    int pageIndex = 0;
    foreach (QSharedPointer<LauncherPage> page, pages) {
        position = page->launcherButtonPosition(desktopFileEntry);
        if (position > -1 ) {
            pageNum = pageIndex;
            break;
        }
        pageIndex++;
    }

    return Launcher::Placement(pageNum, position);
}

Launcher::Launcher(QGraphicsItem *parent, LauncherModel *model) :
    MWidgetController(model != NULL ? model : new LauncherModel, parent),
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

    if (dataStore != NULL) {
        connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
        dataStore->updateDesktopEntryFiles();
    }
}

void Launcher::setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor)
{
    this->packageMonitor = packageMonitor;
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

void Launcher::updateButtonState(const QSharedPointer<MDesktopEntry> &desktopEntry, const QString &packageName, const QString &state, bool packageRemovable)
{
    LauncherButtonModel::State buttonState = buttonStateFromPackageState(state);
    QString entryPath(desktopEntry->fileName());
    Launcher::Placement buttonPlacementInDatastore = entryPlacementInDatastore(entryPath);

    // Check that button is not stored in some other location than launcher
    if (!buttonPlacementInDatastore.location.isEmpty() && buttonPlacementInDatastore.location != Launcher::LOCATION_IDENTIFIER) {
        return;
    }

    // When button state changes to uninstall, remove button right away
    if (buttonState == LauncherButtonModel::Uninstall) {
        removeLauncherButton(entryPath);
        return;
    }

    // If a .desktop file exists for the application itself but is invalid ignore the button
    QString applicationEntryPath(ApplicationPackageMonitor::toApplicationsEntryPath(entryPath));
    if (dataStore != NULL && dataStore->isDesktopEntryKnownToBeInvalid(applicationEntryPath)) {
        return;
    }

    QSharedPointer<LauncherButton> button = placeholderButton(desktopEntry);

    // When package in installed state and desktop entry is currently from installer-extra, then always
    // change to use the desktop entry from applications folder
    if (buttonState == LauncherButtonModel::Installed && ApplicationPackageMonitor::isInstallerExtraEntry(button->desktopEntry())) {
        if (!QFileInfo(applicationEntryPath).exists()) {
            // In error case that package doesn't have desktop entry in applications folder even when in installed state,
            // just remove button from launcher and let launcher datastore
            // to handle button addition when/if desktop entry comes available
            removeLauncherButton(applicationEntryPath);
            return;
        }

        QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(applicationEntryPath));
        updateButtonPlacementInStore(desktopEntry->fileName());
        button->updateFromDesktopEntry(desktopEntry);
    }

    // Update buttons package info
    button->setPackageName(packageName);
    button->setState(buttonState);
    button->setPackageRemovable(packageRemovable);
}

void Launcher::updateProgress(const QString& desktopEntryPath, int already, int total)
{
    Launcher::Placement placement(buttonPlacement(desktopEntryPath));
    if (!placement.isNull()) {
        QSharedPointer<LauncherPage> page = model()->launcherPages().at(placement.page);
        QSharedPointer<LauncherButton> button = page->model()->launcherButtons().at(placement.position);

        // This is needed to workaround bug #268255
        button->setState(LauncherButtonModel::Downloading);

        button->setOperationProgress(already, total);
    } else {
        // This is needed to workaround bug #268255
        QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(desktopEntryPath));
        QSharedPointer<LauncherButton> button = placeholderButton(desktopEntry);
    }
}

Launcher::Placement Launcher::entryPlacementInDatastore(const QString &desktopEntryPath)
{
    Launcher::Placement placement;
    QString entryFileName = QFileInfo(desktopEntryPath).fileName();
    QHash<QString, QVariant> allDesktopEntryPlacements = dataStore->dataForAllDesktopEntries();
    foreach (const QString &desktopEntryPath, allDesktopEntryPlacements.keys()) {
        if (QFileInfo(desktopEntryPath).fileName() == entryFileName) {
            placement.setPlacement(allDesktopEntryPlacements.value(desktopEntryPath).toString());
        }
    }
    return placement;
}

QSharedPointer<LauncherButton> Launcher::placeholderButton(const QSharedPointer<MDesktopEntry> &desktopEntry)
{
    QSharedPointer<LauncherButton> button;
    Launcher::Placement placement(buttonPlacement(desktopEntry->fileName()));
    if (!placement.isNull()) {
        // If button for package is already found in launcher, then just return the button as placeholder.
        QSharedPointer<LauncherPage> page = model()->launcherPages().at(placement.page);
        button = page->model()->launcherButtons().at(placement.position);
    } else {
        // If there is no button yet for the application, then we need to create a placeholder
        button = createLauncherButton(desktopEntry);
        QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();
        Placement placement =  appendButtonToPages(button, pages);
        model()->setLauncherPages(pages);
        dataStore->updateDataForDesktopEntry(desktopEntry->fileName(), placement.toString());
    }

    return button;
}

void Launcher::removePlaceholderButton(const QString &desktopEntryPath)
{
     Launcher::Placement placement(buttonPlacement(QFileInfo(desktopEntryPath).fileName()));
     if (!placement.isNull()) {
        QSharedPointer<LauncherPage> page = model()->launcherPages().at(placement.page);
        QSharedPointer<LauncherButton> buttonForDesktopEntry = page->model()->launcherButtons().at(placement.position);

        // Only remove button if the paths match so that we don't remove installed buttons
        if (buttonForDesktopEntry->desktopEntry() == desktopEntryPath) {
            removeButtonPlacementFromStore(desktopEntryPath);
            removeLauncherButton(desktopEntryPath);
            updateButtonPlacementsOnPage(page.data());
        } else {
            // Set existing button that is not a placeholder to installed state
            buttonForDesktopEntry->setPackageName(QString());
            buttonForDesktopEntry->setState(LauncherButtonModel::Installed);
        }
    }
}

void Launcher::updatePagesFromDataStore()
{
    if (dataStore != NULL) {
        // Stop listening to dataStoreChanged() and start listening to individual updates instead.
        disconnect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updatePagesFromDataStore()));
        connect(dataStore, SIGNAL(desktopEntryAdded(QSharedPointer<MDesktopEntry>)), this, SLOT(addLauncherButton(QSharedPointer<MDesktopEntry>)), Qt::UniqueConnection);
        connect(dataStore, SIGNAL(desktopEntryRemoved(QString)), this, SLOT(removeLauncherButton(QString)), Qt::UniqueConnection);
        connect(dataStore, SIGNAL(desktopEntryChanged(QSharedPointer<MDesktopEntry>)), this, SLOT(updateLauncherButton(QSharedPointer<MDesktopEntry>)), Qt::UniqueConnection);
    }

    // Update the pages
    QList<QSharedPointer<LauncherPage> > pages;
    addDesktopEntriesWithKnownPlacements(pages);
    addDesktopEntriesWithUnknownPlacements(pages);
    removeEmptyPages(pages);
    model()->setLauncherPages(pages);

    // After updating launcher from launcher data store we can connect package listener and update the states
    if (packageMonitor != NULL) {
        connect(packageMonitor, SIGNAL(packageStateUpdated(QSharedPointer<MDesktopEntry>, QString, QString, bool)),
            this, SLOT(updateButtonState(QSharedPointer<MDesktopEntry>, QString, QString, bool)), Qt::UniqueConnection);
        connect(packageMonitor, SIGNAL(installExtraEntryRemoved(QString)),
            this, SLOT(removePlaceholderButton(QString)), Qt::UniqueConnection);
        connect(packageMonitor, SIGNAL(downloadProgressUpdated(QString, int, int)),
            this, SLOT(updateProgress(QString, int, int)), Qt::UniqueConnection);

        packageMonitor->updatePackageStates();
    }
}

void Launcher::addDesktopEntriesWithKnownPlacements(QList<QSharedPointer<LauncherPage> > &pages)
{
    if (dataStore == NULL) {
        return;
    }

    // Put the desktop entries with known placements in the desired pages
    QMapIterator<Placement, QString> iterator(createPlacementMap(dataStore->dataForAllDesktopEntries()));
    while (iterator.hasNext()) {
        iterator.next();
        Placement placement(iterator.key());
        QString desktopEntryPath(iterator.value());

        if (placement.page >= 0) {
            while (placement.page >= pages.count()) {
                // Create pages until the desired page exists. This may create empty pages which must be removed in the end
                QSharedPointer<LauncherPage> page = createLauncherPage();
                setMaximumPageSizeIfNecessary(page);
                pages.append(page);
            }

            // Create a launcher button for the desktop entry
            QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(desktopEntryPath));
            int insertedIndex = pages.at(placement.page)->insertButton(createLauncherButton(desktopEntry), placement.position);

            // If real button position is different than in store, update the new position to store
            if (insertedIndex != placement.position && dataStore != NULL) {
                dataStore->updateDataForDesktopEntry(desktopEntryPath, PLACEMENT_TEMPLATE.arg(placement.page).arg(insertedIndex));
            }
        }
    }
}

void Launcher::updateButtonPlacementsOnPage(LauncherPage *page)
{
    int pageNum = pageIndex(page);
    if (pageNum < 0) {
        return;
    }

    QHash<QString, QString> newValues;
    int position = 0;
    foreach(const QSharedPointer<LauncherButton> &button, page->model()->launcherButtons()) {
        QString entry = button->desktopEntry();
        newValues.insert(entry, Placement(pageNum, position++).toString());
    }
    dataStore->updateDataForDesktopEntries(newValues);
}

void Launcher::addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages)
{
    if (dataStore == NULL) {
        return;
    }

    // Put the desktop entries with no known placement on the last page
    QHash<QString, QVariant> allDesktopEntryPlacements = dataStore->dataForAllDesktopEntries();
    QHash<QString, QString> addedDesktopEntries;
    foreach (const QString &desktopEntryPath, allDesktopEntryPlacements.keys()) {
        Placement placementInDatastore(allDesktopEntryPlacements.value(desktopEntryPath).toString());
        if (placementInDatastore.location.isEmpty() || placementInDatastore.location == LOCATION_IDENTIFIER) {
            // Check that desktop entry doesn't already have a button in launcher
            Placement placementInPages = buttonPlacementInLauncherPages(desktopEntryPath, pages);
            if (placementInDatastore.isNull() && placementInPages.isNull()) {
                QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(desktopEntryPath));
                placementInPages = appendButtonToPages(createLauncherButton(desktopEntry), pages);
                addedDesktopEntries.insert(desktopEntryPath, placementInPages.toString());
            }
        }
    }
    dataStore->updateDataForDesktopEntries(addedDesktopEntries);
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
            QHash<QString, QString> movedEntries;
            for (int page = firstRemovedPage; page < pages.count(); page++) {
                const QList<QSharedPointer<LauncherButton> > &launcherButtons = pages.at(page)->model()->launcherButtons();
                for (int position = 0; position < launcherButtons.count(); position++) {
                    movedEntries.insert(launcherButtons.at(position)->desktopEntry(), PLACEMENT_TEMPLATE.arg(page).arg(position));
                }
            }
            dataStore->updateDataForDesktopEntries(movedEntries);
        }
    }
}

QSharedPointer<LauncherButton> Launcher::createLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry)
{
    QSharedPointer<LauncherButton> button = QSharedPointer<LauncherButton> (new LauncherButton(desktopEntry));
    button->setObjectName("LauncherButton");
    connect(button.data(), SIGNAL(clicked()), this, SIGNAL(launcherButtonClicked()));
    return button;
}

QSharedPointer<LauncherPage> Launcher::createLauncherPage()
{
    QSharedPointer<LauncherPage> page(new LauncherPage());
    page->setObjectName("LauncherPage");
    connect(page.data(), SIGNAL(buttonRemoved()), this, SLOT(prunePage()));

    return page;
}

void Launcher::addLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry)
{
    Launcher::Placement placement = buttonPlacement(desktopEntry->fileName());
    if (placement.isNull()) {
        QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();
        appendButtonToPages(createLauncherButton(desktopEntry), pages);
        model()->setLauncherPages(pages);

        updateButtonPlacementInStore(desktopEntry->fileName());
    } else {
        updateLauncherButton(desktopEntry);
    }
}

Launcher::Placement Launcher::appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages)
{
    QSharedPointer<LauncherPage> page;

    bool added = false;
    if (!pages.isEmpty()) {
        page = pages.last();
        added = page->appendButton(button);
    }

    if (!added) {
        page = createLauncherPage();

        setMaximumPageSizeIfNecessary(page);
        pages.append(page);
        //We created a page so update the model
        model()->setLauncherPages(pages);
        page->appendButton(button);
    }

    int pageIndex =  pages.count() - 1;
    int position = page->launcherButtonPosition(button->desktopEntry());
    return Placement(pageIndex, position);
}

void Launcher::removeLauncherButton(const QString &desktopEntryPath)
{
    QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();

    foreach (QSharedPointer<LauncherPage> page, pages) {
        int removedButtonPosition = page->launcherButtonPosition(desktopEntryPath);
        if (removedButtonPosition > -1) {
            page->removeButton(desktopEntryPath);
            break;
        }
    }
}

void Launcher::prunePage()
{
    QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();

    foreach (QSharedPointer<LauncherPage> page, pages) {
        if(page.data() == sender()) {
            QList<QSharedPointer<LauncherButton> > buttons = page->model()->launcherButtons();
            if (buttons.count() == 0) {
                // remove empty page
                pages.removeOne(page);
                model()->setLauncherPages(pages);
            } else {
                updateButtonPlacementsOnPage(page.data());
            }
            break;
        }
    }
}

bool Launcher::updateLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry)
{
    bool found = false;
    foreach (QSharedPointer<LauncherPage> page, model()->launcherPages()) {
        QSharedPointer<LauncherButton> button = page->button(desktopEntry->fileName());
        if (!button.isNull()) {
            updateButtonPlacementInStore(desktopEntry->fileName());

            button->updateFromDesktopEntry(desktopEntry);
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

int Launcher::focusToButton(const QString &desktopFileEntry)
{
    int page = buttonPlacement(desktopFileEntry).page;
    if (page >= 0) {
        emit focusToButtonRequested(desktopFileEntry);
    }
    return page;
}

void Launcher::setPage(uint page)
{
    emit focusToPageRequested(page);
}

void Launcher::setMaximumPageSizeIfNecessary(QSharedPointer<LauncherPage> &page)
{
    if (maximumPageSize >= 0) {
        page->setMaximumButtonCount(maximumPageSize);
    }
}

Launcher::Placement Launcher::buttonPlacement(const QString &desktopFileEntry)
{
    if (!desktopFileEntry.isEmpty()) {
        QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();
        return buttonPlacementInLauncherPages(desktopFileEntry, pages);
    }

    return Placement();
}


void Launcher::updateButtonPlacementInStore(const QString &desktopEntryPath)
{
    // Updating the entry path (possible from installer-extra to applications or other way around)
    // we need to make sure we remove all previous entry paths to avoid multiple instances of one application
    removeButtonPlacementFromStore(ApplicationPackageMonitor::toInstallerExtraEntryPath(desktopEntryPath));
    removeButtonPlacementFromStore(ApplicationPackageMonitor::toApplicationsEntryPath(desktopEntryPath));

    Placement placement = buttonPlacement(desktopEntryPath);
    dataStore->updateDataForDesktopEntry(desktopEntryPath, placement.toString());
}

void Launcher::removeButtonPlacementFromStore(const QString &desktopEntryPath)
{
    dataStore->removeDataForDesktopEntry(desktopEntryPath);
}

int Launcher::pageIndex(LauncherPage *page)
{
    int pageNum = -1;
    int index = 0;
    foreach(const QSharedPointer<LauncherPage> &sharedPage, model()->launcherPages()) {
        if (page == sharedPage.data()) {
            pageNum = index;
            break;
        }
        index++;
    }
    return pageNum;
}

Launcher::Placement::Placement() : page(-1), position(-1) {
}

Launcher::Placement::Placement(const QString &placementString) : page(-1), position(-1) {
    setPlacement(placementString);
}

Launcher::Placement::Placement(int page, int position)
        : location(LOCATION_IDENTIFIER), page(page), position(position) {
}

void Launcher::Placement::setPlacement(const QString &placementString)
{
    location = placementString.section(SECTION_SEPARATOR, 0, 0);
    bool placementKnown = false;
    if (location == LOCATION_IDENTIFIER) {
        page = placementString.section(SECTION_SEPARATOR, 1, 1).toInt(&placementKnown);
        position = placementString.section(SECTION_SEPARATOR, 2, 2).toInt(&placementKnown);
    }

    if (!placementKnown) {
        page = -1;
        position = -1;
    }
}

QString Launcher::Placement::toString()
{
    return PLACEMENT_TEMPLATE.arg(page).arg(position);
}

inline bool operator<(const Launcher::Placement &p1, const Launcher::Placement &p2)
{
    if (p1.page != p2.page) {
        return p1.page < p2.page;
    }

    return p1.position < p2.position;
}

LauncherButtonModel::State Launcher::buttonStateFromPackageState(const QString &packageState)
{
    LauncherButtonModel::State buttonState;
    if (packageState == ApplicationPackageMonitor::PACKAGE_STATE_INSTALLED) {
        buttonState = LauncherButtonModel::Installed;
    } else if (packageState == ApplicationPackageMonitor::PACKAGE_STATE_BROKEN) {
        buttonState = LauncherButtonModel::Broken;
    } else if (packageState == ApplicationPackageMonitor::PACKAGE_STATE_INSTALLING) {
        buttonState = LauncherButtonModel::Installing;
    } else if (packageState == ApplicationPackageMonitor::PACKAGE_STATE_DOWNLOADING) {
        buttonState = LauncherButtonModel::Downloading;
    } else if (packageState == ApplicationPackageMonitor::PACKAGE_STATE_UNINSTALLING) {
        buttonState = LauncherButtonModel::Uninstall;
    } else {
        buttonState = LauncherButtonModel::Installed;
    }
    return buttonState;
}
