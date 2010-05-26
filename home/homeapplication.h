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

#ifndef HOMEAPPLICATION_H_
#define HOMEAPPLICATION_H_

#include <MApplication>
//#include <contentsearchif.h>
#include "windowinfo.h"
#include <QTimer>

class MainWindow;
class HomeScreenService;


/*!
 * HomeApplication extends MApplication by keeping track of open X windows.
 */
class HomeApplication : public MApplication
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
     * \brief A Signal to request launcher focus on specific launcher application
     */
     void focusToLauncherAppRequested(const QString &fileEntryPath);

    /*!
     * \brief A signal for notifying that the window list has been updated
     */
    void windowListUpdated(const QList<WindowInfo> &windowList);

    /*!
     * \brief A signal for notifying that the window visibility has changed
     */
    void windowVisibilityChanged(Window window);

    /*!
     * \brief A signal for notifying that the title of a window has changed
     */
    void windowTitleChanged(Window window, const QString &title);

    /*!
     * \brief A signal to indicate that the window stacking order has changed.
     * the window list will only contain windows that the homeapplication is
     * interrested in, as an example it will not contain desktop or
     * notification windows
     */
    void windowStackingChanged(const QList<WindowInfo> &windowList);

#ifdef BENCHMARKS_ON
    void startBenchmarking();
    void stopBenchmarking();
#endif

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
    Atom windowTypeDialogAtom;
    Atom windowTypeMenuAtom;
    Atom windowTypeCallAtom;
    Atom clientListAtom;
    Atom stackedClientListAtom;
    Atom closeWindowAtom;
    Atom skipTaskbarAtom;
    Atom windowStateAtom;
    Atom netWindowNameAtom;
    Atom windowNameAtom;

    //! Content search service interface. Used to launch the content search service.
    //ContentSearchIf contentSearchIf;

    //! A list of windows that are being closed
    QList<Window> windowsBeingClosed;


    /*!
     * Update the title of the given window from X.
     */
    void updateWindowTitle(Window window);

    /*!
     * Retreives the filtered window list (see HomeApplication::filterWindows)
     * and emits the windowListUpdated with the current window list as the
     * parameter.
     */
    void updateWindowList();

    /*!
     * Gets the current client window list from X and filters it based on
     *  - The given \c Atom
     *  - Windows that are bigger than 0x0, are Input/Output windows, are not unmapped and type includes Normal are included. Additionally:
     *  - Windows that have a type Notification or Desktop are not included.
     *  - Windows that have the "skip taskbar" state flag set, are not included.
     * \param atom The atom that fill be used to filter teh window list
     * \return The filtered window list
     */
    QList<WindowInfo> filterWindows(Atom atom);

    //! Flag that indicates whether duihome was started by upstart or not
    bool upstartMode;

    //! Timer used for sending startup notifications
    QTimer startupNotificationTimer;

    //! Implementations for com.meego.core.HomeScreen interface.
    HomeScreenService *homeScreenService;
};

#endif /* HOMEAPPLICATION_H_ */
