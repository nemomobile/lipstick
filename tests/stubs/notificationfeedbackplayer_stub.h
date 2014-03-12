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
#ifndef NOTIFICATIONFEEDBACKPLAYER_STUB
#define NOTIFICATIONFEEDBACKPLAYER_STUB

#include "notificationfeedbackplayer.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationFeedbackPlayerStub : public StubBase {
  public:
  virtual void NotificationFeedbackPlayerConstructor(QObject *parent);
  virtual int minimumPriority() const;
  virtual void setMinimumPriority(int minimumPriority);
  virtual void init();
  virtual void addNotification(uint id);
  virtual void removeNotification(uint id);
}; 

// 2. IMPLEMENT STUB
void NotificationFeedbackPlayerStub::NotificationFeedbackPlayerConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
int NotificationFeedbackPlayerStub::minimumPriority() const {
  stubMethodEntered("minimumPriority");
  return stubReturnValue<int>("minimumPriority");
}

void NotificationFeedbackPlayerStub::setMinimumPriority(int minimumPriority) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(minimumPriority));
  stubMethodEntered("setMinimumPriority",params);
}

void NotificationFeedbackPlayerStub::init() {
  stubMethodEntered("init");
}

void NotificationFeedbackPlayerStub::addNotification(uint id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("addNotification",params);
}

void NotificationFeedbackPlayerStub::removeNotification(uint id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("removeNotification",params);
}



// 3. CREATE A STUB INSTANCE
NotificationFeedbackPlayerStub gDefaultNotificationFeedbackPlayerStub;
NotificationFeedbackPlayerStub* gNotificationFeedbackPlayerStub = &gDefaultNotificationFeedbackPlayerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationFeedbackPlayer::NotificationFeedbackPlayer(QObject *parent) {
  gNotificationFeedbackPlayerStub->NotificationFeedbackPlayerConstructor(parent);
}

int NotificationFeedbackPlayer::minimumPriority() const {
  return gNotificationFeedbackPlayerStub->minimumPriority();
}

void NotificationFeedbackPlayer::setMinimumPriority(int minimumPriority) {
  gNotificationFeedbackPlayerStub->setMinimumPriority(minimumPriority);
}

void NotificationFeedbackPlayer::init() {
  gNotificationFeedbackPlayerStub->init();
}

void NotificationFeedbackPlayer::addNotification(uint id) {
  gNotificationFeedbackPlayerStub->addNotification(id);
}

void NotificationFeedbackPlayer::removeNotification(uint id) {
  gNotificationFeedbackPlayerStub->removeNotification(id);
}


#endif
