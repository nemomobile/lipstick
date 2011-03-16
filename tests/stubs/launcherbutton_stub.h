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
  virtual void setState(LauncherButtonModel::State state, int progress);
  virtual int operationProgress() const;
  virtual void init();
  virtual LauncherButtonModel::State buttonState() const;
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

void LauncherButtonStub::setState(LauncherButtonModel::State state, int progress) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherButtonModel::State>(state));
  params.append( new Parameter<int>(progress));
  stubMethodEntered("setState",params);
}

int LauncherButtonStub::operationProgress() const {
  stubMethodEntered("operationProgress");
  return stubReturnValue<int>("operationProgress");
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

void LauncherButton::setState(LauncherButtonModel::State state, int progress)
{
    gLauncherButtonStub->setState(state, progress);
}

int LauncherButton::operationProgress() const {
    return gLauncherButtonStub->operationProgress();
}

#endif
