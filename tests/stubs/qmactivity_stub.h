#ifndef QMACTIVITY_STUB
#define QMACTIVITY_STUB

#include "qmactivity.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QmActivityStub : public StubBase {
  public:
  virtual void QmActivityConstructor(QObject *parent);
  virtual void QmActivityDestructor();
  virtual MeeGo::QmActivity::Activity get() const;
  virtual void connectNotify(const QMetaMethod &signal);
  virtual void disconnectNotify(const QMetaMethod &signal);
}; 

// 2. IMPLEMENT STUB
void QmActivityStub::QmActivityConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void QmActivityStub::QmActivityDestructor() {

}
MeeGo::QmActivity::Activity QmActivityStub::get() const {
  stubMethodEntered("get");
  return stubReturnValue<MeeGo::QmActivity::Activity>("get");
}

void QmActivityStub::connectNotify(const QMetaMethod &signal) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod & >(signal));
  stubMethodEntered("connectNotify",params);
}

void QmActivityStub::disconnectNotify(const QMetaMethod &signal) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod & >(signal));
  stubMethodEntered("disconnectNotify",params);
}



// 3. CREATE A STUB INSTANCE
QmActivityStub gDefaultQmActivityStub;
QmActivityStub* gQmActivityStub = &gDefaultQmActivityStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
namespace MeeGo
{

QmActivity::QmActivity(QObject *parent) {
  gQmActivityStub->QmActivityConstructor(parent);
}

QmActivity::~QmActivity() {
  gQmActivityStub->QmActivityDestructor();
}

MeeGo::QmActivity::Activity QmActivity::get() const {
  return gQmActivityStub->get();
}

void QmActivity::connectNotify(const QMetaMethod &signal) {
  gQmActivityStub->connectNotify(signal);
}

void QmActivity::disconnectNotify(const QMetaMethod &signal) {
  gQmActivityStub->disconnectNotify(signal);
}

}

#endif
