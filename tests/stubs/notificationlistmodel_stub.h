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
#ifndef NOTIFICATIONLISTMODEL_STUB
#define NOTIFICATIONLISTMODEL_STUB

#include "notificationlistmodel.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationListModelStub : public StubBase {
  public:
  virtual void NotificationListModelConstructor(QObject *parent);
  virtual void NotificationListModelDestructor();
  virtual void updateNotification(uint id);
  virtual void removeNotification(uint id);
}; 

// 2. IMPLEMENT STUB
void NotificationListModelStub::NotificationListModelConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void NotificationListModelStub::NotificationListModelDestructor() {

}
void NotificationListModelStub::updateNotification(uint id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("updateNotification",params);
}

void NotificationListModelStub::removeNotification(uint id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("removeNotification",params);
}



// 3. CREATE A STUB INSTANCE
NotificationListModelStub gDefaultNotificationListModelStub;
NotificationListModelStub* gNotificationListModelStub = &gDefaultNotificationListModelStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationListModel::NotificationListModel(QObject *parent) {
  gNotificationListModelStub->NotificationListModelConstructor(parent);
}

NotificationListModel::~NotificationListModel() {
  gNotificationListModelStub->NotificationListModelDestructor();
}

void NotificationListModel::updateNotification(uint id) {
  gNotificationListModelStub->updateNotification(id);
}

void NotificationListModel::removeNotification(uint id) {
  gNotificationListModelStub->removeNotification(id);
}


#endif
