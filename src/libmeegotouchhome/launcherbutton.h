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
#include "launcheraction.h"

/*!
 * A button widget that represents a .desktop file. Triggers the default
 * action associated with the .desktop file when clicked.
 */
class LauncherButton : public MButton
{
    Q_OBJECT
    M_CONTROLLER(LauncherButton)

    Q_PROPERTY(QString desktopEntryPath READ desktopEntry)
    Q_PROPERTY(LauncherButtonModel::State buttonState READ buttonState)
    Q_PROPERTY(int operationProgress READ operationProgress)

public:
    /*!
     * Creates a launcher button representing a .desktop file.
     *
     * \param desktopEntryPath path to the .desktop file to create a launcher button from
     * \param parent the parent widget, defaults to NULL
     */
    LauncherButton(const QString &desktopEntryPath = QString(), MWidget *parent = 0,
                   LauncherButtonModel *model = new LauncherButtonModel);

    /*!
     * Destroys the Launcher.
     */
    virtual ~LauncherButton();

    /*!
     * Returns the file name of the desktop entry where this launcher button was constructed from.
     * \return the desktop entry file name.
     */
    QString desktopEntry() const;

    /*!
     * Returns current button state.
     * \return the button state.
     */
    LauncherButtonModel::State buttonState() const;

    /*!
     * Updates button from given desktop entry.
     * \param desktopEntryPath Path to the desktop entry to update the button from.
     */
    void updateFromDesktopEntry(const QString &desktopEntryPath);

    /*!
     * Sets the state and progress of a button
     * Progress property is changed only if parameter progress is valid (0..100)
     * \param state State of a button
     * \param progress Progress of operation
     */
    void setState(LauncherButtonModel::State state, int progress);

    /*!
     * Returns progress of button's ongoing operation
     * \return operation progress
     */
    int operationProgress() const;

protected:
    //! \reimp
    virtual void retranslateUi();
    //! \reimp_end

private slots:
    /*!
     * Attempts to launch the configured object.
     */
    void launch();

    /*!
     * Stops the launch animation.
     */
    void stopLaunchProgress();

private:
    /*!
     * Initializes the launcher button.
     */
    void init();

    //! Whether an object represented by any launcher button is being launched or not
    static bool launching;

    //! The action for this button
    LauncherAction action;

#ifdef UNIT_TEST
    friend class Ut_LauncherButton;
#endif
};

#endif /* MLAUNCHAPPBUTTON_H */
