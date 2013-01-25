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
#ifndef QOBJECTLISTMODEL_STUB
#define QOBJECTLISTMODEL_STUB

#include "qobjectlistmodel.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QObjectListModelStub : public StubBase {
  public:
  virtual void QObjectListModelConstructor(QObject *parent, QList<QObject *> *list);
  virtual int rowCount(const QModelIndex &parent) const;
  virtual int itemCount() const;
  virtual int columnCount(const QModelIndex &parent) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
  virtual void reset();
  virtual void move(int oldRow, int newRow);
  virtual void insertItem(int index, QObject *item);
  virtual void addItem(QObject *item);
  virtual void removeItem(QObject *item);
  virtual void removeItem(int index);
  virtual QObject * get(int index);
  virtual int indexOf(QObject *obj) const;
  virtual QList<QObject *> * getList();
  virtual void setList(QList<QObject *> *list);
  virtual void removeDestroyedItem();
}; 

// 2. IMPLEMENT STUB
void QObjectListModelStub::QObjectListModelConstructor(QObject *parent, QList<QObject *> *list) {
  Q_UNUSED(parent);
  Q_UNUSED(list);

}
int QObjectListModelStub::rowCount(const QModelIndex &parent) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QModelIndex & >(parent));
  stubMethodEntered("rowCount",params);
  return stubReturnValue<int>("rowCount");
}

int QObjectListModelStub::itemCount() const {
  stubMethodEntered("itemCount");
  return stubReturnValue<int>("itemCount");
}

int QObjectListModelStub::columnCount(const QModelIndex &parent) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QModelIndex & >(parent));
  stubMethodEntered("columnCount",params);
  return stubReturnValue<int>("columnCount");
}

QVariant QObjectListModelStub::data(const QModelIndex &index, int role) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QModelIndex & >(index));
  params.append( new Parameter<int >(role));
  stubMethodEntered("data",params);
  return stubReturnValue<QVariant>("data");
}

bool QObjectListModelStub::setData(const QModelIndex &index, const QVariant &value, int role) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QModelIndex & >(index));
  params.append( new Parameter<const QVariant & >(value));
  params.append( new Parameter<int >(role));
  stubMethodEntered("setData",params);
  return stubReturnValue<bool>("setData");
}

void QObjectListModelStub::reset() {
  stubMethodEntered("reset");
}

void QObjectListModelStub::move(int oldRow, int newRow) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(oldRow));
  params.append( new Parameter<int >(newRow));
  stubMethodEntered("move",params);
}

void QObjectListModelStub::insertItem(int index, QObject *item) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(index));
  params.append( new Parameter<QObject * >(item));
  stubMethodEntered("insertItem",params);
}

void QObjectListModelStub::addItem(QObject *item) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QObject * >(item));
  stubMethodEntered("addItem",params);
}

void QObjectListModelStub::removeItem(QObject *item) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QObject * >(item));
  stubMethodEntered("removeItem",params);
}

void QObjectListModelStub::removeItem(int index) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(index));
  stubMethodEntered("removeItem",params);
}

QObject * QObjectListModelStub::get(int index) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(index));
  stubMethodEntered("get",params);
  return stubReturnValue<QObject *>("get");
}

int QObjectListModelStub::indexOf(QObject *obj) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QObject * >(obj));
  stubMethodEntered("indexOf",params);
  return stubReturnValue<int>("indexOf");
}

QList<QObject *> * QObjectListModelStub::getList() {
  stubMethodEntered("getList");
  return stubReturnValue<QList<QObject *> *>("getList");
}

void QObjectListModelStub::setList(QList<QObject *> *list) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QObject *> * >(list));
  stubMethodEntered("setList",params);
}

void QObjectListModelStub::removeDestroyedItem() {
  stubMethodEntered("removeDestroyedItem");
}



// 3. CREATE A STUB INSTANCE
QObjectListModelStub gDefaultQObjectListModelStub;
QObjectListModelStub* gQObjectListModelStub = &gDefaultQObjectListModelStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
QObjectListModel::QObjectListModel(QObject *parent, QList<QObject *> *list) {
  gQObjectListModelStub->QObjectListModelConstructor(parent, list);
}

int QObjectListModel::rowCount(const QModelIndex &parent) const {
  return gQObjectListModelStub->rowCount(parent);
}

int QObjectListModel::itemCount() const {
  return gQObjectListModelStub->itemCount();
}

int QObjectListModel::columnCount(const QModelIndex &parent) const {
  return gQObjectListModelStub->columnCount(parent);
}

QVariant QObjectListModel::data(const QModelIndex &index, int role) const {
  return gQObjectListModelStub->data(index, role);
}

bool QObjectListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  return gQObjectListModelStub->setData(index, value, role);
}

void QObjectListModel::reset() {
  gQObjectListModelStub->reset();
}

void QObjectListModel::insertItem(int index, QObject *item) {
  gQObjectListModelStub->insertItem(index, item);
}

void QObjectListModel::addItem(QObject *item) {
  gQObjectListModelStub->addItem(item);
}

void QObjectListModel::removeItem(QObject *item) {
  gQObjectListModelStub->removeItem(item);
}

void QObjectListModel::removeItem(int index) {
  gQObjectListModelStub->removeItem(index);
}

QObject * QObjectListModel::get(int index) {
  return gQObjectListModelStub->get(index);
}

void QObjectListModel::move(int oldRow, int newRow) {
  return gQObjectListModelStub->move(oldRow, newRow);
}

int QObjectListModel::indexOf(QObject *obj) const {
  return gQObjectListModelStub->indexOf(obj);
}

QList<QObject *> * QObjectListModel::getList() {
  return gQObjectListModelStub->getList();
}

void QObjectListModel::setList(QList<QObject *> *list) {
  gQObjectListModelStub->setList(list);
}

void QObjectListModel::removeDestroyedItem() {
  gQObjectListModelStub->removeDestroyedItem();
}


#endif
