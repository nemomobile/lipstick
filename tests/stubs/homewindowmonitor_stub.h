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
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class HomeWindowMonitorStub : public StubBase {
public:
    virtual void registerWindowId(WId wid);
    virtual bool isOwnWindow(WId wid);
};

// 2. IMPLEMENT STUB
void HomeWindowMonitorStub::registerWindowId(WId wid) {
    QList<ParameterBase*> params;
    params.append(new Parameter<WId>(wid));
    stubMethodEntered("registerWindowId", params);
}

bool HomeWindowMonitorStub::isOwnWindow(WId wid) {
    QList<ParameterBase*> params;
    params.append(new Parameter<WId>(wid));
    stubMethodEntered("isOwnWindow", params);
    stubReturnValue<bool>("isOwnWindow");
    return stubReturnValue<bool>("isOwnWindow");
}

// 3. CREATE A STUB INSTANCE
HomeWindowMonitorStub gDefaultHomeWindowMonitorStub;
HomeWindowMonitorStub* gHomeWindowMonitorStub = &gDefaultHomeWindowMonitorStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
void HomeWindowMonitor::registerWindowId(WId wid) {
    gHomeWindowMonitorStub->registerWindowId(wid);
}

bool HomeWindowMonitor::isOwnWindow(WId wid) const {
    return gHomeWindowMonitorStub->isOwnWindow(wid);
}

#endif
