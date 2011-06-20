#ifndef APPLICATIONPACKAGEMONITOR_STUB
#define APPLICATIONPACKAGEMONITOR_STUB

#include "applicationpackagemonitor.h"
#include <stubbase.h>

const QString ApplicationPackageMonitor::INSTALLER_EXTRA_FOLDER = "installer-extra/";
const QString ApplicationPackageMonitor::PACKAGE_STATE_INSTALLED = "installed";
const QString ApplicationPackageMonitor::PACKAGE_STATE_INSTALLABLE = "installable";
const QString ApplicationPackageMonitor::PACKAGE_STATE_BROKEN = "broken";
const QString ApplicationPackageMonitor::PACKAGE_STATE_UPDATEABLE = "updateable";
const QString ApplicationPackageMonitor::PACKAGE_STATE_INSTALLING ="installing";
const QString ApplicationPackageMonitor::PACKAGE_STATE_DOWNLOADING ="downloading";
const QString ApplicationPackageMonitor::PACKAGE_STATE_UNINSTALLING ="uninstalling";

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ApplicationPackageMonitorStub : public StubBase {
  public:
  virtual void ApplicationPackageMonitorConstructor();
  virtual void ApplicationPackageMonitorDestructor();
  virtual void updatePackageStates();
  virtual QString packageName(const QString &dekstopEntryPath);
  virtual bool isInstallerExtraEntry(const QString &desktopEntryPath);
  virtual QString toInstallerExtraEntryPath(const QString &desktopEntryPath);
  virtual QString toApplicationsEntryPath(const QString &desktopEntryPath);
  virtual void packageDownloadProgress(const QString &operation, const QString &packageName, const QString &packageVersion, int already, int total);
  virtual void packageOperationStarted(const QString &operation, const QString &packageName, const QString &packageVersion);
  virtual void packageOperationComplete(const QString &operation, const QString &packageName, const QString &packageVersion, const QString &error, bool need_reboot);
  virtual void updatePackageState(const QSharedPointer<MDesktopEntry> &desktopEntry);
  virtual void packageRemoved(const QString &desktopEntryPath);
  virtual bool isValidOperation(const QString &desktopEntryPath, const QString &operation);
  virtual bool isPackageRemovable(const MDesktopEntry *desktopEntry);
};

// 2. IMPLEMENT STUB
void ApplicationPackageMonitorStub::ApplicationPackageMonitorConstructor() {

}
void ApplicationPackageMonitorStub::ApplicationPackageMonitorDestructor() {

}
void ApplicationPackageMonitorStub::updatePackageStates() {
  stubMethodEntered("updatePackageStates");
}

QString ApplicationPackageMonitorStub::packageName(const QString &dekstopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(dekstopEntryPath));
  stubMethodEntered("packageName",params);
  return stubReturnValue<QString>("packageName");
}

bool ApplicationPackageMonitorStub::isInstallerExtraEntry(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("isInstallerExtraEntry",params);
  return stubReturnValue<bool>("isInstallerExtraEntry");
}

QString ApplicationPackageMonitorStub::toInstallerExtraEntryPath(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("toInstallerExtraEntryPath",params);
  return stubReturnValue<QString>("toInstallerExtraEntryPath");
}

QString ApplicationPackageMonitorStub::toApplicationsEntryPath(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("toApplicationsEntryPath",params);
  return stubReturnValue<QString>("toApplicationsEntryPath");
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

void ApplicationPackageMonitorStub::packageOperationComplete(const QString &operation, const QString &packageName, const QString &packageVersion, const QString &error, bool need_reboot) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(operation));
  params.append( new Parameter<const QString & >(packageName));
  params.append( new Parameter<const QString & >(packageVersion));
  params.append( new Parameter<const QString & >(error));
  params.append( new Parameter<bool >(need_reboot));
  stubMethodEntered("packageOperationComplete",params);
}

void ApplicationPackageMonitorStub::updatePackageState(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<MDesktopEntry> >(desktopEntry));
  stubMethodEntered("updatePackageState",params);
}

void ApplicationPackageMonitorStub::packageRemoved(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("packageRemoved",params);
}

bool ApplicationPackageMonitorStub::isValidOperation(const QString &desktopEntryPath, const QString &operation) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  params.append( new Parameter<const QString & >(operation));
  stubMethodEntered("isValidOperation",params);
  return stubReturnValue<bool>("isValidOperation");
}

bool ApplicationPackageMonitorStub::isPackageRemovable(const MDesktopEntry *desktopEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MDesktopEntry *>(desktopEntry));
  stubMethodEntered("isPackageRemovable",params);
  return stubReturnValue<bool>("isPackageRemovable");
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

void ApplicationPackageMonitor::updatePackageStates() {
  gApplicationPackageMonitorStub->updatePackageStates();
}

QString ApplicationPackageMonitor::packageName(const QString &dekstopEntryPath) {
  return gApplicationPackageMonitorStub->packageName(dekstopEntryPath);
}

bool ApplicationPackageMonitor::isInstallerExtraEntry(const QString &desktopEntryPath) {
  return gApplicationPackageMonitorStub->isInstallerExtraEntry(desktopEntryPath);
}

QString ApplicationPackageMonitor::toInstallerExtraEntryPath(const QString &desktopEntryPath) {
  return gApplicationPackageMonitorStub->toInstallerExtraEntryPath(desktopEntryPath);
}

QString ApplicationPackageMonitor::toApplicationsEntryPath(const QString &desktopEntryPath) {
  return gApplicationPackageMonitorStub->toApplicationsEntryPath(desktopEntryPath);
}

void ApplicationPackageMonitor::packageDownloadProgress(const QString &operation, const QString &packageName, const QString &packageVersion, int already, int total) {
  gApplicationPackageMonitorStub->packageDownloadProgress(operation, packageName, packageVersion, already, total);
}

void ApplicationPackageMonitor::packageOperationStarted(const QString &operation, const QString &packageName, const QString &packageVersion) {
  gApplicationPackageMonitorStub->packageOperationStarted(operation, packageName, packageVersion);
}

void ApplicationPackageMonitor::packageOperationComplete(const QString &operation, const QString &packageName, const QString &packageVersion, const QString &error, bool need_reboot) {
  gApplicationPackageMonitorStub->packageOperationComplete(operation, packageName, packageVersion, error, need_reboot);
}

void ApplicationPackageMonitor::updatePackageState(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  gApplicationPackageMonitorStub->updatePackageState(desktopEntry);
}

void ApplicationPackageMonitor::packageRemoved(const QString &desktopEntryPath) {
  gApplicationPackageMonitorStub->packageRemoved(desktopEntryPath);
}

bool ApplicationPackageMonitor::isValidOperation(const QString &desktopEntryPath, const QString &operation) {
  return gApplicationPackageMonitorStub->isValidOperation(desktopEntryPath, operation);
}

bool ApplicationPackageMonitor::isPackageRemovable(const MDesktopEntry *desktopEntry) {
  return gApplicationPackageMonitorStub->isPackageRemovable(desktopEntry);
}


#endif
