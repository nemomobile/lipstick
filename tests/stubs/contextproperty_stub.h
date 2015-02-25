#ifndef CONTEXTPROPERTY_STUB
#define CONTEXTPROPERTY_STUB

#include "contextproperty.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ContextPropertyStub : public StubBase {
  public:
  virtual void ContextPropertyConstructor(const QString &key, QObject *parent);
  virtual void ContextPropertyDestructor();
  virtual QString key() const;
  virtual QVariant value(const QVariant &def) const;
  virtual QVariant value() const;
  virtual const ContextPropertyInfo * info() const;
  virtual void subscribe() const;
  virtual void unsubscribe() const;
  virtual void waitForSubscription() const;
  virtual void waitForSubscription(bool block) const;
  virtual void ignoreCommander();
  virtual void setTypeCheck(bool typeCheck);
};

// 2. IMPLEMENT STUB
void ContextPropertyStub::ContextPropertyConstructor(const QString &key, QObject *parent) {
  Q_UNUSED(key);
  Q_UNUSED(parent);

}
void ContextPropertyStub::ContextPropertyDestructor() {

}
QString ContextPropertyStub::key() const {
  stubMethodEntered("key");
  return stubReturnValue<QString>("key");
}

QVariant ContextPropertyStub::value(const QVariant &def) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QVariant & >(def));
  stubMethodEntered("value",params);
  return stubReturnValue<QVariant>("value");
}

QVariant ContextPropertyStub::value() const {
  stubMethodEntered("value");
  return stubReturnValue<QVariant>("value");
}

const ContextPropertyInfo * ContextPropertyStub::info() const {
  stubMethodEntered("info");
  return stubReturnValue<const ContextPropertyInfo *>("info");
}

void ContextPropertyStub::subscribe() const {
  stubMethodEntered("subscribe");
}

void ContextPropertyStub::unsubscribe() const {
  stubMethodEntered("unsubscribe");
}

void ContextPropertyStub::waitForSubscription() const {
  stubMethodEntered("waitForSubscription");
}

void ContextPropertyStub::waitForSubscription(bool block) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(block));
  stubMethodEntered("waitForSubscription",params);
}

void ContextPropertyStub::ignoreCommander() {
  stubMethodEntered("ignoreCommander");
}

void ContextPropertyStub::setTypeCheck(bool typeCheck) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(typeCheck));
  stubMethodEntered("setTypeCheck",params);
}



// 3. CREATE A STUB INSTANCE
ContextPropertyStub gDefaultContextPropertyStub;
typedef QMap<QString, QSharedPointer<ContextPropertyStub> > ContextPropertyStubMap;
static ContextPropertyStubMap stubs;
QSharedPointer<ContextPropertyStub> getContextPropertyStub(QString const &name)
{
  ContextPropertyStubMap::iterator it = stubs.find(name);
  if (it == stubs.end()) {
    QSharedPointer<ContextPropertyStub> stub(new ContextPropertyStub());
    it = stubs.insert(name, stub);
  }
  return *it;
}

class ContextPropertyPrivate
{
public:
  ContextPropertyPrivate(QString const &k)
    : key(k), stub(getContextPropertyStub(key))
  {}
  QString key;
  QSharedPointer<ContextPropertyStub> stub;
};

// 4. CREATE A PROXY WHICH CALLS THE STUB
ContextProperty::ContextProperty(const QString &key, QObject *parent)
  : priv(new ContextPropertyPrivate(key))
{
  priv->stub->ContextPropertyConstructor(key, parent);
}

ContextProperty::~ContextProperty() {
  priv->stub->ContextPropertyDestructor();
}

QString ContextProperty::key() const {
  return priv->key;
}

QVariant ContextProperty::value(const QVariant &def) const {
  return priv->stub->value(def);
}

QVariant ContextProperty::value() const {
  return priv->stub->value();
}

const ContextPropertyInfo * ContextProperty::info() const {
  return priv->stub->info();
}

void ContextProperty::subscribe() const {
  priv->stub->subscribe();
}

void ContextProperty::unsubscribe() const {
  priv->stub->unsubscribe();
}

void ContextProperty::waitForSubscription() const {
  priv->stub->waitForSubscription();
}

void ContextProperty::waitForSubscription(bool block) const {
  priv->stub->waitForSubscription(block);
}

void ContextProperty::ignoreCommander() {
  // do nothing
}

void ContextProperty::setTypeCheck(bool) {
  // do nothing
}


#endif
