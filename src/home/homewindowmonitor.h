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

#ifndef HOMEWINDOWMONITOR_H
#define HOMEWINDOWMONITOR_H

#include "windowmonitor.h"
#include <QSet>

/*!
 * A window monitor implementation to be used with the home screen application.
 */
class HomeWindowMonitor : public WindowMonitor
{
public:
    /*!
     * Registers a window id to be monitored by this window monitor.
     * \param wid the id to register
     */
    void registerWindowId(WId wid);

    //! \reimp
    virtual bool isOwnWindow(WId wid) const;
    //! \reimp_end

private:
    //! The currently registered window ids
    QSet<WId> windowIds;
};

#endif // HOMEWINDOWMONITOR_H
