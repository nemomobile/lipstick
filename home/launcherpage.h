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

#ifndef LAUNCHERPAGE_H
#define LAUNCHERPAGE_H

#include <QObject>
#include <DuiWidgetController>
#include "launcherpagemodel.h"


class LauncherPage : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(LauncherPage)
public:

    /*! 
     * Constructs a LauncherPage
     * \param parent The parent widget of this page.
     */
    LauncherPage(DuiWidget *parent = NULL);

    /*!
     * Add button to page.
     * Fixed positions for buttons are not maintained, so when multiple buttons are added,
     * adding order must be position index order to assure inserting to correct index in button list.
     * Two scenarios exist when the button is not added to a page:
     * \li If the position index is higher than the maximum items allowed per page, 
     * \li If the page is full
     * 
     * The maximum items per page is currently defined as 12
     *
     * \param button Button to be added.
     * \param positionIndex Position where button is to be added.
     * \return whether button was added to page. True if button was added, false if not (eg. if page is full).
     */
    bool insertButton(QSharedPointer<LauncherButton> button, int positionIndex);

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
     * \param button Button to be removed.
     */
    void removeButton(QSharedPointer<LauncherButton> button);

    /*!
     * Checks if this page contains a button representing specific desktop entry
     * \param desktopEntry Desktop entry to be checked
     * \return true if the page contains a button representing desktop entry
     */
    bool contains(const DuiDesktopEntry &desktopEntry) const;

     /*!
     * Removes non-existing buttons from this page.
     * \param entryList List of entries to compare against
     * \param directory Directory path that is pruned
     * \return true if page still contais buttons after pruning
     */
    bool prune(QStringList entryList, QString directory);
};

#endif
