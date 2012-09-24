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

#ifndef WINDOWINFO_H_
#define WINDOWINFO_H_

#include <QObject>
#include <QString>

#include "lipstickglobal.h"
#include "xtools/xeventlistener.h"

/*!
 * WindowInfo is a helper class for storing information about an open window.
 */
class LIPSTICK_EXPORT WindowInfo : public QObject, XEventListener
{
    Q_OBJECT

    Q_PROPERTY(qulonglong pixmapSerial READ pixmapSerial WRITE setPixmapSerial NOTIFY pixmapSerialChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(qulonglong window READ window NOTIFY windowChanged)


public:
    static WindowInfo *windowFor(Qt::HANDLE wid);

    /*!
     * Destroys a WindowInfo object.
     */
    ~WindowInfo();

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
    QList<Qt::HANDLE> types() const;

    /*!
     * Gets the states for this window \s WindowType
     * \return the states
     */
    QList<Qt::HANDLE> states() const;

    /*!
     * Gets the window ID.
     *
     * \return the Window
     */
    Qt::HANDLE window() const;

    /*!
     * Gets the window ID of the window this window is transient for.
     *
     * \return the ID of the window this window is transient for
     */
    Qt::HANDLE transientFor() const;

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

    Qt::HANDLE pixmapSerial() const;
    void setPixmapSerial(Qt::HANDLE pixmapSerial);

    bool visibleInSwitcher();

signals:
    void pixmapSerialChanged();
    void titleChanged();
    void windowChanged();
    void visibleInSwitcherChanged();

private:
    virtual bool handleXEvent(const XEvent &event);

    WindowInfo(Qt::HANDLE window);

    /*!
     * Gets the atoms and places them into the list
     */
    QList<Qt::HANDLE> getWindowProperties(Qt::HANDLE winId, Qt::HANDLE propertyAtom, long maxCount = 16L);

    //! The explicitly shared data object \c WindowData
    class WindowData;
    WindowData * const d;

};

//! Comparison operator for WindowInfo objects
bool operator==(const WindowInfo &, const WindowInfo &);

#endif /* WINDOWINFO_H_ */
