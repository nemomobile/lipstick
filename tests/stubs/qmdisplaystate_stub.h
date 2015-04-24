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
#ifndef QMDISPLAYSTATE_STUB
#define QMDISPLAYSTATE_STUB

#include <qmdisplaystate.h>
#include <stubbase.h>


/*
 * XXX: This stub contains only those methods which are
 * used by the  DeviceModeBusinessLogic class ...
 */


/** DECLARE STUB */
class QmDisplayStateStub : public StubBase {
  public:
  // enum DisplayState { Off=-1, Dimmed=0, On=1, Unknown } ;
  virtual void QmDisplayStateConstructor(QObject *parent);
  virtual void QmDisplayStateDestructor();
  virtual MeeGo::QmDisplayState::DisplayState get() const;
  virtual bool set(MeeGo::QmDisplayState::DisplayState state);
  virtual void connectNotify(const QMetaMethod &signal);
  virtual void disconnectNotify(const QMetaMethod &signal);
}; 

// 2. IMPLEMENT STUB
void QmDisplayStateStub::QmDisplayStateConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void QmDisplayStateStub::QmDisplayStateDestructor() {

}
MeeGo::QmDisplayState::DisplayState QmDisplayStateStub::get() const {
  stubMethodEntered("get");
  return stubReturnValue<MeeGo::QmDisplayState::DisplayState>("get");
}

bool QmDisplayStateStub::set(MeeGo::QmDisplayState::DisplayState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmDisplayState::DisplayState >(state));
  stubMethodEntered("set",params);
  return stubReturnValue<bool>("set");
}

void QmDisplayStateStub::connectNotify(const QMetaMethod &signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod &>(signal));
  stubMethodEntered("connectNotify",params);
}


void QmDisplayStateStub::disconnectNotify(const QMetaMethod &signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod &>(signal));
  stubMethodEntered("disconnectNotify",params);
}






// 3. CREATE A STUB INSTANCE
QmDisplayStateStub gDefaultQmDisplayStateStub;
QmDisplayStateStub* gQmDisplayStateStub = &gDefaultQmDisplayStateStub;

namespace MeeGo {

// 4. CREATE A PROXY WHICH CALLS THE STUB
QmDisplayState::QmDisplayState(QObject *parent) {
  gQmDisplayStateStub->QmDisplayStateConstructor(parent);
}

QmDisplayState::~QmDisplayState() {
  gQmDisplayStateStub->QmDisplayStateDestructor();
}

MeeGo::QmDisplayState::DisplayState QmDisplayState::get() const {
  return gQmDisplayStateStub->get();
}

bool QmDisplayState::set(DisplayState state) {
  return gQmDisplayStateStub->set(state);
}

void QmDisplayState::connectNotify(const QMetaMethod &signal)
{
	gQmDisplayStateStub->connectNotify(signal);
}
void QmDisplayState::disconnectNotify(const QMetaMethod & signal)
{
    gQmDisplayStateStub->disconnectNotify(signal);
}

} //Namespace meego

#endif
