#ifndef QSYSTEMBATTERYINFO_STUB
#define QSYSTEMBATTERYINFO_STUB

#include "qsystembatteryinfo.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QSystemBatteryInfoStub : public StubBase {
  public:
  virtual void QSystemBatteryInfoConstructor(QObject *parent);
  virtual void QSystemBatteryInfoDestructor();
  virtual QtMobility::QSystemBatteryInfo::ChargerType chargerType() const;
  virtual QtMobility::QSystemBatteryInfo::ChargingState chargingState() const;
  virtual int nominalCapacity() const;
  virtual int remainingCapacityPercent() const;
  virtual int remainingCapacity() const;
  virtual int voltage() const;
  virtual int remainingChargingTime() const;
  virtual int currentFlow() const;
  virtual int remainingCapacityBars() const;
  virtual int maxBars() const;
  virtual QtMobility::QSystemBatteryInfo::BatteryStatus batteryStatus() const;
  virtual QtMobility::QSystemBatteryInfo::EnergyUnit energyMeasurementUnit() const;
}; 

// 2. IMPLEMENT STUB
void QSystemBatteryInfoStub::QSystemBatteryInfoConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void QSystemBatteryInfoStub::QSystemBatteryInfoDestructor() {

}
QtMobility::QSystemBatteryInfo::ChargerType QSystemBatteryInfoStub::chargerType() const {
  stubMethodEntered("chargerType");
  return stubReturnValue<QtMobility::QSystemBatteryInfo::ChargerType>("chargerType");
}

QtMobility::QSystemBatteryInfo::ChargingState QSystemBatteryInfoStub::chargingState() const {
  stubMethodEntered("chargingState");
  return stubReturnValue<QtMobility::QSystemBatteryInfo::ChargingState>("chargingState");
}

int QSystemBatteryInfoStub::nominalCapacity() const {
  stubMethodEntered("nominalCapacity");
  return stubReturnValue<int>("nominalCapacity");
}

int QSystemBatteryInfoStub::remainingCapacityPercent() const {
  stubMethodEntered("remainingCapacityPercent");
  return stubReturnValue<int>("remainingCapacityPercent");
}

int QSystemBatteryInfoStub::remainingCapacity() const {
  stubMethodEntered("remainingCapacity");
  return stubReturnValue<int>("remainingCapacity");
}

int QSystemBatteryInfoStub::voltage() const {
  stubMethodEntered("voltage");
  return stubReturnValue<int>("voltage");
}

int QSystemBatteryInfoStub::remainingChargingTime() const {
  stubMethodEntered("remainingChargingTime");
  return stubReturnValue<int>("remainingChargingTime");
}

int QSystemBatteryInfoStub::currentFlow() const {
  stubMethodEntered("currentFlow");
  return stubReturnValue<int>("currentFlow");
}

int QSystemBatteryInfoStub::remainingCapacityBars() const {
  stubMethodEntered("remainingCapacityBars");
  return stubReturnValue<int>("remainingCapacityBars");
}

int QSystemBatteryInfoStub::maxBars() const {
  stubMethodEntered("maxBars");
  return stubReturnValue<int>("maxBars");
}

QtMobility::QSystemBatteryInfo::BatteryStatus QSystemBatteryInfoStub::batteryStatus() const {
  stubMethodEntered("batteryStatus");
  return stubReturnValue<QtMobility::QSystemBatteryInfo::BatteryStatus>("batteryStatus");
}

QtMobility::QSystemBatteryInfo::EnergyUnit QSystemBatteryInfoStub::energyMeasurementUnit() const {
  stubMethodEntered("energyMeasurementUnit");
  return stubReturnValue<QtMobility::QSystemBatteryInfo::EnergyUnit>("energyMeasurementUnit");
}



// 3. CREATE A STUB INSTANCE
QSystemBatteryInfoStub gDefaultQSystemBatteryInfoStub;
QSystemBatteryInfoStub* gQSystemBatteryInfoStub = &gDefaultQSystemBatteryInfoStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
QtMobility::QSystemBatteryInfo::QSystemBatteryInfo(QObject *parent) : QObject(parent) {
  gQSystemBatteryInfoStub->QSystemBatteryInfoConstructor(parent);
}

QtMobility::QSystemBatteryInfo::~QSystemBatteryInfo() {
  gQSystemBatteryInfoStub->QSystemBatteryInfoDestructor();
}

QtMobility::QSystemBatteryInfo::ChargerType QtMobility::QSystemBatteryInfo::chargerType() const {
  return gQSystemBatteryInfoStub->chargerType();
}

QtMobility::QSystemBatteryInfo::ChargingState QtMobility::QSystemBatteryInfo::chargingState() const {
  return gQSystemBatteryInfoStub->chargingState();
}

int QtMobility::QSystemBatteryInfo::nominalCapacity() const {
  return gQSystemBatteryInfoStub->nominalCapacity();
}

int QtMobility::QSystemBatteryInfo::remainingCapacityPercent() const {
  return gQSystemBatteryInfoStub->remainingCapacityPercent();
}

int QtMobility::QSystemBatteryInfo::remainingCapacity() const {
  return gQSystemBatteryInfoStub->remainingCapacity();
}

int QtMobility::QSystemBatteryInfo::voltage() const {
  return gQSystemBatteryInfoStub->voltage();
}

int QtMobility::QSystemBatteryInfo::remainingChargingTime() const {
  return gQSystemBatteryInfoStub->remainingChargingTime();
}

int QtMobility::QSystemBatteryInfo::currentFlow() const {
  return gQSystemBatteryInfoStub->currentFlow();
}

int QtMobility::QSystemBatteryInfo::remainingCapacityBars() const {
  return gQSystemBatteryInfoStub->remainingCapacityBars();
}

int QtMobility::QSystemBatteryInfo::maxBars() const {
  return gQSystemBatteryInfoStub->maxBars();
}

QtMobility::QSystemBatteryInfo::BatteryStatus QtMobility::QSystemBatteryInfo::batteryStatus() const {
  return gQSystemBatteryInfoStub->batteryStatus();
}

QtMobility::QSystemBatteryInfo::EnergyUnit QtMobility::QSystemBatteryInfo::energyMeasurementUnit() const {
  return gQSystemBatteryInfoStub->energyMeasurementUnit();
}

void QtMobility::QSystemBatteryInfo::connectNotify(const char *)
{
}

void QtMobility::QSystemBatteryInfo::disconnectNotify(const char *)
{
}

#endif
