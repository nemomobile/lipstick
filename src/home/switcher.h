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
     * Constructs a Switcher widget.
     *
     * \param parent the parent widget of the Switcher, defaults to NULL
     */
    Switcher(MWidget *parent = NULL);

    /*!
     * Destroys the Switcher.
     */
    ~Switcher();

private slots:
    /*!
     * \brief A slot for notifying that the window list has been updated
     */
    void updateWindowList(const QList<WindowInfo> &newList);

    /*!
     * \brief A slot for notifying that a window should be brought to front
     */
    void windowToFront(Window window);

    /*!
     * \brief A slot for notifying that a window should be closed
     */
    void closeWindow(Window window);

    /*!
     * \brief A slot for getting information about changes in the viewport in which the switcher resides
     */
    void viewportSizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos);

    /*!
     * \brief A slot for changing a window title of a switcher button
     */
    void changeWindowTitle(Window window,  const QString &title);

    /*!
     * \brief Updates the buttons in the model based on the new window list
     */
    void updateButtons();

private:

    /*!
     * \brief Starts a timer which eventually calls updateButtons
     */
    void scheduleUpdate();

    //! X11 Atom for the close window message type
    Atom closeWindowAtom;

    //! X11 Atom for the active window message type
    Atom activeWindowAtom;

    //! A mapping from known X Window ids to SwitcherButtons
    QMap<Window, SwitcherButton *> windowMap;

    //! The latest window list supplied through updateWindowList
    QList<WindowInfo> windowList;

    //! True if the window list has been updated since the last updateButtons
    bool windowListUpdated;

#ifdef UNIT_TEST
    friend class Ut_Switcher;
#endif
};

#endif // SWITCHER_H
