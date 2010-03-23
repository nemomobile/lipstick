/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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
#include <QFileSystemWatcher>
#include <DuiWidgetController>
#include "launchermodel.h"

#ifdef ENABLE_QTTRACKER
#include <QtTracker/Tracker>
#endif

class DuiDesktopEntry;
class LauncherDataStore;
class DuiDataStore;

/*!
 * Widget for launching and browsing installed applications.
 * The widget monitors a desktop file entry directory and creates buttons
 * that represent the applications that can be launched.
 *
 * Each application .desktop file must define the Name, Type and Icon keys where type is Application. The Exec key must define the application binary to be launched when the icon is selected.
 *
 * Shortcuts are stored in tracker in \c DesktopBookmarkFolder as \c nfo:bookmark objects. Bookmarks define title, bookmark content iri, and bookmark thumbnail graphics for each shortcut. All shortcuts are displayed in the Launcher top level menu. Each shortcut is bound to a !DuiServiceAction that is triggered when shortcut is clicked. This will commence default action associated with the content iri in the system.
 *
 * Convenience API for creating shortcuts on homescreen launcher menu is provided in the \c libduihome package through the \c DuiHomeShortcutManager class.
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
 *
 * Example sparql query to list homescreen shortcut items and its output:
 * \code
 * tracker-sparql -q '
 *    SELECT ?ref ?thumb ?title WHERE {
 *        <maemo:DesktopBookmarkFolder> nfo:containsBookmark ?x.
 *        ?x maemo:bookmarkThumbnail ?thumb ;
 *           nfo:bookmarks ?ref ;
 *           nie:title ?title .
 *   }'
 *   file:///home/user/image.jpeg, file:///home/user/.thumbnails/large/0912830981230.png, Lovely image
 *   file:///home/user/image.jpeg, file:///home/user/.thumbnails/large/0912830981230.png, Lovely image
 * \endcode
 */
class Launcher : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(Launcher)

public:
    /*!
     * Constructs a Launcher widget.
     *
     * \param parent Parent for the widget, defaults to NULL
     */
    Launcher(DuiWidget *parent = NULL);

    /*!
     * Destroys the Launcher.
     */
    virtual ~Launcher();

    /*!
     * If enabled is true, the launcher is activated (Launcher::activateLauncher()) and enabled (items can be clicked, button is visible); otherwise, it is disabled.
     *
     * \param enabled if true, the launcher is enabled; otherwise, it is disabled
     */
    void setEnabled(bool enabled);

    /*!
     * Starts an application.
     *
     * \param application the application to be started
     * \return \c true if starting succeeded, \c false otherwise
     */
    static bool startApplication(const QString &application);

    /*!
     * Starts a DUI application.
     *
     * \param serviceName the service name of the application to be started
     * \return \c true if starting succeeded, \c false otherwise
     */
    static bool startDuiApplication(const QString &serviceName);

signals:
    /*!
     * Signal sent when a launcher button was clicked.
     */
    void launcherButtonClicked();

private slots:

    /*!
     * Updates buttons list according to a desktop entry directory.
     * Removes buttons representing non-existing entries and adds new buttons for new entries.
     * \param path the file path that gets searched for desktop entry files.
     */
    void updateButtonListFromDirectory(const QString &path);

    /*!
     * \brief Launches an application.
     */
    void launchApplication(const QString &application);

    /*!
     * \brief Launches a DUI application.
     */
    void launchDuiApplication(const QString &service);

    /*!
     * \brief Launches a link
     */
    void launchLink(const QString &link);

private:
    //! A file system watcher for the desktop entry file directory
    QFileSystemWatcher watcher;

    //! DataStore handle for storing launcher button positions and entries
    LauncherDataStore *dataStore;

#ifdef ENABLE_QTTRACKER
    //! The LibQtTracker item model for shortcuts
    SopranoLive::LiveNodes shortcutItemModel;
#endif

    //! Whether the launcher has been initialized or not
    bool initialized;

    /*!
     * Activates launcher by initializing the Launcher if necessary and by updating the buttons.
     * Initialization restores launcher content from data store, reads the contents of the desktop
     * entry file directories, update buttons according to entries and starts to monitor desktop
     * entry directories.
     */
    void activateLauncher();

    /*!
     * Updates the list of buttons based on the watched desktop entry directories.
     */
    void updateButtonList();

    /*!
     * Creates a launcher button instance from a DuiDesktopEntry.
     *
     * \param entry the DuiDesktopEntry to create a launcher button from
     * \return a LauncherButton representing the DuiDesktopEntry
     */
    LauncherButton *createLauncherButton(const DuiDesktopEntry &entry);

    /*!
     * Connects the necessary signal so that when the laucher button is clicked
     * the correct actions are taken.
     *
     * \param launcherButton The laucher button to connect
     */
    void connectLauncherButton(LauncherButton* launcherButton);

    /*!
     * Checks if launcher contains a button representing specific desktop entry
     *
     * \param desktopEntry Desktop entry to be checked
     * \return true if the launcher contains button representing the given desktop entry
     */
    bool contains(const DuiDesktopEntry &entry);

    /*!
     * Updates buttons list data in data store.
     */
    void updateButtonsInDataStore();

    /*!
     * Restores buttons list data from data store.
     */
    void restoreButtonsFromDataStore();

    /*!
     * Updates buttons list from specific entry files in a directory.
     * Checks if some desktop entries should be included in the launcher or not.
     * \param path the file path that gets searched for desktop entry files.
     * \param nameFilter a filter for the desktop entry files. E.g. "*.desktop".
     * \param acceptedTypes the types that should be accepted. This list is checked against the desktop files' Type value.
     */
    void updateButtonListFromEntries(const QString &path, const QString &nameFilter, const QStringList &acceptedTypes);

    /*!
     * Adds a new button to launcher
     * \param entry Desktop entry from which to create the button
     */
    void addNewLauncherButton(const DuiDesktopEntry &entry);

    /*!
     * Checks if desktop entry is valid for launcher
     * \param entry Desktop entry to be validated
     * \param acceptedTypes List of accepted entry types
     * \return is desktop entry valid
     */
    bool isDesktopEntryValid(const DuiDesktopEntry &entry, const QStringList &acceptedTypes);

#ifdef ENABLE_QTTRACKER
    /*!
     * Creates a launcher button instance from a shortcut IRI.
     *
     * \param shortcut the Tracker object to create a shortcut launcher button from
     * \return a LauncherButton representing the shortcut IRI
     */
    LauncherButton *createShortcutLauncherButton(SopranoLive::LiveNode shortcut);
#endif

};

#endif /* LAUNCHER_H */
