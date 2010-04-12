#ifndef LAUNCHERBUTTON_STUB
#define LAUNCHERBUTTON_STUB

#include "launcherbutton.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherButtonStub : public StubBase {
  public:
  virtual void LauncherButtonConstructor(MWidget *parent);
  virtual void LauncherButtonConstructor(const MDesktopEntry &entry, MWidget *parent);
  virtual void LauncherButtonDestructor();
  virtual void setTargetType(const QString &type);
  virtual QString targetType() const;
  virtual void setTarget(const QString &target);
  virtual QString target() const;
  virtual QString desktopEntry() const;
  virtual void launch();
}; 

// 2. IMPLEMENT STUB
void LauncherButtonStub::LauncherButtonConstructor(MWidget *parent) {
  Q_UNUSED(parent);

}
void LauncherButtonStub::LauncherButtonConstructor(const MDesktopEntry &entry, MWidget *parent) {
  Q_UNUSED(entry);
  Q_UNUSED(parent);

}
void LauncherButtonStub::LauncherButtonDestructor() {

}
void LauncherButtonStub::setTargetType(const QString &type) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(type));
  stubMethodEntered("setTargetType",params);
}

QString LauncherButtonStub::targetType() const {
  stubMethodEntered("targetType");
  return stubReturnValue<QString>("targetType");
}

void LauncherButtonStub::setTarget(const QString &target) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(target));
  stubMethodEntered("setTarget",params);
}

QString LauncherButtonStub::target() const {
  stubMethodEntered("target");
  return stubReturnValue<QString>("target");
}

QString LauncherButtonStub::desktopEntry() const {
  stubMethodEntered("desktopEntry");
  return stubReturnValue<QString>("desktopEntry");
}

void LauncherButtonStub::launch() {
  stubMethodEntered("launch");
}



// 3. CREATE A STUB INSTANCE
LauncherButtonStub gDefaultLauncherButtonStub;
LauncherButtonStub* gLauncherButtonStub = &gDefaultLauncherButtonStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherButton::LauncherButton(MWidget *parent) {
  gLauncherButtonStub->LauncherButtonConstructor(parent);
}

LauncherButton::LauncherButton(const MDesktopEntry &entry, MWidget *parent) {
  gLauncherButtonStub->LauncherButtonConstructor(entry, parent);
}

LauncherButton::~LauncherButton() {
  gLauncherButtonStub->LauncherButtonDestructor();
}

void LauncherButton::setTargetType(const QString &type) {
  gLauncherButtonStub->setTargetType(type);
}

QString LauncherButton::targetType() const {
  return gLauncherButtonStub->targetType();
}

void LauncherButton::setTarget(const QString &target) {
  gLauncherButtonStub->setTarget(target);
}

QString LauncherButton::target() const {
  return gLauncherButtonStub->target();
}

QString LauncherButton::desktopEntry() const {
  return gLauncherButtonStub->desktopEntry();
}

void LauncherButton::launch() {
  gLauncherButtonStub->launch();
}


#endif
