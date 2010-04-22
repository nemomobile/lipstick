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
#include <QTimer>
#include <QList>
#include <QMutex>
#include <QFileSystemWatcher>
#include <MWidgetController>
#include "launchermodel.h"

class MDesktopEntry;
class LauncherDataStore;
class MDataStore;

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
    /*!
     * Constructs a Launcher widget.
     *
     * \param parent Parent for the widget, defaults to NULL
     */
    Launcher(MWidget *parent = NULL);

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
     * Starts a M application.
     *
     * \param serviceName the service name of the application to be started
     * \return \c true if starting succeeded, \c false otherwise
     */
    static bool startMApplication(const QString &serviceName);

    /*!
     * Returns a pointer to the launcher data store
     */
    LauncherDataStore *dataStore();

signals:
    /*!
     * Signal sent when a launcher button was clicked.
     */
    void launcherButtonClicked();

private slots:

    /*!
     * Updates buttons list according to a desktop entry directory.
     */
    void updateButtonList();

    /*!
     * \brief Launches an application.
     */
    void launchApplication(const QString &application);

    /*!
     * \brief Launches a M application.
     */
    void launchMApplication(const QString &service);

    /*!
     * Restores buttons list data from data store.
     */
    void restoreButtonsFromDataStore();

    /*!
     * Starts a thread to updates button list data in data store.
     */
    void startButtonStoreThread();


private:
    //! A file system watcher for the desktop entry file directory
    QFileSystemWatcher watcher;

    //! DataStore handle for storing launcher button positions and entries
    LauncherDataStore *dataStore_;

    //! List of file types to support for desktop entries.
    QStringList supportedDesktopEntryFileTypes;

    //! Whether the launcher has been initialized or not
    bool initialized;

    //! Timer used for updating datastore
    QTimer updateButtonsTimer;

    //! Mutex used for updating datastore in a thread
    QMutex updateButtonsMutex;

    /*!
     * Activates launcher by initializing the Launcher if necessary and by updating the buttons.
     * Initialization restores launcher content from data store, reads the contents of the desktop
     * entry file directories, update buttons according to entries and starts to monitor desktop
     * entry directories.
     */
    void activateLauncher();

    /*!
     * Creates a launcher button instance from a MDesktopEntry.
     *
     * \param entry the MDesktopEntry to create a launcher button from
     * \return a LauncherButton representing the MDesktopEntry
     */
    LauncherButton *createLauncherButton(const MDesktopEntry &entry);

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
     * \param desktopEntryFile Path of a desktop entry to be checked
     * \return true if the launcher contains button representing the given desktop entry
     */
    bool contains(const QString &desktopEntryFile);

    /*!
     * Updates buttons list data in data store.
     */
    void updateButtonsInDataStore();

    /*!
     * Adds a new button to launcher
     * \param entry Desktop entry from which to create the button
     */
    void addNewLauncherButton(const MDesktopEntry &entry);

    /*!
     * Checks if desktop entry is valid for launcher
     * \param entry Desktop entry to be validated
     * \param acceptedTypes List of accepted entry types
     * \return is desktop entry valid
     */
    bool isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes);

#ifdef UNIT_TEST
    friend class Ut_Launcher;
#endif

};

#endif /* LAUNCHER_H */
