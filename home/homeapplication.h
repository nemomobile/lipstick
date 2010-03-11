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

#ifndef HOMEAPPLICATION_H_
#define HOMEAPPLICATION_H_

#include <DuiApplication>
#include <contentsearchif.h>
#include "windowinfo.h"
#include "applicationcontext.h"

class MainWindow;

/*!
 * HomeApplication extends DuiApplication by keeping track of open X windows.
 */
class HomeApplication : public DuiApplication
{
    Q_OBJECT

public:
    /*!
     * Initializes the window system and constructs an application object.
     *
     * \param argc number of arguments passed to the application from the command line
     * \param argv argument strings passed to the application from the command line
     */
    HomeApplication(int &argc, char **argv);

    /*!
     * Destroys the HomeApplication.
     */
    ~HomeApplication();

public slots:
    /*!
     * Launches content search service
     */
    void launchContentSearchService();

signals:
    /*!
     * \brief A signal for notifying that the window list has been updated
     */
    void windowListUpdated(const QList<WindowInfo> &windowList);

    /*!
     * \brief A signal for notifying that the window visibility has changed
     */
    void windowVisibilityChanged(Window window);

protected:
    /*! \reimp
     * The _NET_CLIENT_LIST property of the root window is interesting and
     * causes an update to the window list.
     */
    virtual bool x11EventFilter(XEvent *event);
    //! \reimp_end

private slots:
    /*!
     * Sends a dbus-signal after UI is visible, stops the process if
     * duihome has been started by upstart
     */
    void sendStartupNotifications();

private:
    // X11 Atoms for different window types
    Atom windowTypeAtom;
    Atom windowTypeNormalAtom;
    Atom windowTypeDesktopAtom;
    Atom windowTypeNotificationAtom;
    Atom windowTypeDockAtom;
    Atom windowTypeCallAtom;
    Atom clientListAtom;
    Atom closeWindowAtom;
    Atom skipTaskbarAtom;
    Atom windowStateAtom;

    //! Content search service interface. Used to launch the content search service.
    ContentSearchIf contentSearchIf;

    //! A list of windows that are being closed
    QList<Window> windowsBeingClosed;

    /*!
     * Gets the current client window list from X and filters it.
     * - Windows that are bigger than 0x0, are Input/Output windows, are not unmapped and type includes Normal are included. Additionally:
     *   - Windows that have a type Notification or Desktop are not included.
     *   - Windows that have the "skip taskbar" state flag set, are not included.
     * Emits windowListUpdated with the current window list as the parameter.
     */
    void updateWindowList();

    //! Flag that indicates whether duihome was started by upstart or not
    bool upstartMode;

    //! Timer used for sending startup notifications
    QTimer startupNotificationTimer;
};

#endif /* HOMEAPPLICATION_H_ */
