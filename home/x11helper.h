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

#ifndef X11HELPER_H_
#define X11HELPER_H_

#include <QtGlobal>

#ifdef Q_WS_X11

#include <QVector>
#include <X11/Xlib.h>

class X11Helper
{
public:
    /*!
     * Gets a list of the window's _NET_WM_STATE properties.
     * \param display the display to be used.
     * \param window the window whose properties are queried.
     * \return a list of properties.
     */
    static QVector<Atom> getNetWmState(Display *display, Window window);
};

#endif /* Q_WS_X11 */

#endif /* X11HELPER_H_ */
