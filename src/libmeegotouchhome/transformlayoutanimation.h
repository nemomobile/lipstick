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
#ifndef TRANSFORMLAYOUTANIMATION_H
#define TRANSFORMLAYOUTANIMATION_H

#include <MLayoutAnimation>
#include <QVariantAnimation>
#include "transformlayoutanimationstyle.h"

class QParallelAnimationGroup;

/*!
 * Animates a widget's translation and scale transform
 */
class TransformAnimation : public QVariantAnimation
{
public:
    /*!
     * Constructor
     * \param widget the widget to animate
     */
    TransformAnimation(QGraphicsWidget *widget);

    /*! \brief Destructor */
    virtual ~TransformAnimation();

    /*! \reimp */
    virtual void updateCurrentValue(const QVariant &value);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    /*! \reimp_end */

    /*! Returns the widget controlled by this animation */
    QGraphicsWidget *widget() const;

    /*! Sets the destination rectangle */
    void setEndValue(const QRectF &rect);

private:
    /*! The widget controlled by this animation */
    QGraphicsWidget *animatedWidget;

    /*! Inverse destination size */
    QSizeF invDestSize;
};

/*!
 * A layout animation for animating the widget's position, scaling and
 * the position of the widget's container.
 */
class TransformLayoutAnimation : public MLayoutAnimation
{
    M_ANIMATION(TransformLayoutAnimationStyle)
    Q_OBJECT

public:
    /*!
     * Constructor
     * \param layout the layout to animate
     */
    TransformLayoutAnimation(MLayout *layout);

    /*! \brief Destructor */
    virtual ~TransformLayoutAnimation();

    /*! \reimp */
    virtual int duration() const;
    virtual void itemAddedToLayout(int index);
    virtual void itemRemovedFromLayout(int index);
    virtual void animatedDeleteItem(int index);
    virtual void setItemGeometry(int index, const QRectF &geometry);
    virtual void hideItem(int index);
    /*! \reimp_end */

    /*! Sets the animation manual control mode
        \param enabled If true, animation stays at current progress. Otherwise, animation runs
                       automatically to the end */
    void setManualControl(bool enabled);

    /*! Returns the current manual control status */
    bool manualControl() const;

    /*! Cancels the animation. The layout items animate back to the original positions */
    void cancelAnimation();

    /*! Returns true if the animation was canceled */
    bool isCanceled();

    /*! Sets the current progress of the animation
        \param t the new progress, with range 0 to 1 */
    void setProgress(float t);

    /*! Returns the current progress of the animation */
    float progress() const;

    /*! Returns the current value of the animation curve */
    float currentCurveValue() const;

    /*! Returns the current animation speed */
    float speed() const;

signals:
    /*! Sent when the animation is finishing. The animation must be stopped separately
        after receiving this signal. This makes it possible to cancel the geometry changes
        of the layout items without making them jump at the end of the animation. */
    void willFinish();

protected:
    /*! \reimp */
    virtual void updateCurrentTime(int currentTime);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    /*! \reimp_end */

    /*! Called by the layout animation to create animation objects for each widget in the layout
        \param widget the widget to animate
        \return new instance of the animation object */
    virtual TransformAnimation *createAnimation(QGraphicsWidget *widget);

    /*! Transform animations for the widgets in the layout */
    QList<TransformAnimation*> animations;

private slots:
    /*! Updates the container position for animating it when the animation is active */
    void updateContainerPosition();

private:
    /*! Animation source position of the container */
    QPointF containerSourcePos;

    /*! Current position of the container on the screen */
    QPointF containerPos;

    /*! The container (parent layout item) of the layout */
    QGraphicsWidget *container;

    /*! Animation group of the widget animations */
    QParallelAnimationGroup *animationGroup;

    /*! True if the animation is currently manually controlled */
    bool manualControlMode;

    /*! True if the animation was canceled */
    bool canceled;

    /*! True if animation finishing status has been signaled */
    bool finishNotified;

    /*! Current animation progress */
    float currentProgress;

    /*! Animation progress of last update */
    float lastProgress;

    /*! Current animation speed */
    float currentSpeed;

    /*! Animation speed of last update */
    float lastSpeed;
};

#endif
