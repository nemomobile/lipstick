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
  virtual void onValueChanged();
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

void ContextPropertyStub::onValueChanged() {
  stubMethodEntered("onValueChanged");
}



// 3. CREATE A STUB INSTANCE
ContextPropertyStub gDefaultContextPropertyStub;
ContextPropertyStub* gContextPropertyStub = &gDefaultContextPropertyStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ContextProperty::ContextProperty(const QString &key, QObject *parent) {
  gContextPropertyStub->ContextPropertyConstructor(key, parent);
}

ContextProperty::~ContextProperty() {
  gContextPropertyStub->ContextPropertyDestructor();
}

QString ContextProperty::key() const {
  return gContextPropertyStub->key();
}

QVariant ContextProperty::value(const QVariant &def) const {
  return gContextPropertyStub->value(def);
}

QVariant ContextProperty::value() const {
  return gContextPropertyStub->value();
}

const ContextPropertyInfo * ContextProperty::info() const {
  return gContextPropertyStub->info();
}

void ContextProperty::subscribe() const {
  gContextPropertyStub->subscribe();
}

void ContextProperty::unsubscribe() const {
  gContextPropertyStub->unsubscribe();
}

void ContextProperty::waitForSubscription() const {
  gContextPropertyStub->waitForSubscription();
}

void ContextProperty::waitForSubscription(bool block) const {
  gContextPropertyStub->waitForSubscription(block);
}

void ContextProperty::ignoreCommander() {
  gContextPropertyStub->ignoreCommander();
}

void ContextProperty::setTypeCheck(bool typeCheck) {
  gContextPropertyStub->setTypeCheck(typeCheck);
}

void ContextProperty::onValueChanged() {
  gContextPropertyStub->onValueChanged();
}


#endif
