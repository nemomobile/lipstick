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
#ifndef NOTIFICATION_STUB
#define NOTIFICATION_STUB

#include "notification.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationStub : public StubBase {
  public:
  virtual void NotificationConstructor(const QString &appName, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout, QObject *parent);
  virtual QString appName() const;
  virtual void setAppName(const QString &appName);
  virtual QString appIcon() const;
  virtual void setAppIcon(const QString &appIcon);
  virtual QString summary() const;
  virtual void setSummary(const QString &summary);
  virtual QString body() const;
  virtual void setBody(const QString &body);
  virtual QStringList actions() const;
  virtual void setActions(const QStringList &actions);
  virtual QVariantHash hints() const;
  virtual void setHints(const QVariantHash &hints);
  virtual int expireTimeout() const;
  virtual void setExpireTimeout(int expireTimeout);
  virtual QString icon() const;
  virtual QDateTime timestamp() const;
  virtual QString localizedTimestamp() const;
}; 

// 2. IMPLEMENT STUB
void NotificationStub::NotificationConstructor(const QString &appName, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout, QObject *parent) {
  Q_UNUSED(appName);
  Q_UNUSED(appIcon);
  Q_UNUSED(summary);
  Q_UNUSED(body);
  Q_UNUSED(actions);
  Q_UNUSED(hints);
  Q_UNUSED(expireTimeout);
  Q_UNUSED(parent);

}
QString NotificationStub::appName() const {
  stubMethodEntered("appName");
  return stubReturnValue<QString>("appName");
}

void NotificationStub::setAppName(const QString &appName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(appName));
  stubMethodEntered("setAppName",params);
}

QString NotificationStub::appIcon() const {
  stubMethodEntered("appIcon");
  return stubReturnValue<QString>("appIcon");
}

void NotificationStub::setAppIcon(const QString &appIcon) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(appIcon));
  stubMethodEntered("setAppIcon",params);
}

QString NotificationStub::summary() const {
  stubMethodEntered("summary");
  return stubReturnValue<QString>("summary");
}

void NotificationStub::setSummary(const QString &summary) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(summary));
  stubMethodEntered("setSummary",params);
}

QString NotificationStub::body() const {
  stubMethodEntered("body");
  return stubReturnValue<QString>("body");
}

void NotificationStub::setBody(const QString &body) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(body));
  stubMethodEntered("setBody",params);
}

QStringList NotificationStub::actions() const {
  stubMethodEntered("actions");
  return stubReturnValue<QStringList>("actions");
}

void NotificationStub::setActions(const QStringList &actions) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QStringList & >(actions));
  stubMethodEntered("setActions",params);
}

QVariantHash NotificationStub::hints() const {
  stubMethodEntered("hints");
  return stubReturnValue<QVariantHash>("hints");
}

void NotificationStub::setHints(const QVariantHash &hints) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QVariantHash & >(hints));
  stubMethodEntered("setHints",params);
}

int NotificationStub::expireTimeout() const {
  stubMethodEntered("expireTimeout");
  return stubReturnValue<int>("expireTimeout");
}

void NotificationStub::setExpireTimeout(int expireTimeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(expireTimeout));
  stubMethodEntered("setExpireTimeout",params);
}

QString NotificationStub::icon() const {
  stubMethodEntered("icon");
  return stubReturnValue<QString>("icon");
}

QDateTime NotificationStub::timestamp() const {
  stubMethodEntered("timestamp");
  return stubReturnValue<QDateTime>("timestamp");
}

QString NotificationStub::localizedTimestamp() const {
  stubMethodEntered("localizedTimestamp");
  return stubReturnValue<QString>("localizedTimestamp");
}



// 3. CREATE A STUB INSTANCE
NotificationStub gDefaultNotificationStub;
NotificationStub* gNotificationStub = &gDefaultNotificationStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Notification::Notification(const QString &appName, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout, QObject *parent) {
  gNotificationStub->NotificationConstructor(appName, appIcon, summary, body, actions, hints, expireTimeout, parent);
}

QString Notification::appName() const {
  return gNotificationStub->appName();
}

void Notification::setAppName(const QString &appName) {
  gNotificationStub->setAppName(appName);
}

QString Notification::appIcon() const {
  return gNotificationStub->appIcon();
}

void Notification::setAppIcon(const QString &appIcon) {
  gNotificationStub->setAppIcon(appIcon);
}

QString Notification::summary() const {
  return gNotificationStub->summary();
}

void Notification::setSummary(const QString &summary) {
  gNotificationStub->setSummary(summary);
}

QString Notification::body() const {
  return gNotificationStub->body();
}

void Notification::setBody(const QString &body) {
  gNotificationStub->setBody(body);
}

QStringList Notification::actions() const {
  return gNotificationStub->actions();
}

void Notification::setActions(const QStringList &actions) {
  gNotificationStub->setActions(actions);
}

QVariantHash Notification::hints() const {
  return gNotificationStub->hints();
}

void Notification::setHints(const QVariantHash &hints) {
  gNotificationStub->setHints(hints);
}

int Notification::expireTimeout() const {
  return gNotificationStub->expireTimeout();
}

void Notification::setExpireTimeout(int expireTimeout) {
  gNotificationStub->setExpireTimeout(expireTimeout);
}

QString Notification::icon() const {
  return gNotificationStub->icon();
}

QDateTime Notification::timestamp() const {
  return gNotificationStub->timestamp();
}

QString Notification::localizedTimestamp() const {
  return gNotificationStub->localizedTimestamp();
}


#endif
