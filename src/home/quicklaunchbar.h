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
     * Constructs a new QuickLaunchBar.
     * \param configuration an object that is used for configuration.
     * \param parent the parent object for this object if any.
     */
    explicit QuickLaunchBar(LauncherDataStore* configuration, QGraphicsItem *parent = NULL);

    /*!
     * Destroys the QuickLaunchBar.
     */
    ~QuickLaunchBar();

signals:
    /*!
     * \brief A signal for notifying that the launcher button has been clicked
     */
    void toggleLauncherButtonClicked();

    /*!
     * \brief A signal for notifying that the applet space button has been clicked
     */
    void toggleAppletSpaceButtonClicked();

private slots:
    /*!
     * \brief Updates the widget list based on the contents of the data store
     */
    void updateWidgetList();

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
};

#endif
