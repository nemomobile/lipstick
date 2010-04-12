#ifndef MSCENEMANAGER_STUB
#define MSCENEMANAGER_STUB

#include "mscenemanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSceneManagerStub : public StubBase {
  public:
  virtual void MSceneManagerConstructor(MScene *scene, QObject *parent);
  virtual void MSceneManagerDestructor();
  virtual MScene * scene();
  virtual M::Orientation orientation() const;
  virtual M::OrientationAngle orientationAngle() const;
  virtual QSize visibleSceneSize(M::Orientation orientation) const;
  virtual QSize visibleSceneSize() const;
  virtual void setOrientationAngle(M::OrientationAngle angle, MSceneManager::TransitionMode mode = MSceneManager::AnimatedTransition);
  virtual void requestSoftwareInputPanel(QGraphicsWidget *inputWidget);
  virtual void closeSoftwareInputPanel();
  virtual void appearSceneWindow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy);
  virtual void appearSceneWindowNow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy);
  virtual int execDialog(MDialog *dialog);
  virtual void disappearSceneWindow(MSceneWindow *window);
  virtual void disappearSceneWindowNow(MSceneWindow *window);
  virtual void dismissSceneWindow(MSceneWindow *window);
  virtual void dismissSceneWindowNow(MSceneWindow *window);
  virtual void orientationAboutToChange(M::Orientation orientation);
  virtual void orientationChanged(M::Orientation orientation);
  virtual void orientationChangeFinished(M::Orientation orientation);
  virtual bool eventFilter(QObject *watched, QEvent *event);
}; 

// 2. IMPLEMENT STUB
void MSceneManagerStub::MSceneManagerConstructor(MScene *scene, QObject *parent) {
  Q_UNUSED(scene);
  Q_UNUSED(parent);

}
void MSceneManagerStub::MSceneManagerDestructor() {

}
MScene * MSceneManagerStub::scene() {
  stubMethodEntered("scene");
  return stubReturnValue<MScene *>("scene");
}

M::Orientation MSceneManagerStub::orientation() const {
  stubMethodEntered("orientation");
  return stubReturnValue<M::Orientation>("orientation");
}

M::OrientationAngle MSceneManagerStub::orientationAngle() const {
  stubMethodEntered("orientationAngle");
  return stubReturnValue<M::OrientationAngle>("orientationAngle");
}

QSize MSceneManagerStub::visibleSceneSize(M::Orientation orientation) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<M::Orientation >(orientation));
  stubMethodEntered("visibleSceneSize",params);
  return stubReturnValue<QSize>("visibleSceneSize");
}

QSize MSceneManagerStub::visibleSceneSize() const {
  stubMethodEntered("visibleSceneSize");
  return stubReturnValue<QSize>("visibleSceneSize");
}

void MSceneManagerStub::setOrientationAngle(M::OrientationAngle angle, MSceneManager::TransitionMode mode) {
  QList<ParameterBase*> params;
  params.append( new Parameter<M::OrientationAngle >(angle));
  params.append( new Parameter<MSceneManager::TransitionMode >(mode));
  stubMethodEntered("setOrientationAngle",params);
}

void MSceneManagerStub::requestSoftwareInputPanel(QGraphicsWidget *inputWidget) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsWidget * >(inputWidget));
  stubMethodEntered("requestSoftwareInputPanel",params);
}

void MSceneManagerStub::closeSoftwareInputPanel() {
  stubMethodEntered("closeSoftwareInputPanel");
}

void MSceneManagerStub::appearSceneWindow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MSceneWindow * >(window));
  params.append( new Parameter<MSceneWindow::DeletionPolicy >(policy));
  stubMethodEntered("appearSceneWindow",params);
}

void MSceneManagerStub::appearSceneWindowNow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MSceneWindow * >(window));
  params.append( new Parameter<MSceneWindow::DeletionPolicy >(policy));
  stubMethodEntered("appearSceneWindowNow",params);
}

int MSceneManagerStub::execDialog(MDialog *dialog) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MDialog * >(dialog));
  stubMethodEntered("execDialog",params);
  return stubReturnValue<int>("execDialog");
}

void MSceneManagerStub::disappearSceneWindow(MSceneWindow *window) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MSceneWindow * >(window));
  stubMethodEntered("disappearSceneWindow",params);
}

void MSceneManagerStub::disappearSceneWindowNow(MSceneWindow *window) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MSceneWindow * >(window));
  stubMethodEntered("disappearSceneWindowNow",params);
}

void MSceneManagerStub::dismissSceneWindow(MSceneWindow *window) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MSceneWindow * >(window));
  stubMethodEntered("dismissSceneWindow",params);
}

void MSceneManagerStub::dismissSceneWindowNow(MSceneWindow *window) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MSceneWindow * >(window));
  stubMethodEntered("dismissSceneWindowNow",params);
}

void MSceneManagerStub::orientationAboutToChange(M::Orientation orientation) {
  QList<ParameterBase*> params;
  params.append( new Parameter<M::Orientation >(orientation));
  stubMethodEntered("orientationAboutToChange",params);
}

void MSceneManagerStub::orientationChanged(M::Orientation orientation) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const M::Orientation >(orientation));
  stubMethodEntered("orientationChanged",params);
}

void MSceneManagerStub::orientationChangeFinished(M::Orientation orientation) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const M::Orientation >(orientation));
  stubMethodEntered("orientationChangeFinished",params);
}

bool MSceneManagerStub::eventFilter(QObject *watched, QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QObject * >(watched));
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("eventFilter",params);
  return stubReturnValue<bool>("eventFilter");
}



// 3. CREATE A STUB INSTANCE
MSceneManagerStub gDefaultMSceneManagerStub;
MSceneManagerStub* gMSceneManagerStub = &gDefaultMSceneManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSceneManager::MSceneManager(MScene *scene, QObject *parent) : d_ptr(NULL) {
  gMSceneManagerStub->MSceneManagerConstructor(scene, parent);
}

MSceneManager::~MSceneManager() {
  gMSceneManagerStub->MSceneManagerDestructor();
}

MScene * MSceneManager::scene() {
  return gMSceneManagerStub->scene();
}

M::Orientation MSceneManager::orientation() const {
  return gMSceneManagerStub->orientation();
}

M::OrientationAngle MSceneManager::orientationAngle() const {
  return gMSceneManagerStub->orientationAngle();
}

QSize MSceneManager::visibleSceneSize(M::Orientation orientation) const {
  return gMSceneManagerStub->visibleSceneSize(orientation);
}

QSize MSceneManager::visibleSceneSize() const {
  return gMSceneManagerStub->visibleSceneSize();
}

void MSceneManager::setOrientationAngle(M::OrientationAngle angle, MSceneManager::TransitionMode mode) 
{
  gMSceneManagerStub->setOrientationAngle(angle, mode);
}

void MSceneManager::requestSoftwareInputPanel(QGraphicsWidget *inputWidget) {
  gMSceneManagerStub->requestSoftwareInputPanel(inputWidget);
}

void MSceneManager::closeSoftwareInputPanel() {
  gMSceneManagerStub->closeSoftwareInputPanel();
}

void MSceneManager::appearSceneWindow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy) {
  gMSceneManagerStub->appearSceneWindow(window, policy);
}

void MSceneManager::appearSceneWindowNow(MSceneWindow *window, MSceneWindow::DeletionPolicy policy) {
  gMSceneManagerStub->appearSceneWindowNow(window, policy);
}

int MSceneManager::execDialog(MDialog *dialog) {
  return gMSceneManagerStub->execDialog(dialog);
}

void MSceneManager::disappearSceneWindow(MSceneWindow *window) {
  gMSceneManagerStub->disappearSceneWindow(window);
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *window) {
  gMSceneManagerStub->disappearSceneWindowNow(window);
}

void MSceneManager::dismissSceneWindow(MSceneWindow *window) {
  gMSceneManagerStub->dismissSceneWindow(window);
}

void MSceneManager::dismissSceneWindowNow(MSceneWindow *window) {
  gMSceneManagerStub->dismissSceneWindowNow(window);
}

void MSceneManager::orientationAboutToChange(M::Orientation orientation) {
  gMSceneManagerStub->orientationAboutToChange(orientation);
}

void MSceneManager::orientationChanged(M::Orientation orientation) {
  gMSceneManagerStub->orientationChanged(orientation);
}

void MSceneManager::orientationChangeFinished(M::Orientation orientation) {
  gMSceneManagerStub->orientationChangeFinished(orientation);
}

bool MSceneManager::eventFilter(QObject *watched, QEvent *event) {
  return gMSceneManagerStub->eventFilter(watched, event);
}


#endif
