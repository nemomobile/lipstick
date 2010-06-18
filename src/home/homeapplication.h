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
#include "windowinfo.h"
#include <QTimer>
#include <QMap>
#include <QSet>

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
     * \brief A signal to indicate that the window ordering has changed
     */
    void windowStackingOrderChanged(const QList<WindowInfo> &windowList);

    /*!
     * \brief A signal for notifying that the window visibility has changed
     */
    void windowVisibilityChanged(Window window);

    /*!
     * \brief A signal for notifying that the title of a window has changed
     */
    void windowTitleChanged(Window window, const QString &title);

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
    // X11 Atoms
    Atom windowTypeAtom;
    Atom clientListAtom;
    Atom stackedClientListAtom;
    Atom closeWindowAtom;
    Atom windowStateAtom;
    Atom netWindowNameAtom;
    Atom windowNameAtom;

    //! A list of windows that are being closed
    QSet<Window> windowsBeingClosed;

    /*!
     * Gets the current client window list from X and filters it based on
     *  - Windows that are bigger than 0x0, are Input/Output windows, are not unmapped and type includes Normal are included. Additionally:
     *  - Windows that have a type Notification or Desktop are not included.
     *  - Windows that have the "skip taskbar" state flag set, are not included.
     *  - Windows that are non application windows, such as desktop, menu, notification.
     */
    void updateWindowMapping();

    /*!
     * Updates the window properties changes.
     * \param window The window to update.
     */
    void updateWindowProperties(Window window);

    /*!
     * Helper method to create a \c WindowInfos into the current mapping
     * \param openedWindowSet A set of new windows
     * \return true if application windows were added
     */
    bool createWindowInfos(QSet<Window> openedWindowSet);

    /*!
     * Update the title of the given window and emit \c windowTitleChanged
     * \param window The X window to update
     */
    void updateWindowTitle(Window window);

    /*!
     * Helper method to remove a \c WindowInfos from the current mapping
     * \param closedWindowSet A set of closed windows
     * \return true if application windows were removed
     */
    bool removeWindowInfos(QSet<Window> closedWindowSet);

    /*!
     * Helper method to check if the window is an applciation window
     */
    bool isApplicationWindow(const WindowInfo &window);

    //! Flag that indicates whether duihome was started by upstart or not
    bool upstartMode;

    //! Timer used for sending startup notifications
    QTimer startupNotificationTimer;

    //! Implementations for com.meego.core.HomeScreen interface.
    HomeScreenService *homeScreenService;

    //! Mapping of the current X windows
    QMap<Window, WindowInfo> windowMap;

    //! List of application windows
    QList<WindowInfo> applicationWindows;

    //! A set of window type atoms that are used to finlter the application windows
    QSet<Atom> excludeAtoms;

};

#endif /* HOMEAPPLICATION_H_ */
