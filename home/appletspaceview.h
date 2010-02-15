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

#ifndef APPLETSPACEVIEW_H_
#define APPLETSPACEVIEW_H_

#include <duiextendingbackgroundview.h>
#include "appletspacestyle.h"
#include "appletspacemodel.h"

class AppletSpace;
class DuiMashupCanvas;
class DuiOverlay;
class DuiButton;

/*!
 * The applet space view contains a mashup
 */
class AppletSpaceView : public DuiExtendingBackgroundView
{
    Q_OBJECT
    DUI_VIEW(AppletSpaceModel, AppletSpaceStyle)

public:
    /*!
     * Constructs an AppletSpaceView.
     *
     * \param container the AppletSpace controller to be used
     */
    AppletSpaceView(AppletSpace *container);

    /*!
     * Destroys the AppletSpaceView.
     */
    virtual ~AppletSpaceView();

private slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! The AppletSpace controller
    AppletSpace *controller;
    //! The mashup canvas
    DuiMashupCanvas *mashupCanvas;
    //! An overlay for the close button
    DuiOverlay *closeButtonOverlay;
    //! A close button
    DuiButton *closeButton;
};

#endif /* APPLETSPACEVIEW_H_ */
