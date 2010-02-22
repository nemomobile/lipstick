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

#ifndef QUICKLAUNCHBAR_H
#define QUICKLAUNCHBAR_H

#include <DuiWidgetController>
#include "quicklaunchbarmodel.h"

class DuiFileDataStore;

/*!
 * A widget for showing a quick launch bar.
 */
class QuickLaunchBar : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(QuickLaunchBar)

public:
    /*!
     * Constructs a new QuickLaunchBar.
     */
    explicit QuickLaunchBar(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the QuickLaunchBar.
     */
    ~QuickLaunchBar();

signals:
    /*!
     * \brief A signal for notifying that the launcher button has been clicked
     */
    void launcherButtonClicked();

    /*!
     * \brief A signal for notifying that the applet space button has been clicked
     */
    void appletSpaceButtonClicked();

private slots:
    /*!
     * \brief Launches an application
     *
     * \param application the full path of the application to be launched
     */
    void launchApplication(const QString &application);

    /*!
     * \brief Launches a DUI application
     *
     * \param service the service framework service name of the application to be launched
     */
    void launchDuiApplication(const QString &service);

private:
    //! Initializes the configuration data store
    void initializeDataStore();

    //! Updates the widget list based on the contents of the data store
    void updateWidgetList();

    //! The data store for quick launch bar configuration
    DuiFileDataStore *fileDataStore;
};

#endif
