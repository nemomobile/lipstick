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

#ifndef HOMEWINDOWMONITOR_H
#define HOMEWINDOWMONITOR_H

#include <QObject>
#include <QSet>
#include "windowmonitor.h"
#include "xeventlistener.h"

/*!
 * A window monitor implementation to be used with the home screen application.
 */
class LIPSTICK_EXPORT HomeWindowMonitor : public WindowMonitor, public XEventListener
{
    Q_OBJECT

public:

    //! brief Get HomeWindowMonitor singleton instance.
    static const HomeWindowMonitor *instance();

    /*!
     * Destructor.
     */
    virtual ~HomeWindowMonitor();

    //! \reimp
    virtual bool isOwnWindow(WId wid) const;
    virtual bool handleXEvent(const XEvent& event);
    //! \reimp_end

    //! Returns whether Home is the topmost window
    bool isHomeWindowOnTop() const;

    //! Returns true if Home is highest window excluding windows defined by ignoredWindows
    bool isHomeWindowOnTop(QSet<Atom> ignoredWindows) const;

signals:
    void isHomeWindowOnTopChanged();

protected:
    /*!
     * Constructor.
     */
    HomeWindowMonitor();

private:

    // HomeWindowMonitor singleton instance.
    static QSharedPointer<HomeWindowMonitor> windowMonitorInstance;

    //! The window types for windows that are not considered to be full screen
    //! application windows
    const QSet<Atom> nonFullscreenApplicationWindowTypes;

    //! An X atom for _NET_CLIENT_LIST_STACKING
    const Atom netClientListStacking;

    /*!
     * Queries the current window stacking order from X and returns the windows
     * in that order. The topmost window is the last one in the list.
     */
    QList<Window> windowStackingOrder() const;

#ifdef UNIT_TEST
    friend class Ut_HomeWindowMonitor;
    friend class Ut_Switcher;
    friend class Ut_DesktopView;
    friend class Ut_LauncherButton;
#endif

};

#endif // HOMEWINDOWMONITOR_H
