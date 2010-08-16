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

#ifndef LAUNCHERPAGEVIEW_H
#define LAUNCHERPAGEVIEW_H

#include <MWidgetView>
#include "launcherpage.h"
#include "launcherpagemodel.h"
#include "launcherpagestyle.h"

class Launcher;
class MFlowLayoutPolicy;
class MLayout;
class QGraphicsLinearLayout;


class LauncherPageView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(LauncherPageModel, LauncherPageStyle)

public:
    /*!
     * Constructs an LauncherPageView.
     *
     * \param container the LauncherPage controller to be used
     */
    LauncherPageView(LauncherPage *controller);

    /*!
     * Destroys the LauncherPageView.
     */
    virtual ~LauncherPageView();
protected:
    //! \reimp
    virtual void setupModel();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:

    /*!
     * Updates layout from models button list.
     *
     * Adds new buttons and removes non-exitent.
     */
    void updateLayoutFromModel();

    /*!
     * Removes a button from layout and sets its parent to null.
     *
     * View or layout are not responsible for deleting the button after this.
     *
     * \param button Button to be removed.
     */
    void removeButtonFromLayout(LauncherButton *button);


    //! A layout for the buttons
    MLayout *layout;
    //! A flow layout policy for the widgets
    MFlowLayoutPolicy *policy;
};

#endif // LAUNCHERVIEW_H
