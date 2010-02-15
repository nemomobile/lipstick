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
#ifndef GPSSTATUSINDICATOR_STUB
#define GPSSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class GPSStatusIndicatorStub : public StubBase
{
public:
    virtual void GPSStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void GPSStatusIndicatorDestructor();
    virtual void gpsChanged();
    ContextItem *gps ;
};

// 2. IMPLEMENT STUB
void GPSStatusIndicatorStub::GPSStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void GPSStatusIndicatorStub::GPSStatusIndicatorDestructor()
{

}
void GPSStatusIndicatorStub::gpsChanged()
{
    stubMethodEntered("gpsChanged");
}



// 3. CREATE A STUB INSTANCE
GPSStatusIndicatorStub gDefaultGPSStatusIndicatorStub;
GPSStatusIndicatorStub *gGPSStatusIndicatorStub = &gDefaultGPSStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
GPSStatusIndicator::GPSStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gGPSStatusIndicatorStub->GPSStatusIndicatorConstructor(context, parent);
}

GPSStatusIndicator::~GPSStatusIndicator()
{
    gGPSStatusIndicatorStub->GPSStatusIndicatorDestructor();
}

void GPSStatusIndicator::gpsChanged()
{
    gGPSStatusIndicatorStub->gpsChanged();
}


#endif
