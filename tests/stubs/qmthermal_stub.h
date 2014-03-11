/***************************************************************************
**
** Copyright (C) 2012-2014 Jolla Ltd.
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
#ifndef QMTHERMAL_STUB
#define QMTHERMAL_STUB

#include "qmthermal.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QmThermalStub : public StubBase {
  public:
  virtual void QmThermalConstructor(QObject *parent);
  virtual void QmThermalDestructor();
  virtual MeeGo::QmThermal::ThermalState get() const;
  virtual void connectNotify(const QMetaMethod &signal);
  virtual void disconnectNotify(const QMetaMethod &signal);
}; 

// 2. IMPLEMENT STUB
void QmThermalStub::QmThermalConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void QmThermalStub::QmThermalDestructor() {

}
MeeGo::QmThermal::ThermalState QmThermalStub::get() const {
  stubMethodEntered("get");
  return stubReturnValue<MeeGo::QmThermal::ThermalState>("get");
}
void QmThermalStub::connectNotify(const QMetaMethod &signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod &>(signal));
  stubMethodEntered("connectNotify",params);
}
void QmThermalStub::disconnectNotify(const QMetaMethod &signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod &>(signal));
  stubMethodEntered("disconnectNotify",params);
}



// 3. CREATE A STUB INSTANCE
QmThermalStub gDefaultQmThermalStub;
QmThermalStub* gQmThermalStub = &gDefaultQmThermalStub;

namespace MeeGo {

// 4. CREATE A PROXY WHICH CALLS THE STUB
QmThermal::QmThermal(QObject *parent) {
  gQmThermalStub->QmThermalConstructor(parent);
}

QmThermal::~QmThermal() {
  gQmThermalStub->QmThermalDestructor();
}

MeeGo::QmThermal::ThermalState QmThermal::get() const {
  return gQmThermalStub->get();
}

void QmThermal::connectNotify(const QMetaMethod &signal)
{
        gQmThermalStub->connectNotify(signal);
}

void QmThermal::disconnectNotify(const QMetaMethod & signal)
{
    gQmThermalStub->disconnectNotify(signal);
}

}

#endif
