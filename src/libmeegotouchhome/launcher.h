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

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QList>
#include <MWidgetController>
#include "launchermodel.h"

class MDesktopEntry;
class LauncherDataStore;
class ApplicationPackageMonitorListener;

/*!
 * Widget for launching and browsing installed applications.
 * The widget monitors a desktop file entry directory and creates buttons
 * that represent the applications that can be launched.
 *
 * For updating the launcher first time from launcher data store, we listen to dataStoreChanged() signal.
 * After the first initialization we disconnect from dataStoreChanged signal and rely on
 * signals for individual updates to provide the changes in desktop entries.
 *
 * Each application .desktop file must define the Name, Type and Icon keys where type is Application. The Exec key must define the application binary to be launched when the icon is selected.
 *
 * Example my_application.desktop file:
 * \code
 * [Desktop Entry]
 * Version=1.0
 * Type=Application
 * Name=my_localized_application_name
 * Comment=my_localized_application_comment
 * Exec=/usr/bin/my-app
 * Icon=my_app_icon_id
 * \endcode
 */
class Launcher : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(Launcher)

public:
    //! A class for storing, parsing and ordering the placement information of the items
    class Placement {
    public:
        Placement();
        Placement(const QString &placement);
        Placement(int page, int position);

        void setPlacement(const QString &placementString);
        QString toString();

        QString location;
        int page;
        int position;

        bool isNull() {
            return (page < 0 || position < 0);
        }
    };

    /*!
     * Constructs a Launcher widget. The Launcher will not store/restore
     * launcher button positions and entries before a LauncherDataStore
     * has been set using setLauncherDataStore().
     *
     * \param parent parent graphics item for the widget, defaults to NULL
     * \param model model for the widget, defaults to a LauncherModel
     */
    Launcher(QGraphicsItem *parent = NULL, LauncherModel *model = NULL);

    /*!
     * Destroys the Launcher.
     */
    virtual ~Launcher();

    /*!
     * Takes a LauncherDataStore into use.
     *
     * \param dataStore LauncherDataStore for storing launcher button positions and entries
     */
    void setLauncherDataStore(LauncherDataStore *dataStore);

    /*!
     * Connects the Launcher to an ApplicationPackageMonitor for monitoring
     * installation and update progress of application packages.
     *
     * \param packageMonitorListener Listens to signals from application package monitor
     */
    void setApplicationPackageMonitorListener(ApplicationPackageMonitorListener *packageMonitorListener);

    /*!
     * Sets the maximum size of the Launcher pages. Negative values are ignored.
     * If a LauncherPage already has more items than the desired maximum the
     * page is not resized.
     *
     * \param maximumPageSize the maximum number of buttons on a single LauncherPage
     */
    void setMaximumPageSize(int maximumPageSize);

signals:
    /*!
     * Signal sent when a launcher button was clicked.
     */
    void launcherButtonClicked();

    /*!
     *  Signal to request panning to specific page
     *
     * \param page number of page to pan to
     */
    void panningRequested(uint page);

    /*!
     * Signal to request moving launcher focus to a page
     */
     void focusToPageRequested(uint page);

     /*!
      * Signal requesting launcher to focus to an button specified by a desktopfile.
      */
     void focusToButtonRequested(const QString& desktopFileEntry);


public slots:

    /*!
     * Requests Launcher to focus to a Launcher Button specified by a desktop file.
     *
     * \param desktopFileEntry is application's desktop file.
     * Name should be given in format "filename.desktop" or in absolute path.
     *
     * \return index of page that contains application. Returns -1 if buttons
     * is not found from launcher.
     */
    int focusToButton(const QString &desktopFileEntry);

    /*!
     * Set launcher to show a page.
     *
     *\param number of page to show.
     */
    void setPage(uint page);

    /*!
     * Updates the state and operation progress of a launcher button.
     * Creates a new placeholder button if one doesn't exist for the given desktopentryfile.
     *
     * \param desktopEntryPath Desktop entry of the package that button represents
     * \param state State button should be set to
     * \param progress Progress of operation
     */
    void updateButtonState(const QString& desktopEntryPath, LauncherButtonModel::State state, int progress);

    /*!
     * Removes placeholder launcher button for an application if application is not installed.
     *
     * We can only remove button if it is found from launcher and desktop entry file paths match.
     * Different desktop entry file paths means that button is not placeholder and hence needs to be removed by
     * launcher data store.
     *
     * \param desktopEntryPath of an application
     */
    void removePlaceholderButton(const QString &desktopEntryPath);

private slots:

    /*!
     * Add a launcher button to launcher.
     *
     * If button already exists in launcher then it is updated.
     *
     * \param desktopEntryPath Path to desktop entry that button should represent
     */
    void addLauncherButton(const QString &desktopEntryPath);

    /*!
     * Remove a launcher button from launcher.
     *
     * \param desktopEntryPath Path to desktop entry that button to be removed represents
     */
    void removeLauncherButton(const QString &desktopEntryPath);

    /*!
     * Updates a launcher button from a .desktop entry file.
     *
     * \param desktopEntryPath Entry the path of the .desktop entry file to update the launcher button from
     *
     * \return Whether button for the desktop entry was found
     */
    bool updateLauncherButton(const QString &desktopEntryPath);

    /*!
     * Updates pages according to the contents of the data store.
     */
    void updatePagesFromDataStore();

private:

    /*!
     * Get desktop entry placement in datastore
     *
     * \param desktopEntryPath Path to desktop entry
     * \return Placement of the entry, if entry is not in datastore then null placement is returned
     */
    Launcher::Placement entryPlacementInDatastore(const QString &desktopEntryPath);

    /*!
     * Add a new launcher button to given pages.
     *
     * \param desktopEntryPath Path to desktop entry that button should represent
     * \param pages List of pages where button is to be added
     */
    void addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages);

    /*!
     * Appends given launcher button to given pages.
     *
     * Finds first available space from last page or adds a new page if last page doesn't have space.
     *
     * \param button Button to be appended
     * \param pages List of pages where button is to be appended
     * \return placement where button was added
     */
    Launcher::Placement appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages);

    /*!
     * Returns Placement for button.
     *
     * \param desktopFileEntry is application's desktop file.
     * Name should be given in format "filename.desktop" or in absolute path.
     * Default applications directory path is used if only file name is given.
     *\return Placement object. If placement is not found for button, placement
     * is initialized with placement.page=-1 and placement.position=-1
     */
    Placement buttonPlacement(const QString &desktopEntryFile);

    /*!
     * Update the given pages list by putting the desktop entries with known
     * placements in the desired pages. Pages are created as necessary.
     * There may be empty pages in the page list after calling this.
     *
     * \param pages the page list to be updated
     */
    void addDesktopEntriesWithKnownPlacements(QList<QSharedPointer<LauncherPage> > &pages);

    /*!
     * Update the given pages list by putting the desktop entries with no known
     * placements on the last page. Pages are created as necessary.
     *
     * \param pages the page list to be updated
     */
    void addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages);

    /*!
     * Remove empty pages from the given page list.
     *
     * \param pages the page list to be updated
     */
    void removeEmptyPages(QList<QSharedPointer<LauncherPage> > &pages);

    /*!
     * Creates a launcher button instance from a .desktop entry file.
     *
     * \param entry the path of the .desktop entry file to create a launcher button from
     * \return a LauncherButton representing the .desktop entry file
     */
    virtual QSharedPointer<LauncherButton> createLauncherButton(const QString &desktopEntryPath);

    /*!
     * Creates a map that contains the placement of each given desktop entry in the launcher.
     * Only the items that have a recognized placement are included in the map.
     * The placement is recognized when it is in the launcher/page/position format
     * (for example "launcher/1/3").
     *
     * \return map containing the placement of each given desktop entry in the launcher
     */
    QMap<Launcher::Placement, QString> createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements);

    /*!
     * Gets a placeholder button for given entry.
     * Finds and returns a button that represents the given entry. If button is not yet found in launcher
     * we add a new placeholder button.
     *
     * \param desktopEntryPath Path to the buttons desktop entry
     */
    QSharedPointer<LauncherButton> placeholderButton(const QString& desktopEntryPath);

    /*!
     * Updates button placement to the launcher data store.
     *
     * If button is not already in store, a new value is created.
     *
     * \param desktopEntryPath Desktop entry path to the desktop entry that button represents
     */
    void updateButtonPlacementInStore(const QString &desktopEntryPath);

    /*!
     * Removes button placement from the launcher data store.
     *
     * \param desktopEntryPath Desktop entry path to the desktop entry that button represents
     */
    void removeButtonPlacementFromStore(const QString &desktopEntryPath);

    /*!
     * Sets the maximum page size of a LauncherPage if defined.
     *
     * \param page the LauncherPage whose maximum size should be set
     */
    void setMaximumPageSizeIfNecessary(QSharedPointer<LauncherPage> &page);

    //! A string used for identifying content to be placed in the launcher
    static const QString LOCATION_IDENTIFIER;

    //! Separator character for the launcher data store values
    static const char SECTION_SEPARATOR;

    //! A template for the launcher content placement string in the data store
    static const QString PLACEMENT_TEMPLATE;

    //! LauncherDataStore for storing launcher button positions and entries
    LauncherDataStore *dataStore;

    //! PackageMonitorListener which listens to signals from application package monitor
    ApplicationPackageMonitorListener *packageMonitorListener;

    //! The maximum page size of each LauncherPage
    int maximumPageSize;

    //! Whether the launcher has been initialized or not
    bool initialized;

#ifdef UNIT_TEST
    friend class Ut_Launcher;
#endif
};

#endif /* LAUNCHER_H */
