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

#ifndef APPLETSPACE_H_
#define APPLETSPACE_H_

#include <DuiWidgetController>
#include "appletspacemodel.h"

/*!
 * A widget that contains an applet space (mashup canvas).
 */
class AppletSpace : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(AppletSpace)

public:
    /*!
     * Constructs a AppletSpace widget.
     *
     * \param parent Parent for the widget, defaults to NULL
     */
    AppletSpace(DuiWidget *parent = NULL);

    /*!
     * Destroys the AppletSpace.
     */
    virtual ~AppletSpace();

    /*!
     * If enabled is true, the applet space is enabled (items can be clicked, button is visible); otherwise, it is disabled.
     *
     * The applet space is disabled by default.
     *
     * \param enabled if true, the applet space is enabled; otherwise, it is disabled
     */
    void setEnabled(bool enabled);

signals:
    //! Sent when the applet space should be closed
    void closed();
};

#endif /* APPLETSPACE_H_ */
