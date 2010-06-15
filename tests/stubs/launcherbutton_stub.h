#ifndef LAUNCHERBUTTON_STUB
#define LAUNCHERBUTTON_STUB

#include "launcherbutton.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherButtonStub : public StubBase {
  public:
  virtual void LauncherButtonConstructor(MWidget *parent);
  virtual void LauncherButtonConstructor(const QString &entry, MWidget *parent);
  virtual void LauncherButtonDestructor();
  virtual void setAction(const LauncherAction &action);
  virtual LauncherAction action() const;
  virtual QString desktopEntry() const;
  virtual void launch();
  virtual void updateFromDesktopEntry(const QString &entry);
  virtual void retranslateUi();
}; 


// 2. IMPLEMENT STUB
void LauncherButtonStub::LauncherButtonConstructor(MWidget *parent) {
  Q_UNUSED(parent);

}
void LauncherButtonStub::LauncherButtonConstructor(const QString &entry, MWidget *parent) {
  Q_UNUSED(entry);
  Q_UNUSED(parent);

}
void LauncherButtonStub::LauncherButtonDestructor() {
  stubMethodEntered("~LauncherButton");
}

void LauncherButtonStub::setAction(const LauncherAction &action) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherAction >(action));
  stubMethodEntered("setTarget",params);
}

LauncherAction LauncherButtonStub::action() const {
  stubMethodEntered("action");
  return stubReturnValue<LauncherAction>("action");
}

QString LauncherButtonStub::desktopEntry() const {
  stubMethodEntered("desktopEntry");
  return stubReturnValue<QString>("desktopEntry");
}

void LauncherButtonStub::launch() {
  stubMethodEntered("launch");
}

void LauncherButtonStub::updateFromDesktopEntry(const QString &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString &>(entry));
  stubMethodEntered("updateFromDesktopEntry",params);
}

void LauncherButtonStub::retranslateUi()
{
  stubMethodEntered("retranslateUi");
}


// 3. CREATE A STUB INSTANCE
LauncherButtonStub gDefaultLauncherButtonStub;
LauncherButtonStub* gLauncherButtonStub = &gDefaultLauncherButtonStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherButton::LauncherButton(MWidget *parent) {
  gLauncherButtonStub->LauncherButtonConstructor(parent);
}

LauncherButton::LauncherButton(const QString &entry, MWidget *parent) {
  gLauncherButtonStub->LauncherButtonConstructor(entry, parent);
}

LauncherButton::~LauncherButton() {
  gLauncherButtonStub->LauncherButtonDestructor();
}

void LauncherButton::setAction(const LauncherAction &action) {
  gLauncherButtonStub->setAction(action);
}

LauncherAction LauncherButton::action() const {
  return gLauncherButtonStub->action();
}

QString LauncherButton::desktopEntry() const {
  return gLauncherButtonStub->desktopEntry();
}

void LauncherButton::launch() {
  gLauncherButtonStub->launch();
}

void LauncherButton::updateFromDesktopEntry(const QString &entry) {
    gLauncherButtonStub->updateFromDesktopEntry(entry);
}

void LauncherButton::retranslateUi()
{
    gLauncherButtonStub->retranslateUi();
}

#endif
