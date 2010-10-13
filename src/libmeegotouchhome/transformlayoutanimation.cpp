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
#include <QParallelAnimationGroup>
#include "transformlayoutanimation.h"

TransformAnimation::TransformAnimation(QGraphicsWidget *widget) : animatedWidget(widget)
{
}

TransformAnimation::~TransformAnimation()
{
}

QGraphicsWidget *TransformAnimation::widget() const
{
    return animatedWidget;
}

void TransformAnimation::setEndValue(const QRectF &rect)
{
    invDestSize = QSizeF(1.0f / qMax(qreal(1.0f), rect.width()),
                         1.0f / qMax(qreal(1.0f), rect.height()));

    QVariantAnimation::setEndValue(rect);
}

void TransformAnimation::updateCurrentValue(const QVariant &value)
{
    QRectF  rect      = value.toRectF();
    QRectF  destRect  = endValue().toRectF();
    QPointF translate = rect.topLeft() - destRect.topLeft();
    QPointF scale     = QPointF(rect.size().width()  * invDestSize.width(),
                                rect.size().height() * invDestSize.height());

    animatedWidget->setTransform(QTransform::fromTranslate(translate.x(), translate.y()).scale(scale.x(), scale.y()));
}

void TransformAnimation::updateState(State newState, State)
{
    if(newState == Stopped) {
        // snap to nearest
        if(currentTime() < duration() / 2) {
            animatedWidget->setTransform(QTransform());
            animatedWidget->setGeometry(startValue().toRectF());
        } else {
            setStartValue(endValue());
        }
    }
}

TransformLayoutAnimation::TransformLayoutAnimation(MLayout *layout) : MLayoutAnimation(layout),
    manualControlMode(false), canceled(false), finishNotified(false),
    currentProgress(0), lastProgress(0), currentSpeed(0), lastSpeed(0)
{
    animationGroup = new QParallelAnimationGroup;

    if((container = dynamic_cast<QGraphicsWidget*>(layout->parentLayoutItem()))) {
        connect(container, SIGNAL(xChanged()), this, SLOT(updateContainerPosition()));
        connect(container, SIGNAL(yChanged()), this, SLOT(updateContainerPosition()));
        containerSourcePos = container->pos();
    }
}

TransformLayoutAnimation::~TransformLayoutAnimation()
{
    delete animationGroup;
}

void TransformLayoutAnimation::setManualControl(bool enabled)
{
    manualControlMode = enabled;
}

bool TransformLayoutAnimation::manualControl() const
{
    return manualControlMode;
}

void TransformLayoutAnimation::cancelAnimation()
{
    canceled = true;
}

bool TransformLayoutAnimation::isCanceled()
{
    return canceled;
}

void TransformLayoutAnimation::setProgress(float t)
{
    currentProgress = qBound(0.0f, t, 1.0f);
}

float TransformLayoutAnimation::progress() const
{
    return currentProgress;
}

float TransformLayoutAnimation::currentCurveValue() const
{
    return style()->easingCurve().valueForProgress(currentProgress);
}

float TransformLayoutAnimation::speed() const
{
    return currentSpeed;
}

int TransformLayoutAnimation::duration() const
{
    /* undefined duration, animation runs until separately stopped */
    return -1;
}

void TransformLayoutAnimation::updateContainerPosition()
{
    if(animationGroup->state() == Stopped) {
        containerSourcePos = container->pos();
    } else {
        updateCurrentTime(currentTime());
    }
}

TransformAnimation *TransformLayoutAnimation::createAnimation(QGraphicsWidget *widget)
{
    return new TransformAnimation(widget);
}

void TransformLayoutAnimation::itemAddedToLayout(int index)
{
    QGraphicsWidget *item = dynamic_cast<QGraphicsWidget *>(layout()->itemAt(index));

    if(item == 0) {
        animations.insert(index, 0);
    } else {
        TransformAnimation *anim = createAnimation(item);
        anim->setEasingCurve(style()->easingCurve());

        // the time of the item animations is controlled by the layout animation, so
        // duration here doesn't mean a real duration, but is just a large enough
        // value which is multiplied by the progress factor in the layout animation
        anim->setDuration(1000000);
        animations.insert(index, anim);

        animationGroup->addAnimation(anim);
    }
}

void TransformLayoutAnimation::itemRemovedFromLayout(int index)
{
    TransformAnimation *animation = animations.takeAt(index);

    if(animation) {
        animationGroup->removeAnimation(animation);
        delete animation;
    }
}

void TransformLayoutAnimation::animatedDeleteItem(int index)
{
    layout()->removeAt(index);
}

void TransformLayoutAnimation::hideItem(int index)
{
    TransformAnimation *animation = animations[index];

    if(animation) {
        QGraphicsWidget *item = animations[index]->widget();

        if(item) {
            hideItemNow(item);
        }
    }
}

void TransformLayoutAnimation::updateCurrentTime(int)
{
    // set transforms of the layout items
    animationGroup->setCurrentTime((animationGroup->duration() - 1) * currentProgress);

    // set the container transform
    if(container) {
        qreal t = 1.0f - style()->easingCurve().valueForProgress(currentProgress);

        container->setTransform(QTransform::fromTranslate((containerSourcePos.x() - container->pos().x()) * t,
                                                          (containerSourcePos.y() - container->pos().y()) * t));

        containerPos = container->pos() + QPointF(container->transform().dx(), container->transform().dy());
    }

    // calculate speed if controlled manually, or advance progress when running freely
    if(manualControlMode) {
        // calculate the current speed, filter out any sudden changes in progress
        float smoothness = style()->speedSmoothness();
        currentSpeed = lastSpeed * smoothness + qAbs(currentProgress - lastProgress) * (1.0f - smoothness);
    } else {
        currentProgress += qMax(style()->minimumSpeed(), qreal(currentSpeed)) * (canceled ? -1.0f : 1.0f);

        if(currentProgress > 1.0f || currentProgress < 0.0f) {
            if(!finishNotified) {
                finishNotified = true;
                emit willFinish();
            }
        }

        currentProgress = qBound(0.0f, currentProgress, 1.0f);
    }

    lastProgress = currentProgress;
    lastSpeed    = currentSpeed;
}

void TransformLayoutAnimation::updateState(State newState, State)
{
    switch(newState) {
    case Running:
        if(animationGroup->state() == Stopped) {
            animationGroup->start();
            animationGroup->pause();
        }
        currentProgress = 0.0f;
        lastProgress    = 0.0f;
        currentSpeed    = 0.0f;
        lastSpeed       = 0.0f;
        canceled        = false;
        finishNotified  = false;
        break;
    case Stopped:
        if(animationGroup->state() != Stopped) {
            animationGroup->stop();
        }

        if(container) {
            if(canceled) {
                container->setPos(containerPos);
            } else {
                containerSourcePos = container->pos();
            }
            container->setTransform(QTransform());
        }
        break;
    default:
        break;
    }
}

void TransformLayoutAnimation::setItemGeometry(int index, const QRectF &geometry)
{
    TransformAnimation *animation = animations[index];

    if(animation) {
        QGraphicsWidget *item = animation->widget();

        if(item) {
            if(!item->isVisible()) {
                item->setTransform(QTransform());
                item->setGeometry(geometry);
                showItemNow(item);
                animation->setStartValue(geometry);
            } else {
                if(animation->state() == Stopped) {
                    animation->setStartValue(item->geometry());
                }

                animation->setEndValue(geometry);
                item->setGeometry(geometry);
            }
        }
    }
}
