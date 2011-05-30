#ifndef APPLICATIONPACKAGEMONITORLISTENER_STUB
#define APPLICATIONPACKAGEMONITORLISTENER_STUB

#include "applicationpackagemonitorlistener.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ApplicationPackageMonitorListenerStub : public StubBase {
  public:
  virtual void ApplicationPackageMonitorListenerConstructor();
  virtual void ApplicationPackageMonitorListenerDestructor();
  virtual void updatePackageStates();
  virtual void setDownloadProgress(const QString &desktopEntryPath, const QString &packageName, int bytesLoaded, int bytesTotal, bool packageRemovable);
  virtual void setInstallProgress(const QString &desktopEntryPath, const QString &packageName, int percentage, bool packageRemovable);
  virtual void setOperationSuccess(const QString &desktopEntryPath, const QString &packageName, bool packageRemovable);
  virtual void setOperationError(const QString &desktopEntryPath, const QString &packageName, const QString &error, bool packageRemovable);
  virtual bool isInstallerExtraEntry(const QString &desktopEntryPath);
  virtual QString toInstallerExtraEntryPath(const QString &entryPath);
  virtual QString toApplicationsEntryPath(const QString &entryPath);
  virtual void setPackageUninstall(const QString &desktopEntryPath, const QString &packageName, bool packageRemovable);
}; 

// 2. IMPLEMENT STUB
void ApplicationPackageMonitorListenerStub::ApplicationPackageMonitorListenerConstructor() {

}
void ApplicationPackageMonitorListenerStub::ApplicationPackageMonitorListenerDestructor() {

}
void ApplicationPackageMonitorListenerStub::updatePackageStates() {
  stubMethodEntered("updatePackageStates");
}

void ApplicationPackageMonitorListenerStub::setDownloadProgress(const QString &desktopEntryPath, const QString &packageName, int bytesLoaded, int bytesTotal, bool packageRemovable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<int >(bytesLoaded));
  params.append( new Parameter<int >(bytesTotal));
  params.append( new Parameter<bool>(packageRemovable));
  stubMethodEntered("setDownloadProgress",params);
}

void ApplicationPackageMonitorListenerStub::setInstallProgress(const QString &desktopEntryPath, const QString &packageName, int percentage, bool packageRemovable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<int >(percentage));
  params.append( new Parameter<bool>(packageRemovable));
  stubMethodEntered("setInstallProgress",params);
}

void ApplicationPackageMonitorListenerStub::setOperationSuccess(const QString &desktopEntryPath, const QString &packageName, bool packageRemovable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<bool>(packageRemovable));
  stubMethodEntered("setOperationSuccess",params);
}

void ApplicationPackageMonitorListenerStub::setOperationError(const QString &desktopEntryPath, const QString &packageName, const QString &error, bool packageRemovable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<QString>(error));
  params.append( new Parameter<bool>(packageRemovable));
  stubMethodEntered("setOperationError",params);
}

bool ApplicationPackageMonitorListenerStub::isInstallerExtraEntry(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("isInstallerExtraEntry",params);
  return stubReturnValue<bool>("isInstallerExtraEntry");
}

QString ApplicationPackageMonitorListenerStub::toInstallerExtraEntryPath(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("toInstallerExtraEntryPath",params);
  return stubReturnValue<QString>("toInstallerExtraEntryPath");
}

QString ApplicationPackageMonitorListenerStub::toApplicationsEntryPath(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("toApplicationsEntryPath",params);
  return stubReturnValue<QString>("toApplicationsEntryPath");
}

void ApplicationPackageMonitorListenerStub::setPackageUninstall(const QString &desktopEntryPath, const QString &packageName, bool packageRemovable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<bool>(packageRemovable));
  stubMethodEntered("setPackageUninstall",params);
}


// 3. CREATE A STUB INSTANCE
ApplicationPackageMonitorListenerStub gDefaultApplicationPackageMonitorListenerStub;
ApplicationPackageMonitorListenerStub* gApplicationPackageMonitorListenerStub = &gDefaultApplicationPackageMonitorListenerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ApplicationPackageMonitorListener::ApplicationPackageMonitorListener() {
  gApplicationPackageMonitorListenerStub->ApplicationPackageMonitorListenerConstructor();
}

ApplicationPackageMonitorListener::~ApplicationPackageMonitorListener() {
  gApplicationPackageMonitorListenerStub->ApplicationPackageMonitorListenerDestructor();
}

void ApplicationPackageMonitorListener::updatePackageStates() {
  gApplicationPackageMonitorListenerStub->updatePackageStates();
}

void ApplicationPackageMonitorListener::setDownloadProgress(const QString &desktopEntryPath, const QString &packageName, int bytesLoaded, int bytesTotal, bool packageRemovable) {
  gApplicationPackageMonitorListenerStub->setDownloadProgress(desktopEntryPath, packageName, bytesLoaded, bytesTotal, packageRemovable);
}

void ApplicationPackageMonitorListener::setInstallProgress(const QString &desktopEntryPath, const QString &packageName, int percentage, bool packageRemovable) {
  gApplicationPackageMonitorListenerStub->setInstallProgress(desktopEntryPath, packageName, percentage, packageRemovable);
}

void ApplicationPackageMonitorListener::setOperationSuccess(const QString &desktopEntryPath, const QString &packageName, bool packageRemovable) {
  gApplicationPackageMonitorListenerStub->setOperationSuccess(desktopEntryPath, packageName, packageRemovable);
}

void ApplicationPackageMonitorListener::setOperationError(const QString &desktopEntryPath, const QString &packageName, const QString &error, bool packageRemovable) {
  gApplicationPackageMonitorListenerStub->setOperationError(desktopEntryPath, packageName, error, packageRemovable);
}

bool ApplicationPackageMonitorListener::isInstallerExtraEntry(const QString &desktopEntryPath) {
  return gApplicationPackageMonitorListenerStub->isInstallerExtraEntry(desktopEntryPath);
}

QString ApplicationPackageMonitorListener::toInstallerExtraEntryPath(const QString &desktopEntryPath) {
  return gApplicationPackageMonitorListenerStub->toInstallerExtraEntryPath(desktopEntryPath);
}

QString ApplicationPackageMonitorListener::toApplicationsEntryPath(const QString &desktopEntryPath) {
  return gApplicationPackageMonitorListenerStub->toApplicationsEntryPath(desktopEntryPath);
}

void ApplicationPackageMonitorListener::setPackageUninstall(const QString &desktopEntryPath, const QString &packageName, bool packageRemovable)
{
    gApplicationPackageMonitorListenerStub->setPackageUninstall(desktopEntryPath, packageName, packageRemovable);
}

#endif
