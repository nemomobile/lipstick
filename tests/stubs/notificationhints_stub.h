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
#ifndef NOTIFICATIONHINTS_STUB
#define NOTIFICATIONHINTS_STUB

#include "notificationhints.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationHintsStub : public StubBase {
  public:
  virtual void NotificationHintsConstructor();
  virtual void setHint(const QString &hint, const QVariant &hintValue);
  virtual QVariant hintValue(const QString &hint) const;
  virtual QStringList hints() const;
   QHash<QString, QVariant> hintValues ;
}; 

// 2. IMPLEMENT STUB
void NotificationHintsStub::NotificationHintsConstructor() {

}
void NotificationHintsStub::setHint(const QString &hint, const QVariant &hintValue) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(hint));
  params.append( new Parameter<const QVariant & >(hintValue));
  stubMethodEntered("setHint",params);
}

QVariant NotificationHintsStub::hintValue(const QString &hint) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(hint));
  stubMethodEntered("hintValue",params);
  return stubReturnValue<QVariant>("hintValue");
}

QStringList NotificationHintsStub::hints() const {
  stubMethodEntered("hints");
  return stubReturnValue<QStringList>("hints");
}



// 3. CREATE A STUB INSTANCE
NotificationHintsStub gDefaultNotificationHintsStub;
NotificationHintsStub* gNotificationHintsStub = &gDefaultNotificationHintsStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
const char *NotificationHints::HINT_URGENCY = "urgency";
const char *NotificationHints::HINT_CATEGORY = "category";
const char *NotificationHints::HINT_DESKTOP_ENTRY = "desktop-entry";
const char *NotificationHints::HINT_IMAGE_DATA = "image_data";
const char *NotificationHints::HINT_SOUND_FILE = "sound-file";
const char *NotificationHints::HINT_SUPPRESS_SOUND = "suppress-sound";
const char *NotificationHints::HINT_X = "x";
const char *NotificationHints::HINT_Y = "y";
const char *NotificationHints::HINT_CLASS = "x-nemo-class";
const char *NotificationHints::HINT_ICON = "x-nemo-icon";
const char *NotificationHints::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationHints::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationHints::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationHints::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationHints::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationHints::HINT_USER_REMOVABLE = "x-nemo-user-removable";
const char *NotificationHints::HINT_GENERIC_TEXT_TRANSLATION_ID = "x-nemo-generic-text-translation-id";
const char *NotificationHints::HINT_GENERIC_TEXT_TRANSLATION_CATALOGUE = "x-nemo-generic-text-translation-catalogue";

NotificationHints::NotificationHints() {
  gNotificationHintsStub->NotificationHintsConstructor();
}

void NotificationHints::setHint(const QString &hint, const QVariant &hintValue) {
  gNotificationHintsStub->setHint(hint, hintValue);
}

QVariant NotificationHints::hintValue(const QString &hint) const {
  return gNotificationHintsStub->hintValue(hint);
}

QStringList NotificationHints::hints() const {
  return gNotificationHintsStub->hints();
}

QDBusArgument &operator<<(QDBusArgument &argument, const NotificationHints &)
{
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, NotificationHints &)
{
    return argument;
}


#endif
