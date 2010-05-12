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

/*!
 * Widget for launching and browsing installed applications.
 * The widget monitors a desktop file entry directory and creates buttons
 * that represent the applications that can be launched.
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
    Launcher(LauncherDataStore *dataStore, QGraphicsItem *parent = NULL);

    /*!
     * Destroys the Launcher.
     */
    virtual ~Launcher();

    /*!
     * Starts an application.
     *
     * \param application the application to be started
     * \return \c true if starting succeeded, \c false otherwise
     */
    static bool startApplication(const QString &application);

    /*!
     * Starts a M application.
     *
     * \param serviceName the service name of the application to be started
     * \return \c true if starting succeeded, \c false otherwise
     */
    static bool startMApplication(const QString &serviceName);

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

private slots:
    /*!
     * Updates pages according to the contents of the data store.
     */
    void updatePagesFromDataStore();

private:

    /*!
     * Returns page number of requested application by searching application's
     * desktop file entry path from launcher's datastorage.
     *
     * \param desktopFileEntry is application's desktop file.
     * Name should be given in format "filename.desktop" or in absolute path.
     * Default applications directory path is used if only file name is given.
     */
    int pageNumber(const QString &desktopFileEntry);

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

    //! A string used for identifying content to be placed in the launcher
    static const QString LOCATION_IDENTIFIER;

    //! Separator character for the launcher data store values
    static const char SECTION_SEPARATOR;

    //! A template for the launcher content placement string in the data store
    static const QString PLACEMENT_TEMPLATE;

    //! LauncherDataStore for storing launcher button positions and entries
    LauncherDataStore *dataStore;

    //! Whether the launcher has been initialized or not
    bool initialized;
};

#endif /* LAUNCHER_H */
