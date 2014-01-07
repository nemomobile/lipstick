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
  virtual QBatteryInfo::ChargerType chargerType() const;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
  virtual QBatteryInfo::EnergyUnit energyUnit() const;
  virtual QBatteryInfo::ChargingState chargingState(int battery) const;
  virtual QBatteryInfo::BatteryStatus batteryStatus(int battery) const;
  virtual int currentFlow(int battery) const;
  virtual int maximumCapacity(int battery) const;
  virtual int remainingCapacity(int battery) const;
  virtual int remainingChargingTime(int battery) const;
  virtual int voltage(int battery) const;
#else
  virtual QBatteryInfo::ChargingState chargingState() const;
  virtual QBatteryInfo::LevelStatus levelStatus() const;
  virtual int currentFlow() const;
  virtual int maximumCapacity() const;
  virtual int remainingCapacity() const;
  virtual int remainingChargingTime() const;
  virtual int voltage() const;
#endif
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

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfoStub::currentFlow(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("currentFlow",params);
  return stubReturnValue<int>("currentFlow");
}
#else
int QBatteryInfoStub::currentFlow() const {
  stubMethodEntered("currentFlow");
  return stubReturnValue<int>("currentFlow");
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfoStub::maximumCapacity(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("maximumCapacity",params);
  return stubReturnValue<int>("maximumCapacity");
}
#else
int QBatteryInfoStub::maximumCapacity() const {
  stubMethodEntered("maximumCapacity");
  return stubReturnValue<int>("maximumCapacity");
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfoStub::remainingCapacity(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("remainingCapacity",params);
  return stubReturnValue<int>("remainingCapacity");
}
#else
int QBatteryInfoStub::remainingCapacity() const {
  stubMethodEntered("remainingCapacity");
  return stubReturnValue<int>("remainingCapacity");
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfoStub::remainingChargingTime(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("remainingChargingTime",params);
  return stubReturnValue<int>("remainingChargingTime");
}
#else
int QBatteryInfoStub::remainingChargingTime() const {
  stubMethodEntered("remainingChargingTime");
  return stubReturnValue<int>("remainingChargingTime");
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfoStub::voltage(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("voltage",params);
  return stubReturnValue<int>("voltage");
}
#else
int QBatteryInfoStub::voltage() const {
  stubMethodEntered("voltage");
  return stubReturnValue<int>("voltage");
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
QBatteryInfo::ChargingState QBatteryInfoStub::chargingState(int battery) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(battery));
  stubMethodEntered("chargingState",params);
  return stubReturnValue<QBatteryInfo::ChargingState>("chargingState");
}
#else
QBatteryInfo::ChargingState QBatteryInfoStub::chargingState() const {
  stubMethodEntered("chargingState");
  return stubReturnValue<QBatteryInfo::ChargingState>("chargingState");
}
#endif

QBatteryInfo::ChargerType QBatteryInfoStub::chargerType() const {
  stubMethodEntered("chargerType");
  return stubReturnValue<QBatteryInfo::ChargerType>("chargerType");
}

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
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
#else
QBatteryInfo::LevelStatus QBatteryInfoStub::levelStatus() const {
  stubMethodEntered("levelStatus");
  return stubReturnValue<QBatteryInfo::LevelStatus>("levelStatus");
}
#endif

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

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfo::currentFlow(int battery) const {
  return gQBatteryInfoStub->currentFlow(battery);
}
#else
int QBatteryInfo::currentFlow() const {
  return gQBatteryInfoStub->currentFlow();
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfo::maximumCapacity(int battery) const {
  return gQBatteryInfoStub->maximumCapacity(battery);
}
#else
int QBatteryInfo::maximumCapacity() const {
  return gQBatteryInfoStub->maximumCapacity();
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfo::remainingCapacity(int battery) const {
  return gQBatteryInfoStub->remainingCapacity(battery);
}
#else
int QBatteryInfo::remainingCapacity() const {
  return gQBatteryInfoStub->remainingCapacity();
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfo::remainingChargingTime(int battery) const {
  return gQBatteryInfoStub->remainingChargingTime(battery);
}
#else
int QBatteryInfo::remainingChargingTime() const {
  return gQBatteryInfoStub->remainingChargingTime();
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
int QBatteryInfo::voltage(int battery) const {
  return gQBatteryInfoStub->voltage(battery);
}
#else
int QBatteryInfo::voltage() const {
  return gQBatteryInfoStub->voltage();
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
QBatteryInfo::ChargingState QBatteryInfo::chargingState(int battery) const {
  return gQBatteryInfoStub->chargingState(battery);
}
#else
QBatteryInfo::ChargingState QBatteryInfo::chargingState() const {
  return gQBatteryInfoStub->chargingState();
}
#endif

QBatteryInfo::ChargerType QBatteryInfo::chargerType() const {
  return gQBatteryInfoStub->chargerType();
}

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
QBatteryInfo::EnergyUnit QBatteryInfo::energyUnit() const {
  return gQBatteryInfoStub->energyUnit();
}

QBatteryInfo::BatteryStatus QBatteryInfo::batteryStatus(int battery) const {
  return gQBatteryInfoStub->batteryStatus(battery);
}
#else
QBatteryInfo::LevelStatus QBatteryInfo::levelStatus() const {
  return gQBatteryInfoStub->levelStatus();
}
#endif

void QBatteryInfo::connectNotify(const QMetaMethod &signal) {
  gQBatteryInfoStub->connectNotify(signal);
}

void QBatteryInfo::disconnectNotify(const QMetaMethod &signal) {
  gQBatteryInfoStub->disconnectNotify(signal);
}


#endif
