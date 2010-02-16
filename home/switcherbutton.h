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

#ifndef SWITCHERBUTTON_H
#define SWITCHERBUTTON_H

#include <DuiButton>
#include "switcherbuttonmodel.h"
#include "windowinfo.h"
#include <X11/Xlib.h>

/*!
 * SwitcherButton widget represents an open window in the Switcher.
 */
class SwitcherButton : public DuiButton
{
    Q_OBJECT
    DUI_CONTROLLER(SwitcherButton)

public:
    /*!
     * Constructs an SwitcherButton.
     *
     * \param title the Title of the window represented by this button
     * \param parent parent DuiWidget
     * \param window the X Window represented by this button
     * \param priority the priority of the X Window represented by this button
     */
    SwitcherButton(const QString &title, DuiWidget *parent = NULL, Window window = 0, WindowInfo::WindowPriority windowPriority = WindowInfo::Normal);

    /*!
     * Destroys the SwitcherButton.
     */
    virtual ~SwitcherButton();

    /*!
     * Gets the X Window represented by this button
     */
    Window xWindow();

    /*!
     * Gets the priority of the X Window represented by this button
     */
    WindowInfo::WindowPriority windowPriority() const;

    /*!
     * Sets the priority of the X Window represented by this button
     */
    void setWindowPriority(WindowInfo::WindowPriority windowPriority);

    /*! \reimp
     *
     * Reimplemented here as public because it's protected in the base class so the view cannot access it.
     */
    virtual void prepareGeometryChange();
    virtual void setGeometry(const QRectF &rect);
    //! \reimp_end

    /*! 
     * Sets the emphasis for this switcher button, currently this sets the 
     * correct style mode.
     */
    void setEmphasis();
      
    /*! 
     * Unsets the emphasis for this switcher button, currently this reset the 
     * button style to default.
     */
    void unsetEmphasis();

signals:
    /*!
     * \brief A signal for notifying that a window should be brought to front
     */
    void windowToFront(Window window);

    /*!
     * \brief A signal for notifying that the window should be closed
     */
    void closeWindow(Window window);

public slots:
    /*!
     * \brief Slot for notifying that the window represented by this button should be brought to front
     */
    void switchToWindow();

    /*!
     * \brief Slot for notifying that the window represented by this button should be closed
     */
    void close();

    /*!
     * \brief Updates the _NET_WM_ICON_GEOMETRY X property for the Window
     */
    void updateIconGeometry();

private:

    //! X11 Atom for the icon geometry
    static Atom iconGeometryAtom;

    //! Window priority
    WindowInfo::WindowPriority priority;
};

#endif // SWITCHERBUTTON_H
