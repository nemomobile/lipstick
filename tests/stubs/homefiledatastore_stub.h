/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef HOMEFILEDATASTORE_STUB
#define HOMEFILEDATASTORE_STUB

#include "homefiledatastore.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class HomeFileDataStoreStub : public StubBase {
  public:
  virtual void HomeFileDataStoreConstructor(const QString &filePath);
  virtual void HomeFileDataStoreDestructor();
  virtual bool createValue(const QString &key, const QVariant &value);
  virtual bool setValue(const QString &key, const QVariant &value);
  virtual QVariant value(const QString &key) const;
  virtual QStringList allKeys() const;
  virtual void remove(const QString &key);
  virtual void clear();
  virtual bool contains(const QString &key) const;
  virtual bool isReadable() const;
  virtual bool isWritable() const;
  virtual bool createValues(const QHash<QString, QVariant> &values);
  virtual void takeSnapshot();
};

// 2. IMPLEMENT STUB
void HomeFileDataStoreStub::HomeFileDataStoreConstructor(const QString &filePath) {
  Q_UNUSED(filePath);

}
void HomeFileDataStoreStub::HomeFileDataStoreDestructor() {

}
bool HomeFileDataStoreStub::createValue(const QString &key, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(key));
  params.append( new Parameter<const QVariant & >(value));
  stubMethodEntered("createValue",params);
  return stubReturnValue<bool>("createValue");
}

bool HomeFileDataStoreStub::setValue(const QString &key, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(key));
  params.append( new Parameter<const QVariant & >(value));
  stubMethodEntered("setValue",params);
  return stubReturnValue<bool>("setValue");
}

QVariant HomeFileDataStoreStub::value(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(key));
  stubMethodEntered("value",params);
  return stubReturnValue<QVariant>("value");
}

QStringList HomeFileDataStoreStub::allKeys() const {
  stubMethodEntered("allKeys");
  return stubReturnValue<QStringList>("allKeys");
}

void HomeFileDataStoreStub::remove(const QString &key) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(key));
  stubMethodEntered("remove",params);
}

void HomeFileDataStoreStub::clear() {
  stubMethodEntered("clear");
}

bool HomeFileDataStoreStub::contains(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(key));
  stubMethodEntered("contains",params);
  return stubReturnValue<bool>("contains");
}

bool HomeFileDataStoreStub::isReadable() const {
  stubMethodEntered("isReadable");
  return stubReturnValue<bool>("isReadable");
}

bool HomeFileDataStoreStub::isWritable() const {
  stubMethodEntered("isWritable");
  return stubReturnValue<bool>("isWritable");
}

bool HomeFileDataStoreStub::createValues(const QHash<QString, QVariant> &values) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QHash<QString, QVariant> >(values));
  stubMethodEntered("createValues",params);
  return stubReturnValue<bool>("createValues");
}

void HomeFileDataStoreStub::takeSnapshot() {
  stubMethodEntered("takeSnapshot");
}



// 3. CREATE A STUB INSTANCE
HomeFileDataStoreStub gDefaultHomeFileDataStoreStub;
HomeFileDataStoreStub* gHomeFileDataStoreStub = &gDefaultHomeFileDataStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
HomeFileDataStore::HomeFileDataStore(const QString &filePath) : d_ptr(NULL) {
  gHomeFileDataStoreStub->HomeFileDataStoreConstructor(filePath);
}

HomeFileDataStore::~HomeFileDataStore() {
  gHomeFileDataStoreStub->HomeFileDataStoreDestructor();
}

bool HomeFileDataStore::createValue(const QString &key, const QVariant &value) {
  return gHomeFileDataStoreStub->createValue(key, value);
}

bool HomeFileDataStore::setValue(const QString &key, const QVariant &value) {
  return gHomeFileDataStoreStub->setValue(key, value);
}

QVariant HomeFileDataStore::value(const QString &key) const {
  return gHomeFileDataStoreStub->value(key);
}

QStringList HomeFileDataStore::allKeys() const {
  return gHomeFileDataStoreStub->allKeys();
}

void HomeFileDataStore::remove(const QString &key) {
  gHomeFileDataStoreStub->remove(key);
}

void HomeFileDataStore::clear() {
  gHomeFileDataStoreStub->clear();
}

bool HomeFileDataStore::contains(const QString &key) const {
  return gHomeFileDataStoreStub->contains(key);
}

bool HomeFileDataStore::isReadable() const {
  return gHomeFileDataStoreStub->isReadable();
}

bool HomeFileDataStore::isWritable() const {
  return gHomeFileDataStoreStub->isWritable();
}

bool HomeFileDataStore::createValues(const QHash<QString, QVariant> &values) {
  return gHomeFileDataStoreStub->createValues(values);
}

void HomeFileDataStore::takeSnapshot() {
  gHomeFileDataStoreStub->takeSnapshot();
}


#endif
