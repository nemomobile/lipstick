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
#ifndef MAPPLETINSTANCEMANAGER_STUB
#define MAPPLETINSTANCEMANAGER_STUB

#include "mappletinstancemanager.h"
#include "mappletid_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletInstanceManagerStub : public StubBase
{
public:
    virtual void MAppletInstanceManagerConstructor(const QString &identifier, MDataStore *dataStore);
    virtual void MAppletInstanceManagerDestructor();
    virtual bool restoreApplets();
    virtual bool instantiateApplet(const QString &metadataFile);
    virtual void instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData);
    virtual void appletInstantiated(MWidget *widget, MDataStore &store) const;
    virtual void appletRemoved(MWidget *widget);
    virtual void removeActionTriggered(bool checked);
    virtual void appletUninstalled(const QString &desktopFile);
    virtual bool removeApplet(MAppletId appletId);
    virtual void MAppletInstanceManagerConstructor(MAppletInstanceManagerPrivate &dd, const QString &identifier, MDataStore *dataStore);
};

// 2. IMPLEMENT STUB
void MAppletInstanceManagerStub::MAppletInstanceManagerConstructor(const QString &identifier, MDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(identifier));
    params.append(new Parameter<MDataStore * >(dataStore));
    stubMethodEntered("constructor", params);
}
void MAppletInstanceManagerStub::MAppletInstanceManagerDestructor()
{

}
bool MAppletInstanceManagerStub::restoreApplets()
{
    stubMethodEntered("restoreApplets");
    return stubReturnValue<bool>("restoreApplets");
}

bool MAppletInstanceManagerStub::instantiateApplet(const QString &metadataFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(metadataFile));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<bool>("instantiateApplet");
}

void MAppletInstanceManagerStub::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    params.append(new Parameter<const QMap<QString, QVariant> & >(metaData));
    stubMethodEntered("instantiateAppletFromPackage", params);
}

void MAppletInstanceManagerStub::appletInstantiated(MWidget *widget, MDataStore &store) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    params.append(new Parameter<MDataStore & >(store));
    stubMethodEntered("appletInstantiated", params);
}

void MAppletInstanceManagerStub::appletRemoved(MWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    stubMethodEntered("appletRemoved", params);
}

void MAppletInstanceManagerStub::removeActionTriggered(bool checked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(checked));
    stubMethodEntered("removeActionTriggered", params);
}

void MAppletInstanceManagerStub::appletUninstalled(const QString &desktopFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(desktopFile));
    stubMethodEntered("appletUninstalled", params);
}

bool MAppletInstanceManagerStub::removeApplet(MAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletId >(appletId));
    stubMethodEntered("removeApplet", params);
    return stubReturnValue<bool>("removeApplet");
}

void MAppletInstanceManagerStub::MAppletInstanceManagerConstructor(MAppletInstanceManagerPrivate &dd, const QString &identifier, MDataStore *dataStore)
{
    Q_UNUSED(dd);
    Q_UNUSED(identifier);
    Q_UNUSED(dataStore);

}


// 3. CREATE A STUB INSTANCE
MAppletInstanceManagerStub gDefaultMAppletInstanceManagerStub;
MAppletInstanceManagerStub *gMAppletInstanceManagerStub = &gDefaultMAppletInstanceManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAppletInstanceManager::MAppletInstanceManager(const QString &identifier, MDataStore *dataStore) : d_ptr(NULL)
{
    gMAppletInstanceManagerStub->MAppletInstanceManagerConstructor(identifier, dataStore);
}

MAppletInstanceManager::~MAppletInstanceManager()
{
    gMAppletInstanceManagerStub->MAppletInstanceManagerDestructor();
}

bool MAppletInstanceManager::restoreApplets()
{
    return gMAppletInstanceManagerStub->restoreApplets();
}

bool MAppletInstanceManager::instantiateApplet(const QString &metadataFile)
{
    return gMAppletInstanceManagerStub->instantiateApplet(metadataFile);
}

void MAppletInstanceManager::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    gMAppletInstanceManagerStub->instantiateAppletFromPackage(packageName, metaData);
}

void MAppletInstanceManager::removeActionTriggered(bool checked)
{
    gMAppletInstanceManagerStub->removeActionTriggered(checked);
}

void MAppletInstanceManager::appletUninstalled(const QString &desktopFile)
{
    gMAppletInstanceManagerStub->appletUninstalled(desktopFile);
}

bool MAppletInstanceManager::removeApplet(MAppletId appletId)
{
    return gMAppletInstanceManagerStub->removeApplet(appletId);
}

MAppletInstanceManager::MAppletInstanceManager(MAppletInstanceManagerPrivate &dd, const QString &identifier, MDataStore *dataStore) : d_ptr(&dd)
{
    gMAppletInstanceManagerStub->MAppletInstanceManagerConstructor(dd, identifier, dataStore);
}


#endif
