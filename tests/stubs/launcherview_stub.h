#ifndef LAUNCHERVIEW_STUB
#define LAUNCHERVIEW_STUB

#include "launcherview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherViewStub : public StubBase {
  public:
  virtual void LauncherViewDestructor();
  virtual void updateData(const QList<const char *> &modifications);
  virtual void panToPage(uint page);
  virtual void focusToPage(uint page);
  virtual void focusToButton(const QString &desktopFileEntry);
  virtual void updateLayoutFromModel();
  virtual void removePageFromLayout(LauncherPage *page);
};

// 2. IMPLEMENT STUB
void LauncherViewStub::LauncherViewDestructor() {

}
void LauncherViewStub::updateData(const QList<const char *> &modifications) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<const char *> >(modifications));
  stubMethodEntered("updateData",params);
}

void LauncherViewStub::panToPage(uint page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(page));
  stubMethodEntered("panToPage",params);
}

void LauncherViewStub::focusToPage(uint page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(page));
  stubMethodEntered("focusToPage",params);
}

void LauncherViewStub::focusToButton(const QString &desktopFileEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(desktopFileEntry));
  stubMethodEntered("focusToButton",params);
}

void LauncherViewStub::updateLayoutFromModel() {
  stubMethodEntered("updateLayoutFromModel");
}

void LauncherViewStub::removePageFromLayout(LauncherPage *page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherPage * >(page));
  stubMethodEntered("removePageFromLayout",params);
}



// 3. CREATE A STUB INSTANCE
LauncherViewStub gDefaultLauncherViewStub;
LauncherViewStub* gLauncherViewStub = &gDefaultLauncherViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherView::~LauncherView() {
  gLauncherViewStub->LauncherViewDestructor();
}

void LauncherView::updateData(const QList<const char *> &modifications) {
  gLauncherViewStub->updateData(modifications);
}

void LauncherView::panToPage(uint page) {
  gLauncherViewStub->panToPage(page);
}

void LauncherView::focusToPage(uint page) {
  gLauncherViewStub->focusToPage(page);
}

void LauncherView::focusToButton(const QString &desktopFileEntry) {
  gLauncherViewStub->focusToButton(desktopFileEntry);
}

void LauncherView::updateLayoutFromModel() {
  gLauncherViewStub->updateLayoutFromModel();
}

void LauncherView::removePageFromLayout(LauncherPage *page) {
  gLauncherViewStub->removePageFromLayout(page);
}


#endif
