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
#ifndef SWITCHERVIEWBASE_H
#define SWITCHERVIEWBASE_H

#include <MWidgetView>
#include "switchermodel.h"
#include "switcherbasestyle.h"

class Switcher;
class MLayout;
class MPannableViewport;
class QGraphicsLinearLayout;
class TransformLayoutAnimation;
class QPropertyAnimation;

/*!
 * The switcher view draws a background for the switcher.
 */
class SwitcherViewBase : public MWidgetView
{
    Q_OBJECT
    M_VIEW(SwitcherModel, SwitcherBaseStyle)

public:
    /*!
     * Constructs a SwitcherViewBase.
     *
     * \param container the Switcher controller to be used
     */
    SwitcherViewBase(Switcher *controller);

    /*!
     * Destroys the SwitcherViewBase.
     */
    virtual ~SwitcherViewBase();

    /*! \reimp
     * Re-implented event handler. Needed to enable gestures.
     *
     * \param e event to be handled
     * \return true if event was accepted, otherwise false
     */
    bool event(QEvent *e);
    //! \reimp_end

protected:
    /*! \reimp
     * Re-implemented here to get an update when the model is set
     */
    virtual void setupModel();

    /*!
      * Re-implemented to handle the mode switch
      *
      * \param event Gesture event class for general event handling.
      * \param gesture Pinch gesture class. Contains all pinch specific information
      */
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);

    /*!
     * Filters QGraphicsSceneMouseMove events from SwitcherButtons.
     */
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    //! \reimp_end

    /*! Selects the layout policy and sets up switcher mode dependent signals */
    virtual void applySwitcherMode();

    /*!
     * Remove all buttons from layout and set parents to NULL
     * Parents are set to NULL to avoid double deletion as buttons are QSharedPointer's in model
     */
    void removeButtonsFromLayout();

    /*! Returns the index of a SwitcherButton in the model or -1, if not found */
    int buttonIndex(const SwitcherButton* button) const;

    //! Calculates which switcher button is nearest to point centerPoint
    void calculateNearestButtonAt(const QPointF &centerPoint);

    /*! Starts the bounce animation */
    virtual void startBounceAnimation();

    /*! Sets the direction of the bounce animation */
    void setInwardBounceAnimation(bool i);

    /*! Called when the pinch gesture starts
        \param centerPoint the center point of the pinch gesture */
    virtual void pinchBegin(const QPointF &centerPoint);

    /*! Called to update the scale factor during the pinch gesture
        \param scaleFactor the updated scale factor */
    virtual void pinchUpdate(float scaleFactor);

    /*! Called when the pinch gesture ends */
    virtual void pinchEnd();

    /*! Enables or disables the physics of the viewports under this widget, to prevent unwanted movement
      \param enabled the enabled or disabled state to set */
    void setViewportPhysicsEnabled(bool enabled);

    /*! The switcher controller */
    Switcher *controller;

    /*! Layouts */
    QGraphicsLinearLayout *mainLayout;

    /*! The widget that will contain all of the switcher buttons */
    MWidget* pannedWidget;

    /*! Layout for the panned widget */
    MLayout* pannedLayout;

    /*! The button being currently pinched */
    int pinchedButtonPosition;

    /*! Layout animation for animating switcher buttons */
    TransformLayoutAnimation *layoutAnimation;

    /*! True if the pinch direction is to the mode we're already in */
    bool overpinch;

    /*! Animation for the bounce when going to overview mode and when overpinching */
    QPropertyAnimation *bounceAnimation;

    /*! The target mode for pinch gesture */
    SwitcherModel::Mode pinchGestureTargetMode;

    /*! The viewport that shows the switcher buttons */
    MPannableViewport *viewport;

    /*! True if switcher is currently animating */
    bool animating;

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    /*! Ends the transition between switcher modes */
    virtual void endTransition();

    /*! Ends the bounce effect */
    virtual void endBounce();

    /*! Sets the switcher mode according to current pinch gesture target */
    virtual void applyPinchGestureTargetMode();

    /*! Runs the bounce animation if currently in overview mode */
    void runOverviewBounceAnimation();

    /*! Updates the animation status and sends animationStateChanged signal if status changed */
    virtual void updateAnimationStatus();

signals:
    /*! Sent when switcher starts or stops animating */
    void animationStateChanged(bool animating);
};

#endif
