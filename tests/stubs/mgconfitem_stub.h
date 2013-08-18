#ifndef MGCONFITEM_STUB
#define MGCONFITEM_STUB

#include "mgconfitem.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MGConfItemStub : public StubBase {
  public:
  virtual void MGConfItemConstructor(const QString &key, QObject *parent);
  virtual void MGConfItemDestructor();
  virtual QString key() const;
  virtual QVariant value() const;
  virtual QVariant value(const QVariant &def) const;
  virtual void set(const QVariant &val);
  virtual void unset();
}; 

// 2. IMPLEMENT STUB
void MGConfItemStub::MGConfItemConstructor(const QString &key, QObject *parent) {
  Q_UNUSED(key);
  Q_UNUSED(parent);

}
void MGConfItemStub::MGConfItemDestructor() {

}
QString MGConfItemStub::key() const {
  stubMethodEntered("key");
  return stubReturnValue<QString>("key");
}

QVariant MGConfItemStub::value() const {
  stubMethodEntered("value");
  return stubReturnValue<QVariant>("value");
}

QVariant MGConfItemStub::value(const QVariant &def) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QVariant & >(def));
  stubMethodEntered("value",params);
  return stubReturnValue<QVariant>("value");
}

void MGConfItemStub::set(const QVariant &val) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QVariant & >(val));
  stubMethodEntered("set",params);
}

void MGConfItemStub::unset() {
  stubMethodEntered("unset");
}



// 3. CREATE A STUB INSTANCE
MGConfItemStub gDefaultMGConfItemStub;
MGConfItemStub* gMGConfItemStub = &gDefaultMGConfItemStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MGConfItem::MGConfItem(const QString &key, QObject *parent) {
  gMGConfItemStub->MGConfItemConstructor(key, parent);
}

MGConfItem::~MGConfItem() {
  gMGConfItemStub->MGConfItemDestructor();
}

QString MGConfItem::key() const {
  return gMGConfItemStub->key();
}

QVariant MGConfItem::value() const {
  return gMGConfItemStub->value();
}

QVariant MGConfItem::value(const QVariant &def) const {
  return gMGConfItemStub->value(def);
}

void MGConfItem::set(const QVariant &val) {
  gMGConfItemStub->set(val);
}

void MGConfItem::unset() {
  gMGConfItemStub->unset();
}


#endif
