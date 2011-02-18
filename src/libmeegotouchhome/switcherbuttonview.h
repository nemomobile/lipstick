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

#ifndef SWITCHERBUTTONVIEW_H
#define SWITCHERBUTTONVIEW_H

#include <MButtonView>
#include <QTimer>
#include "switcherbuttonmodel.h"
#include "switcherbuttonstyle.h"
#include "x11wrapper.h"

class SwitcherButton;
class XEventListener;

/*!
 * \class SwitcherButtonView
 *
 * \brief SwitcherButtonView implements a switcher style view
 */
class SwitcherButtonView : public MButtonView
{
    Q_OBJECT
    M_VIEW(SwitcherButtonModel, SwitcherButtonStyle)

public:
    /*!
     * \brief Constructs a new SwitcherButtonView.
     *
     * \param controller The button's controller
     */
    SwitcherButtonView(SwitcherButton *controller);

    /*!
     * \brief Destroys the SwitcherButtonView.
     */
    virtual ~SwitcherButtonView();

    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    //! \reimp_end

    /*!
     * A method for notifying that a window got fully obscured. If the fully obscured window
     * is the same window that this switcher button is representing, then the button gets updated.
     * \return \c true if \a window was the window id this switcher button is representing.
     */
    bool windowFullyObscured(Window window);

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    /*!
     * \brief A slot for receiving information about pixmap changes
     */
    virtual void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);

    /*!
     * \brief Starts getting damage events for the current X window.
     */
    void setOnDisplay();

    /*!
     * \brief Stops getting damage events for the current X window.
     */
    void unsetOnDisplay();

private slots:
    /*!
     * \brief Updates the _NET_WM_ICON_GEOMETRY X property for the Window
     */
    void updateXWindowIconGeometry();

    /*!
     * \brief Updates the X window pixmap.
     */
    void updateXWindowPixmap();

protected:
    /*!
     * Creates an X damage structure based on the current X window pixmap.
     */
    void createDamage();

    /*!
     * Destroys the current X damage structure.
     */
    void destroyDamage();

    //! Returns the thumbnail position in parent coordinates.
    virtual QPoint thumbnailPosition() const;

    //! \reimp
    virtual void setupModel();
    //! \reimp_end

    //! SwitcherButton controller
    SwitcherButton *controller;

    //! Starts a timer for updating the icon geometry if the icon geometry has changed after the last update
    void updateXWindowIconGeometryIfNecessary() const;

    //! Updates the button style to reflect the current view mode.
    void updateViewMode();

#ifdef Q_WS_X11
    //! Handles X BadMatch errors
    static int handleXError(Display *display, XErrorEvent *event);

    //! Whether a BadMatch X error has occurred
    static bool badMatchOccurred;
#endif

    //! The X window's pixmap
    Pixmap xWindowPixmap;

    //! XDamage handle for the X window's pixmap
    Damage xWindowPixmapDamage;

    //! The X window's pixmap as a shared QPixmap
    QPixmap qWindowPixmap;

    //! The point where dragging started
    QPointF dragStartPos;

    //! Whether the button is being displayed or not
    bool onDisplay;

    //! Timer for updating the icon's position in scene coordination
    mutable QTimer updateXWindowIconGeometryTimer;

    //! Timer for updating the icon's pixmap if it wasn't ready and
    //! a badmatch occurred
    QTimer updateXWindowPixmapRetryTimer;

    //! Current count of retries for updating icon's pixmap
    int updateXWindowPixmapRetryCount;

    //! The icon's current geometry in scene coordinates
    QRectF updatedXWindowIconGeometry;

    //! X11 Atom for the icon geometry
    static Atom iconGeometryAtom;

    //! A listener object for X events
    QSharedPointer<XEventListener> xEventListener;

#ifdef UNIT_TEST
    friend class Ut_SwitcherButtonView;
#endif

    Q_DISABLE_COPY(SwitcherButtonView);
};

#endif
