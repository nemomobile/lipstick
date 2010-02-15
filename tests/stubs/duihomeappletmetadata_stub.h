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
#ifndef DUIHOMEAPPLETMETADATA_STUB
#define DUIHOMEAPPLETMETADATA_STUB

#include "duihomeappletmetadata.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiHomeAppletMetaDataStub : public StubBase
{
public:
    virtual void DuiHomeAppletMetaDataConstructor(const QString &filename);
    virtual void DuiHomeAppletMetaDataDestructor();
    virtual bool isValid() const;
    virtual QString runnerBinary() const;
    virtual QString appletBinary() const;
};

// 2. IMPLEMENT STUB
void DuiHomeAppletMetaDataStub::DuiHomeAppletMetaDataConstructor(const QString &filename)
{
    Q_UNUSED(filename);

}
void DuiHomeAppletMetaDataStub::DuiHomeAppletMetaDataDestructor()
{

}
bool DuiHomeAppletMetaDataStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

QString DuiHomeAppletMetaDataStub::runnerBinary() const
{
    stubMethodEntered("runnerBinary");
    return stubReturnValue<QString>("runnerBinary");
}

QString DuiHomeAppletMetaDataStub::appletBinary() const
{
    stubMethodEntered("appletBinary");
    return stubReturnValue<QString>("appletBinary");
}



// 3. CREATE A STUB INSTANCE
DuiHomeAppletMetaDataStub gDefaultDuiHomeAppletMetaDataStub;
DuiHomeAppletMetaDataStub *gDuiHomeAppletMetaDataStub = &gDefaultDuiHomeAppletMetaDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiHomeAppletMetaData::DuiHomeAppletMetaData(const QString &filename)
{
    gDuiHomeAppletMetaDataStub->DuiHomeAppletMetaDataConstructor(filename);
}

DuiHomeAppletMetaData::~DuiHomeAppletMetaData()
{
    gDuiHomeAppletMetaDataStub->DuiHomeAppletMetaDataDestructor();
}

bool DuiHomeAppletMetaData::isValid() const
{
    return gDuiHomeAppletMetaDataStub->isValid();
}

QString DuiHomeAppletMetaData::runnerBinary() const
{
    return gDuiHomeAppletMetaDataStub->runnerBinary();
}

QString DuiHomeAppletMetaData::appletBinary() const
{
    return gDuiHomeAppletMetaDataStub->appletBinary();
}


#endif
