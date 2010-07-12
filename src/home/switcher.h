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

#ifndef SWITCHER_H
#define SWITCHER_H

#include <MWidgetController>
#include "switchermodel.h"
#include <X11/Xlib.h>

class WindowInfo;

/*!
 * Switcher is a widget that shows the available windows.
 */
class Switcher : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(Switcher)

public:
    /*!
     * Returns an instance of the Switcher.
     *
     * \return an instance of the Switcher
     */
    static Switcher *instance();

    /*!
     * Destroys the Switcher.
     */
    ~Switcher();

    /*!
     * Handles an X11 event if it is related to the Switcher.
     *
     * \param event the XEvent to be handled
     * \return \c true if the event was handled, \c false otherwise
     */
    bool handleX11Event(XEvent *event);

signals:
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

private slots:
   /*!
     * \brief A slot for notifying that a window should be brought to front
     */
    void windowToFront(Window window);

    /*!
     * \brief A slot for notifying that a window should be closed
     */
    void closeWindow(Window window);

    /*!
     * \brief Updates the buttons in the model based on the new window list
     */
    void updateButtons();

private:
    /*!
     * Constructs a Switcher widget.
     *
     * \param parent the parent widget of the Switcher, defaults to NULL
     */
    Switcher(MWidget *parent = NULL);

    /*!
     * \brief Starts a timer which eventually calls updateButtons
     */
    void scheduleUpdate();

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

    //! A singleton switcher instance
    static Switcher *switcher;

    //! X11 Atom for the close window message type
    Atom closeWindowAtom;

    //! X11 Atom for the active window message type
    Atom activeWindowAtom;

    //! X11 Atoms for window types
    Atom windowTypeAtom;
    Atom clientListAtom;
    Atom stackedClientListAtom;
    Atom windowStateAtom;
    Atom netWindowNameAtom;
    Atom windowNameAtom;

    //! A mapping from known X Window ids to SwitcherButtons
    QMap<Window, SwitcherButton *> switcherButtonMap;

    //! A list of windows that are being closed
    QSet<Window> windowsBeingClosed;

    //! Mapping of the current X windows
    QMap<Window, WindowInfo> windowMap;

    //! List of application windows
    QList<WindowInfo> applicationWindows;

    //! A set of window type atoms that are used to finlter the application windows
    QSet<Atom> excludeAtoms;

#ifdef UNIT_TEST
    friend class Ut_Switcher;
#endif
};

#endif // SWITCHER_H
