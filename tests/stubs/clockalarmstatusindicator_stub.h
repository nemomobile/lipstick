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
#ifndef CLOCKALARMSTATUSINDICATOR_STUB
#define CLOCKALARMSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ClockAlarmStatusIndicatorStub : public StubBase
{
public:
    virtual void ClockAlarmStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void ClockAlarmStatusIndicatorDestructor();
    virtual void clockAlarmChanged();
    ContextItem *clockAlarm ;
};

// 2. IMPLEMENT STUB
void ClockAlarmStatusIndicatorStub::ClockAlarmStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void ClockAlarmStatusIndicatorStub::ClockAlarmStatusIndicatorDestructor()
{

}
void ClockAlarmStatusIndicatorStub::clockAlarmChanged()
{
    stubMethodEntered("clockAlarmChanged");
}



// 3. CREATE A STUB INSTANCE
ClockAlarmStatusIndicatorStub gDefaultClockAlarmStatusIndicatorStub;
ClockAlarmStatusIndicatorStub *gClockAlarmStatusIndicatorStub = &gDefaultClockAlarmStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ClockAlarmStatusIndicator::ClockAlarmStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gClockAlarmStatusIndicatorStub->ClockAlarmStatusIndicatorConstructor(context, parent);
}

ClockAlarmStatusIndicator::~ClockAlarmStatusIndicator()
{
    gClockAlarmStatusIndicatorStub->ClockAlarmStatusIndicatorDestructor();
}

void ClockAlarmStatusIndicator::clockAlarmChanged()
{
    gClockAlarmStatusIndicatorStub->clockAlarmChanged();
}


#endif
