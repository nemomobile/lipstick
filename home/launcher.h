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

/*!
 * Widget for launching and browsing installed applications.
 * The widget monitors a desktop file entry directory and creates buttons
 * that represent the applications that can be launched.
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
     * If enabled is true, the launcher is enabled (items can be clicked, button is visible); otherwise, it is disabled.
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

public slots:
    /*!
     * Opens root category of launcher. Launcher can have subcategories under root category
     * (f.ex. games, demos). This opens the launcher in root category which contains all other categories.
     */
    void openRootCategory();

private slots:
    /*!
     * \brief Reads the contents of a .desktop or .directory file directory
     */
    void readDirectory(const QString &path, bool updateWidgetList = true);

    /*!
     * \brief Launches an application and returns to the root category
     */
    void launchApplication(const QString &application);

    /*!
     * \brief Launches a DUI application and returns to the root category
     */
    void launchDuiApplication(const QString &service);

    /*!
     * \brief Launches a link
     */
    void launchLink(const QString &link);

    /*!
     * \brief Launches a directory and moves into that category
     */
    void launchDirectory(const QString &directory, const QString &title, const QString &iconId);

private:
    //! A file system watcher for the desktop entry file directory
    QFileSystemWatcher watcher;

    //! A string that describes the current category
    QString currentCategory;

    //! A type for a container that can hold DuiDesktopEntry objects
    typedef QList<DuiDesktopEntry *> DesktopEntryContainer;

    //! Desktop entries
    DesktopEntryContainer applicationDesktopEntries;

    //! Desktop entries for directories
    DesktopEntryContainer categoryDesktopEntries;

#ifdef ENABLE_QTTRACKER
    //! The LibQtTracker item model for shortcuts
    SopranoLive::LiveNodes shortcutItemModel;
#endif

    //! Whether the launcher has been initialized or not
    bool initialized;

    /*!
     * Initializes the Launcher if necessary by reading the contents of the
     * desktop entry file directory and starting to monitor it.
     */
    void initializeIfNecessary();

    /*!
     * Sets the category of this Launcher.
     *
     * \param category the category of this Launcher
     * \param title text to use as the title, defaults to an empty string
     * \param iconId the ID of the icon to use in the title, defaults to an empty string
     */
    void setCategory(const QString &category, const QString &title = QString(), const QString &iconId = QString());

    /*!
     * Creates a launcher button instance from a DuiDesktopEntry.
     *
     * \param entry the DuiDesktopEntry to create a launcher button from
     * \return a DuiWidget representing the DuiDesktopEntry
     */
    DuiWidget *createLauncherButton(const DuiDesktopEntry &entry);

    /*!
     * Helper method to update an internal desktop entry list. Checks if some desktop entries
     * should be included in the launcher or not.
     * \param desktopEntryContainer the container that will get manipulated.
     * \param path the file path that gets searched for desktop entry files.
     * \param nameFilter a filter for the desktop entry files. E.g. "*.desktop".
     * \param acceptedTypes the types that should be accepted. This list is checked against the desktop files' Type value.
     */
    void updateDesktopEntryList(DesktopEntryContainer &desktopEntryContainer, const QString &path, const QString &nameFilter, const QStringList &acceptedTypes) const;

#ifdef ENABLE_QTTRACKER
    /*!
     * Creates a launcher button instance from a shortcut IRI.
     *
     * \param shortcut the Tracker object to create a shortcut launcher button from
     * \return a DuiWidget representing the shortcut IRI
     */
    DuiWidget *createShortcutLauncherButton(SopranoLive::LiveNode shortcut);
#endif

    /*!
     * Updates the list of widgets based on the DuiDesktopEntries.
     */
    void updateWidgetList();
};

#endif /* LAUNCHER_H */
