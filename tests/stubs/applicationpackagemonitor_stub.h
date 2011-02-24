#ifndef APPLICATIONPACKAGEMONITOR_STUB
#define APPLICATIONPACKAGEMONITOR_STUB

#include "applicationpackagemonitor.h"
#include <stubbase.h>

class ApplicationPackageMonitor::ExtraDirWatcher
{
public:
    ExtraDirWatcher(){}
    ~ExtraDirWatcher(){}
};

const QString ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER = "installer-extra/";

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ApplicationPackageMonitorStub : public StubBase {
  public:
  virtual void ApplicationPackageMonitorConstructor();
  virtual void ApplicationPackageMonitorDestructor();
  virtual void packageDownloadProgress(const QString &operation, const QString &packageName, const QString &packageVersion, int already, int total);
  virtual void packageOperationStarted(const QString &operation, const QString &packageName, const QString &packageVersion);
  virtual void packageOperationProgress(const QString &operation, const QString &packageame, const QString &packageVersion, int percentage);
  virtual void packageOperationComplete(const QString &operation, const QString &packageName, const QString &packageVersion, const QString &error, bool need_reboot);
  virtual void updatePackageState(const QString &desktopEntryPath);
  virtual QString desktopEntryName(const QString &packageName);
  virtual void packageRemoved(const QString &desktopEntryPath);
  virtual void updatePackageStates();
};

// 2. IMPLEMENT STUB
void ApplicationPackageMonitorStub::ApplicationPackageMonitorConstructor() {

}
void ApplicationPackageMonitorStub::ApplicationPackageMonitorDestructor() {

}

void ApplicationPackageMonitorStub::packageDownloadProgress(const QString &operation, const QString &packageName, const QString &packageVersion, int already, int total) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(operation));
  params.append( new Parameter<const QString & >(packageName));
  params.append( new Parameter<const QString & >(packageVersion));
  params.append( new Parameter<int >(already));
  params.append( new Parameter<int >(total));
  stubMethodEntered("packageDownloadProgress",params);
}

void ApplicationPackageMonitorStub::packageOperationStarted(const QString &operation, const QString &packageName, const QString &packageVersion) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(operation));
  params.append( new Parameter<const QString & >(packageName));
  params.append( new Parameter<const QString & >(packageVersion));
  stubMethodEntered("packageOperationStarted",params);
}

void ApplicationPackageMonitorStub::packageOperationProgress(const QString &operation, const QString &packageame, const QString &packageVersion, int percentage) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(operation));
  params.append( new Parameter<const QString & >(packageame));
  params.append( new Parameter<const QString & >(packageVersion));
  params.append( new Parameter<int >(percentage));
  stubMethodEntered("packageOperationProgress",params);
}

void ApplicationPackageMonitorStub::packageOperationComplete(const QString &operation, const QString &packageName, const QString &packageVersion, const QString &error, bool need_reboot) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(operation));
  params.append( new Parameter<const QString & >(packageName));
  params.append( new Parameter<const QString & >(packageVersion));
  params.append( new Parameter<const QString & >(error));
  params.append( new Parameter<bool >(need_reboot));
  stubMethodEntered("packageOperationComplete",params);
}

void ApplicationPackageMonitorStub::updatePackageState(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("updatePackageState",params);
}

QString ApplicationPackageMonitorStub::desktopEntryName(const QString &packageName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(packageName));
  stubMethodEntered("desktopEntryName",params);
  return stubReturnValue<QString>("desktopEntryName");
}

void ApplicationPackageMonitorStub::packageRemoved(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("packageRemoved",params);
}

void ApplicationPackageMonitorStub::updatePackageStates() {
  stubMethodEntered("updatePackageStates");
}

// 3. CREATE A STUB INSTANCE
ApplicationPackageMonitorStub gDefaultApplicationPackageMonitorStub;
ApplicationPackageMonitorStub* gApplicationPackageMonitorStub = &gDefaultApplicationPackageMonitorStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
ApplicationPackageMonitor::ApplicationPackageMonitor() : con(QDBusConnection::systemBus()) {
  gApplicationPackageMonitorStub->ApplicationPackageMonitorConstructor();
}

ApplicationPackageMonitor::~ApplicationPackageMonitor() {
  gApplicationPackageMonitorStub->ApplicationPackageMonitorDestructor();
}

void ApplicationPackageMonitor::packageDownloadProgress(const QString &operation, const QString &packageName, const QString &packageVersion, int already, int total) {
  gApplicationPackageMonitorStub->packageDownloadProgress(operation, packageName, packageVersion, already, total);
}

void ApplicationPackageMonitor::packageOperationStarted(const QString &operation, const QString &packageName, const QString &packageVersion) {
  gApplicationPackageMonitorStub->packageOperationStarted(operation, packageName, packageVersion);
}

void ApplicationPackageMonitor::packageOperationProgress(const QString &operation, const QString &packageame, const QString &packageVersion, int percentage) {
  gApplicationPackageMonitorStub->packageOperationProgress(operation, packageame, packageVersion, percentage);
}

void ApplicationPackageMonitor::packageOperationComplete(const QString &operation, const QString &packageName, const QString &packageVersion, const QString &error, bool need_reboot) {
  gApplicationPackageMonitorStub->packageOperationComplete(operation, packageName, packageVersion, error, need_reboot);
}

void ApplicationPackageMonitor::updatePackageState(const QString &desktopEntryPath) {
  gApplicationPackageMonitorStub->updatePackageState(desktopEntryPath);
}

QString ApplicationPackageMonitor::desktopEntryName(const QString &packageName) {
  return gApplicationPackageMonitorStub->desktopEntryName(packageName);
}

void ApplicationPackageMonitor::packageRemoved(const QString &desktopEntryPath) {
    gApplicationPackageMonitorStub->packageRemoved(desktopEntryPath);
}

void ApplicationPackageMonitor::updatePackageStates(){
    gApplicationPackageMonitorStub->updatePackageStates();
}

#endif
