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
#ifndef EVENTEATERUI_STUB
#define EVENTEATERUI_STUB

#include "eventeater.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class EventEaterStub : public StubBase {
  public:
  virtual void EventEaterConstructor(QObject *);
  virtual void EventEaterDestructor();
  virtual void show();
  virtual void hide();
  virtual bool handleXEvent(const XEvent &event);
}; 

// 2. IMPLEMENT STUB
void EventEaterStub::EventEaterConstructor(QObject *) {
}
void EventEaterStub::EventEaterDestructor() {
}

void EventEaterStub::show() {
  stubMethodEntered("show");
}

void EventEaterStub::hide() {
  stubMethodEntered("hide");
}

bool EventEaterStub::handleXEvent(const XEvent &event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<XEvent>(event));
  stubMethodEntered("eventFilter",params);
  return stubReturnValue<XEvent *>("eventFilter");
}

// 3. CREATE A STUB INSTANCE
EventEaterStub gDefaultEventEaterStub;
EventEaterStub* gEventEaterStub = &gDefaultEventEaterStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
EventEater::EventEater(QObject *parent) {
  gEventEaterStub->EventEaterConstructor(parent);
}
EventEater::~EventEater() {
  gEventEaterStub->EventEaterDestructor();
}

void EventEater::show() {
  gEventEaterStub->show();
}

void EventEater::hide() {
  gEventEaterStub->hide();
}

bool EventEater::handleXEvent(const XEvent &event) {
    return gEventEaterStub->handleXEvent(event);
}

#endif
