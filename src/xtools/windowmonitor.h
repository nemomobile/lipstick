/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QObject>
#include "windowinfo.h"

/*!
 * An interface defining window monitoring functionality.
 * A window monitor monitors the windows of an application and performs
 * various things when something happens to the windows. It is only a
 * monitor: it doesn't change anything on the windows.
 */
class LIPSTICK_EXPORT WindowMonitor : public QObject {

    Q_OBJECT

public:
    /*!
     * Queries if the window specified by \a wid belongs to the application
     * this window monitor belongs to.
     * \param wid the window id
     * \return \c true if the window belongs to this application
     */
    virtual bool isOwnWindow(WId wid) const = 0;

signals:
    /*!
     * A signal that gets emitted when the stacking order of the windows changes.
     * The topmost window is the last one in the argument list.
     */
    void windowStackingOrderChanged(QList<WindowInfo *>);

    /*!
     * A signal that gets emitted when a fullscreen window appears on top of application's
     * own window's and covers it completely.
     */
    void fullscreenWindowOnTopOfOwnWindow();
    /*!
     * A signal that gets emitted when a window appears on top of application's
     * own window's.
     * \param window the WindowInfo for the window that appeared.
     */
    void anyWindowOnTopOfOwnWindow(WindowInfo *windowInfo);
};

#endif // WINDOWMONITOR_H
