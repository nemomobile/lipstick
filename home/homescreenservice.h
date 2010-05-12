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

#ifndef HOMESCREENIMPL_H
#define HOMESCREENIMPL_H

#include <QObject>

/*!
 * HomeScreenService implements com.meego.core.HomeScreen service's interface.
 */
class HomeScreenService : public QObject
{
    Q_OBJECT
public:
    /*!
     * Initializes HomeScreenService object.
     */
    HomeScreenService();
    /*!
     * Destroys HomeScreenService.
     */
    virtual ~HomeScreenService();


public Q_SLOTS:
    /*!
     * Implements HomeScreen service's showLauncher function.
     * Show launcher and pans to page that has specified application.
     *
     * \param desktop file of application. Desktop file can be given with
     * or without absolute path. If no absolute path is given default
     * applications desktop file directory path is used. If empty string is
     * given launcher is opened from a page it was closed from.
     */
    void showLauncher(const QString &desktopFile);

signals:
    /*!
     * A signal to request launcher to focus on a specified application.
     *
     * \param desktop file of application.
     */
    void focusToLauncherApp(const QString &desktopFile);

};
#endif




