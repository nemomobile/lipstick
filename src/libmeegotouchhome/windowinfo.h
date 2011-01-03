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

#ifndef WINDOWINFO_H_
#define WINDOWINFO_H_

#include <QString>
#include <QHash>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QExplicitlySharedDataPointer>

class WindowData;

/*!
 * WindowInfo is a helper class for storing information about an open window.
 */
class WindowInfo
{
public:
    // X11 atoms
    static Atom TypeAtom;
    static Atom StateAtom;
    static Atom NormalAtom;
    static Atom DesktopAtom;
    static Atom NotificationAtom;
    static Atom DialogAtom;
    static Atom CallAtom;
    static Atom DockAtom;
    static Atom MenuAtom;
    static Atom SkipTaskbarAtom;
    static Atom InputWindowAtom;
    static Atom NameAtom;

    /*!
     * Constructs a WindowInfo that contains information about an open window.
     *
     * \param window The X window id
     */
    explicit WindowInfo(Window window);

    /*!
     * Copy constructor.
     */
    WindowInfo(const WindowInfo &other);

    /*!
     * Destroys a WindowInfo object.
     */
    ~WindowInfo();

    /*!
     * Assignment operator.
     */
    WindowInfo& operator=(const WindowInfo &rhs);

    /*!
     * Initializes the X11 atoms
     */
    static void initializeAtoms();

    /*!
     * Gets the title of the window.
     *
     * \return the title of the window
     */
    const QString &title() const;

    /*!
     * Gets the types for this window \s WindowType
     * \return the types
     */
    QList<Atom> types() const;

    /*!
     * Gets the states for this window \s WindowType
     * \return the states
     */
    QList<Atom> states() const;

    /*!
     * Gets the window ID.
     *
     * \return the Window
     */
    Window window() const;

    /*!
     * Gets the window ID of the window this window is transient for.
     *
     * \return the ID of the window this window is transient for
     */
    Window transientFor() const;

    /*!
     * Retrieves the window title. First the title is retrieved with atom _NET_WM_NAME,
     * if this failes then XGetWMName will be used.
     */
    bool updateWindowTitle();

    /*!
     * Updates the window types and window states from the window manager
     */
    void updateWindowProperties();

private:
    //! Storage for the WindowInfo data objects. A central storage enables constructing
    //! new WindowInfo objects with shared data.
    static QHash<Window, QExplicitlySharedDataPointer<WindowData> > windowDatas;

    /*!
     * Gets the atoms and places them into the list
     */
    QList<Atom> getWindowProperties(Window winId, Atom propertyAtom, long maxCount = 16L);

    //! The explicitly shared data object \c WindowData
    QExplicitlySharedDataPointer<WindowData> d;

};

//! Comparison operator for WindowInfo objects
bool operator==(const WindowInfo &, const WindowInfo &);

//! Calculates a hash of a WindowInfo
uint qHash(WindowInfo wi);

#endif /* WINDOWINFO_H_ */
