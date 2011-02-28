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
  virtual void setDownloadProgress(const QString &desktopEntryPath, int bytesLoaded, int bytesTotal);
  virtual void setInstallProgress(const QString &desktopEntryPath, int percentage);
  virtual void setOperationSuccess(const QString &desktopEntryPath);
  virtual void setOperationError(const QString &desktopEntryPath, const QString &error);
  virtual bool isInstallerExtraEntry(const QString &desktopEntryPath);
}; 

// 2. IMPLEMENT STUB
void ApplicationPackageMonitorListenerStub::ApplicationPackageMonitorListenerConstructor() {

}
void ApplicationPackageMonitorListenerStub::ApplicationPackageMonitorListenerDestructor() {

}
void ApplicationPackageMonitorListenerStub::updatePackageStates() {
  stubMethodEntered("updatePackageStates");
}

void ApplicationPackageMonitorListenerStub::setDownloadProgress(const QString &desktopEntryPath, int bytesLoaded, int bytesTotal) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<int >(bytesLoaded));
  params.append( new Parameter<int >(bytesTotal));
  stubMethodEntered("setDownloadProgress",params);
}

void ApplicationPackageMonitorListenerStub::setInstallProgress(const QString &desktopEntryPath, int percentage) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<int >(percentage));
  stubMethodEntered("setInstallProgress",params);
}

void ApplicationPackageMonitorListenerStub::setOperationSuccess(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("setOperationSuccess",params);
}

void ApplicationPackageMonitorListenerStub::setOperationError(const QString &desktopEntryPath, const QString &error) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QString>(error));
  stubMethodEntered("setOperationError",params);
}

bool ApplicationPackageMonitorListenerStub::isInstallerExtraEntry(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("isInstallerExtraEntry",params);
  return stubReturnValue<bool>("isInstallerExtraEntry");
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

void ApplicationPackageMonitorListener::setDownloadProgress(const QString &desktopEntryPath, int bytesLoaded, int bytesTotal) {
  gApplicationPackageMonitorListenerStub->setDownloadProgress(desktopEntryPath, bytesLoaded, bytesTotal);
}

void ApplicationPackageMonitorListener::setInstallProgress(const QString &desktopEntryPath, int percentage) {
  gApplicationPackageMonitorListenerStub->setInstallProgress(desktopEntryPath, percentage);
}

void ApplicationPackageMonitorListener::setOperationSuccess(const QString &desktopEntryPath) {
  gApplicationPackageMonitorListenerStub->setOperationSuccess(desktopEntryPath);
}

void ApplicationPackageMonitorListener::setOperationError(const QString &desktopEntryPath, const QString &error) {
  gApplicationPackageMonitorListenerStub->setOperationError(desktopEntryPath, error);
}


bool ApplicationPackageMonitorListener::isInstallerExtraEntry(const QString &desktopEntryPath) {
  return gApplicationPackageMonitorListenerStub->isInstallerExtraEntry(desktopEntryPath);
}
#endif
