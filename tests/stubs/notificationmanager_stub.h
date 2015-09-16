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
  virtual NotificationManager * instance(bool owner = true);
  virtual LipstickNotification * notification(uint id) const;
  virtual QList<uint> notificationIds() const;
  virtual QStringList GetCapabilities();
  virtual uint Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout);
  virtual void CloseNotification(uint id, NotificationManager::NotificationClosedReason closeReason);
  virtual void MarkNotificationDisplayed(uint id);
  virtual QString GetServerInformation(QString &name, QString &vendor, QString &version);
  virtual NotificationList GetNotifications(const QString &appName);
  virtual void removeNotificationsWithCategory(const QString &category);
  virtual void updateNotificationsWithCategory(const QString &category);
  virtual void commit();
  virtual void invokeAction(const QString &action);
  virtual void removeNotificationIfUserRemovable(uint id);
  virtual void removeUserRemovableNotifications();
  virtual void expire();
  virtual void reportModifications();
  virtual void NotificationManagerConstructor(QObject *parent, bool owner);
  virtual void NotificationManagerDestructor();
}; 

// 2. IMPLEMENT STUB
NotificationManager * NotificationManagerStub::instance(bool owner) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(owner));
  stubMethodEntered("instance",params);
  return stubReturnValue<NotificationManager *>("instance");
}

LipstickNotification * NotificationManagerStub::notification(uint id) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("notification",params);
  return stubReturnValue<LipstickNotification *>("notification");
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

void NotificationManagerStub::MarkNotificationDisplayed(uint id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("MarkNotificationDisplayed",params);
}

QString NotificationManagerStub::GetServerInformation(QString &name, QString &vendor, QString &version) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString & >(name));
  params.append( new Parameter<QString & >(vendor));
  params.append( new Parameter<QString & >(version));
  stubMethodEntered("GetServerInformation",params);
  return stubReturnValue<QString>("GetServerInformation");
}

NotificationList NotificationManagerStub::GetNotifications(const QString &appName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(appName));
  stubMethodEntered("GetNotifications",params);
  return stubReturnValue<NotificationList>("GetNotifications");
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

void NotificationManagerStub::removeNotificationIfUserRemovable(uint id)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("removeNotificationIfUserRemovable",params);
}

void NotificationManagerStub::removeUserRemovableNotifications() {
  stubMethodEntered("removeUserRemovableNotifications");
}

void NotificationManagerStub::expire() {
  stubMethodEntered("expire");
}

void NotificationManagerStub::reportModifications() {
  stubMethodEntered("reportModifications");
}

void NotificationManagerStub::NotificationManagerConstructor(QObject *parent, bool owner) {
  Q_UNUSED(parent);
  Q_UNUSED(owner);
}
void NotificationManagerStub::NotificationManagerDestructor() {

}



// 3. CREATE A STUB INSTANCE
NotificationManagerStub gDefaultNotificationManagerStub;
NotificationManagerStub* gNotificationManagerStub = &gDefaultNotificationManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
const char *NotificationManager::HINT_CATEGORY = "category";
const char *NotificationManager::HINT_URGENCY = "urgency";
const char *NotificationManager::HINT_TRANSIENT = "transient";
const char *NotificationManager::HINT_RESIDENT = "resident";
const char *NotificationManager::HINT_IMAGE_PATH = "image-path";
const char *NotificationManager::HINT_ICON = "x-nemo-icon";
const char *NotificationManager::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationManager::HINT_PRIORITY = "x-nemo-priority";
const char *NotificationManager::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationManager::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationManager::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationManager::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationManager::HINT_REMOTE_ACTION_PREFIX = "x-nemo-remote-action-";
const char *NotificationManager::HINT_REMOTE_ACTION_ICON_PREFIX = "x-nemo-remote-action-icon-";
const char *NotificationManager::HINT_HIDDEN = "x-nemo-hidden";
const char *NotificationManager::HINT_USER_REMOVABLE = "x-nemo-user-removable";
const char *NotificationManager::HINT_LED_DISABLED_WITHOUT_BODY_AND_SUMMARY = "x-nemo-led-disabled-without-body-and-summary";
const char *NotificationManager::HINT_ORIGIN = "x-nemo-origin";
const char *NotificationManager::HINT_OWNER = "x-nemo-owner";
const char *NotificationManager::HINT_MAX_CONTENT_LINES = "x-nemo-max-content-lines";
const char *NotificationManager::HINT_RESTORED = "x-nemo-restored";

NotificationManager *NotificationManager::instance_ = 0;
NotificationManager * NotificationManager::instance(bool owner) {
  if (instance_ == 0) {
    instance_ = new NotificationManager(qApp, owner);
  }
  return instance_;
}

LipstickNotification * NotificationManager::notification(uint id) const {
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

void NotificationManager::MarkNotificationDisplayed(uint id) {
  gNotificationManagerStub->MarkNotificationDisplayed(id);
}

QString NotificationManager::GetServerInformation(QString &name, QString &vendor, QString &version) {
  return gNotificationManagerStub->GetServerInformation(name, vendor, version);
}

NotificationList NotificationManager::GetNotifications(const QString &appName) {
  return gNotificationManagerStub->GetNotifications(appName);
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

void NotificationManager::removeNotificationIfUserRemovable(uint id) {
  gNotificationManagerStub->removeNotificationIfUserRemovable(id);
}

void NotificationManager::removeUserRemovableNotifications() {
  gNotificationManagerStub->removeUserRemovableNotifications();
}

void NotificationManager::expire() {
  gNotificationManagerStub->expire();
}

void NotificationManager::reportModifications() {
  gNotificationManagerStub->reportModifications();
}

NotificationManager::NotificationManager(QObject *parent, bool owner) {
  gNotificationManagerStub->NotificationManagerConstructor(parent, owner);
}

NotificationManager::~NotificationManager() {
  gNotificationManagerStub->NotificationManagerDestructor();
}


#endif
