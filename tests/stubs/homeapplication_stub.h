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
#ifndef HOMEAPPLICATION_STUB
#define HOMEAPPLICATION_STUB

#include "homeapplication.h"
#include <stubbase.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class HomeApplicationStub : public StubBase
{
public:
    virtual void HomeApplicationConstructor(int &argc, char **argv);
    virtual void HomeApplicationDestructor();
    virtual void addXEventListener(XEventListener *listener);
    virtual void removeXEventListener(XEventListener *listener);
    virtual bool x11EventFilter(XEvent *event);
    virtual void sendStartupNotifications();
};

// 2. IMPLEMENT STUB
void HomeApplicationStub::HomeApplicationConstructor(int &argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
}
void HomeApplicationStub::HomeApplicationDestructor()
{
}

void HomeApplicationStub::addXEventListener(XEventListener *listener)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<XEventListener*>(listener));
    stubMethodEntered("addXEventListener", params);
}

void HomeApplicationStub::removeXEventListener(XEventListener *listener)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<XEventListener*>(listener));
    stubMethodEntered("removeXEventListener", params);
}

bool HomeApplicationStub::x11EventFilter(XEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<XEvent * >(event));
    stubMethodEntered("x11EventFilter", params);
    return stubReturnValue<bool>("x11EventFilter");
}

void HomeApplicationStub::sendStartupNotifications()
{
    stubMethodEntered("sendStartupNotifications");
}

// 3. CREATE A STUB INSTANCE
HomeApplicationStub gDefaultHomeApplicationStub;
HomeApplicationStub *gHomeApplicationStub = &gDefaultHomeApplicationStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
HomeApplication::HomeApplication(int &argc, char **argv) : MApplication(argc, argv)
{
    gHomeApplicationStub->HomeApplicationConstructor(argc, argv);
}

HomeApplication::~HomeApplication()
{
    gHomeApplicationStub->HomeApplicationDestructor();
}

void HomeApplication::addXEventListener(XEventListener *listener)
{
    gHomeApplicationStub->addXEventListener(listener);
}

void HomeApplication::removeXEventListener(XEventListener *listener)
{
    gHomeApplicationStub->removeXEventListener(listener);
}

bool HomeApplication::x11EventFilter(XEvent *event)
{
    return gHomeApplicationStub->x11EventFilter(event);
}

void HomeApplication::sendStartupNotifications()
{
    gHomeApplicationStub->sendStartupNotifications();
}


#endif
