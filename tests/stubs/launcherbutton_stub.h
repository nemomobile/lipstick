#ifndef LAUNCHERBUTTON_STUB
#define LAUNCHERBUTTON_STUB

#include "launcherbutton.h"
#include <stubbase.h>

#include "launcherbuttonmodel.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherButtonStub : public StubBase {
  public:
  virtual void LauncherButtonConstructor(const QString &desktopEntryPath, MWidget *parent, LauncherButtonModel *model);
  virtual void LauncherButtonDestructor();
  virtual QString desktopEntry() const;
  virtual void updateFromDesktopEntry(const QString &desktopEntryPath);
  virtual void retranslateUi();
  virtual void launch();
  virtual void stopLaunchProgress();
  virtual void setState(LauncherButtonModel::State state);
  virtual int operationProgress() const;
  virtual void init();
  virtual LauncherButtonModel::State buttonState() const;
  virtual void setPackageName(const QString packageName);
  virtual QString packageName() const;
  virtual void setPackageRemovable(const bool packageRemovable);
  virtual bool packageRemovable() const;
  virtual void setOperationProgress(int newProgress, int total);
};

// 2. IMPLEMENT STUB
void LauncherButtonStub::LauncherButtonConstructor(const QString &desktopEntryPath, MWidget *parent, LauncherButtonModel *model) {
  Q_UNUSED(desktopEntryPath);
  Q_UNUSED(parent);
  Q_UNUSED(model);
}

void LauncherButtonStub::LauncherButtonDestructor() {

}

QString LauncherButtonStub::desktopEntry() const {
  stubMethodEntered("desktopEntry");
  return stubReturnValue<QString>("desktopEntry");
}

void LauncherButtonStub::updateFromDesktopEntry(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(desktopEntryPath));
  stubMethodEntered("updateFromDesktopEntry",params);
}

void LauncherButtonStub::retranslateUi() {
  stubMethodEntered("retranslateUi");
}

void LauncherButtonStub::launch() {
  stubMethodEntered("launch");
}

void LauncherButtonStub::stopLaunchProgress() {
  stubMethodEntered("stopLaunchProgress");
}

void LauncherButtonStub::init() {
  stubMethodEntered("init");
}

LauncherButtonModel::State LauncherButtonStub::buttonState() const {
  stubMethodEntered("buttonState");
  return stubReturnValue<LauncherButtonModel::State>("buttonState");
}

void LauncherButtonStub::setState(LauncherButtonModel::State state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherButtonModel::State>(state));
  stubMethodEntered("setState",params);
}

int LauncherButtonStub::operationProgress() const {
  stubMethodEntered("operationProgress");
  return stubReturnValue<int>("operationProgress");
}

void LauncherButtonStub::setPackageName(const QString packageName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(packageName));
  stubMethodEntered("setPackageName", params);
}

QString LauncherButtonStub::packageName() const {
  stubMethodEntered("packageName");
  return stubReturnValue<QString>("packageName");
}

void LauncherButtonStub::setPackageRemovable(const bool packageRemovable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool>(packageRemovable));
  stubMethodEntered("setPackageRemovable", params);
}

bool LauncherButtonStub::packageRemovable() const {
  stubMethodEntered("packageRemovable");
  return stubReturnValue<bool>("packageRemovable");
}


void LauncherButtonStub::setOperationProgress(int newProgress, int total)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<int>(newProgress));
  params.append( new Parameter<int>(total));
  stubMethodEntered("setOperationProgress", params);
}

// 3. CREATE A STUB INSTANCE
LauncherButtonStub gDefaultLauncherButtonStub;
LauncherButtonStub* gLauncherButtonStub = &gDefaultLauncherButtonStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherButton::LauncherButton(const QString &desktopEntryPath, MWidget *parent, LauncherButtonModel *model) {
  gLauncherButtonStub->LauncherButtonConstructor(desktopEntryPath, parent, model);
}

LauncherButton::~LauncherButton() {
  gLauncherButtonStub->LauncherButtonDestructor();
}

QString LauncherButton::desktopEntry() const {
  return gLauncherButtonStub->desktopEntry();
}

void LauncherButton::updateFromDesktopEntry(const QString &desktopEntryPath) {
  gLauncherButtonStub->updateFromDesktopEntry(desktopEntryPath);
}

void LauncherButton::retranslateUi() {
  gLauncherButtonStub->retranslateUi();
}

void LauncherButton::launch() {
  gLauncherButtonStub->launch();
}

void LauncherButton::stopLaunchProgress() {
  gLauncherButtonStub->stopLaunchProgress();
}

void LauncherButton::init() {
    gLauncherButtonStub->init();
}

LauncherButtonModel::State LauncherButton::buttonState() const {
    return gLauncherButtonStub->buttonState();
}

void LauncherButton::setState(LauncherButtonModel::State state)
{
    gLauncherButtonStub->setState(state);
}

int LauncherButton::operationProgress() const {
    return gLauncherButtonStub->operationProgress();
}

void LauncherButton::setPackageName(const QString &packageName)
{
    gLauncherButtonStub->setPackageName(packageName);
}

QString LauncherButton::packageName() const
{
    return gLauncherButtonStub->packageName();
}

void LauncherButton::setPackageRemovable(const bool removable)
{
    gLauncherButtonStub->setPackageRemovable(removable);
}

bool LauncherButton::packageRemovable() const
{
    return gLauncherButtonStub->packageRemovable();
}

void LauncherButton::setOperationProgress(int newProgress, int total)
{
    gLauncherButtonStub->setOperationProgress(newProgress, total);
}

#endif
