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
#ifndef SWITCHERBUTTONVIEW_H
#define SWITCHERBUTTONVIEW_H

#include <QTimer>
#include <DuiWidgetView>
#include "switcherbuttonmodel.h"
#include "switcherbuttonstyle.h"
#include "x11wrapper.h"

class SwitcherButton;

/*!
 * \class SwitcherButtonView
 *
 * \brief SwitcherButtonView implements a switcher style view
 */
class SwitcherButtonView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(SwitcherButtonModel, SwitcherButtonStyle)

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
    virtual QRectF boundingRect() const;
    virtual void applyStyle();
    //! \reimp_end

signals:
    /*!
     * \brief Signals that the button has been released
     */
    void clicked();

    /*!
     * \brief Signals when mouse button has been pressed
     */
    void pressed();

    /*!
     * \brief Signals when the window should be brought to front
     */
    void switched();

    /*!
     * \brief Signals when the window should be closed
     */
    void closed();

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    /*!
     * A slot that should be called when window closing has been requested but the window has not closed during a certain time.
     */
    void resetState();

    /*!
     * \brief Slot for getting information about window visibility changes
     */
    virtual void windowVisibilityChanged(Window window);

protected:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    virtual void setupModel();
    //! \reimp_end

    /*!
     * \brief Updates the X window pixmap.
     */
    void updateXWindowPixmap();

    /*!
     * \brief Updates the thumbnail pixmap.
     */
    virtual void updateThumbnail();

    /*!
     * \brief Draws the backend specific parts of the background
     */
    virtual void backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * \brief Does the backend specific parts of updating the X Window pixmap
     */
    virtual void backendSpecificUpdateXWindowPixmap();

    //! Returns the bounding rectangle of the button without the closing icon
    QRectF buttonRect() const;

    //! Returns the bounding rectangle of the closing icon
    QRectF closeRect() const;

    //! Returns the bounding rectangle of the title part of the button
    QRectF titleRect() const;

#ifdef Q_WS_X11
    //! Handles X BadMatch errors
    static int handleXError(Display *display, XErrorEvent *event);

    //! Whether a BadMatch X error has occurred
    static bool badMatchOccurred;
#endif

    //! SwitcherButton controller
    SwitcherButton *controller;

    //! The X window's pixmap
    Pixmap xWindowPixmap;

    //! XDamage handle for the X window's pixmap
    Damage xWindowPixmapDamage;

    //! The point where dragging started
    QPointF dragStartPos;

    //! A timer to check if the window is closed when switcher button is dragged outside switcher before timer expires
    QTimer windowCloseTimer;

    Q_DISABLE_COPY(SwitcherButtonView);
};

#endif
