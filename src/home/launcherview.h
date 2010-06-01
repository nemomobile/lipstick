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
class MFlowLayoutPolicy;
class MLayout;
class Switcher;
class QGraphicsLinearLayout;
class MOverlay;
class MButton;
class MWidget;
class PagedViewport;
class MLayout;
class MLinearLayoutPolicy;

/*!
 * The launcher view draws a background for the container and
 * manages a layout related to the container. The widgets are inside a flow
 * layout. The view also contains an exit button for closing the container.
 */
class LauncherView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(LauncherModel, LauncherStyle)

public:
    /*!
     * Constructs an LauncherView.
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

    /*! Pans launcher to requested page.
     *
     * \param page number of requested page
     */
    void panToPage(uint page);

private:

    /*!
     * Updates layout from models page list.
     *
     * Adds new pages and removes non-exitent.
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
    //! A layout policy for page
    MLinearLayoutPolicy *policy;
    //! The widget that be inside the paged viewport. This will contain all of the launcher items
    MWidget* pannedWidget;
    //! The paged view port used to diaply the pannedWidget
    PagedViewport* pagedViewport;

};

#endif // LAUNCHERVIEW_H
