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
#ifndef NOTIFICATIONMANAGER_STUB
#define NOTIFICATIONMANAGER_STUB

#include "notificationmanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationManagerStub : public StubBase {
  public:
   enum NotificationClosedReason { NotificationExpired=1, NotificationDismissedByUser, CloseNotificationCalled } ;
  virtual NotificationManager * instance();
  virtual Notification * notification(uint id) const;
  virtual QList<uint> notificationIds() const;
  virtual QStringList GetCapabilities();
  virtual uint Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout);
  virtual void CloseNotification(uint id, NotificationManager::NotificationClosedReason closeReason);
  virtual QString GetServerInformation(QString &name, QString &vendor, QString &version);
  virtual void removeNotificationsWithCategory(const QString &category);
  virtual void updateNotificationsWithCategory(const QString &category);
  virtual void commit();
  virtual void invokeAction(const QString &action);
  virtual void removeUserRemovableNotifications();
  virtual void NotificationManagerConstructor(QObject *parent);
  virtual void NotificationManagerDestructor();
}; 

// 2. IMPLEMENT STUB
NotificationManager * NotificationManagerStub::instance() {
  stubMethodEntered("instance");
  return stubReturnValue<NotificationManager *>("instance");
}

Notification * NotificationManagerStub::notification(uint id) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("notification",params);
  return stubReturnValue<Notification *>("notification");
}

QList<uint> NotificationManagerStub::notificationIds() const {
  stubMethodEntered("notificationIds");
  return stubReturnValue<QList<uint>>("notificationIds");
}

QStringList NotificationManagerStub::GetCapabilities() {
  stubMethodEntered("GetCapabilities");
  return stubReturnValue<QStringList>("GetCapabilities");
}

uint NotificationManagerStub::Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(appName));
  params.append( new Parameter<uint >(replacesId));
  params.append( new Parameter<QString >(appIcon));
  params.append( new Parameter<QString >(summary));
  params.append( new Parameter<QString >(body));
  params.append( new Parameter<QStringList >(actions));
  params.append( new Parameter<QVariantHash >(hints));
  params.append( new Parameter<int >(expireTimeout));
  stubMethodEntered("Notify",params);
  return stubReturnValue<uint>("Notify");
}

void NotificationManagerStub::CloseNotification(uint id, NotificationManager::NotificationClosedReason closeReason) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  params.append( new Parameter<NotificationManager::NotificationClosedReason >(closeReason));
  stubMethodEntered("CloseNotification",params);
}

QString NotificationManagerStub::GetServerInformation(QString &name, QString &vendor, QString &version) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString & >(name));
  params.append( new Parameter<QString & >(vendor));
  params.append( new Parameter<QString & >(version));
  stubMethodEntered("GetServerInformation",params);
  return stubReturnValue<QString>("GetServerInformation");
}

void NotificationManagerStub::removeNotificationsWithCategory(const QString &category) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(category));
  stubMethodEntered("removeNotificationsWithCategory",params);
}

void NotificationManagerStub::updateNotificationsWithCategory(const QString &category) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(category));
  stubMethodEntered("updateNotificationsWithCategory",params);
}

void NotificationManagerStub::commit() {
  stubMethodEntered("commit");
}

void NotificationManagerStub::invokeAction(const QString &action) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(action));
  stubMethodEntered("invokeAction",params);
}

void NotificationManagerStub::removeUserRemovableNotifications() {
  stubMethodEntered("removeUserRemovableNotifications");
}

void NotificationManagerStub::NotificationManagerConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void NotificationManagerStub::NotificationManagerDestructor() {

}



// 3. CREATE A STUB INSTANCE
NotificationManagerStub gDefaultNotificationManagerStub;
NotificationManagerStub* gNotificationManagerStub = &gDefaultNotificationManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
const char *NotificationManager::HINT_CATEGORY = "category";
const char *NotificationManager::HINT_URGENCY = "urgency";
const char *NotificationManager::HINT_ICON = "x-nemo-icon";
const char *NotificationManager::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationManager::HINT_PRIORITY = "x-nemo-priority";
const char *NotificationManager::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationManager::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationManager::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationManager::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationManager::HINT_HIDDEN = "x-nemo-hidden";

NotificationManager *NotificationManager::instance_ = 0;
NotificationManager * NotificationManager::instance() {
  if (instance_ == 0) {
    instance_ = new NotificationManager;
  }
  return instance_;
}

Notification * NotificationManager::notification(uint id) const {
  return gNotificationManagerStub->notification(id);
}

QList<uint> NotificationManager::notificationIds() const {
  return gNotificationManagerStub->notificationIds();
}

QStringList NotificationManager::GetCapabilities() {
  return gNotificationManagerStub->GetCapabilities();
}

uint NotificationManager::Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout) {
  return gNotificationManagerStub->Notify(appName, replacesId, appIcon, summary, body, actions, hints, expireTimeout);
}

void NotificationManager::CloseNotification(uint id, NotificationClosedReason closeReason) {
  gNotificationManagerStub->CloseNotification(id, closeReason);
}

QString NotificationManager::GetServerInformation(QString &name, QString &vendor, QString &version) {
  return gNotificationManagerStub->GetServerInformation(name, vendor, version);
}

void NotificationManager::removeNotificationsWithCategory(const QString &category) {
  gNotificationManagerStub->removeNotificationsWithCategory(category);
}

void NotificationManager::updateNotificationsWithCategory(const QString &category) {
  gNotificationManagerStub->updateNotificationsWithCategory(category);
}

void NotificationManager::commit() {
  gNotificationManagerStub->commit();
}

void NotificationManager::invokeAction(const QString &action) {
  gNotificationManagerStub->invokeAction(action);
}

void NotificationManager::removeUserRemovableNotifications() {
  gNotificationManagerStub->removeUserRemovableNotifications();
}

NotificationManager::NotificationManager(QObject *parent) {
  gNotificationManagerStub->NotificationManagerConstructor(parent);
}

NotificationManager::~NotificationManager() {
  gNotificationManagerStub->NotificationManagerDestructor();
}


#endif
