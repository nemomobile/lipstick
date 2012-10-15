/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <contextproperty.h>
#include <QTimeLine>
#include "statusindicator.h"

StatusIndicator::StatusIndicator(QObject *parent) :
    QObject(parent),
    animationDuration_(0),
    isOnDisplay_(true),
    animationTimeLine(new QTimeLine(1000, this)),
    valueIndex_(0)
{
    animationTimeLine->setCurveShape(QTimeLine::LinearCurve);
    animationTimeLine->setLoopCount(0);
    connect(animationTimeLine, SIGNAL(frameChanged(int)), this, SLOT(setCurrentValueIndex(int)));
}

QString StatusIndicator::mode() const
{
    return mode_;
}

void StatusIndicator::setMode(const QString &mode)
{
    mode_ = mode;
    if (isOnDisplay_) {
        emit modeChanged();
    }
}

QString StatusIndicator::value() const
{
    return value_;
}

void StatusIndicator::setValue(const QString &value)
{
    value_ = value;
    if (isOnDisplay_) {
        emit valueChanged();
    }
}

void StatusIndicator::setValueList(const QStringList &valueList)
{
    valueList_ = valueList;
    setInitialValueIndex(valueIndex_);
}

QStringList StatusIndicator::valueList() const
{
    return valueList_;
}

void StatusIndicator::setAnimationDuration(int animationDuration)
{
    animationDuration_ = animationDuration;
    if (isOnDisplay_) {
        emit animationDurationChanged();
    }
    setAnimationTimeLineState();
}

int StatusIndicator::animationDuration() const
{
    return animationDuration_;
}

void StatusIndicator::setIsOnDisplay(bool isOnDisplay)
{
    isOnDisplay_ = isOnDisplay;
    if (isOnDisplay_) {
        foreach(ContextProperty *property, contextProperties) {
            property->blockSignals(false);
            property->subscribe();
        }
        emit subscriptionMade();
        emit modeChanged();
        emit valueChanged();
    } else {
        foreach(ContextProperty *property, contextProperties) {
            property->unsubscribe();
            property->blockSignals(true);
        }
    }
    setAnimationTimeLineState();
    emit isOnDisplayChanged();
}

bool StatusIndicator::isOnDisplay() const
{
    return isOnDisplay_;
}

ContextProperty *StatusIndicator::createContextProperty(const QString &key)
{
    ContextProperty *property = new ContextProperty(key, this);
    contextProperties.append(property);
    connect(this, SIGNAL(subscriptionMade()), property, SIGNAL(valueChanged()));
    if (isOnDisplay_) {
        property->subscribe();
    } else {
        property->blockSignals(true);
    }
    return property;
}

void StatusIndicator::setInitialValueIndex(int index)
{
    valueIndex_ = qMax(qMin(index, valueList_.count() - 1), 0);
    setCurrentValueIndex(valueIndex_);
    setAnimationTimeLineState();
}

void StatusIndicator::setCurrentValueIndex(int index)
{
    setValue(valueList_.isEmpty() ? QString() : valueList_.at(qMax(qMin(index, valueList_.count() - 1), 0)));
}

void StatusIndicator::setAnimationTimeLineState()
{
    bool animate = isOnDisplay_ ? (animationDuration_ > 0 && valueList_.count() > 1) : false;

    if (animate) {
        int frames = valueList_.count() - valueIndex_;
        int loopDuration = animationDuration_ * frames / (valueList_.isEmpty() ? 1 : valueList_.count());
        animationTimeLine->setFrameRange(valueIndex_, valueList_.count());
        animationTimeLine->setDuration(loopDuration);
        animationTimeLine->setUpdateInterval(animationDuration_ / valueList_.count());
    }

    if (animate && animationTimeLine->state() == QTimeLine::NotRunning) {
        animationTimeLine->start();
    } else if (!animate && animationTimeLine->state() == QTimeLine::Running) {
        animationTimeLine->stop();
    }    
}
