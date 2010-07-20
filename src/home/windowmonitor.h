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

#ifndef WINDOWMONITOR_H
#define WINDOWMONITOR_H

#include <QWidget>

/*!
 * An interface defining window monitoring functionality.
 * A window monitor monitors the windows of an application and performs
 * various things when something happens to the windows. It is only a
 * monitor: it doesn't change anything on the windows.
 */
class WindowMonitor {
public:
    /*!
     * Queries if the window specified by \a wid belongs to the application
     * this window monitor belongs to.
     * \param wid the window id
     * \return \c true if the window belongs to this application
     */
    virtual bool isOwnWindow(WId wid) const = 0;
};

#endif // WINDOWMONITOR_H
