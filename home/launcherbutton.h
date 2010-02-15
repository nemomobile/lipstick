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

#ifndef LAUNCHERBUTTON_H
#define LAUNCHERBUTTON_H

#include <DuiButton>
#include "launcherbuttonmodel.h"

class LauncherButtonPrivate;

/*!
 * Button widget that can launch an application when its being clicked.
 *
 * User has to set the target to be launched using setTarget() method and
 * the target type using the setTargetType() method. The target type can
 * be an Application or a Link. If the target type is an Application and
 * the target is set this button widget launches the specified application
 * into its own process when the button is clicked.
 */
class LauncherButton : public DuiButton
{
    Q_OBJECT
    DUI_CONTROLLER(LauncherButton)

public:
    /*!
     * Constructs a LauncherButton widget.
     *
     * \param parent the parent widget, defaults to NULL
     */
    LauncherButton(DuiWidget *parent = 0);

    /*!
     * Destroys the Launcher.
     */
    virtual ~LauncherButton();

    /*!
     * \brief Sets the type of the object to be launched.
     *
     * \param type Type of the object to be launched
     */
    void setTargetType(const QString &type);

    /*!
     * \brief Returns the type of the target launched by this button widget.
     *
     * \return Type of the object to be launched
     */
    const QString &targetType() const;

    /*!
     * \brief Sets the target to be launched when this button widget
     * instance is clicked.
     *
     * \param target Full path to the application that will be launched.
     */
    void setTarget(const QString &target);

    /*!
     * \brief Returns full path to the target file launched by this button
     * widget.
     *
     * \return Full path to the application file
     */
    const QString &target() const;

    /*!
     * \brief Sets the path of the thumbnail of this button
     *
     * \param thumbnail The path of the thumbnail to be displayed
     */
    void setThumbnail(const QString &thumbnail);

    /*!
     * \brief Returns the path of the thumbnail of this button widget.
     *
     * \return Thumbnail
     */
    const QString &thumbnail() const;

private slots:
    /*!
     * Attempts to launch the configured application.
     */
    void launch();

signals:
    /*!
     * \brief A signal for notifying that an object of type Application has been launched
     *
     * \param application the name of the application
     */
    void applicationLaunched(const QString &application);

    /*!
     * \brief A signal for notifying that an object of type Application that has an X-Maemo-Service has been launched
     *
     * \param service the name of the dbus service to launch
     */
    void duiApplicationLaunched(const QString &service);

    /*!
     * \brief A signal for notifying that an object of type Link has been launched
     *
     * \param link the link to be launched
     */
    void linkLaunched(const QString &link);

    /*!
     * \brief A signal for notifying that an object of type Directory has been launched
     *
     * \param directory the name of the directory
     * \param title the title representing the directory
     * \param iconId the ID of the icon representing the directory
     */
    void directoryLaunched(const QString &directory, const QString &title, const QString &iconId);
};

#endif /* DUILAUNCHAPPBUTTON_H */
