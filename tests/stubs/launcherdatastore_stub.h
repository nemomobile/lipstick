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
#ifndef LAUNCHERDATASTORE_STUB
#define LAUNCHERDATASTORE_STUB

#include "launcherdatastore.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherDataStoreStub : public StubBase {
  public:
  virtual void LauncherDataStoreConstructor(MDataStore *dataStore);
  virtual void LauncherDataStoreDestructor();
  virtual QHash<QString, QVariant> dataForAllDesktopEntries();
  virtual bool updateDataForDesktopEntry(const QString &entryPath, const QVariant &data);
  virtual void updateDataFromDesktopEntryFiles();
  virtual void processUpdateQueue();
  virtual void startProcessingUpdateQueue();
  virtual bool isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes);
  virtual QString entryPathToKey(QString entryPath);
  virtual QString keyToEntryPath(QString key);
  virtual void updateDesktopEntry(const QString &desktopEntryPath);
  bool isInQueue(const QString &key);
};

// 2. IMPLEMENT STUB
void LauncherDataStoreStub::LauncherDataStoreConstructor(MDataStore *dataStore) {
  Q_UNUSED(dataStore);

}
void LauncherDataStoreStub::LauncherDataStoreDestructor() {

}
QHash<QString, QVariant> LauncherDataStoreStub::dataForAllDesktopEntries() {
  stubMethodEntered("dataForAllDesktopEntries");
  return stubReturnValue<QHash<QString, QVariant> >("dataForAllDesktopEntries");
}

bool LauncherDataStoreStub::updateDataForDesktopEntry(const QString &entryPath, const QVariant &data) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(entryPath));
  params.append( new Parameter<const QVariant & >(data));
  stubMethodEntered("updateDataForDesktopEntry",params);
  return stubReturnValue<bool>("updateDataForDesktopEntry");
}

void LauncherDataStoreStub::updateDataFromDesktopEntryFiles() {
  stubMethodEntered("updateDataFromDesktopEntryFiles");
}

void LauncherDataStoreStub::processUpdateQueue() {
  stubMethodEntered("processUpdateQueue");
}

void LauncherDataStoreStub::startProcessingUpdateQueue() {
  stubMethodEntered("startProcessingUpdateQueue");
}

bool LauncherDataStoreStub::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MDesktopEntry & >(entry));
  params.append( new Parameter<const QStringList & >(acceptedTypes));
  stubMethodEntered("isDesktopEntryValid",params);
  return stubReturnValue<bool>("isDesktopEntryValid");
}

QString LauncherDataStoreStub::entryPathToKey(QString entryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(entryPath));
  stubMethodEntered("entryPathToKey",params);
  return stubReturnValue<QString>("entryPathToKey");
}

QString LauncherDataStoreStub::keyToEntryPath(QString key) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(key));
  stubMethodEntered("keyToEntryPath",params);
  return stubReturnValue<QString>("keyToEntryPath");
}

void LauncherDataStoreStub::updateDesktopEntry(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString >(desktopEntryPath));
  stubMethodEntered("updateDesktopEntry",params);
}

bool LauncherDataStoreStub::isInQueue(const QString &key) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const QString >(key));
    stubMethodEntered("isInQueue",params);
}

// 3. CREATE A STUB INSTANCE
LauncherDataStoreStub gDefaultLauncherDataStoreStub;
LauncherDataStoreStub* gLauncherDataStoreStub = &gDefaultLauncherDataStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherDataStore::LauncherDataStore(MDataStore *dataStore) {
  gLauncherDataStoreStub->LauncherDataStoreConstructor(dataStore);
}

LauncherDataStore::~LauncherDataStore() {
  gLauncherDataStoreStub->LauncherDataStoreDestructor();
}

QHash<QString, QVariant> LauncherDataStore::dataForAllDesktopEntries() {
  return gLauncherDataStoreStub->dataForAllDesktopEntries();
}

bool LauncherDataStore::updateDataForDesktopEntry(const QString &entryPath, const QVariant &data) {
  return gLauncherDataStoreStub->updateDataForDesktopEntry(entryPath, data);
}

void LauncherDataStore::updateDataFromDesktopEntryFiles() {
  gLauncherDataStoreStub->updateDataFromDesktopEntryFiles();
}

void LauncherDataStore::processUpdateQueue() {
  gLauncherDataStoreStub->processUpdateQueue();
}

void LauncherDataStore::startProcessingUpdateQueue() {
  gLauncherDataStoreStub->startProcessingUpdateQueue();
}

bool LauncherDataStore::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes) {
  return gLauncherDataStoreStub->isDesktopEntryValid(entry, acceptedTypes);
}

QString LauncherDataStore::entryPathToKey(QString entryPath) {
  return gLauncherDataStoreStub->entryPathToKey(entryPath);
}

QString LauncherDataStore::keyToEntryPath(QString key) {
  return gLauncherDataStoreStub->keyToEntryPath(key);
}

void LauncherDataStore::updateDesktopEntry(const QString &desktopEntryPath) {
    gLauncherDataStoreStub->updateDesktopEntry(desktopEntryPath);
}

bool LauncherDataStore::isInQueue(const QString &key) {
    return gLauncherDataStoreStub->isInQueue(key);
}

#endif
