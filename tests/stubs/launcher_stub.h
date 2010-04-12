#ifndef LAUNCHER_STUB
#define LAUNCHER_STUB

#include "launcher.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherStub : public StubBase {
  public:
  virtual void LauncherConstructor(MWidget *parent);
  virtual void LauncherDestructor();
  virtual void setEnabled(bool enabled);
  virtual bool startApplication(const QString &application);
  virtual bool startMApplication(const QString &serviceName);
  virtual void updateButtonListFromDirectory(const QString &path);
  virtual void launchApplication(const QString &application);
  virtual void launchMApplication(const QString &service);
  virtual void activateLauncher();
  virtual void updateButtonList();
  virtual LauncherButton * createLauncherButton(const MDesktopEntry &entry);
  virtual void connectLauncherButton(LauncherButton *launcherButton);
  virtual bool contains(const QString &desktopEntryFile);
  virtual void updateButtonsInDataStore();
  virtual void restoreButtonsFromDataStore();
  virtual void updateButtonListFromEntries(const QStringList &modifiedPaths, const QStringList &allPaths, const QString &nameFilter, const QStringList &acceptedTypes);
  virtual void addNewLauncherButton(const MDesktopEntry &entry);
  virtual bool isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes);
}; 

// 2. IMPLEMENT STUB
void LauncherStub::LauncherConstructor(MWidget *parent) {
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

bool LauncherStub::startMApplication(const QString &serviceName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(serviceName));
  stubMethodEntered("startMApplication",params);
  return stubReturnValue<bool>("startMApplication");
}

void LauncherStub::updateButtonListFromDirectory(const QString &path) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(path));
  stubMethodEntered("updateButtonListFromDirectory",params);
}

void LauncherStub::launchApplication(const QString &application) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(application));
  stubMethodEntered("launchApplication",params);
}

void LauncherStub::launchMApplication(const QString &service) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(service));
  stubMethodEntered("launchMApplication",params);
}

void LauncherStub::activateLauncher() {
  stubMethodEntered("activateLauncher");
}

void LauncherStub::updateButtonList() {
  stubMethodEntered("updateButtonList");
}

LauncherButton * LauncherStub::createLauncherButton(const MDesktopEntry &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MDesktopEntry & >(entry));
  stubMethodEntered("createLauncherButton",params);
  return stubReturnValue<LauncherButton *>("createLauncherButton");
}

void LauncherStub::connectLauncherButton(LauncherButton *launcherButton) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherButton * >(launcherButton));
  stubMethodEntered("connectLauncherButton",params);
}

bool LauncherStub::contains(const QString &desktopEntryFile) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString& >(desktopEntryFile));
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

void LauncherStub::addNewLauncherButton(const MDesktopEntry &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MDesktopEntry & >(entry));
  stubMethodEntered("addNewLauncherButton",params);
}

bool LauncherStub::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MDesktopEntry & >(entry));
  params.append( new Parameter<const QStringList & >(acceptedTypes));
  stubMethodEntered("isDesktopEntryValid",params);
  return stubReturnValue<bool>("isDesktopEntryValid");
}



// 3. CREATE A STUB INSTANCE
LauncherStub gDefaultLauncherStub;
LauncherStub* gLauncherStub = &gDefaultLauncherStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Launcher::Launcher(MWidget* parent) : MWidgetController(parent) {
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

bool Launcher::startMApplication(const QString &serviceName) {
  return gLauncherStub->startMApplication(serviceName);
}

void Launcher::updateButtonListFromDirectory(const QString &path) {
  gLauncherStub->updateButtonListFromDirectory(path);
}

void Launcher::launchApplication(const QString &application) {
  gLauncherStub->launchApplication(application);
}

void Launcher::launchMApplication(const QString &service) {
  gLauncherStub->launchMApplication(service);
}

void Launcher::activateLauncher() {
  gLauncherStub->activateLauncher();
}

void Launcher::updateButtonList() {
  gLauncherStub->updateButtonList();
}

LauncherButton * Launcher::createLauncherButton(const MDesktopEntry &entry) {
  return gLauncherStub->createLauncherButton(entry);
}

void Launcher::connectLauncherButton(LauncherButton *launcherButton) {
  gLauncherStub->connectLauncherButton(launcherButton);
}

bool Launcher::contains(const QString &desktopEntryFile) {
  return gLauncherStub->contains(desktopEntryFile);
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

void Launcher::addNewLauncherButton(const MDesktopEntry &entry) {
  gLauncherStub->addNewLauncherButton(entry);
}

bool Launcher::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes) {
  return gLauncherStub->isDesktopEntryValid(entry, acceptedTypes);
}


#endif
