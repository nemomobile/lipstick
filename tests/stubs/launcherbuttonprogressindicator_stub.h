#ifndef LAUNCHERBUTTONPROGRESSINDICATOR_STUB
#define LAUNCHERBUTTONPROGRESSINDICATOR_STUB

#include "launcherbuttonprogressindicator.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherButtonProgressIndicatorStub : public StubBase {
  public:
  virtual void LauncherButtonProgressIndicatorConstructor(QGraphicsItem *parent);
  virtual void LauncherButtonProgressIndicatorDestructor();
  virtual void setIndicatorState(LauncherButtonModel::State state);
  virtual LauncherButtonModel::State indicatorState();
}; 

// 2. IMPLEMENT STUB
void LauncherButtonProgressIndicatorStub::LauncherButtonProgressIndicatorConstructor(QGraphicsItem *parent) {
    Q_UNUSED(parent);
}

void LauncherButtonProgressIndicatorStub::LauncherButtonProgressIndicatorDestructor() {
}

void LauncherButtonProgressIndicatorStub::setIndicatorState(LauncherButtonModel::State state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherButtonModel::State >(state));
  stubMethodEntered("setIndicatorState",params);
}

LauncherButtonModel::State LauncherButtonProgressIndicatorStub::indicatorState() {
  stubMethodEntered("indicatorState");
  return stubReturnValue<LauncherButtonModel::State>("indicatorState");
}

// 3. CREATE A STUB INSTANCE
LauncherButtonProgressIndicatorStub gDefaultLauncherButtonProgressIndicatorStub;
LauncherButtonProgressIndicatorStub* gLauncherButtonProgressIndicatorStub = &gDefaultLauncherButtonProgressIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherButtonProgressIndicator::LauncherButtonProgressIndicator(QGraphicsItem *parent)
        : MProgressIndicator(parent) {
    gLauncherButtonProgressIndicatorStub->LauncherButtonProgressIndicatorConstructor(parent);
}

LauncherButtonProgressIndicator::~LauncherButtonProgressIndicator() {
    gLauncherButtonProgressIndicatorStub->LauncherButtonProgressIndicatorDestructor();
}

void LauncherButtonProgressIndicator::setIndicatorState(LauncherButtonModel::State state) {
  gLauncherButtonProgressIndicatorStub->setIndicatorState(state);
}

LauncherButtonModel::State LauncherButtonProgressIndicator::indicatorState() {
  return gLauncherButtonProgressIndicatorStub->indicatorState();
}


#endif
