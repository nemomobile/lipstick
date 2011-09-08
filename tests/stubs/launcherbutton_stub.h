#ifndef LAUNCHERBUTTON_STUB
#define LAUNCHERBUTTON_STUB

#include "launcherbutton.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherButtonStub : public StubBase {
  public:
  virtual void LauncherButtonConstructor(const QSharedPointer<MDesktopEntry> &entry, MWidget *parent, LauncherButtonModel *model);
  virtual void LauncherButtonDestructor();
  virtual QString desktopEntry() const;
  virtual LauncherButtonModel::State buttonState() const;
  virtual void updateFromDesktopEntry(const QSharedPointer<MDesktopEntry> &entry);
  virtual void setState(LauncherButtonModel::State state);
  virtual void setOperationProgress(int newProgress, int total);
  virtual int operationProgress() const;
  virtual void setPackageName(const QString &packageName);
  virtual QString packageName() const;
  virtual void setPackageRemovable(const bool removable);
  virtual bool packageRemovable() const;
  virtual void retranslateUi();
  virtual void enableLaunchingState();
  virtual void disableLaunchingState();
  virtual void updateData(const QList<const char *> &modifications);
  virtual void launch();
  virtual void windowOnTopOfHome(const WindowInfo &window);
  virtual void init();
};

// 2. IMPLEMENT STUB
void LauncherButtonStub::LauncherButtonConstructor(const QSharedPointer<MDesktopEntry> &entry, MWidget *parent, LauncherButtonModel *model) {
  Q_UNUSED(entry);
  Q_UNUSED(parent);
  Q_UNUSED(model);
}

void LauncherButtonStub::LauncherButtonDestructor() {

}
QString LauncherButtonStub::desktopEntry() const {
  stubMethodEntered("desktopEntry");
  return stubReturnValue<QString>("desktopEntry");
}

LauncherButtonModel::State LauncherButtonStub::buttonState() const {
  stubMethodEntered("buttonState");
  return stubReturnValue<LauncherButtonModel::State>("buttonState");
}

void LauncherButtonStub::updateFromDesktopEntry(const QSharedPointer<MDesktopEntry> &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QSharedPointer<MDesktopEntry> >(entry));
  stubMethodEntered("updateFromDesktopEntry",params);
}

void LauncherButtonStub::setState(LauncherButtonModel::State state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherButtonModel::State >(state));
  stubMethodEntered("setState",params);
}

void LauncherButtonStub::setOperationProgress(int newProgress, int total) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(newProgress));
  params.append( new Parameter<int >(total));
  stubMethodEntered("setOperationProgress",params);
}

int LauncherButtonStub::operationProgress() const {
  stubMethodEntered("operationProgress");
  return stubReturnValue<int>("operationProgress");
}

void LauncherButtonStub::setPackageName(const QString &packageName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(packageName));
  stubMethodEntered("setPackageName",params);
}

QString LauncherButtonStub::packageName() const {
  stubMethodEntered("packageName");
  return stubReturnValue<QString>("packageName");
}

void LauncherButtonStub::setPackageRemovable(const bool removable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool>(removable));
  stubMethodEntered("setPackageRemovable",params);
}

bool LauncherButtonStub::packageRemovable() const {
  stubMethodEntered("packageRemovable");
  return stubReturnValue<bool>("packageRemovable");
}

void LauncherButtonStub::retranslateUi() {
  stubMethodEntered("retranslateUi");
}

void LauncherButtonStub::enableLaunchingState() {
  stubMethodEntered("enableLaunchingState");
}

void LauncherButtonStub::disableLaunchingState() {
  stubMethodEntered("disableLaunchingState");
}

void LauncherButtonStub::updateData(const QList<const char *> &modifications) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<const char *> & >(modifications));
  stubMethodEntered("updateData",params);
}

void LauncherButtonStub::launch() {
  stubMethodEntered("launch");
}

void LauncherButtonStub::windowOnTopOfHome(const WindowInfo &window) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const WindowInfo & >(window));
  stubMethodEntered("windowOnTopOfHome",params);
}

void LauncherButtonStub::init() {
  stubMethodEntered("init");
}



// 3. CREATE A STUB INSTANCE
LauncherButtonStub gDefaultLauncherButtonStub;
LauncherButtonStub* gLauncherButtonStub = &gDefaultLauncherButtonStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherButton::LauncherButton(const QSharedPointer<MDesktopEntry> &entry, MWidget *parent, LauncherButtonModel *model) {
  gLauncherButtonStub->LauncherButtonConstructor(entry, parent, model);
}

LauncherButton::~LauncherButton() {
  gLauncherButtonStub->LauncherButtonDestructor();
}

QString LauncherButton::desktopEntry() const {
  return gLauncherButtonStub->desktopEntry();
}

LauncherButtonModel::State LauncherButton::buttonState() const {
  return gLauncherButtonStub->buttonState();
}

void LauncherButton::updateFromDesktopEntry(const QSharedPointer<MDesktopEntry> &entry) {
  gLauncherButtonStub->updateFromDesktopEntry(entry);
}

void LauncherButton::setState(LauncherButtonModel::State state) {
  gLauncherButtonStub->setState(state);
}

void LauncherButton::setOperationProgress(int newProgress, int total) {
  gLauncherButtonStub->setOperationProgress(newProgress, total);
}

int LauncherButton::operationProgress() const {
  return gLauncherButtonStub->operationProgress();
}

void LauncherButton::setPackageName(const QString &packageName) {
  gLauncherButtonStub->setPackageName(packageName);
}

QString LauncherButton::packageName() const {
  return gLauncherButtonStub->packageName();
}

void LauncherButton::setPackageRemovable(const bool removable) {
  gLauncherButtonStub->setPackageRemovable(removable);
}

bool LauncherButton::packageRemovable() const {
  return gLauncherButtonStub->packageRemovable();
}

void LauncherButton::retranslateUi() {
  gLauncherButtonStub->retranslateUi();
}

void LauncherButton::enableLaunchingState() {
  gLauncherButtonStub->enableLaunchingState();
}

void LauncherButton::disableLaunchingState() {
  gLauncherButtonStub->disableLaunchingState();
}

void LauncherButton::updateData(const QList<const char *> &modifications) {
  gLauncherButtonStub->updateData(modifications);
}

void LauncherButton::launch() {
  gLauncherButtonStub->launch();
}

void LauncherButton::windowOnTopOfHome(const WindowInfo &window) {
  gLauncherButtonStub->windowOnTopOfHome(window);
}

void LauncherButton::init() {
  gLauncherButtonStub->init();
}


#endif
