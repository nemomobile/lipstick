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
#ifndef APPLETINVENTORY_STUB
#define APPLETINVENTORY_STUB

#include "appletinventory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class AppletInventoryStub : public StubBase
{
public:
    virtual void AppletInventoryConstructor();
    virtual void AppletInventoryDestructor();
    virtual AppletInventory::InitializationError start(DuiDynamicContainer *containerWidget);
    virtual DuiWidget *instantiateApplet(const DuiHomeAppletMetaData &metadata);
    virtual void appletPathChanged(const QString &path);
    virtual void appletButtonClicked();
    virtual uint getUniqueAppletId();
    virtual void freeAppletId(uint id);
    QQueue<uint> freeAppletIds ;
    uint nextFreeAppletId ;
    QFileSystemWatcher *_watcher ;
    QDir _appletPath ;
    QStringList _availableApplets ;
};

// 2. IMPLEMENT STUB
void AppletInventoryStub::AppletInventoryConstructor()
{

}
void AppletInventoryStub::AppletInventoryDestructor()
{

}

AppletInventory::InitializationError AppletInventoryStub::start(DuiDynamicContainer *containerWidget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiDynamicContainer *>(containerWidget));
    stubMethodEntered("start", params);
    return stubReturnValue<AppletInventory::InitializationError>("start");
}

DuiWidget *AppletInventoryStub::instantiateApplet(const DuiHomeAppletMetaData &metadata)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiHomeAppletMetaData & >(metadata));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<DuiWidget *>("instantiateApplet");
}

void AppletInventoryStub::appletPathChanged(const QString &path)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(path));
    stubMethodEntered("appletPathChanged", params);
}

void AppletInventoryStub::appletButtonClicked()
{
    stubMethodEntered("appletButtonClicked");
}

uint AppletInventoryStub::getUniqueAppletId()
{
    stubMethodEntered("getUniqueAppletId");
    return stubReturnValue<uint>("getUniqueAppletId");
}

void AppletInventoryStub::freeAppletId(uint id)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(id));
    stubMethodEntered("freeAppletId", params);
}



// 3. CREATE A STUB INSTANCE
AppletInventoryStub gDefaultAppletInventoryStub;
AppletInventoryStub *gAppletInventoryStub = &gDefaultAppletInventoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
AppletInventory::AppletInventory(DuiWidget *parent)
    : DuiDynamicContainer(Qt::Vertical, parent)
{
    gAppletInventoryStub->AppletInventoryConstructor();
}

AppletInventory::~AppletInventory()
{
    gAppletInventoryStub->AppletInventoryDestructor();
}

AppletInventory::InitializationError AppletInventory::start(DuiDynamicContainer *containerWidget)
{
    return gAppletInventoryStub->start(containerWidget);
}

DuiWidget *AppletInventory::instantiateApplet(const DuiHomeAppletMetaData &metadata)
{
    return gAppletInventoryStub->instantiateApplet(metadata);
}

void AppletInventory::appletPathChanged(const QString &path)
{
    gAppletInventoryStub->appletPathChanged(path);
}

void AppletInventory::appletButtonClicked()
{
    gAppletInventoryStub->appletButtonClicked();
}

uint AppletInventory::getUniqueAppletId()
{
    return gAppletInventoryStub->getUniqueAppletId();
}

void AppletInventory::freeAppletId(uint id)
{
    gAppletInventoryStub->freeAppletId(id);
}


#endif
