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
#ifndef STATUSINDICATOR_STUB
#define STATUSINDICATOR_STUB

#include "statusindicator.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorStub : public StubBase
{
public:
    virtual void StatusIndicatorConstructor(DuiWidget *parent);
    virtual void StatusIndicatorDestructor();
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    virtual void setValue(QVariant v);
    virtual QVariant value() const;
    virtual void updateAnimationStatus();
    virtual void setModelUpdatesEnabled(bool modelUpdatesEnabled);
};

// 2. IMPLEMENT STUB
void StatusIndicatorStub::StatusIndicatorConstructor(DuiWidget *parent)
{
    Q_UNUSED(parent);

}
void StatusIndicatorStub::StatusIndicatorDestructor()
{

}
void StatusIndicatorStub::enterDisplayEvent()
{
    stubMethodEntered("enterDisplayEvent");
}

void StatusIndicatorStub::exitDisplayEvent()
{
    stubMethodEntered("exitDisplayEvent");
}

void StatusIndicatorStub::setValue(QVariant v)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QVariant >(v));
    stubMethodEntered("setValue", params);
}

QVariant StatusIndicatorStub::value() const
{
    stubMethodEntered("value");
    return stubReturnValue<QVariant>("value");
}

void StatusIndicatorStub::updateAnimationStatus()
{
    stubMethodEntered("updateAnimationStatus");
}

void StatusIndicatorStub::setModelUpdatesEnabled(bool modelUpdatesEnabled)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(modelUpdatesEnabled));
    stubMethodEntered("setModelUpdatesEnabled", params);
}



// 3. CREATE A STUB INSTANCE
StatusIndicatorStub gDefaultStatusIndicatorStub;
StatusIndicatorStub *gStatusIndicatorStub = &gDefaultStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicator::StatusIndicator(DuiWidget *parent)
{
    gStatusIndicatorStub->StatusIndicatorConstructor(parent);
}

StatusIndicator::~StatusIndicator()
{
    gStatusIndicatorStub->StatusIndicatorDestructor();
}

void StatusIndicator::setValue(QVariant v)
{
    gStatusIndicatorStub->setValue(v);
}

QVariant StatusIndicator::value() const
{
    return gStatusIndicatorStub->value();
}

void StatusIndicator::updateAnimationStatus()
{
    gStatusIndicatorStub->updateAnimationStatus();
}

void StatusIndicator::enterDisplayEvent()
{
    gStatusIndicatorStub->enterDisplayEvent();
}

void StatusIndicator::exitDisplayEvent()
{
    gStatusIndicatorStub->exitDisplayEvent();
}

void StatusIndicator::setModelUpdatesEnabled(bool modelUpdatesEnabled)
{
    gStatusIndicatorStub->setModelUpdatesEnabled(modelUpdatesEnabled);
}


#endif
