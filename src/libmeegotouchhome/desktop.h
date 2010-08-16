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

#ifndef DESKTOP_H
#define DESKTOP_H

#include <MWidgetController>
#include "desktopmodel.h"

/*!
 * The desktop widget is a container for the switcher,
 * launcher and quick launch bar.
 */
class Desktop : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(Desktop)

public:
    /*!
     * Constructs a Desktop widget.
     *
     * \param parent the parent widget of the Desktop, defaults to NULL
     */
    Desktop(MWidget *parent = NULL);

    /*!
     * Destroys the Desktop.
     */
    virtual ~Desktop();
};

#endif // DESKTOP_H
