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

#include <QTimer>
#include <MWidgetController>
#include "switchermodel.h"
#include "xeventlistener.h"
#include "windowinfo.h"
#include <X11/Xlib.h>


/*!
 * Switcher is a widget that shows the available windows.
 */
class Switcher : public MWidgetController, public XEventListener
{
    Q_OBJECT
    M_CONTROLLER(Switcher)

public:
    /*!
     * Constructs a Switcher widget.
     * \param parent the parent widget of the Switcher, defaults to NULL
     * \param model model to be used with Switcher, defaults SwitcherModel
     */
    Switcher(MWidget *parent = NULL, SwitcherModel *model = new SwitcherModel);

    /*!
     * Destroys the Switcher.
     */
    ~Switcher();

    /*!
     * Handles an X event if it is related to the Switcher.
     *
     * \param event the XEvent to be handled
     * \return \c true if the event was handled, \c false otherwise
     */
    bool handleXEvent(const XEvent &event);

signals:
    /*!
     * \brief A signal for notifying that the window list has been updated
     */
    void windowListUpdated(const QList<WindowInfo> &windowList);

    /*!
     * Sent when something starts or stops animating in switcher
     * \param animating if true, something is animating
     */
    void animationStateChanged(bool animating);

public slots:
    /*!
     * Gets the current stacked client window list as parameter and checks
     * whether new windows are added or removed.
     * Adds and removes such windows from the switcher and emits the updated stacked window list.
     *
     * \param newWindowList QList<WindowInfo> that contains all windows in stacking order
     * \see WindowMonitor::handleWindowInfoList()
     */
    void handleWindowInfoList(QList<WindowInfo> newWindowList);

    /*!
     * Restore a button that were being removed.
     *
     * \param window
     * \param forceUpdateButtons Whether buttons should be updated
     * \return Whether windon was removed from list
     */
    bool restoreButtonBeingRemoved(Window window, bool forceUpdateButtons = true);

protected:
    //! \reimp
    /*!
     * When QEvent::TouchEnd event arrives, stops the Switcher from being an event
     * filter for the SwitcherButtons
     */
    virtual bool sceneEvent(QEvent *event);
    //! \reimp_end

    /*!
     * Create switcher button
     */
    virtual QSharedPointer<SwitcherButton> createSwitcherButton();

    //! The window monitor instance this switcher uses

private slots:
    /*!
     * \brief Updates the buttons in the model based on the current window list
     */
    void updateButtons();

    /*!
     * \brief Requests a window to be brought to front
     */
    void windowToFront(Window window);

    /*!
     * \brief Requests a window should be closed
     */
    void closeWindow(Window window);

    /*!
     * \brief Requests all windows to be closed
     */
    void closeAllWindows();

    /*!
     * Updates the animation status of switcher by disabling the visibility property updates in switcher buttons.
     * Also sends an animation state change signal.
     * \param animating if true, switcher is currently animating
     */
    void updateAnimationStatus(bool animating);

private:
    /*!
     * Adds all windows in the given set to the Switcher.
     *
     * \param windows the set of windows to add to the Switcher
     * \return \c true if the application window list was changed, \c false otherwise
     */
    bool addWindows(const QSet<WindowInfo> &windowInfos);

    /*!
     * Adds a window to the set of windows tracked by the Switcher.
     *
     * \param window the ID of the window to be added
     * \return \c true if the application window list was changed, \c false otherwise
     */
    bool addWindowInfo(const WindowInfo &window);

    /*!
     * Removes all windows in the given set to the Switcher.
     *
     * \param windows the set of windows to remove from the Switcher
     * \return \c true if the application window list was changed, \c false otherwise
     */
    bool removeWindows(const QSet<WindowInfo> &windowInfos);

    /*!
     * Removes a window from the set of windows tracked by the Switcher.
     *
     * \param window the ID of the window to be removed
     * \return \c true if the application window list was changed, \c false otherwise
     */
    bool removeWindow(Window window);

    /*!
     * Marks a window as being closed.
     *
     * \param window the ID of the window to mark as being closed
     */
    void markWindowBeingClosed(Window window);

    /*!
     * Marks a window transient for another window.
     *
     * \param window the ID of the window to mark transient for another window
     * \param transientFor the ID of the window the other window is transient for
     */
    void markWindowTransientFor(Window window, Window transientFor);

    /*!
     * Unmarks a window transient for another window.
     *
     * \param window the ID of the window to unmark transient for another window
     * \param transientFor the ID of the window the other window is not transient for
     */
    void unmarkWindowTransientFor(Window window, Window transientFor);

    /*!
     * Checks whether a window is interesting from the Switcher's point of view.
     * Such windows are bigger than 0x0, are Input/Output windows and are not unmapped.
     *
     * \param window the ID of the window whose relevance is to be checked
     * \return \c true if the window is relevant, \c false otherwise
     */
    bool isRelevantWindow(Window window);

    /*!
     * Checks whether a window is an application window. A window is not considered to be an application window if
     *  - its type is Desktop, Menu, Dock, Dialog or Notification
     *  - it has the "skip taskbar" state flag set
     *  - it is transient for another window
     *
     * \param windowInfo information for the window to be checked
     * \return \c true if the window is an application window, \c false otherwise
     */
    bool isApplicationWindow(const WindowInfo &windowInfo);

    /*!
     * Returns the topmost transient window for a window.
     * If the window has no transient windows the window itself is returned.
     *
     * \param window the ID of the window to get the topmost transient window for
     * \return the ID of the topmost transient window
     */
    Window topmostTransientWindowFor(Window window);

    /*!
     * \brief Schedules update of the buttons in the model based on the current window list
     */
    void scheduleUpdateButtons();

    /*!
     * Update the title of the given window and emit \c windowTitleChanged
     * \param window The X window to update
     */
    void updateWindowTitle(Window window);

    /*!
     * Updates the window properties changes.
     * \param window The window to update.
     */
    void updateWindowProperties(Window window);

    /*!
     * Returns a pointer to a window info if it exists in the given set.
     *
     * \param windowInfos a set of WindowInfos to get the window info from
     * \param window the ID of the window to get info for
     * \return WindowInfo for the window if it exists in the given set or \c NULL otherwise
     */
    static const WindowInfo *windowInfoFromSet(const QSet<WindowInfo> &windowInfos, Window window);

    //! X11 Atom for the close window message type
    Atom closeWindowAtom;

    //! X11 Atom for the active window message type
    Atom activeWindowAtom;

    //! X11 Atoms for window types
    Atom clientListAtom;
    Atom netWindowNameAtom;
    Atom windowNameAtom;

    //! A mapping from known X Window IDs to SwitcherButtons
    QHash<Window, QSharedPointer<SwitcherButton> > switcherButtonMap;

    //! A list of windows that are being closed
    QSet<Window> windowsBeingClosed;

    //! Mapping of the current X windows
    QSet<WindowInfo> windowInfoSet;

    //! List of application windows
    QList<WindowInfo> applicationWindows;

    //! A set of window type atoms that are used to filter the application windows
    QSet<Atom> excludeAtoms;

    //! The list of windows which are transient for a window
    QHash<Window, QList<Window> > transientMap;

    //! A timer for scheduling button updates
    QTimer updateButtonsTimer;

    //! Reflects the top most window id of the _NET_CLIENT_LIST_STACKING
    Window topmostWindow;

#ifdef UNIT_TEST
    friend class Ut_Switcher;
#endif
};

#endif // SWITCHER_H
