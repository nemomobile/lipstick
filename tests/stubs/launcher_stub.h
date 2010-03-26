#ifndef LAUNCHER_STUB
#define LAUNCHER_STUB

#include "launcher.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherStub : public StubBase {
  public:
  virtual void LauncherConstructor(DuiWidget *parent);
  virtual void LauncherDestructor();
  virtual void setEnabled(bool enabled);
  virtual bool startApplication(const QString &application);
  virtual bool startDuiApplication(const QString &serviceName);
  virtual void activateLauncher();
  virtual void launchApplication(const QString &application);
  virtual void launchDuiApplication(const QString &service);
  virtual void launchLink(const QString &link);
  virtual void updateButtonListFromDirectory(const QString &path);
  virtual void updateButtonList();
  virtual LauncherButton * createLauncherButton(const DuiDesktopEntry &entry);
  virtual bool contains(const DuiDesktopEntry &entry);
  virtual void updateButtonsInDataStore();
  virtual void restoreButtonsFromDataStore();
  virtual void updateButtonListFromEntries(const QStringList &modifiedPaths, const QStringList &allPaths, const QString &nameFilter, const QStringList &acceptedTypes);
  virtual void addNewLauncherButton(const DuiDesktopEntry &entry);
  virtual bool isDesktopEntryValid(const DuiDesktopEntry &entry, const QStringList &acceptedTypes);
}; 

// 2. IMPLEMENT STUB
void LauncherStub::LauncherConstructor(DuiWidget *parent) {
  Q_UNUSED(parent);

}
void LauncherStub::LauncherDestructor() {

}
void LauncherStub::setEnabled(bool enabled) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(enabled));
  stubMethodEntered("setEnabled",params);
}

bool LauncherStub::startApplication(const QString &application) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(application));
  stubMethodEntered("startApplication",params);
  return stubReturnValue<bool>("startApplication");
}

bool LauncherStub::startDuiApplication(const QString &serviceName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(serviceName));
  stubMethodEntered("startDuiApplication",params);
  return stubReturnValue<bool>("startDuiApplication");
}

void LauncherStub::activateLauncher() {
  stubMethodEntered("activateLauncher");
}

void LauncherStub::launchApplication(const QString &application) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(application));
  stubMethodEntered("launchApplication",params);
}

void LauncherStub::launchDuiApplication(const QString &service) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(service));
  stubMethodEntered("launchDuiApplication",params);
}

void LauncherStub::launchLink(const QString &link) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(link));
  stubMethodEntered("launchLink",params);
}

void LauncherStub::updateButtonListFromDirectory(const QString &path) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(path));
  stubMethodEntered("updateButtonListFromDirectory",params);
}

void LauncherStub::updateButtonList() {
  stubMethodEntered("updateButtonList");
}

LauncherButton * LauncherStub::createLauncherButton(const DuiDesktopEntry &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiDesktopEntry & >(entry));
  stubMethodEntered("createLauncherButton",params);
  return stubReturnValue<LauncherButton *>("createLauncherButton");
}

bool LauncherStub::contains(const DuiDesktopEntry &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiDesktopEntry & >(entry));
  stubMethodEntered("contains",params);
  return stubReturnValue<bool>("contains");
}

void LauncherStub::updateButtonsInDataStore() {
  stubMethodEntered("updateButtonsInDataStore");
}

void LauncherStub::restoreButtonsFromDataStore() {
  stubMethodEntered("restoreButtonsFromDataStore");
}

void LauncherStub::updateButtonListFromEntries(const QStringList &modifiedPaths, const QStringList &allPaths, const QString &nameFilter, const QStringList &acceptedTypes) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QStringList & >(modifiedPaths));
  params.append( new Parameter<const QStringList & >(allPaths));
  params.append( new Parameter<const QString & >(nameFilter));
  params.append( new Parameter<const QStringList & >(acceptedTypes));
  stubMethodEntered("updateButtonListFromEntries",params);
}

void LauncherStub::addNewLauncherButton(const DuiDesktopEntry &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiDesktopEntry & >(entry));
  stubMethodEntered("addNewLauncherButton",params);
}

bool LauncherStub::isDesktopEntryValid(const DuiDesktopEntry &entry, const QStringList &acceptedTypes) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiDesktopEntry & >(entry));
  params.append( new Parameter<const QStringList & >(acceptedTypes));
  stubMethodEntered("isDesktopEntryValid",params);
  return stubReturnValue<bool>("isDesktopEntryValid");
}


// 3. CREATE A STUB INSTANCE
LauncherStub gDefaultLauncherStub;
LauncherStub* gLauncherStub = &gDefaultLauncherStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Launcher::Launcher(DuiWidget *parent) {
  gLauncherStub->LauncherConstructor(parent);
}

Launcher::~Launcher() {
  gLauncherStub->LauncherDestructor();
}

void Launcher::setEnabled(bool enabled) {
  gLauncherStub->setEnabled(enabled);
}

bool Launcher::startApplication(const QString &application) {
  return gLauncherStub->startApplication(application);
}

bool Launcher::startDuiApplication(const QString &serviceName) {
  return gLauncherStub->startDuiApplication(serviceName);
}

void Launcher::activateLauncher() {
  gLauncherStub->activateLauncher();
}

void Launcher::launchApplication(const QString &application) {
  gLauncherStub->launchApplication(application);
}

void Launcher::launchDuiApplication(const QString &service) {
  gLauncherStub->launchDuiApplication(service);
}

void Launcher::launchLink(const QString &link) {
  gLauncherStub->launchLink(link);
}

void Launcher::updateButtonListFromDirectory(const QString &path) {
  gLauncherStub->updateButtonListFromDirectory(path);
}

void Launcher::updateButtonList() {
  gLauncherStub->updateButtonList();
}

LauncherButton * Launcher::createLauncherButton(const DuiDesktopEntry &entry) {
  return gLauncherStub->createLauncherButton(entry);
}

bool Launcher::contains(const DuiDesktopEntry &entry) {
  return gLauncherStub->contains(entry);
}

void Launcher::updateButtonsInDataStore() {
  gLauncherStub->updateButtonsInDataStore();
}

void Launcher::restoreButtonsFromDataStore() {
  gLauncherStub->restoreButtonsFromDataStore();
}

void Launcher::updateButtonListFromEntries(const QStringList &modifiedPaths, const QStringList &allPaths, const QString &nameFilter, const QStringList &acceptedTypes) {
  gLauncherStub->updateButtonListFromEntries(modifiedPaths, allPaths, nameFilter, acceptedTypes);
}

void Launcher::addNewLauncherButton(const DuiDesktopEntry &entry) {
  gLauncherStub->addNewLauncherButton(entry);
}

bool Launcher::isDesktopEntryValid(const DuiDesktopEntry &entry, const QStringList &acceptedTypes) {
  return gLauncherStub->isDesktopEntryValid(entry, acceptedTypes);
}

#endif
