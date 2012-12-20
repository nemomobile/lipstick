/***************************************************************************
**
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
#ifndef PULSEAUDIOCONTROL_STUB
#define PULSEAUDIOCONTROL_STUB

#include "pulseaudiocontrol.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class PulseAudioControlStub : public StubBase {
  public:
  virtual void PulseAudioControlConstructor(QObject *parent);
  virtual void PulseAudioControlDestructor();
  virtual void update();
  virtual void setVolume(int volume);
  virtual void openConnection();
  virtual void setSteps(quint32 currentStep, quint32 stepCount);
  virtual void addSignalMatch();
  virtual DBusHandlerResult stepsUpdatedSignalHandler(DBusConnection *conn, DBusMessage *message, void *control);
   DBusConnection *dbusConnection ;
}; 

// 2. IMPLEMENT STUB
void PulseAudioControlStub::PulseAudioControlConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void PulseAudioControlStub::PulseAudioControlDestructor() {

}
void PulseAudioControlStub::update() {
  stubMethodEntered("update");
}

void PulseAudioControlStub::setVolume(int volume) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(volume));
  stubMethodEntered("setVolume",params);
}

void PulseAudioControlStub::openConnection() {
  stubMethodEntered("openConnection");
}

void PulseAudioControlStub::setSteps(quint32 currentStep, quint32 stepCount) {
  QList<ParameterBase*> params;
  params.append( new Parameter<quint32 >(currentStep));
  params.append( new Parameter<quint32 >(stepCount));
  stubMethodEntered("setSteps",params);
}

void PulseAudioControlStub::addSignalMatch() {
  stubMethodEntered("addSignalMatch");
}

DBusHandlerResult PulseAudioControlStub::stepsUpdatedSignalHandler(DBusConnection *conn, DBusMessage *message, void *control) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DBusConnection * >(conn));
  params.append( new Parameter<DBusMessage * >(message));
  params.append( new Parameter<void * >(control));
  stubMethodEntered("stepsUpdatedSignalHandler",params);
  return stubReturnValue<DBusHandlerResult>("stepsUpdatedSignalHandler");
}



// 3. CREATE A STUB INSTANCE
PulseAudioControlStub gDefaultPulseAudioControlStub;
PulseAudioControlStub* gPulseAudioControlStub = &gDefaultPulseAudioControlStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
PulseAudioControl::PulseAudioControl(QObject *parent) {
  gPulseAudioControlStub->PulseAudioControlConstructor(parent);
}

PulseAudioControl::~PulseAudioControl() {
  gPulseAudioControlStub->PulseAudioControlDestructor();
}

void PulseAudioControl::update() {
  gPulseAudioControlStub->update();
}

void PulseAudioControl::setVolume(int volume) {
  gPulseAudioControlStub->setVolume(volume);
}

void PulseAudioControl::openConnection() {
  gPulseAudioControlStub->openConnection();
}

void PulseAudioControl::setSteps(quint32 currentStep, quint32 stepCount) {
  gPulseAudioControlStub->setSteps(currentStep, stepCount);
}

void PulseAudioControl::addSignalMatch() {
  gPulseAudioControlStub->addSignalMatch();
}

DBusHandlerResult PulseAudioControl::stepsUpdatedSignalHandler(DBusConnection *conn, DBusMessage *message, void *control) {
  return gPulseAudioControlStub->stepsUpdatedSignalHandler(conn, message, control);
}


#endif
