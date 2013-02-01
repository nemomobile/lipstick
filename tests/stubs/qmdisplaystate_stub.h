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
  virtual bool setBlankingPause(void);
  virtual bool cancelBlankingPause(void);
  virtual int getMaxDisplayBrightnessValue();
  virtual int getDisplayBrightnessValue();
  virtual int getDisplayBlankTimeout();
  virtual int getDisplayDimTimeout();
  virtual bool getBlankingWhenCharging();
  virtual void setDisplayBrightnessValue(int brightness);
  virtual void setDisplayBlankTimeout(int timeout);
  virtual void setDisplayDimTimeout(int timeout);
  virtual void setBlankingWhenCharging(bool blanking);
  virtual void connectNotify(const char *signal);
  virtual void disconnectNotify(const char *signal);
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

bool QmDisplayStateStub::setBlankingPause(void) {
  stubMethodEntered("setBlankingPause");
  return stubReturnValue<bool>("setBlankingPause");
}

bool QmDisplayStateStub::cancelBlankingPause(void) {
  stubMethodEntered("cancelBlankingPause");
  return stubReturnValue<bool>("cancelBlankingPause");
}

int QmDisplayStateStub::getMaxDisplayBrightnessValue() {
  stubMethodEntered("getMaxDisplayBrightnessValue");
  return stubReturnValue<int>("getMaxDisplayBrightnessValue");
}

int QmDisplayStateStub::getDisplayBrightnessValue() {
  stubMethodEntered("getDisplayBrightnessValue");
  return stubReturnValue<int>("getDisplayBrightnessValue");
}

int QmDisplayStateStub::getDisplayBlankTimeout() {
  stubMethodEntered("getDisplayBlankTimeout");
  return stubReturnValue<int>("getDisplayBlankTimeout");
}

int QmDisplayStateStub::getDisplayDimTimeout() {
  stubMethodEntered("getDisplayDimTimeout");
  return stubReturnValue<int>("getDisplayDimTimeout");
}

bool QmDisplayStateStub::getBlankingWhenCharging() {
  stubMethodEntered("getBlankingWhenCharging");
  return stubReturnValue<bool>("getBlankingWhenCharging");
}

void QmDisplayStateStub::setDisplayBrightnessValue(int brightness) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(brightness));
  stubMethodEntered("setDisplayBrightnessValue",params);
}

void QmDisplayStateStub::setDisplayBlankTimeout(int timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(timeout));
  stubMethodEntered("setDisplayBlankTimeout",params);
}

void QmDisplayStateStub::setDisplayDimTimeout(int timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(timeout));
  stubMethodEntered("setDisplayDimTimeout",params);
}

void QmDisplayStateStub::setBlankingWhenCharging(bool blanking) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(blanking));
  stubMethodEntered("setBlankingWhenCharging",params);
}

void QmDisplayStateStub::connectNotify(const char *signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const char *>(signal));
  stubMethodEntered("connectNotify",params);
}


void QmDisplayStateStub::disconnectNotify(const char *signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const char *>(signal));
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

bool QmDisplayState::setBlankingPause(void) {
  return gQmDisplayStateStub->setBlankingPause();
}

bool QmDisplayState::cancelBlankingPause(void) {
  return gQmDisplayStateStub->cancelBlankingPause();
}

int QmDisplayState::getMaxDisplayBrightnessValue() {
  return gQmDisplayStateStub->getMaxDisplayBrightnessValue();
}

int QmDisplayState::getDisplayBrightnessValue() {
  return gQmDisplayStateStub->getDisplayBrightnessValue();
}

int QmDisplayState::getDisplayBlankTimeout() {
  return gQmDisplayStateStub->getDisplayBlankTimeout();
}

int QmDisplayState::getDisplayDimTimeout() {
  return gQmDisplayStateStub->getDisplayDimTimeout();
}

bool QmDisplayState::getBlankingWhenCharging() {
  return gQmDisplayStateStub->getBlankingWhenCharging();
}

void QmDisplayState::setDisplayBrightnessValue(int brightness) {
  gQmDisplayStateStub->setDisplayBrightnessValue(brightness);
}

void QmDisplayState::setDisplayBlankTimeout(int timeout) {
  gQmDisplayStateStub->setDisplayBlankTimeout(timeout);
}

void QmDisplayState::setDisplayDimTimeout(int timeout) {
  gQmDisplayStateStub->setDisplayDimTimeout(timeout);
}

void QmDisplayState::setBlankingWhenCharging(bool blanking) {
  gQmDisplayStateStub->setBlankingWhenCharging(blanking);
}

void QmDisplayState::connectNotify(const char *signal)
{
	gQmDisplayStateStub->connectNotify(signal);
}
void QmDisplayState::disconnectNotify(const char * signal)
{
    gQmDisplayStateStub->disconnectNotify(signal);
}

} //Namespace meego

#endif
