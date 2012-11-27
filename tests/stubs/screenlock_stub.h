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
#ifndef SCREENLOCK_STUB
#define SCREENLOCK_STUB

#include "screenlock.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ScreenLockStub : public StubBase {
  public:
  virtual void ScreenLockConstructor(QObject *parent);
  virtual void ScreenLockDestructor();
  virtual int tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker);
  virtual int tklock_close(bool silent);
  virtual void toggleScreenLockUI(bool toggle);
  virtual void toggleEventEater(bool toggle);
  virtual void lockScreen();
  virtual void unlockScreen();
  virtual void showScreenLock();
  virtual void showLowPowerMode();
  virtual void setDisplayOffMode();
  virtual void hideScreenLockAndEventEater();
  virtual void showEventEater();
  virtual void hideEventEater();
  virtual bool isScreenLocked();
#ifdef HAVE_QMSYSTEM
  virtual void systemStateChanged(MeeGo::QmSystemState::StateIndication what);
#endif
}; 

// 2. IMPLEMENT STUB
void ScreenLockStub::ScreenLockConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void ScreenLockStub::ScreenLockDestructor() {

}
int ScreenLockStub::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
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

int ScreenLockStub::tklock_close(bool silent) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(silent));
  stubMethodEntered("tklock_close",params);
  return stubReturnValue<int>("tklock_close");
}

void ScreenLockStub::toggleScreenLockUI(bool toggle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(toggle));
  stubMethodEntered("toggleScreenLockUI",params);
}

void ScreenLockStub::toggleEventEater(bool toggle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(toggle));
  stubMethodEntered("toggleEventEater",params);
}

void ScreenLockStub::lockScreen() {
  stubMethodEntered("lockScreen");
}

void ScreenLockStub::unlockScreen() {
  stubMethodEntered("unlockScreen");
}

void ScreenLockStub::showScreenLock() {
  stubMethodEntered("showScreenLock");
}

void ScreenLockStub::showLowPowerMode() {
  stubMethodEntered("showLowPowerMode");
}

void ScreenLockStub::setDisplayOffMode() {
  stubMethodEntered("setDisplayOffMode");
}

void ScreenLockStub::hideScreenLockAndEventEater() {
  stubMethodEntered("hideScreenLockAndEventEater");
}

void ScreenLockStub::showEventEater() {
  stubMethodEntered("showEventEater");
}

void ScreenLockStub::hideEventEater() {
  stubMethodEntered("hideEventEater");
}

bool ScreenLockStub::isScreenLocked() {
  stubMethodEntered("isScreenLocked");
  return stubReturnValue<bool>("isScreenLocked");
}

#ifdef HAVE_QMSYSTEM
void ScreenLockStub::systemStateChanged(MeeGo::QmSystemState::StateIndication what) {
    QList<ParameterBase*> params;
    params.append( new Parameter<MeeGo::QmSystemState::StateIndication >(what));
    stubMethodEntered("systemStateChanged",params);
}
#endif



// 3. CREATE A STUB INSTANCE
ScreenLockStub gDefaultScreenLockStub;
ScreenLockStub* gScreenLockStub = &gDefaultScreenLockStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ScreenLock::ScreenLock(QObject *parent) {
  gScreenLockStub->ScreenLockConstructor(parent);
}

ScreenLock::~ScreenLock() {
  gScreenLockStub->ScreenLockDestructor();
}

int ScreenLock::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool silent, bool flicker) {
  return gScreenLockStub->tklock_open(service, path, interface, method, mode, silent, flicker);
}

int ScreenLock::tklock_close(bool silent) {
  return gScreenLockStub->tklock_close(silent);
}

void ScreenLock::toggleScreenLockUI(bool toggle) {
  gScreenLockStub->toggleScreenLockUI(toggle);
}

void ScreenLock::toggleEventEater(bool toggle) {
  gScreenLockStub->toggleEventEater(toggle);
}

void ScreenLock::lockScreen() {
  gScreenLockStub->lockScreen();
}

void ScreenLock::unlockScreen() {
  gScreenLockStub->unlockScreen();
}

void ScreenLock::showScreenLock() {
  gScreenLockStub->showScreenLock();
}

void ScreenLock::showLowPowerMode() {
  gScreenLockStub->showLowPowerMode();
}

void ScreenLock::setDisplayOffMode() {
  gScreenLockStub->setDisplayOffMode();
}

void ScreenLock::hideScreenLockAndEventEater() {
  gScreenLockStub->hideScreenLockAndEventEater();
}

void ScreenLock::showEventEater() {
  gScreenLockStub->showEventEater();
}

void ScreenLock::hideEventEater() {
  gScreenLockStub->hideEventEater();
}

bool ScreenLock::isScreenLocked() const {
  return gScreenLockStub->isScreenLocked();
}

#ifdef HAVE_QMSYSTEM
void ScreenLock::systemStateChanged(MeeGo::QmSystemState::StateIndication what) {
  gScreenLockStub->systemStateChanged(what);
}
#endif


#endif
