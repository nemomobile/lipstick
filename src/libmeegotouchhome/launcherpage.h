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

#ifndef LAUNCHERPAGE_H
#define LAUNCHERPAGE_H

#include <QObject>
#include <MWidgetController>
#include "launcherpagemodel.h"


class LauncherPage : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(LauncherPage)
public:

    /*!
     * Constructs a LauncherPage
     * \param parent The parent widget of this page.
     */
    LauncherPage(MWidget *parent = NULL);

    /*!
     * Sets the maximum number of buttons on the page.
     * The page is not resized if it already has more than the desired maximum
     * number of items.
     *
     * \param maximumButtonCount the maximum number of buttons on the page.
     */
    void setMaximumButtonCount(int maximumButtonCount);

    /*!
     * Add button to page.
     * Fixed positions for buttons are not maintained, so when multiple buttons are added,
     * adding order must be position index order to assure inserting to correct index in button list.
     * Two scenarios exist when the button is not added to a page:
     * \li If the position index is higher than the maximum items allowed per page,
     * \li If the page is full
     *
     * The default maximum number of items per page is currently defined as 12.
     *
     * \param button Button to be added.
     * \param positionIndex Position where button is to be added.
     * \return Page index on which buttons was added to. -1 if button was not inserted (eg. if page is full).
     */
    int insertButton(QSharedPointer<LauncherButton> button, int positionIndex);

    /*!
     * Tries to appends a button to the end of this page. This is a convenience function for
     * \link LauncherPage::insertButton. See possible error scenarios there.
     *
     * \param button Button to be added.
     * \return whether button was added to page. True if button was added, false if not (eg. if page is full).
     */
    bool appendButton(QSharedPointer<LauncherButton> button);

    /*!
     * Remove button from page.
     *
     * \param button Button to be removed.
     */
    void removeButton(QSharedPointer<LauncherButton> button);

    /*!
     * Remove button from page.
     * Removes a button only if button representing the given desktop entry is found on page.
     *
     * \param desktopEntryPath Desktop entry path specifying the button to remove
     *
     * \return True if button representing the given desktop entry was found from the page
     */
    bool removeButton(const QString &desktopEntryPath);

    /*!
     * Updates a button if button representing the given desktop entry is found from page.
     *
     * \param desktopEntryPath Desktop entry path to check
     *
     * \return True if button representing the given desktop entry was found from the page
     */
    bool updateButton(const QString &desktopEntryPath);

    /*!
     * Get position of button from launcher page.
     *
     *\param desktopEntryPath Desktop entry path of button.
     *\return int position of button on page. Returns -1 if button is not found.
     */
    int launcherButtonPosition(const QString &desktopEntryPath);

    /*! \brief Returns button representing the given entry.
     *
     * \param desktopEntryPath Desktop entry path specifying the button to return
     * \return Button representing the given desktop entry or NULL shared pointer if button is not found
     */
    QSharedPointer<LauncherButton> button(const QString &desktopEntryPath);
};

#endif
