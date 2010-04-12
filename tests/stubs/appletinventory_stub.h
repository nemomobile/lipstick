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
    virtual AppletInventory::InitializationError start(MDynamicContainer *containerWidget);
    virtual MWidget *instantiateApplet(const MHomeAppletMetaData &metadata);
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

AppletInventory::InitializationError AppletInventoryStub::start(MDynamicContainer *containerWidget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MDynamicContainer *>(containerWidget));
    stubMethodEntered("start", params);
    return stubReturnValue<AppletInventory::InitializationError>("start");
}

MWidget *AppletInventoryStub::instantiateApplet(const MHomeAppletMetaData &metadata)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MHomeAppletMetaData & >(metadata));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<MWidget *>("instantiateApplet");
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
AppletInventory::AppletInventory(MWidget *parent)
    : MDynamicContainer(Qt::Vertical, parent)
{
    gAppletInventoryStub->AppletInventoryConstructor();
}

AppletInventory::~AppletInventory()
{
    gAppletInventoryStub->AppletInventoryDestructor();
}

AppletInventory::InitializationError AppletInventory::start(MDynamicContainer *containerWidget)
{
    return gAppletInventoryStub->start(containerWidget);
}

MWidget *AppletInventory::instantiateApplet(const MHomeAppletMetaData &metadata)
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
