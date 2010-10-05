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
#ifndef LAUNCHERVIEW_H
#define LAUNCHERVIEW_H

#include <mextendingbackgroundview.h>
#include "launcherstyle.h"
#include "launchermodel.h"

class Launcher;
class PagedViewport;
class MLayout;
class MLinearLayoutPolicy;

/*!
 * The launcher manages a layout for the launcher pages.
 */
class LauncherView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(LauncherModel, LauncherStyle)

public:
    /*!
     * Constructs a LauncherView.
     *
     * \param container the Launcher controller to be used
     */
    LauncherView(Launcher *container);

    /*!
     * Destroys the LauncherView.
     */
    virtual ~LauncherView();

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private slots:
    /*!
     * Pans launcher to the requested page.
     *
     * \param page number of requested page
     */
    void panToPage(uint page);

    /*!
     * Focuses launcher to a page.
     */
    void focusToPage(uint page);

    /*!
     * Focuses launcher to a button.
     */
    void focusToButton(const QString &desktopFileEntry);

private:
    /*!
     * Updates layout from models page list.
     *
     * Adds new pages and removes non-existent ones.
     */
    void updateLayoutFromModel();

    /*!
     * Removes a page from layout and sets its parent to null.
     *
     * View or layout are not responsible for deleting the page after this.
     *
     * \param page Page to be removed.
     */
    void removePageFromLayout(LauncherPage *page);

    //! A layout for the widgets
    MLayout *layout;
    //! A layout policy for pages
    MLinearLayoutPolicy *policy;
    //! The widget that is inside the paged viewport. This will contain all of the launcher pages.
    MWidget* pannedWidget;
    //! The paged viewport used to display the pannedWidget
    PagedViewport* pagedViewport;

#ifdef UNIT_TEST
    friend class Ut_LauncherView;
#endif
};

#endif // LAUNCHERVIEW_H
