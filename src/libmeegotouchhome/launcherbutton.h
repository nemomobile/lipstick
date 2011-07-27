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

#include <QTimer>
#include <MButton>
#include "launcherbuttonmodel.h"
#include "launcheraction.h"

class WindowInfo;

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
    LauncherButton(const QString &desktopEntryPath = QString(), MWidget *parent = 0, LauncherButtonModel *model = new LauncherButtonModel);

    /*!
     * Creates a launcher button representing a .desktop file.
     *
     * \param entry The desktop entry that button represents
     * \param parent Parent object
     * \param model Model for controller
     */
    LauncherButton(const QSharedPointer<MDesktopEntry> &entry, MWidget *parent = 0, LauncherButtonModel *model = new LauncherButtonModel);

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
     * \param entry The desktop entry for the button
     */
    void updateFromDesktopEntry(const QSharedPointer<MDesktopEntry> &entry);

    /*!
     * Sets the state of a button
     *
     * \param state State of a button
     */
    void setState(LauncherButtonModel::State state);

    /*!
     * Sets the progress of a button
     * Progress is normalized between (0..100)
     *
     * \param newProgress Progress of operation
     * \param total Maximum value for progress
     */
    void setOperationProgress(int newProgress, int total = 100);

    /*!
     * Returns progress of button's ongoing operation
     * \return operation progress
     */
    int operationProgress() const;

    /*!
     * Sets the package name of the application that button is representing
     * \param packageName Package name
     */
    void setPackageName(const QString& packageName);

    /*!
     * Returns the package name of the application that button is representing
     * \return name Package name
     */
    QString packageName() const;

    /*!
     * Sets whether the package represented by button is removable
     * \param removable Is package removable
     */
    void setPackageRemovable(const bool removable);

    /*!
     * Return whether the package represented by button is removable
     * \return \c true if package is removable, \c false otherwise
     */
    bool packageRemovable() const;

protected:
    //! \reimp
    virtual void retranslateUi();
    //! \reimp_end

public slots:
    /*!
     * Enables the launching state for button
     */
    void enableLaunchingState();

    /*!
     * Disables the launching state for button
     */
    void disableLaunchingState();

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private slots:
    /*!
     * Attempts to launch the configured object.
     */
    void launch();

    /*!
     * Handles window on top of home window.
     */
    void windowOnTopOfHome(const WindowInfo &window);

private:
    /*!
     * Initializes the launcher button.
     */
    void init();

    //! Sets the text to the given text or a single whitespace if the text is empty. This is to work around bug #256910.
    void setTextEnsuringItIsNotEmpty(const QString &text = QString());

    //! The action for this button
    LauncherAction action;

    //! Button state when launching is initiated
    LauncherButtonModel::State stateBeforeLaunch;

    //! Timer for resetting the button state after launching
    QTimer launchStateResetTimer;


#ifdef UNIT_TEST
    friend class Ut_LauncherButton;
#endif
};

#endif /* MLAUNCHAPPBUTTON_H */
