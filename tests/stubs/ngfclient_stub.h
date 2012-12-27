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
#ifndef CLIENT_STUB
#define CLIENT_STUB

#include "ngfclient.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ClientStub : public StubBase {
  public:
  virtual void ClientConstructor(QObject *parent);
  virtual void ClientDestructor();
  virtual bool connect();
  virtual bool isConnected();
  virtual void disconnect();
  virtual quint32 play(const QString &event);
  virtual quint32 play(const QString &event, const QMap<QString, QVariant> &properties);
  virtual bool pause(const quint32 &event_id);
  virtual bool pause(const QString &event);
  virtual bool resume(const quint32 &event_id);
  virtual bool resume(const QString &event);
  virtual bool stop(const quint32 &event_id);
  virtual bool stop(const QString &event);
}; 

// 2. IMPLEMENT STUB
void ClientStub::ClientConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void ClientStub::ClientDestructor() {

}
bool ClientStub::connect() {
  stubMethodEntered("connect");
  return stubReturnValue<bool>("connect");
}

bool ClientStub::isConnected() {
  stubMethodEntered("isConnected");
  return stubReturnValue<bool>("isConnected");
}

void ClientStub::disconnect() {
  stubMethodEntered("disconnect");
}

quint32 ClientStub::play(const QString &event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(event));
  stubMethodEntered("play",params);
  return stubReturnValue<quint32>("play");
}

quint32 ClientStub::play(const QString &event, const QMap<QString, QVariant> &properties) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(event));
  params.append( new Parameter<QMap<QString, QVariant> >(properties));
  stubMethodEntered("play",params);
  return stubReturnValue<quint32>("play");
}

bool ClientStub::pause(const quint32 &event_id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const quint32 & >(event_id));
  stubMethodEntered("pause",params);
  return stubReturnValue<bool>("pause");
}

bool ClientStub::pause(const QString &event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(event));
  stubMethodEntered("pause",params);
  return stubReturnValue<bool>("pause");
}

bool ClientStub::resume(const quint32 &event_id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const quint32 & >(event_id));
  stubMethodEntered("resume",params);
  return stubReturnValue<bool>("resume");
}

bool ClientStub::resume(const QString &event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(event));
  stubMethodEntered("resume",params);
  return stubReturnValue<bool>("resume");
}

bool ClientStub::stop(const quint32 &event_id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<quint32 >(event_id));
  stubMethodEntered("stop",params);
  return stubReturnValue<bool>("stop");
}

bool ClientStub::stop(const QString &event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(event));
  stubMethodEntered("stop",params);
  return stubReturnValue<bool>("stop");
}



// 3. CREATE A STUB INSTANCE
ClientStub gDefaultClientStub;
ClientStub* gClientStub = &gDefaultClientStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Ngf::Client::Client(QObject *parent) : QObject(parent), d_ptr(0) {
  gClientStub->ClientConstructor(parent);
}

Ngf::Client::~Client() {
  gClientStub->ClientDestructor();
}

bool Ngf::Client::connect() {
  return gClientStub->connect();
}

bool Ngf::Client::isConnected() {
  return gClientStub->isConnected();
}

void Ngf::Client::disconnect() {
  gClientStub->disconnect();
}

quint32 Ngf::Client::play(const QString &event) {
  return gClientStub->play(event);
}

quint32 Ngf::Client::play(const QString &event, const QMap<QString, QVariant> &properties) {
  return gClientStub->play(event, properties);
}

bool Ngf::Client::pause(const quint32 &event_id) {
  return gClientStub->pause(event_id);
}

bool Ngf::Client::pause(const QString &event) {
  return gClientStub->pause(event);
}

bool Ngf::Client::resume(const quint32 &event_id) {
  return gClientStub->resume(event_id);
}

bool Ngf::Client::resume(const QString &event) {
  return gClientStub->resume(event);
}

bool Ngf::Client::stop(const quint32 &event_id) {
  return gClientStub->stop(event_id);
}

bool Ngf::Client::stop(const QString &event) {
  return gClientStub->stop(event);
}


#endif
