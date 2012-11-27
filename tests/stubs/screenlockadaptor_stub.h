/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
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
#ifndef SCREENLOCKADAPTOR_STUB
#define SCREENLOCKADAPTOR_STUB

#include "screenlockadaptor.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ScreenLockAdaptorStub : public StubBase {
  public:
  virtual void ScreenLockAdaptorConstructor(ScreenLock *parent);
  virtual void ScreenLockAdaptorDestructor();
  virtual int tklock_close(bool silent);
  virtual int tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker);
}; 

// 2. IMPLEMENT STUB
void ScreenLockAdaptorStub::ScreenLockAdaptorConstructor(ScreenLock *parent) {
  Q_UNUSED(parent);

}
void ScreenLockAdaptorStub::ScreenLockAdaptorDestructor() {

}
int ScreenLockAdaptorStub::tklock_close(bool silent) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(silent));
  stubMethodEntered("tklock_close",params);
  return stubReturnValue<int>("tklock_close");
}

int ScreenLockAdaptorStub::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(service));
  params.append( new Parameter<const QString & >(path));
  params.append( new Parameter<const QString & >(interface));
  params.append( new Parameter<const QString & >(method));
  params.append( new Parameter<uint >(mode));
  params.append( new Parameter<bool >(silent));
  params.append( new Parameter<bool >(flicker));
  stubMethodEntered("tklock_open",params);
  return stubReturnValue<int>("tklock_open");
}



// 3. CREATE A STUB INSTANCE
ScreenLockAdaptorStub gDefaultScreenLockAdaptorStub;
ScreenLockAdaptorStub* gScreenLockAdaptorStub = &gDefaultScreenLockAdaptorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ScreenLockAdaptor::ScreenLockAdaptor(ScreenLock *parent) : QDBusAbstractAdaptor(parent) {
  gScreenLockAdaptorStub->ScreenLockAdaptorConstructor(parent);
}

ScreenLockAdaptor::~ScreenLockAdaptor() {
  gScreenLockAdaptorStub->ScreenLockAdaptorDestructor();
}

int ScreenLockAdaptor::tklock_close(bool silent) {
  return gScreenLockAdaptorStub->tklock_close(silent);
}

int ScreenLockAdaptor::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
  return gScreenLockAdaptorStub->tklock_open(service, path, interface, method, mode, silent, flicker);
}


#endif
