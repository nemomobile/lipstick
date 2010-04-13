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
#include "switcherbuttonmodel.h"
#include "switcherbuttonstyle.h"
#include "x11wrapper.h"

class SwitcherButton;
class MButton;

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
    virtual QRectF boundingRect() const;
    virtual void applyStyle();
    void setGeometry(const QRectF &rect);
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    /*!
     * \brief A slot for receiving information about pixmap changes
     */
    virtual void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);

    /*!
     * \brief Slot for getting information about window visibility changes
     */
    virtual void windowVisibilityChanged(Window window);

    /*!
     * \brief Starts getting damage events for the current X window.
     */
    void setOnDisplay();

    /*!
     * \brief Stops getting damage events for the current X window.
     */
    void unsetOnDisplay();

protected:
    /*!
     * Creates an X damage structure based on the current X window pixmap.
     */
    void createDamage();

    /*!
     * Destroys the current X damage structure.
     */
    void destroyDamage();

    //! \reimp
    virtual void setupModel();
    //! \reimp_end

    /*!
     * \brief Updates the X window pixmap.
     */
    virtual void updateXWindowPixmap();

    /*!
     * \brief Updates the thumbnail pixmap.
     */
    virtual void updateThumbnail();

    /*!
     * \brief Draws the backend specific parts of the background
     */
    virtual void backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect& target) const;

    /*!
     * \brief Does the backend specific parts of updating the X Window pixmap
     */
    virtual void backendSpecificUpdateXWindowPixmap();

    /*! 
     * Updates the button style to reflect the current view mode.
     */
    void updateViewMode();

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

    //! Button for closing the window
    MButton *closeButton;

    //! Whether the button is being displayed or not
    bool onDisplay;

    Q_DISABLE_COPY(SwitcherButtonView);
};

#endif
