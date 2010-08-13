/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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

#ifndef XEVENTLISTENER_STUB
#define XEVENTLISTENER_STUB

#include "xeventlistener.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class XEventListenerStub : public StubBase {
public:
    virtual void xEventListenerConstructor();
    virtual void xEventListenerDestructor();
};

// 2. IMPLEMENT STUB
void XEventListenerStub::xEventListenerConstructor()
{
    stubMethodEntered("xEventListenerConstructor");
}

void XEventListenerStub::xEventListenerDestructor()
{
    stubMethodEntered("xEventListenerDestructor");
}

// 3. CREATE A STUB INSTANCE
XEventListenerStub gDefaultXEventListenerStub;
XEventListenerStub* gXEventListenerStub = &gDefaultXEventListenerStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
XEventListener::XEventListener()
{
    gXEventListenerStub->xEventListenerConstructor();
}

XEventListener::~XEventListener()
{
    gXEventListenerStub->xEventListenerDestructor();
}

#endif
