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

#ifndef DESKTOP_H
#define DESKTOP_H

#include <DuiWidgetController>
#include "desktopmodel.h"

/*!
 * The desktop widget is a container for the status area,
 * notification area, switcher and the applet mashup canvas.
 */
class Desktop : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(Desktop)

public:
    /*!
     * Constructs a Desktop widget.
     *
     * \param parent the parent widget of the Desktop, defaults to NULL
     */
    Desktop(DuiWidget *parent = NULL);

    /*!
     * Destroys the Desktop.
     */
    virtual ~Desktop();

signals:
    /*!
     * \brief Signaled when the viewport size, the panning range or the panning position changes.
     */
    void viewportSizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos);
};

#endif // DESKTOP_H
