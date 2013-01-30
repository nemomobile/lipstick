#ifndef LOWBATTERYNOTIFIER_STUB
#define LOWBATTERYNOTIFIER_STUB

#include "lowbatterynotifier.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LowBatteryNotifierStub : public StubBase {
  public:
  virtual void LowBatteryNotifierConstructor(QObject *parent);
  virtual void LowBatteryNotifierDestructor();
  virtual void sendLowBatteryAlert();
  virtual void setNotificationInterval();
  virtual void setTouchScreenLockActive(bool active);
}; 

// 2. IMPLEMENT STUB
void LowBatteryNotifierStub::LowBatteryNotifierConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void LowBatteryNotifierStub::LowBatteryNotifierDestructor() {

}
void LowBatteryNotifierStub::sendLowBatteryAlert() {
  stubMethodEntered("sendLowBatteryAlert");
}

void LowBatteryNotifierStub::setNotificationInterval() {
  stubMethodEntered("setNotificationInterval");
}

void LowBatteryNotifierStub::setTouchScreenLockActive(bool active) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(active));
  stubMethodEntered("setTouchScreenLockActive",params);
}


// 3. CREATE A STUB INSTANCE
LowBatteryNotifierStub gDefaultLowBatteryNotifierStub;
LowBatteryNotifierStub* gLowBatteryNotifierStub = &gDefaultLowBatteryNotifierStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LowBatteryNotifier::LowBatteryNotifier(QObject *parent) : callContextItem("") {
  gLowBatteryNotifierStub->LowBatteryNotifierConstructor(parent);
}

LowBatteryNotifier::~LowBatteryNotifier() {
  gLowBatteryNotifierStub->LowBatteryNotifierDestructor();
}

void LowBatteryNotifier::sendLowBatteryAlert() {
  gLowBatteryNotifierStub->sendLowBatteryAlert();
}

void LowBatteryNotifier::setNotificationInterval() {
  gLowBatteryNotifierStub->setNotificationInterval();
}

void LowBatteryNotifier::setTouchScreenLockActive(bool active) {
  gLowBatteryNotifierStub->setTouchScreenLockActive(active);
}

#endif
