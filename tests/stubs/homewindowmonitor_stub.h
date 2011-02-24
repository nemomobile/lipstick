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

#ifndef HOMEWINDOWMONITOR_STUB
#define HOMEWINDOWMONITOR_STUB

#include "homewindowmonitor.h"
#include "xeventlistener_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class HomeWindowMonitorStub : public StubBase {
public:
    virtual const HomeWindowMonitor *instance();
    virtual bool isOwnWindow(WId wid);
    virtual bool handleXEvent(const XEvent& event);
};

// 2. IMPLEMENT STUB

const HomeWindowMonitor *HomeWindowMonitorStub::instance() {
    stubMethodEntered("instance");
    return stubReturnValue<HomeWindowMonitor*>("instance");
}

bool HomeWindowMonitorStub::isOwnWindow(WId wid) {
    QList<ParameterBase*> params;
    params.append(new Parameter<WId>(wid));
    stubMethodEntered("isOwnWindow", params);
    return stubReturnValue<bool>("isOwnWindow");
}

bool HomeWindowMonitorStub::handleXEvent(const XEvent& event) {
    QList<ParameterBase*> params;
    params.append(new Parameter<const XEvent&>(event));
    stubMethodEntered("handleXEvent", params);
    return stubReturnValue<bool>("handleXEvent");
}

// 3. CREATE A STUB INSTANCE
HomeWindowMonitorStub gDefaultHomeWindowMonitorStub;
HomeWindowMonitorStub* gHomeWindowMonitorStub = &gDefaultHomeWindowMonitorStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
HomeWindowMonitor::HomeWindowMonitor() : netClientListStacking (0)
{
}

HomeWindowMonitor::~HomeWindowMonitor()
{
}

const HomeWindowMonitor *HomeWindowMonitor::instance() {
    return gHomeWindowMonitorStub->instance();
}

bool HomeWindowMonitor::isOwnWindow(WId wid) const {
    return gHomeWindowMonitorStub->isOwnWindow(wid);
}

bool HomeWindowMonitor::handleXEvent(const XEvent &event)
{
    return gHomeWindowMonitorStub->handleXEvent(event);
}

QList<Window> HomeWindowMonitor::windowStackingOrder() {
    return QList<Window>();
}

#endif
