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
#ifndef TRANSFORMLAYOUTANIMATION_STUB_H
#define TRANSFORMLAYOUTANIMATION_STUB_H

#include <stubbase.h>
#include <transformlayoutanimation.h>

class TransformLayoutAnimationStub : public StubBase
{
public:
    virtual int duration();
    virtual void setManualControl(bool m);
    virtual bool manualControl();
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    virtual void setProgress(float t);
    virtual void cancelAnimation();
    virtual bool isCanceled();
    virtual float progress();
    virtual float curveProgress();
    virtual float speed();
    virtual TransformAnimation *createAnimation(QGraphicsWidget *widget);
};

int TransformLayoutAnimationStub::duration()
{
    stubMethodEntered("duration");
    return stubReturnValue<int>("duration");
}

void TransformLayoutAnimationStub::setManualControl(bool m)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(m));

    stubMethodEntered("setManualControl", params);
}

bool TransformLayoutAnimationStub::manualControl()
{
    stubMethodEntered("manualControl");
    return stubReturnValue<bool>("manualControl");
}

void TransformLayoutAnimationStub::setProgress(float t)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(t));

    stubMethodEntered("setProgress", params);
}

void TransformLayoutAnimationStub::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QAbstractAnimation::State>(newState));
    params.append(new Parameter<QAbstractAnimation::State>(oldState));

    stubMethodEntered("updateState", params);
}

void TransformLayoutAnimationStub::cancelAnimation()
{
    stubMethodEntered("cancelAnimation");
}

bool TransformLayoutAnimationStub::isCanceled()
{
    stubMethodEntered("isCanceled");
    return stubReturnValue<bool>("isCanceled");
}

float TransformLayoutAnimationStub::progress()
{
    stubMethodEntered("progress");
    return stubReturnValue<float>("progress");
}

float TransformLayoutAnimationStub::curveProgress()
{
    stubMethodEntered("curveProgress");
    return stubReturnValue<float>("curveProgress");
}

float TransformLayoutAnimationStub::speed()
{
    stubMethodEntered("speed");
    return stubReturnValue<float>("speed");
}

TransformAnimation *TransformLayoutAnimationStub::createAnimation(QGraphicsWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsWidget *>(widget));
    stubMethodEntered("createAnimation", params);
    return stubReturnValue<TransformAnimation*>("createAnimation");
}


TransformLayoutAnimationStub gDefaultTransformLayoutAnimationStub;
TransformLayoutAnimationStub *gTransformLayoutAnimationStub = &gDefaultTransformLayoutAnimationStub;


TransformLayoutAnimation::TransformLayoutAnimation(MLayout *layout) : MLayoutAnimation(layout)
{
}

TransformLayoutAnimation::~TransformLayoutAnimation()
{
}

int TransformLayoutAnimation::duration() const
{
    return gTransformLayoutAnimationStub->duration();
}

void TransformLayoutAnimation::updateContainerPosition()
{
}

void TransformLayoutAnimation::itemAddedToLayout(int index)
{
    Q_UNUSED(index)
}

void TransformLayoutAnimation::itemRemovedFromLayout(int index)
{
    Q_UNUSED(index)
}

void TransformLayoutAnimation::animatedDeleteItem(int index)
{
    Q_UNUSED(index)
}

void TransformLayoutAnimation::hideItem(int index)
{
    Q_UNUSED(index)
}

void TransformLayoutAnimation::updateCurrentTime(int currentTime)
{
    Q_UNUSED(currentTime)
}

void TransformLayoutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    gTransformLayoutAnimationStub->updateState(newState, oldState);
}

void TransformLayoutAnimation::setItemGeometry(int index, const QRectF &geometry)
{
    Q_UNUSED(index)
    Q_UNUSED(geometry)
}

void TransformLayoutAnimation::setManualControl(bool m)
{
    gTransformLayoutAnimationStub->setManualControl(m);
}

bool TransformLayoutAnimation::manualControl() const
{
    return gTransformLayoutAnimationStub->manualControl();
}

void TransformLayoutAnimation::cancelAnimation()
{
    gTransformLayoutAnimationStub->cancelAnimation();
}

bool TransformLayoutAnimation::isCanceled()
{
    return gTransformLayoutAnimationStub->isCanceled();
}

void TransformLayoutAnimation::setProgress(float t)
{
    gTransformLayoutAnimationStub->setProgress(t);
}

float TransformLayoutAnimation::progress() const
{
    return gTransformLayoutAnimationStub->progress();
}

float TransformLayoutAnimation::currentCurveValue() const
{
    return gTransformLayoutAnimationStub->curveProgress();
}

float TransformLayoutAnimation::speed() const
{
    return gTransformLayoutAnimationStub->speed();
}

TransformAnimation *TransformLayoutAnimation::createAnimation(QGraphicsWidget *widget)
{
    return gTransformLayoutAnimationStub->createAnimation(widget);
}

#endif
