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
#ifndef NOTIFICATIONMANAGERADAPTOR_STUB
#define NOTIFICATIONMANAGERADAPTOR_STUB

#include "notificationmanageradaptor.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationManagerAdaptorStub : public StubBase {
  public:
  virtual void NotificationManagerAdaptorConstructor(NotificationManager *parent);
  virtual void NotificationManagerAdaptorDestructor();
  virtual void CloseNotification(uint id);
  virtual QStringList GetCapabilities();
  virtual QString GetServerInformation(QString &name, QString &vendor, QString &version);
  virtual uint Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expire_timeout);
  virtual QList<Notification> GetNotifications(const QString &app_name);
};

// 2. IMPLEMENT STUB
void NotificationManagerAdaptorStub::NotificationManagerAdaptorConstructor(NotificationManager *parent) {
  Q_UNUSED(parent);

}
void NotificationManagerAdaptorStub::NotificationManagerAdaptorDestructor() {

}
void NotificationManagerAdaptorStub::CloseNotification(uint id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("CloseNotification",params);
}

QStringList NotificationManagerAdaptorStub::GetCapabilities() {
  stubMethodEntered("GetCapabilities");
  return stubReturnValue<QStringList>("GetCapabilities");
}

QString NotificationManagerAdaptorStub::GetServerInformation(QString &name, QString &vendor, QString &version) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString & >(name));
  params.append( new Parameter<QString & >(vendor));
  params.append( new Parameter<QString & >(version));
  stubMethodEntered("GetServerInformation",params);
  return stubReturnValue<QString>("GetServerInformation");
}

uint NotificationManagerAdaptorStub::Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expire_timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(app_name));
  params.append( new Parameter<uint >(replaces_id));
  params.append( new Parameter<const QString & >(app_icon));
  params.append( new Parameter<const QString & >(summary));
  params.append( new Parameter<const QString & >(body));
  params.append( new Parameter<const QStringList & >(actions));
  params.append( new Parameter<const QVariantHash & >(hints));
  params.append( new Parameter<int >(expire_timeout));
  stubMethodEntered("Notify",params);
  return stubReturnValue<uint>("Notify");
}

QList<Notification> NotificationManagerAdaptorStub::GetNotifications(const QString &app_name) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(app_name));
  stubMethodEntered("GetNotifications",params);
  return stubReturnValue<QList<Notification> >("GetNotifications");
}



// 3. CREATE A STUB INSTANCE
NotificationManagerAdaptorStub gDefaultNotificationManagerAdaptorStub;
NotificationManagerAdaptorStub* gNotificationManagerAdaptorStub = &gDefaultNotificationManagerAdaptorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationManagerAdaptor::NotificationManagerAdaptor(NotificationManager *parent) : QDBusAbstractAdaptor(parent) {
  gNotificationManagerAdaptorStub->NotificationManagerAdaptorConstructor(parent);
}

NotificationManagerAdaptor::~NotificationManagerAdaptor() {
  gNotificationManagerAdaptorStub->NotificationManagerAdaptorDestructor();
}

void NotificationManagerAdaptor::CloseNotification(uint id) {
  gNotificationManagerAdaptorStub->CloseNotification(id);
}

QStringList NotificationManagerAdaptor::GetCapabilities() {
  return gNotificationManagerAdaptorStub->GetCapabilities();
}

QString NotificationManagerAdaptor::GetServerInformation(QString &name, QString &vendor, QString &version) {
  return gNotificationManagerAdaptorStub->GetServerInformation(name, vendor, version);
}

uint NotificationManagerAdaptor::Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expire_timeout) {
  return gNotificationManagerAdaptorStub->Notify(app_name, replaces_id, app_icon, summary, body, actions, hints, expire_timeout);
}

QList<Notification> NotificationManagerAdaptor::GetNotifications(const QString &app_name) {
  return gNotificationManagerAdaptorStub->GetNotifications(app_name);
}


#endif
