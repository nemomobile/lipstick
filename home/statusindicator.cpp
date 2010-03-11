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

#include <DuiApplication>
#include "statusindicator.h"
#include "statusindicatormodel.h"
#include "applicationcontext.h"

StatusIndicator::StatusIndicator(DuiWidget *parent) :
    DuiWidgetController(new StatusIndicatorModel, parent),
    animateIfPossible(false),
    modelUpdatesEnabled(true),
    currentValue(QVariant())
{
}

StatusIndicator::~StatusIndicator()
{
}

void StatusIndicator::setValue(QVariant v)
{
    currentValue = v;
    if (modelUpdatesEnabled) {
        model()->setValue(v);
    }
}

QVariant StatusIndicator::value() const
{
    return currentValue;
}

void StatusIndicator::enterDisplayEvent()
{
    setModelUpdatesEnabled(true);
}

void StatusIndicator::exitDisplayEvent()
{
    setModelUpdatesEnabled(false);
}

void StatusIndicator::setModelUpdatesEnabled(bool modelUpdatesEnabled)
{
    this->modelUpdatesEnabled = modelUpdatesEnabled;
    if (modelUpdatesEnabled) {
        model()->setValue(currentValue);
    }
    updateAnimationStatus();
}

void StatusIndicator::updateAnimationStatus()
{
    if (modelUpdatesEnabled) {
        model()->setAnimate(animateIfPossible);
    } else {
        model()->setAnimate(false);
    }
}

PhoneNetworkStatusIndicator::PhoneNetworkStatusIndicator(ApplicationContext &context, DuiWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    networkName = QSharedPointer<ContextItem>(context.createContextItem("Cellular.NetworkName"));
    connect(networkName.data(), SIGNAL(contentsChanged()), this, SLOT(phoneNetworkChanged()));
    phoneNetworkChanged();
}

PhoneNetworkStatusIndicator::~PhoneNetworkStatusIndicator()
{
}

void PhoneNetworkStatusIndicator::phoneNetworkChanged()
{
    QVariant value = networkName.data()->value();
    setValue((value.isValid() && !value.isNull()) ? value : QVariant("Offline"));
}
