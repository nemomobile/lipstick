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

#ifndef LAUNCHERVIEW_H
#define LAUNCHERVIEW_H

#include <duiextendingbackgroundview.h>
#include "launcherstyle.h"
#include "launchermodel.h"

class Launcher;
class DuiFlowLayoutPolicy;
class DuiLayout;
class Switcher;
class QGraphicsLinearLayout;
class DuiOverlay;
class DuiButton;
class DuiWidget;
class PagedViewport;

/*!
 * The launcher view draws a background for the container and
 * manages a layout related to the container. The widgets are inside a flow
 * layout. The view also contains an exit button for closing the container.
 */
class LauncherView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(LauncherModel, LauncherStyle)

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

    /*! \reimp
     * Re-implemented here to adjust the paging page size when the geometry changes
     */
    virtual void setGeometry(const QRectF &rect);
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! The Launcher controller
    Launcher *controller;
    //! A layout for the widgets
    QGraphicsLinearLayout *layout;
    //! The widget that be inside the paged viewport. This will contain all of the launcher items
    DuiWidget* pannedWidget;
    //! The paged view port used to diaply the pannedWidget
    PagedViewport* pagedViewport;

};

#endif // LAUNCHERVIEW_H
