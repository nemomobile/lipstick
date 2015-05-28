/***************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: Matt Vogt <matthew.vogt@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef ANDROIDPRIORITYSTORE_STUB
#define ANDROIDPRIORITYSTORE_STUB

#include "androidprioritystore.h"
#include <stubbase.h>


// 1. DECLARE STUB
class AndroidPriorityStoreStub : public StubBase {
  public:
  virtual void AndroidPriorityStoreStubConstructor(const QString &path, QObject *parent);
  virtual AndroidPriorityStore::PriorityDetails appDetails(const QString &appName) const;
};

// 2. IMPLEMENT STUB
void AndroidPriorityStoreStub::AndroidPriorityStoreStubConstructor(const QString &path, QObject *parent) {
  Q_UNUSED(path);
  Q_UNUSED(parent);
}

AndroidPriorityStore::PriorityDetails AndroidPriorityStoreStub::appDetails(const QString &appName) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(appName));
  stubMethodEntered("appDetails",params);
  return stubReturnValue<AndroidPriorityStore::PriorityDetails >("appDetails");
}

// 3. CREATE A STUB INSTANCE
AndroidPriorityStoreStub gDefaultAndroidPriorityStoreStub;
AndroidPriorityStoreStub* gAndroidPriorityStoreStub = &gDefaultAndroidPriorityStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
AndroidPriorityStore::AndroidPriorityStore(const QString &path, QObject *parent) {
  gAndroidPriorityStoreStub->AndroidPriorityStoreStubConstructor(path, parent);
}

AndroidPriorityStore::PriorityDetails AndroidPriorityStore::appDetails(const QString &appName) const {
  return gAndroidPriorityStoreStub->appDetails(appName);
}

#endif
