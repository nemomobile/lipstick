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
#ifndef QUICKLAUNCHBARVIEW_H
#define QUICKLAUNCHBARVIEW_H

#include <DuiWidgetView>
#include "quicklaunchbarmodel.h"
#include "quicklaunchbarstyle.h"

class QuickLaunchBar;
class QGraphicsLinearLayout;

/*!
 * A view class for the QuickLaunchBar.
 */
class QuickLaunchBarView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(QuickLaunchBarModel, QuickLaunchBarStyle)

public:
    /*!
     * Constructs a new view for QuickLaunchBar.
     */
    QuickLaunchBarView(QuickLaunchBar *controller);

    /*!
     * Destroys the QuickLaunchBarView.
     */
    virtual ~QuickLaunchBarView();

private slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! \reimp
    virtual void setupModel();
    //! \reimp_end

    //! Layout for the launcher buttons
    QGraphicsLinearLayout *quickLaunchButtonLayout;
};

#endif
