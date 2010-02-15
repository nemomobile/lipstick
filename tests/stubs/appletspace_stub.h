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
#ifndef APPLETSPACE_STUB
#define APPLETSPACE_STUB

#include "appletspace.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class AppletSpaceStub : public StubBase
{
public:
    virtual void AppletSpaceConstructor(DuiWidget *parent);
    virtual void AppletSpaceDestructor();
    virtual void setEnabled(bool enabled);
};

// 2. IMPLEMENT STUB
void AppletSpaceStub::AppletSpaceConstructor(DuiWidget *parent)
{
    Q_UNUSED(parent);

}
void AppletSpaceStub::AppletSpaceDestructor()
{

}
void AppletSpaceStub::setEnabled(bool enabled)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enabled));
    stubMethodEntered("setEnabled", params);
}



// 3. CREATE A STUB INSTANCE
AppletSpaceStub gDefaultAppletSpaceStub;
AppletSpaceStub *gAppletSpaceStub = &gDefaultAppletSpaceStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
AppletSpace::AppletSpace(DuiWidget *parent)
{
    gAppletSpaceStub->AppletSpaceConstructor(parent);
}

AppletSpace::~AppletSpace()
{
    gAppletSpaceStub->AppletSpaceDestructor();
}

void AppletSpace::setEnabled(bool enabled)
{
    gAppletSpaceStub->setEnabled(enabled);
}


#endif
