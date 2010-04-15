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
#include <QFileSystemWatcher>
#include "quicklaunchbarmodel.h"

class MDataStore;
class LauncherDataStore;

/*!
 * A widget for showing a quick launch bar.
 */
class QuickLaunchBar : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(QuickLaunchBar)

public:
    /*!
     * Constructs a new QuickLaunchBar.
     * \param parent the parent object for this object if any.
     */
    explicit QuickLaunchBar(QGraphicsItem *parent = NULL);

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
     * \brief Launches an application
     *
     * \param application the full path of the application to be launched
     */
    void launchApplication(const QString &application);

    /*!
     * \brief Launches a M application
     *
     * \param service the service framework service name of the application to be launched
     */
    void launchMApplication(const QString &service);

    //! Updates the widget list based on the contents of the data store
    void updateWidgetList();

private:
    //! Initializes the configuration data store
    void initializeDataStore();

    //! The number of launcher buttons in the quick launch bar
    static const int NUMBER_OF_LAUNCHER_BUTTONS;

    //! The data store for quick launch bar configuration
    LauncherDataStore *configurationDataStore;

    /*!
     * An initialization helper function that should be called once from any constructor.
     */
    void init();

    //! A file system watcher for the desktop entry file directory
    QFileSystemWatcher desktopDirectoryWatcher;
};

#endif
