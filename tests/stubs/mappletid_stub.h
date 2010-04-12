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
#ifndef MAPPLETID_STUB
#define MAPPLETID_STUB

#include "mappletid.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletIdStub : public StubBase
{
public:
    virtual void MAppletIdConstructor();
    virtual void MAppletIdConstructor(const QString &applicationName, const QString &mashupCanvasName, const MAppletId::AppletInstanceID &instanceId);
    virtual void MAppletIdDestructor();
    virtual QString toString() const;
    virtual MAppletId::AppletInstanceID instanceId() const;
};

// 2. IMPLEMENT STUB
void MAppletIdStub::MAppletIdConstructor()
{

}
void MAppletIdStub::MAppletIdConstructor(const QString &applicationName, const QString &mashupCanvasName, const MAppletId::AppletInstanceID &instanceId)
{
    Q_UNUSED(applicationName);
    Q_UNUSED(mashupCanvasName);
    Q_UNUSED(instanceId);

}
void MAppletIdStub::MAppletIdDestructor()
{

}
QString MAppletIdStub::toString() const
{
    stubMethodEntered("toString");
    return stubReturnValue<QString>("toString");
}

MAppletId::AppletInstanceID MAppletIdStub::instanceId() const
{
    stubMethodEntered("instanceId");
    return stubReturnValue<MAppletId::AppletInstanceID>("instanceId");
}



// 3. CREATE A STUB INSTANCE
MAppletIdStub gDefaultMAppletIdStub;
MAppletIdStub *gMAppletIdStub = &gDefaultMAppletIdStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAppletId::MAppletId()
{
    gMAppletIdStub->MAppletIdConstructor();
}

MAppletId::MAppletId(const QString &applicationName, const QString &mashupCanvasName, const AppletInstanceID &instanceId)
{
    gMAppletIdStub->MAppletIdConstructor(applicationName, mashupCanvasName, instanceId);
}

MAppletId::~MAppletId()
{
    gMAppletIdStub->MAppletIdDestructor();
}

QString MAppletId::toString() const
{
    return gMAppletIdStub->toString();
}

MAppletId::AppletInstanceID MAppletId::instanceId() const
{
    return gMAppletIdStub->instanceId();
}


#endif
