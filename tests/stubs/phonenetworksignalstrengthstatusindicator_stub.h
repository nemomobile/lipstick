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
#ifndef PHONENETWORKSIGNALSTRENGTHSTATUSINDICATOR_STUB
#define PHONENETWORKSIGNALSTRENGTHSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class PhoneNetworkSignalStrengthStatusIndicatorStub : public StubBase
{
public:
    virtual void PhoneNetworkSignalStrengthStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void PhoneNetworkSignalStrengthStatusIndicatorDestructor();
    virtual void signalStrengthChanged();
    ContextItem *signalStrength ;
};

// 2. IMPLEMENT STUB
void PhoneNetworkSignalStrengthStatusIndicatorStub::PhoneNetworkSignalStrengthStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void PhoneNetworkSignalStrengthStatusIndicatorStub::PhoneNetworkSignalStrengthStatusIndicatorDestructor()
{

}
void PhoneNetworkSignalStrengthStatusIndicatorStub::signalStrengthChanged()
{
    stubMethodEntered("signalStrengthChanged");
}



// 3. CREATE A STUB INSTANCE
PhoneNetworkSignalStrengthStatusIndicatorStub gDefaultPhoneNetworkSignalStrengthStatusIndicatorStub;
PhoneNetworkSignalStrengthStatusIndicatorStub *gPhoneNetworkSignalStrengthStatusIndicatorStub = &gDefaultPhoneNetworkSignalStrengthStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
PhoneNetworkSignalStrengthStatusIndicator::PhoneNetworkSignalStrengthStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gPhoneNetworkSignalStrengthStatusIndicatorStub->PhoneNetworkSignalStrengthStatusIndicatorConstructor(context, parent);
}

PhoneNetworkSignalStrengthStatusIndicator::~PhoneNetworkSignalStrengthStatusIndicator()
{
    gPhoneNetworkSignalStrengthStatusIndicatorStub->PhoneNetworkSignalStrengthStatusIndicatorDestructor();
}

void PhoneNetworkSignalStrengthStatusIndicator::signalStrengthChanged()
{
    gPhoneNetworkSignalStrengthStatusIndicatorStub->signalStrengthChanged();
}


#endif
