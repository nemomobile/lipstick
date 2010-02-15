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
#ifndef DUIAPPLETINSTANCEMANAGER_STUB
#define DUIAPPLETINSTANCEMANAGER_STUB

#include "duiappletinstancemanager.h"
#include "duiappletid_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletInstanceManagerStub : public StubBase
{
public:
    virtual void DuiAppletInstanceManagerConstructor(const QString &identifier, DuiDataStore *dataStore);
    virtual void DuiAppletInstanceManagerDestructor();
    virtual bool restoreApplets();
    virtual bool instantiateApplet(const QString &metadataFile);
    virtual void instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData);
    virtual void appletInstantiated(DuiWidget *widget, DuiDataStore &store) const;
    virtual void appletRemoved(DuiWidget *widget);
    virtual void removeActionTriggered(bool checked);
    virtual void appletUninstalled(const QString &desktopFile);
    virtual bool removeApplet(DuiAppletId appletId);
    virtual void DuiAppletInstanceManagerConstructor(DuiAppletInstanceManagerPrivate &dd, const QString &identifier, DuiDataStore *dataStore);
};

// 2. IMPLEMENT STUB
void DuiAppletInstanceManagerStub::DuiAppletInstanceManagerConstructor(const QString &identifier, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(identifier));
    params.append(new Parameter<DuiDataStore * >(dataStore));
    stubMethodEntered("constructor", params);
}
void DuiAppletInstanceManagerStub::DuiAppletInstanceManagerDestructor()
{

}
bool DuiAppletInstanceManagerStub::restoreApplets()
{
    stubMethodEntered("restoreApplets");
    return stubReturnValue<bool>("restoreApplets");
}

bool DuiAppletInstanceManagerStub::instantiateApplet(const QString &metadataFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(metadataFile));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<bool>("instantiateApplet");
}

void DuiAppletInstanceManagerStub::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    params.append(new Parameter<const QMap<QString, QVariant> & >(metaData));
    stubMethodEntered("instantiateAppletFromPackage", params);
}

void DuiAppletInstanceManagerStub::appletInstantiated(DuiWidget *widget, DuiDataStore &store) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    params.append(new Parameter<DuiDataStore & >(store));
    stubMethodEntered("appletInstantiated", params);
}

void DuiAppletInstanceManagerStub::appletRemoved(DuiWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    stubMethodEntered("appletRemoved", params);
}

void DuiAppletInstanceManagerStub::removeActionTriggered(bool checked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(checked));
    stubMethodEntered("removeActionTriggered", params);
}

void DuiAppletInstanceManagerStub::appletUninstalled(const QString &desktopFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(desktopFile));
    stubMethodEntered("appletUninstalled", params);
}

bool DuiAppletInstanceManagerStub::removeApplet(DuiAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId >(appletId));
    stubMethodEntered("removeApplet", params);
    return stubReturnValue<bool>("removeApplet");
}

void DuiAppletInstanceManagerStub::DuiAppletInstanceManagerConstructor(DuiAppletInstanceManagerPrivate &dd, const QString &identifier, DuiDataStore *dataStore)
{
    Q_UNUSED(dd);
    Q_UNUSED(identifier);
    Q_UNUSED(dataStore);

}


// 3. CREATE A STUB INSTANCE
DuiAppletInstanceManagerStub gDefaultDuiAppletInstanceManagerStub;
DuiAppletInstanceManagerStub *gDuiAppletInstanceManagerStub = &gDefaultDuiAppletInstanceManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletInstanceManager::DuiAppletInstanceManager(const QString &identifier, DuiDataStore *dataStore) : d_ptr(NULL)
{
    gDuiAppletInstanceManagerStub->DuiAppletInstanceManagerConstructor(identifier, dataStore);
}

DuiAppletInstanceManager::~DuiAppletInstanceManager()
{
    gDuiAppletInstanceManagerStub->DuiAppletInstanceManagerDestructor();
}

bool DuiAppletInstanceManager::restoreApplets()
{
    return gDuiAppletInstanceManagerStub->restoreApplets();
}

bool DuiAppletInstanceManager::instantiateApplet(const QString &metadataFile)
{
    return gDuiAppletInstanceManagerStub->instantiateApplet(metadataFile);
}

void DuiAppletInstanceManager::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    gDuiAppletInstanceManagerStub->instantiateAppletFromPackage(packageName, metaData);
}

void DuiAppletInstanceManager::removeActionTriggered(bool checked)
{
    gDuiAppletInstanceManagerStub->removeActionTriggered(checked);
}

void DuiAppletInstanceManager::appletUninstalled(const QString &desktopFile)
{
    gDuiAppletInstanceManagerStub->appletUninstalled(desktopFile);
}

bool DuiAppletInstanceManager::removeApplet(DuiAppletId appletId)
{
    return gDuiAppletInstanceManagerStub->removeApplet(appletId);
}

DuiAppletInstanceManager::DuiAppletInstanceManager(DuiAppletInstanceManagerPrivate &dd, const QString &identifier, DuiDataStore *dataStore) : d_ptr(&dd)
{
    gDuiAppletInstanceManagerStub->DuiAppletInstanceManagerConstructor(dd, identifier, dataStore);
}


#endif
