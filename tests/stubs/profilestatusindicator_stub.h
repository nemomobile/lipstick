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
#ifndef PROFILESTATUSINDICATOR_STUB
#define PROFILESTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ProfileStatusIndicatorStub : public StubBase
{
public:
    virtual void ProfileStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void ProfileStatusIndicatorDestructor();
    virtual void profileChanged();
    ContextItem *profileStatus ;
};

// 2. IMPLEMENT STUB
void ProfileStatusIndicatorStub::ProfileStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void ProfileStatusIndicatorStub::ProfileStatusIndicatorDestructor()
{

}
void ProfileStatusIndicatorStub::profileChanged()
{
    stubMethodEntered("profileChanged");
}



// 3. CREATE A STUB INSTANCE
ProfileStatusIndicatorStub gDefaultProfileStatusIndicatorStub;
ProfileStatusIndicatorStub *gProfileStatusIndicatorStub = &gDefaultProfileStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ProfileStatusIndicator::ProfileStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gProfileStatusIndicatorStub->ProfileStatusIndicatorConstructor(context, parent);
}

ProfileStatusIndicator::~ProfileStatusIndicator()
{
    gProfileStatusIndicatorStub->ProfileStatusIndicatorDestructor();
}

void ProfileStatusIndicator::profileChanged()
{
    gProfileStatusIndicatorStub->profileChanged();
}


#endif
