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
#include <QObject>
#include <QHash>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QExplicitlySharedDataPointer>

/*!
 * WindowInfo is a helper class for storing information about an open window.
 */
class WindowInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int pixmapSerial READ pixmapSerial WRITE setPixmapSerial NOTIFY pixmapSerialChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(int window READ window NOTIFY windowChanged)


public:
    // X11 atoms
    static Atom TypeAtom;
    static Atom StateAtom;
    static Atom SkipTaskbarAtom;
    static Atom NameAtom;
    static Atom ClientListAtom;
    static Atom CloseWindowAtom;
    static Atom ActiveWindowAtom;
    static Atom Utf8StringAtom;
    static Atom WindowPidAtom;

    static Atom WindowTypeNormalAtom;
    static Atom WindowTypeDesktopAtom;
    static Atom WindowTypeNotificationAtom;
    static Atom WindowTypeDialogAtom;
    static Atom WindowTypeCallAtom;
    static Atom WindowTypeDockAtom;
    static Atom WindowTypeMenuAtom;
    static Atom WindowTypeInputAtom;

    static WindowInfo *windowFor(Window wid);

    /*!
     * Destroys a WindowInfo object.
     */
    ~WindowInfo();

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

    /*! Retrieve the PID associated with this window.
     */
    int pid() const;

    /*! Set the PID associated with this window.
     */
    void setPid(int pid);

    int pixmapSerial() const;
    void setPixmapSerial(int pixmapSerial);

signals:
    void pixmapSerialChanged();
    void titleChanged();
    void windowChanged();

private:
    WindowInfo(Window window);

    /*!
     * Gets the atoms and places them into the list
     */
    QList<Atom> getWindowProperties(Window winId, Atom propertyAtom, long maxCount = 16L);

    //! The explicitly shared data object \c WindowData
    class WindowData;
    WindowData * const d;

};

//! Comparison operator for WindowInfo objects
bool operator==(const WindowInfo &, const WindowInfo &);

#endif /* WINDOWINFO_H_ */
