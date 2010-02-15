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
#ifndef DUIAPPLETID_STUB
#define DUIAPPLETID_STUB

#include "duiappletid.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletIdStub : public StubBase
{
public:
    virtual void DuiAppletIdConstructor();
    virtual void DuiAppletIdConstructor(const QString &applicationName, const QString &mashupCanvasName, const DuiAppletId::AppletInstanceID &instanceId);
    virtual void DuiAppletIdDestructor();
    virtual QString toString() const;
    virtual DuiAppletId::AppletInstanceID instanceId() const;
};

// 2. IMPLEMENT STUB
void DuiAppletIdStub::DuiAppletIdConstructor()
{

}
void DuiAppletIdStub::DuiAppletIdConstructor(const QString &applicationName, const QString &mashupCanvasName, const DuiAppletId::AppletInstanceID &instanceId)
{
    Q_UNUSED(applicationName);
    Q_UNUSED(mashupCanvasName);
    Q_UNUSED(instanceId);

}
void DuiAppletIdStub::DuiAppletIdDestructor()
{

}
QString DuiAppletIdStub::toString() const
{
    stubMethodEntered("toString");
    return stubReturnValue<QString>("toString");
}

DuiAppletId::AppletInstanceID DuiAppletIdStub::instanceId() const
{
    stubMethodEntered("instanceId");
    return stubReturnValue<DuiAppletId::AppletInstanceID>("instanceId");
}



// 3. CREATE A STUB INSTANCE
DuiAppletIdStub gDefaultDuiAppletIdStub;
DuiAppletIdStub *gDuiAppletIdStub = &gDefaultDuiAppletIdStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletId::DuiAppletId()
{
    gDuiAppletIdStub->DuiAppletIdConstructor();
}

DuiAppletId::DuiAppletId(const QString &applicationName, const QString &mashupCanvasName, const AppletInstanceID &instanceId)
{
    gDuiAppletIdStub->DuiAppletIdConstructor(applicationName, mashupCanvasName, instanceId);
}

DuiAppletId::~DuiAppletId()
{
    gDuiAppletIdStub->DuiAppletIdDestructor();
}

QString DuiAppletId::toString() const
{
    return gDuiAppletIdStub->toString();
}

DuiAppletId::AppletInstanceID DuiAppletId::instanceId() const
{
    return gDuiAppletIdStub->instanceId();
}


#endif
