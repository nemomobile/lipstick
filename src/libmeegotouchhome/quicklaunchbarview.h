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
#ifndef QUICKLAUNCHBARVIEW_H
#define QUICKLAUNCHBARVIEW_H

#include <MWidgetView>
#include "quicklaunchbarmodel.h"
#include "quicklaunchbarstyle.h"

class QuickLaunchBar;
class QGraphicsLinearLayout;
class MButton;

/*!
 * A view class for the QuickLaunchBar.
 */
class QuickLaunchBarView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(QuickLaunchBarModel, QuickLaunchBarStyle)

public:
    /*!
     * Constructs a new view for QuickLaunchBar.
     */
    QuickLaunchBarView(QuickLaunchBar *controller);

    /*!
     * Destroys the QuickLaunchBarView.
     */
    virtual ~QuickLaunchBarView();

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

protected:
    //! \reimp
    virtual void applyStyle();
    virtual void setupModel();
    //! \reimp_end

private:
    //! Layout for the launcher buttons
    QGraphicsLinearLayout *launcherButtonLayout;

    //! The button to open the launcher
    MButton *toggleLauncherButton;

#ifdef UNIT_TEST
    friend class Ut_QuickLaunchBarView;
#endif
};

#endif
