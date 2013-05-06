#ifndef QBATTERYINFO_STUB
#define QBATTERYINFO_STUB

#include "qbatteryinfo.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QBatteryInfoStub : public StubBase {
  public:
  virtual void QBatteryInfoConstructor(QObject *parent);
  virtual void QBatteryInfoDestructor();
  virtual int batteryCount() const;
  virtual int currentFlow(int battery) const;
  virtual int maximumCapacity(int battery) const;
  virtual int remainingCapacity(int battery) const;
  virtual int remainingChargingTime(int battery) const;
  virtual int voltage(int battery) const;
  virtual QBatteryInfo::ChargingState chargingState(int battery) const;
  virtual QBatteryInfo::ChargerType chargerType() const;
  virtual QBatteryInfo::EnergyUnit energyUnit() const;
  virtual QBatteryInfo::BatteryStatus batteryStatus(int battery) const;
  virtual void connectNotify(const QMetaMethod &signal);
  virtual void disconnectNotify(const QMetaMethod &signal);
}; 

// 2. IMPLEMENT STUB
void QBatteryInfoStub::QBatteryInfoConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void QBatteryInfoStub::QBatteryInfoDestructor() {

}
int QBatteryInfoStub::batteryCount() const {
  stubMethodEntered("batteryCount");
  return stubReturnValue<int>("batteryCount");
}

int QBatteryInfoStub::currentFlow(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("currentFlow",params);
  return stubReturnValue<int>("currentFlow");
}

int QBatteryInfoStub::maximumCapacity(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("maximumCapacity",params);
  return stubReturnValue<int>("maximumCapacity");
}

int QBatteryInfoStub::remainingCapacity(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("remainingCapacity",params);
  return stubReturnValue<int>("remainingCapacity");
}

int QBatteryInfoStub::remainingChargingTime(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("remainingChargingTime",params);
  return stubReturnValue<int>("remainingChargingTime");
}

int QBatteryInfoStub::voltage(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("voltage",params);
  return stubReturnValue<int>("voltage");
}

QBatteryInfo::ChargingState QBatteryInfoStub::chargingState(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("chargingState",params);
  return stubReturnValue<QBatteryInfo::ChargingState>("chargingState");
}

QBatteryInfo::ChargerType QBatteryInfoStub::chargerType() const {
  stubMethodEntered("chargerType");
  return stubReturnValue<QBatteryInfo::ChargerType>("chargerType");
}

QBatteryInfo::EnergyUnit QBatteryInfoStub::energyUnit() const {
  stubMethodEntered("energyUnit");
  return stubReturnValue<QBatteryInfo::EnergyUnit>("energyUnit");
}

QBatteryInfo::BatteryStatus QBatteryInfoStub::batteryStatus(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("batteryStatus",params);
  return stubReturnValue<QBatteryInfo::BatteryStatus>("batteryStatus");
}

void QBatteryInfoStub::connectNotify(const QMetaMethod &signal) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod & >(signal));
  stubMethodEntered("connectNotify",params);
}

void QBatteryInfoStub::disconnectNotify(const QMetaMethod &signal) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QMetaMethod & >(signal));
  stubMethodEntered("disconnectNotify",params);
}



// 3. CREATE A STUB INSTANCE
QBatteryInfoStub gDefaultQBatteryInfoStub;
QBatteryInfoStub* gQBatteryInfoStub = &gDefaultQBatteryInfoStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
QBatteryInfo::QBatteryInfo(QObject *parent) : d_ptr(0) {
  gQBatteryInfoStub->QBatteryInfoConstructor(parent);
}

QBatteryInfo::~QBatteryInfo() {
  gQBatteryInfoStub->QBatteryInfoDestructor();
}

int QBatteryInfo::batteryCount() const {
  return gQBatteryInfoStub->batteryCount();
}

int QBatteryInfo::currentFlow(int battery) const {
  return gQBatteryInfoStub->currentFlow(battery);
}

int QBatteryInfo::maximumCapacity(int battery) const {
  return gQBatteryInfoStub->maximumCapacity(battery);
}

int QBatteryInfo::remainingCapacity(int battery) const {
  return gQBatteryInfoStub->remainingCapacity(battery);
}

int QBatteryInfo::remainingChargingTime(int battery) const {
  return gQBatteryInfoStub->remainingChargingTime(battery);
}

int QBatteryInfo::voltage(int battery) const {
  return gQBatteryInfoStub->voltage(battery);
}

QBatteryInfo::ChargingState QBatteryInfo::chargingState(int battery) const {
  return gQBatteryInfoStub->chargingState(battery);
}

QBatteryInfo::ChargerType QBatteryInfo::chargerType() const {
  return gQBatteryInfoStub->chargerType();
}

QBatteryInfo::EnergyUnit QBatteryInfo::energyUnit() const {
  return gQBatteryInfoStub->energyUnit();
}

QBatteryInfo::BatteryStatus QBatteryInfo::batteryStatus(int battery) const {
  return gQBatteryInfoStub->batteryStatus(battery);
}

void QBatteryInfo::connectNotify(const QMetaMethod &signal) {
  gQBatteryInfoStub->connectNotify(signal);
}

void QBatteryInfo::disconnectNotify(const QMetaMethod &signal) {
  gQBatteryInfoStub->disconnectNotify(signal);
}


#endif
