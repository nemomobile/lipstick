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

#ifndef LAUNCHERBUTTON_H
#define LAUNCHERBUTTON_H

#include <MButton>
#include "launcherbuttonmodel.h"

class MDesktopEntry;
class LauncherAction;

/*!
 * Button widget that can launch an application when its being clicked.
 *
 * User has to set the target to be launched using setTarget() method and
 * the target type using the setTargetType() method. The target type can
 * be an Application. If the target type is an Application and
 * the target is set this button widget launches the specified application
 * into its own process when the button is clicked.
 */
class LauncherButton : public MButton
{
    Q_OBJECT
    M_CONTROLLER(LauncherButton)

    Q_PROPERTY(QString desktopEntryPath READ desktopEntry)

public:
    /*!
     * Constructs a LauncherButton widget.
     *
     * \param parent the parent widget, defaults to NULL
     */
    LauncherButton(MWidget *parent = 0);

    /*!
     * Creates a launcher button instance from a MDesktopEntry.
     *
     * \param desktopEntryPath Path to the desktop entry to create a launcher button from
     * \param parent the parent widget, defaults to NULL
     */
    LauncherButton(const QString &desktopEntryPath, MWidget *parent = 0);

    /*!
     * Destroys the Launcher.
     */
    virtual ~LauncherButton();

    /*!
     * \brief Sets the action to launch the application described by the
     * desktop file. 
     *
     * \param action The action to launch the application
     */
    void setAction(const LauncherAction &action);

    /*!
     * \brief Returns the action to launch the application described by the
     * desktop file
     *
     * \return The action to launch the application
     */
    LauncherAction action() const;

    /*!
     * Returns the file name of the desktop entry where this launcher button was constructed from.
     * \return the desktop entry file name.
     */
    QString desktopEntry() const;

    /*!
     * Updates button from given desktop entry.
     * \param desktopEntryPath Path to the desktop entry to update the button from.
     */
    void updateFromDesktopEntry(const QString &desktopEntryPath);

    /*!
     * Handles the language change notification and propagets is to MButton.
     */
    void retranslateUi();

private slots:
    /*!
     * Attempts to launch the configured application.
     */
    void launch();

private:
    void updateIcon(const LauncherAction &action);

#ifdef UNIT_TEST
    friend class Ut_LauncherButton;
#endif
};

#endif /* MLAUNCHAPPBUTTON_H */
