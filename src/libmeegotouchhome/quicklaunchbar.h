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
class ApplicationPackageMonitorListener;

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
    ~QuickLaunchBar();

    /*!
     * Takes a LauncherDataStore into use.
     *
     * \param dataStore LauncherDataStore for storing launcher button positions and entries
     */
    void setLauncherDataStore(LauncherDataStore *dataStore);

    /*!
     * Sets the ApplicationPackageMonitorListener for monitoring
     * installation and update progress of application packages.
     *
     * \param packageMonitorListener Listens to signals from application package monitor
     */
    void setApplicationPackageMonitorListener(ApplicationPackageMonitorListener *packageMonitorListener);

signals:
    /*!
     * \brief A signal for notifying that the launcher button has been clicked
     */
    void toggleLauncherButtonClicked();

public slots:
    /*!
     * Finds the correct button based on desktop entry path and updates it's state and operation progress
     *
     * \param desktopEntryPath Desktop entry of the package that button represents
     * \param state State button should be set to
     * \param progress Progress of operation
     */
    void updateButtonState(const QString &desktopEntryPath, LauncherButtonModel::State state, int progress);

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

    //! The number of launcher buttons in the quick launch bar
    static const int NUMBER_OF_LAUNCHER_BUTTONS;

    //! A string used for identifying content to be placed in the quick launch bar
    static const QString LOCATION_IDENTIFIER;

    //! Separator character for the launcher data store values
    static const char SECTION_SEPARATOR;

    //! The data store for quick launch bar configuration
    LauncherDataStore *dataStore;

    //! PackageMonitorListener which listens to signals from application package monitor
    ApplicationPackageMonitorListener *packageMonitorListener;

#ifdef UNIT_TEST
    friend class Ut_QuickLaunchBar;
#endif
};

#endif
