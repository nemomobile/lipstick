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
#ifndef PHONENETWORKSTATUSINDICATOR_STUB
#define PHONENETWORKSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class PhoneNetworkStatusIndicatorStub : public StubBase
{
public:
    virtual void PhoneNetworkStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void PhoneNetworkStatusIndicatorDestructor();
    virtual void phoneNetworkChanged();
    ContextItem *networkName ;
};

// 2. IMPLEMENT STUB
void PhoneNetworkStatusIndicatorStub::PhoneNetworkStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void PhoneNetworkStatusIndicatorStub::PhoneNetworkStatusIndicatorDestructor()
{

}
void PhoneNetworkStatusIndicatorStub::phoneNetworkChanged()
{
    stubMethodEntered("phoneNetworkChanged");
}



// 3. CREATE A STUB INSTANCE
PhoneNetworkStatusIndicatorStub gDefaultPhoneNetworkStatusIndicatorStub;
PhoneNetworkStatusIndicatorStub *gPhoneNetworkStatusIndicatorStub = &gDefaultPhoneNetworkStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
PhoneNetworkStatusIndicator::PhoneNetworkStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gPhoneNetworkStatusIndicatorStub->PhoneNetworkStatusIndicatorConstructor(context, parent);
}

PhoneNetworkStatusIndicator::~PhoneNetworkStatusIndicator()
{
    gPhoneNetworkStatusIndicatorStub->PhoneNetworkStatusIndicatorDestructor();
}

void PhoneNetworkStatusIndicator::phoneNetworkChanged()
{
    gPhoneNetworkStatusIndicatorStub->phoneNetworkChanged();
}


#endif
