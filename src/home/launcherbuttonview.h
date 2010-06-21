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

#ifndef LAUNCHERBUTTONVIEW_H_
#define LAUNCHERBUTTONVIEW_H_

#include <QSharedPointer>
#include <QTimer>
#include <mbuttoniconview.h>
#include "launcherbuttonmodel.h"
#include "launcherbuttonstyle.h"
#include "windowinfo.h"

class QGraphicsAnchorLayout;
class MProgressIndicator;
class LauncherButton;

/*!
 * The launcher button view allows a progress indicator to be displayed on top of it.
 */
class LauncherButtonView : public MButtonIconView
{
    Q_OBJECT
    M_VIEW(LauncherButtonModel, LauncherButtonStyle)

public:
    /*!
     * Constructs a LauncherButtonView.
     *
     * \param controller the MButton controller for the view
     */
    LauncherButtonView(LauncherButton *controller);

    /*!
     * Destroys the LauncherButtonView.
     */
    virtual ~LauncherButtonView();

protected:
    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

private slots:
    //! Shows the progress indicator
    void showProgressIndicator();

    //! Hides the progress indicator
    void hideProgressIndicator();

    //! Hides the progress indicator if the window list contains only normal windows
    void hideProgressIndicatorIfObscured(const QList<WindowInfo> &windowList);

private:
    //! Layout for the progress indicator
    QGraphicsAnchorLayout *progressIndicatorLayout;

    //! Progress indicator shown while the application is being launched
    MProgressIndicator *progressIndicator;

    //! A timer for disabling the progress indicator if the application startup takes a long time
    QTimer progressIndicatorTimer;

#ifdef UNIT_TEST
    friend class Ut_LauncherButtonView;
#endif
};

#endif /* LAUNCHERBUTTONVIEW_H_ */
