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
#ifndef CATEGORYDEFINITIONSTORE_STUB
#define CATEGORYDEFINITIONSTORE_STUB

#include "categorydefinitionstore.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class CategoryDefinitionStoreStub : public StubBase {
  public:
  virtual void CategoryDefinitionStoreConstructor(const QString &categoryDefinitionsPath, uint maxStoredCategoryDefinitions, QObject *parent);
  virtual bool categoryDefinitionExists(const QString &category);
  virtual QList<QString> allKeys(const QString &category);
  virtual bool contains(const QString &category, const QString &key);
  virtual QString value(const QString &category, const QString &key);
  virtual void updateCategoryDefinitionFileList();
  virtual void updateCategoryDefinitionFile(const QString &path);
};

// 2. IMPLEMENT STUB
void CategoryDefinitionStoreStub::CategoryDefinitionStoreConstructor(const QString &categoryDefinitionsPath, uint maxStoredCategoryDefinitions, QObject *parent) {
  Q_UNUSED(categoryDefinitionsPath);
  Q_UNUSED(maxStoredCategoryDefinitions);
  Q_UNUSED(parent);

}
bool CategoryDefinitionStoreStub::categoryDefinitionExists(const QString &category) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(category));
  stubMethodEntered("categoryDefinitionExists",params);
  return stubReturnValue<bool>("categoryDefinitionExists");
}

QList<QString> CategoryDefinitionStoreStub::allKeys(const QString &category) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(category));
  stubMethodEntered("allKeys",params);
  return stubReturnValue<QList<QString>>("allKeys");
}

bool CategoryDefinitionStoreStub::contains(const QString &category, const QString &key) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(category));
  params.append( new Parameter<const QString & >(key));
  stubMethodEntered("contains",params);
  return stubReturnValue<bool>("contains");
}

QString CategoryDefinitionStoreStub::value(const QString &category, const QString &key) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(category));
  params.append( new Parameter<const QString & >(key));
  stubMethodEntered("value",params);
  return stubReturnValue<QString>("value");
}

void CategoryDefinitionStoreStub::updateCategoryDefinitionFileList() {
  stubMethodEntered("updateCategoryDefinitionFileList");
}

void CategoryDefinitionStoreStub::updateCategoryDefinitionFile(const QString &path) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(path));
  stubMethodEntered("updateCategoryDefinitionFile",params);
}



// 3. CREATE A STUB INSTANCE
CategoryDefinitionStoreStub gDefaultCategoryDefinitionStoreStub;
CategoryDefinitionStoreStub* gCategoryDefinitionStoreStub = &gDefaultCategoryDefinitionStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
CategoryDefinitionStore::CategoryDefinitionStore(const QString &categoryDefinitionsPath, uint maxStoredCategoryDefinitions, QObject *parent) {
  gCategoryDefinitionStoreStub->CategoryDefinitionStoreConstructor(categoryDefinitionsPath, maxStoredCategoryDefinitions, parent);
}

bool CategoryDefinitionStore::categoryDefinitionExists(const QString &category) {
  return gCategoryDefinitionStoreStub->categoryDefinitionExists(category);
}

QList<QString> CategoryDefinitionStore::allKeys(const QString &category) {
  return gCategoryDefinitionStoreStub->allKeys(category);
}

bool CategoryDefinitionStore::contains(const QString &category, const QString &key) {
  return gCategoryDefinitionStoreStub->contains(category, key);
}

QString CategoryDefinitionStore::value(const QString &category, const QString &key) {
  return gCategoryDefinitionStoreStub->value(category, key);
}

void CategoryDefinitionStore::updateCategoryDefinitionFileList() {
  gCategoryDefinitionStoreStub->updateCategoryDefinitionFileList();
}

void CategoryDefinitionStore::updateCategoryDefinitionFile(const QString &path) {
  gCategoryDefinitionStoreStub->updateCategoryDefinitionFile(path);
}


#endif
