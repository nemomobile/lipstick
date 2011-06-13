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

#ifndef QUICKLAUNCHBAR_H
#define QUICKLAUNCHBAR_H

#include <MWidgetController>
#include "quicklaunchbarmodel.h"

class LauncherDataStore;
class LauncherButton;
class ApplicationPackageMonitor;

/*!
 * A widget for showing a quick launch bar.
 */
class QuickLaunchBar : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(QuickLaunchBar)

public:
    //! A class for storing, parsing and ordering the placement information of the items
    class Placement {
    public:
        Placement(const QString &placement);

        int position;
    };

    /*!
     * Constructs a new QuickLaunchBar. The QuickLaunchBar will not
     * store/restore launcher button positions and entries before a
     * LauncherDataStore has been set using setLauncherDataStore().
     *
     * \param parent the parent object for this object if any.
     */
    explicit QuickLaunchBar(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the QuickLaunchBar.
     */
    virtual ~QuickLaunchBar();

    /*!
     * Takes a LauncherDataStore into use.
     *
     * \param dataStore LauncherDataStore for storing launcher button positions and entries
     */
    void setLauncherDataStore(LauncherDataStore *dataStore);

    /*!
     * Sets the ApplicationPackageMonitor for signaling
     * installation and update progress of application packages.
     *
     * \param packageMonitor Signals the package state changes
     */
    void setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor);

signals:
    /*!
     * \brief A signal for notifying that the launcher button has been clicked
     */
    void toggleLauncherButtonClicked();

    /*!
     * \brief Sent when a launcher button has been clicked
     */
    void launcherButtonClicked();

public slots:
    /*!
     * Finds the correct button based on desktop entry path and updates it's state and operation progress
     *
     * \param desktopEntryPath Desktop entry of the package that button represents
     * \param packageName name of the package
     * \param state State button should be set to
     * \param packageRemovable is the package represented by this button removable
     */
    void updateButtonState(const QString &desktopEntryPath, const QString &packageName, const QString &state, bool packageRemovable);


    /*!
     * Update progress of a launcher button.
     *
     * \param desktopEntryPath Desktop entry of the package that button represents
     * \param progress Progress of operation
     * \param total Maximum progress of operation
     */
    void updateProgress(const QString& desktopEntryPath, int already, int total);

private slots:
    /*!
     * \brief Updates the widget list based on the contents of the data store
     */
    void updateButtons();

private:
    /*!
     * Creates a launcher button instance from a .desktop entry file.
     *
     * \param entry the path of the .desktop entry file to create a launcher button from
     * \return a LauncherButton representing the .desktop entry file
     */
    LauncherButton *createLauncherButton(const QString &desktopEntryPath);

    /*!
     * Creates a map that contains the placement of each given desktop entry in the launcher.
     * Only the items that have a recognized placement are included in the map.
     * The placement is recognized when it is in the quicklaunchbar/position format
     * (for example "quicklaunchbar/3").
     *
     * \return map containing the placement of each given desktop entry in the quick launch bar
     */
    QMap<QuickLaunchBar::Placement, QString> createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements);

    //! A string used for identifying content to be placed in the quick launch bar
    static const QString LOCATION_IDENTIFIER;

    //! Separator character for the launcher data store values
    static const char SECTION_SEPARATOR;

    //! The data store for quick launch bar configuration
    LauncherDataStore *dataStore;

    //! PackageMonitor signals the package state changes
    ApplicationPackageMonitor *packageMonitor;

#ifdef UNIT_TEST
    friend class Ut_QuickLaunchBar;
#endif
};

#endif
