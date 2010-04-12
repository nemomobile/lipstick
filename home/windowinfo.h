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
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/*!
 * WindowInfo is a helper class for storing information about an open window.
 */
class WindowInfo
{
public:
    /*!
     * Priority values for a window. Smaller value means higher priority.
     * Values are so that different priorities can later be added.
     */
    enum WindowPriority {
        Call = 100,
        Normal = 500
    };

    /*!
     * Constructs a WindowInfo that contains information about an open window.
     *
     * \param title the title of the window
     * \param window the window
     * \param windowAttributes the attributes of the window
     * \param icon the icon of the window
     */
    WindowInfo(QString &title, Window window, XWindowAttributes windowAttributes, Pixmap icon, WindowPriority priority = WindowInfo::Normal);

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
     * Gets the priority of the window.
     *
     * \return the priority of the window
     */
    WindowPriority windowPriority() const;

    /*!
     * Gets the window.
     *
     * \return the Window
     */
    Window window() const;

    /*!
     * Gets the attributes of the window.
     *
     * \return the attributes of the window
     */
    XWindowAttributes windowAttributes() const;

    /*!
     * Gets the icon of the window.
     *
     * \return the icon of the window
     */
    Pixmap icon() const;

private:
    //! The title of the window
    QString title_;

    //! The priority of the window
    WindowPriority windowPriority_;

    //! The window
    Window window_;

    //! The attributes of the window
    XWindowAttributes attributes_;

    //! The icon of the window
    Pixmap pixmap_;
};

//! Comparison operator for WindowInfo objects
bool operator==(const WindowInfo &, const WindowInfo &);

#endif /* WINDOWINFO_H_ */
