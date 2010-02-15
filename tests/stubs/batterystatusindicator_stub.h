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
#ifndef BATTERYSTATUSINDICATOR_STUB
#define BATTERYSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class BatteryStatusIndicatorStub : public StubBase
{
public:
    virtual void BatteryStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void BatteryStatusIndicatorDestructor();
    virtual void batteryLevelChanged();
    virtual void batteryChargingChanged();
    ContextItem *batteryLevel ;
    ContextItem *batteryCharging ;
};

// 2. IMPLEMENT STUB
void BatteryStatusIndicatorStub::BatteryStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void BatteryStatusIndicatorStub::BatteryStatusIndicatorDestructor()
{

}
void BatteryStatusIndicatorStub::batteryLevelChanged()
{
    stubMethodEntered("batteryLevelChanged");
}

void BatteryStatusIndicatorStub::batteryChargingChanged()
{
    stubMethodEntered("batteryChargingChanged");
}



// 3. CREATE A STUB INSTANCE
BatteryStatusIndicatorStub gDefaultBatteryStatusIndicatorStub;
BatteryStatusIndicatorStub *gBatteryStatusIndicatorStub = &gDefaultBatteryStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
BatteryStatusIndicator::BatteryStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gBatteryStatusIndicatorStub->BatteryStatusIndicatorConstructor(context, parent);
}

BatteryStatusIndicator::~BatteryStatusIndicator()
{
    gBatteryStatusIndicatorStub->BatteryStatusIndicatorDestructor();
}

void BatteryStatusIndicator::batteryLevelChanged()
{
    gBatteryStatusIndicatorStub->batteryLevelChanged();
}

void BatteryStatusIndicator::batteryChargingChanged()
{
    gBatteryStatusIndicatorStub->batteryChargingChanged();
}


#endif
