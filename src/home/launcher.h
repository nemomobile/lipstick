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
class ApplicationPackageMonitor;

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
        Placement(const QString &placement);

        QString location;
        int page;
        int position;
    };

    /*!
     * Constructs a Launcher widget.
     *
     * \param dataStore LauncherDataStore for storing launcher button positions and entries
     * \param parent Parent for the widget, defaults to NULL
     */
    Launcher(LauncherDataStore *dataStore, ApplicationPackageMonitor *packageMonitor, QGraphicsItem *parent = NULL);

    /*!
     * Destroys the Launcher.
     */
    virtual ~Launcher();

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
     * Signal to request moving launcher focus to first page
     */
    void focusToFirstPageRequested();

public slots:

    /*!
     * Open launcher and pan to page where launcher button of requested application is placed
     *
     * \param desktopFileEntry is application's desktop file.
     * Name should be given in format "filename.desktop" or in absolute path.
     * Default applications directory path is used if only file name given.
     *
     * \return number of page that contains application. Returns -1 if .desktop
     * file is not found.
     */
    int panToPage(const QString &desktopFileEntry);

    /*!
     * Set launcher to show first page
     */
    void setFirstPage();

    /*!
     * Set button state to "downloading", and calculate and set it's progress
     *
     * \param packageName Name of package related to the operation
     * \param desktopEntryName Desktop entry of the application button represents
     * \param bytesLoaded Amount of bytes loaded
     * \param bytesTotal Total amount of bytes to download
     */
    void setDownloadProgress(const QString& packageName, const QString& desktopEntryPath, int bytesLoaded, int bytesTotal);

    /*!
     * Set button state to "installing", and set it's progress
     *
     * \param packageName Name of package related to the operation
     * \param desktopEntryName Desktop entry of the application button represents
     * \param percentage Percentage of installation completed
     */
    void setInstallProgress(const QString& packageName, const QString& desktopEntryPath, int percentage);

    /*!
     * Set button state to "installed"
     *
     * \param packageName Name of package related to the operation
     * \param desktopEntryName Desktop entry of the application button represents
     */
    void setOperationSuccess(const QString& packageName, const QString& desktopEntryPath);

    /*!
     * Set button state to "broken"
     *
     * \param packageName Name of package related to the operation
     * \param desktopEntryName Desktop entry of the application button represents
     * \param error Error message
     */
    void setOperationError(const QString& packageName, const QString& desktopEntryPath, const QString& error);

private slots:

    /*!
     * Add a launcher button to launcher.
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
     */
    void updateLauncherButton(const QString &desktopEntryPath);

    /*!
     * Updates pages according to the contents of the data store.
     */
    void updatePagesFromDataStore();

private:

    /*!
     * Add a launcher button to given pages.
     *
     * Finds first available space from last page or adds a new page if last page doesn't have space.
     *
     * \param desktopEntryPath Path to desktop entry that button should represent
     * \param pages List of pages where button is to be added
     */
    void addLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages);

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
    QSharedPointer<LauncherButton> createLauncherButton(const QString &desktopEntryPath);

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
     * Updates the state and operation progress of a launcher button.
     * Creates a new placeholder button if one doesn't exist for the given desktopentryfile.
     *
     * \param packageName Name of the package that button represents
     * \param desktopEntryPath Desktop entry of the package that button represents
     * \param state State button should be set to
     * \param progress Progress of operation
     */
    void updateButtonState(const QString &packageName, const QString& desktopEntryPath, LauncherButtonModel::State state, int progress);

    /*!
     * Adds a placeholder button to launcher
     * \param button Placeholder button to be added to launcher
     */
    void addPlaceholderButtonToLauncher(QSharedPointer<LauncherButton> button);

    //! A mapping from packageName to launcher buttons
    QMap<QString, QSharedPointer<LauncherButton> > launcherButtonMap;

    //! A string used for identifying content to be placed in the launcher
    static const QString LOCATION_IDENTIFIER;

    //! Separator character for the launcher data store values
    static const char SECTION_SEPARATOR;

    //! A template for the launcher content placement string in the data store
    static const QString PLACEMENT_TEMPLATE;

    //! LauncherDataStore for storing launcher button positions and entries
    LauncherDataStore *dataStore;

    //! PackageMonitor from which launcher receives signals
    ApplicationPackageMonitor *packageMonitor;

    //! Whether the launcher has been initialized or not
    bool initialized;


#ifdef UNIT_TEST
    friend class Ut_Launcher;
#endif
};

#endif /* LAUNCHER_H */
