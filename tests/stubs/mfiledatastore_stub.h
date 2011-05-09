/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#ifndef MFILEDATASTORE_STUB
#define MFILEDATASTORE_STUB

#include "mfiledatastore.h"
#include <stubbase.h>
#include <QSettings>
#include <QMap>
#include <QScopedPointer>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MFileDataStoreStub : public StubBase {
  public:
  virtual void MFileDataStoreConstructor(const QString &filePath);
  virtual bool createValue(const QString &key, const QVariant &value);
  virtual bool setValue(const QString &key, const QVariant &value);
  virtual QVariant value(const QString &key) const;
  virtual QStringList allKeys() const;
  virtual void remove(const QString &key);
  virtual void clear();
  virtual bool contains(const QString &key) const;
  virtual bool isReadable() const;
  virtual bool isWritable() const;
  virtual void takeSnapshot();
  virtual void fileChanged(const QString &fileName);
  virtual void directoryChanged(const QString &fileName);
  virtual bool createValues(const QHash<QString, QVariant> &values);
  void * d_ptr;
};

// 2. IMPLEMENT STUB
void MFileDataStoreStub::MFileDataStoreConstructor(const QString &filePath) {
  Q_UNUSED(filePath);
  d_ptr=0;
}
bool MFileDataStoreStub::createValue(const QString &key, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString>(key));
  params.append( new Parameter<const QVariant>(value));
  stubMethodEntered("createValue",params);
  return stubReturnValue<bool>("createValue");
}

bool MFileDataStoreStub::setValue(const QString &key, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString>(key));
  params.append( new Parameter<const QVariant>(value));
  stubMethodEntered("setValue",params);
  return stubReturnValue<bool>("setValue");
}

QVariant MFileDataStoreStub::value(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString>(key));
  stubMethodEntered("value",params);
  return stubReturnValue<QVariant>("value");
}

QStringList MFileDataStoreStub::allKeys() const {
  stubMethodEntered("allKeys");
  return stubReturnValue<QStringList>("allKeys");
}

void MFileDataStoreStub::remove(const QString &key) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString>(key));
  stubMethodEntered("remove",params);
}

void MFileDataStoreStub::clear() {
  stubMethodEntered("clear");
}

bool MFileDataStoreStub::contains(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString>(key));
  stubMethodEntered("contains",params);
  return stubReturnValue<bool>("contains");
}

bool MFileDataStoreStub::isReadable() const {
  stubMethodEntered("isReadable");
  return stubReturnValue<bool>("isReadable");
}

bool MFileDataStoreStub::isWritable() const {
  stubMethodEntered("isWritable");
  return stubReturnValue<bool>("isWritable");
}

void MFileDataStoreStub::takeSnapshot() {
  stubMethodEntered("takeSnapshot");
}

void MFileDataStoreStub::fileChanged(const QString &fileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString>(fileName));
  stubMethodEntered("fileChanged",params);
}

void MFileDataStoreStub::directoryChanged(const QString &fileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString>(fileName));
  stubMethodEntered("directoryChanged",params);
}

bool MFileDataStoreStub::createValues(const QHash<QString, QVariant> &values) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QHash<QString, QVariant> >(values));
  stubMethodEntered("createValues",params);
  return stubReturnValue<bool>("createValues");
}


// 3. CREATE A STUB INSTANCE
MFileDataStoreStub gDefaultMFileDataStoreStub;
MFileDataStoreStub* gMFileDataStoreStub = &gDefaultMFileDataStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MFileDataStore::MFileDataStore(const QString &filePath) :
  d_ptr(0)
{
  gMFileDataStoreStub->MFileDataStoreConstructor(filePath);
}

MFileDataStore::~MFileDataStore()
{
}

bool MFileDataStore::createValue(const QString &key, const QVariant &value) {
  return gMFileDataStoreStub->createValue(key, value);
}

bool MFileDataStore::setValue(const QString &key, const QVariant &value) {
  return gMFileDataStoreStub->setValue(key, value);
}

QVariant MFileDataStore::value(const QString &key) const {
  return gMFileDataStoreStub->value(key);
}

QStringList MFileDataStore::allKeys() const {
  return gMFileDataStoreStub->allKeys();
}

void MFileDataStore::remove(const QString &key) {
  gMFileDataStoreStub->remove(key);
}

void MFileDataStore::clear() {
  gMFileDataStoreStub->clear();
}

bool MFileDataStore::contains(const QString &key) const {
  return gMFileDataStoreStub->contains(key);
}

bool MFileDataStore::isReadable() const {
  return gMFileDataStoreStub->isReadable();
}

bool MFileDataStore::isWritable() const {
  return gMFileDataStoreStub->isWritable();
}

void MFileDataStore::takeSnapshot() {
  gMFileDataStoreStub->takeSnapshot();
}

void MFileDataStore::fileChanged(const QString &fileName) {
  gMFileDataStoreStub->fileChanged(fileName);
}

void MFileDataStore::directoryChanged(const QString &fileName) {
  gMFileDataStoreStub->directoryChanged(fileName);
}

bool MFileDataStore::createValues(const QHash<QString, QVariant> &values) {
  return gMFileDataStoreStub->createValues(values);
}

#endif
