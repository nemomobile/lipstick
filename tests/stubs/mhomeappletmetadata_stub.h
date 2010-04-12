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
#ifndef MHOMEAPPLETMETADATA_STUB
#define MHOMEAPPLETMETADATA_STUB

#include "mhomeappletmetadata.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MHomeAppletMetaDataStub : public StubBase
{
public:
    virtual void MHomeAppletMetaDataConstructor(const QString &filename);
    virtual void MHomeAppletMetaDataDestructor();
    virtual bool isValid() const;
    virtual QString runnerBinary() const;
    virtual QString appletBinary() const;
};

// 2. IMPLEMENT STUB
void MHomeAppletMetaDataStub::MHomeAppletMetaDataConstructor(const QString &filename)
{
    Q_UNUSED(filename);

}
void MHomeAppletMetaDataStub::MHomeAppletMetaDataDestructor()
{

}
bool MHomeAppletMetaDataStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

QString MHomeAppletMetaDataStub::runnerBinary() const
{
    stubMethodEntered("runnerBinary");
    return stubReturnValue<QString>("runnerBinary");
}

QString MHomeAppletMetaDataStub::appletBinary() const
{
    stubMethodEntered("appletBinary");
    return stubReturnValue<QString>("appletBinary");
}



// 3. CREATE A STUB INSTANCE
MHomeAppletMetaDataStub gDefaultMHomeAppletMetaDataStub;
MHomeAppletMetaDataStub *gMHomeAppletMetaDataStub = &gDefaultMHomeAppletMetaDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MHomeAppletMetaData::MHomeAppletMetaData(const QString &filename)
{
    gMHomeAppletMetaDataStub->MHomeAppletMetaDataConstructor(filename);
}

MHomeAppletMetaData::~MHomeAppletMetaData()
{
    gMHomeAppletMetaDataStub->MHomeAppletMetaDataDestructor();
}

bool MHomeAppletMetaData::isValid() const
{
    return gMHomeAppletMetaDataStub->isValid();
}

QString MHomeAppletMetaData::runnerBinary() const
{
    return gMHomeAppletMetaDataStub->runnerBinary();
}

QString MHomeAppletMetaData::appletBinary() const
{
    return gMHomeAppletMetaDataStub->appletBinary();
}


#endif
