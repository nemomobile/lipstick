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
#ifndef CLOCK_STUB
#define CLOCK_STUB

#include "clock.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ClockStub : public StubBase
{
public:
    virtual void ClockConstructor(QGraphicsItem *parent);
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    virtual void setModelUpdatesEnabled(bool enabled);
    virtual void timerEvent(QTimerEvent *event);
};

// 2. IMPLEMENT STUB
void ClockStub::ClockConstructor(QGraphicsItem *parent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsItem *>(parent));
    stubMethodEntered("ClockConstructor", params);
}
void ClockStub::enterDisplayEvent()
{
    stubMethodEntered("enterDisplayEvent");
}

void ClockStub::exitDisplayEvent()
{
    stubMethodEntered("exitDisplayEvent");
}

void ClockStub::setModelUpdatesEnabled(bool enabled)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enabled));
    stubMethodEntered("setModelUpdatesEnabled", params);
}

void ClockStub::timerEvent(QTimerEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QTimerEvent * >(event));
    stubMethodEntered("timerEvent", params);
}



// 3. CREATE A STUB INSTANCE
ClockStub gDefaultClockStub;
ClockStub *gClockStub = &gDefaultClockStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Clock::Clock(QGraphicsItem *parent)
{
    gClockStub->ClockConstructor(parent);
}

void Clock::enterDisplayEvent()
{
    gClockStub->enterDisplayEvent();
}

void Clock::exitDisplayEvent()
{
    gClockStub->exitDisplayEvent();
}

void Clock::setModelUpdatesEnabled(bool enabled)
{
    gClockStub->setModelUpdatesEnabled(enabled);
}

void Clock::timerEvent(QTimerEvent *event)
{
    gClockStub->timerEvent(event);
}


#endif
