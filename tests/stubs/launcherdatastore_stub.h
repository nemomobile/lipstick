#ifndef LAUNCHERDATASTORE_STUB_H
#define LAUNCHERDATASTORE_STUB_H

#include <stubbase.h>
#include "launcherdatastore.h"


// 1. DECLARE STUB
class LauncherDataStoreStub : public StubBase
{
public:
    virtual void LauncherDataStoreConstructor(MDataStore* dataStore);
    virtual void LauncherDataStoreDestructor();
    virtual void updateLauncherButtons(const QList< QSharedPointer<LauncherPage> > &pages);
    virtual QList< QSharedPointer<LauncherPage> > launcherButtons();
    virtual QList<LauncherButton*> quickLaunchBarButtons();
    virtual LauncherDataStore::EntryLocation location(const MDesktopEntry &entry);
};

// 2. IMPLEMENT STUB

void LauncherDataStoreStub::LauncherDataStoreConstructor(MDataStore* dataStore)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<MDataStore*>(dataStore));
    stubMethodEntered("LauncherDataStoreConstructor", params);
}

void LauncherDataStoreStub::LauncherDataStoreDestructor()
{
    stubMethodEntered("LauncherDataStoreDestructor");
}

void LauncherDataStoreStub::updateLauncherButtons(const QList< QSharedPointer<LauncherPage> > &pages)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QList< QSharedPointer<LauncherPage> > >(pages));
    stubMethodEntered("updateLauncherButtons", params);
}

QList< QSharedPointer<LauncherPage> > LauncherDataStoreStub::launcherButtons()
{
    return stubReturnValue<QList< QSharedPointer<LauncherPage> > >("launcherButtons");
}

QList<LauncherButton*> LauncherDataStoreStub::quickLaunchBarButtons()
{
    return stubReturnValue<QList<LauncherButton*> >("quickLaunchBarButtons");
}

LauncherDataStore::EntryLocation LauncherDataStoreStub::location(const MDesktopEntry &entry)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const MDesktopEntry*>(&entry));
    stubMethodEntered("location", params);
    return stubReturnValue<LauncherDataStore::EntryLocation>("location");
}

// 3. CREATE A STUB INSTANCE
LauncherDataStoreStub gDefaultLauncherDataStoreStub;
LauncherDataStoreStub* gLauncherDataStoreStub = &gDefaultLauncherDataStoreStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB

LauncherDataStore::LauncherDataStore(MDataStore* dataStore)
{
    gLauncherDataStoreStub->LauncherDataStoreConstructor(dataStore);
}

LauncherDataStore::~LauncherDataStore()
{
    gLauncherDataStoreStub->LauncherDataStoreDestructor();
}

void LauncherDataStore::updateLauncherButtons(const QList< QSharedPointer<LauncherPage> > &pages)
{
    gLauncherDataStoreStub->updateLauncherButtons(pages);
}

QList< QSharedPointer<LauncherPage> > LauncherDataStore::launcherButtons()
{
    return gLauncherDataStoreStub->launcherButtons();
}

QList<LauncherButton*> LauncherDataStore::quickLaunchBarButtons()
{
    return gLauncherDataStoreStub->quickLaunchBarButtons();
}

LauncherDataStore::EntryLocation LauncherDataStore::location(const MDesktopEntry &entry)
{
    return gLauncherDataStoreStub->location(entry);
}

#endif // LAUNCHERDATASTORE_STUB_H
