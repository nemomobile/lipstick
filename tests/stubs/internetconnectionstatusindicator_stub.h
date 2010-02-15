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
#ifndef INTERNETCONNECTIONSTATUSINDICATOR_STUB
#define INTERNETCONNECTIONSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class InternetConnectionStatusIndicatorStub : public StubBase
{
public:
    virtual void InternetConnectionStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void InternetConnectionStatusIndicatorDestructor();
    virtual void wifiSignalStrengthChanged();
    virtual void internetConnectionChanged();
    ContextItem *internetConnection ;
    ContextItem *wifiSignalStrength ;
};

// 2. IMPLEMENT STUB
void InternetConnectionStatusIndicatorStub::InternetConnectionStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void InternetConnectionStatusIndicatorStub::InternetConnectionStatusIndicatorDestructor()
{

}
void InternetConnectionStatusIndicatorStub::wifiSignalStrengthChanged()
{
    stubMethodEntered("wifiSignalStrengthChanged");
}

void InternetConnectionStatusIndicatorStub::internetConnectionChanged()
{
    stubMethodEntered("internetConnectionChanged");
}



// 3. CREATE A STUB INSTANCE
InternetConnectionStatusIndicatorStub gDefaultInternetConnectionStatusIndicatorStub;
InternetConnectionStatusIndicatorStub *gInternetConnectionStatusIndicatorStub = &gDefaultInternetConnectionStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
InternetConnectionStatusIndicator::InternetConnectionStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gInternetConnectionStatusIndicatorStub->InternetConnectionStatusIndicatorConstructor(context, parent);
}

InternetConnectionStatusIndicator::~InternetConnectionStatusIndicator()
{
    gInternetConnectionStatusIndicatorStub->InternetConnectionStatusIndicatorDestructor();
}

void InternetConnectionStatusIndicator::wifiSignalStrengthChanged()
{
    gInternetConnectionStatusIndicatorStub->wifiSignalStrengthChanged();
}

void InternetConnectionStatusIndicator::internetConnectionChanged()
{
    gInternetConnectionStatusIndicatorStub->internetConnectionChanged();
}


#endif
