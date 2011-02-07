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

#include <QTimer>
#include <QTimeLine>
#include <QRectF>
#include <mbuttoniconview.h>
#include "launcherbuttonmodel.h"
#include "launcherbuttonstyle.h"

class LauncherButton;
class LauncherButtonProgressIndicator;
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
    virtual void setupModel();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! Resets the progress indicator according to button state
    void resetProgressIndicator();

    //! Updates the icon id according to button state
    void updateButtonIcon();

    //! Sets the icon from the MDesktopEntry
    void setIconFromDesktopEntry();

    //! The controller for the view
    LauncherButton *controller;

    //! Position and size of the progress indicator
    QRectF progressIndicatorRect;

    //! Progress indicator for operation
    LauncherButtonProgressIndicator *progressIndicator;

    //! A timer for disabling the progress indicator if the application startup takes a long time
    QTimer launchProgressTimeoutTimer;

#ifdef UNIT_TEST
    friend class Ut_LauncherButtonView;
#endif
};

#endif /* LAUNCHERBUTTONVIEW_H_ */
