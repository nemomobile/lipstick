#ifndef LIPSTICKCOMPOSITOR_STUB
#define LIPSTICKCOMPOSITOR_STUB

#include "lipstickcompositor.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LipstickCompositorStub : public StubBase {
  public:
  virtual void LipstickCompositorConstructor();
  virtual void LipstickCompositorDestructor();
  virtual LipstickCompositor * instance();
  virtual void classBegin();
  virtual void componentComplete();
  virtual void surfaceCreated(QWaylandSurface *surface);
  virtual void openUrl(WaylandClient *, const QUrl &);
  virtual void openUrl(const QUrl &);
  virtual int windowCount() const;
  virtual int ghostWindowCount() const;
  virtual bool homeActive() const;
  virtual void setHomeActive(bool);
  virtual void setFullscreenSurface(QWaylandSurface *surface);
  virtual void setTopmostWindowId(int id);
  virtual void setScreenOrientation(Qt::ScreenOrientation screenOrientation);
  virtual bool debug() const;
  virtual QObject * windowForId(int) const;
  virtual void closeClientForWindowId(int);
  virtual void clearKeyboardFocus();
  virtual void setDisplayOff();
  virtual LipstickCompositorProcWindow * mapProcWindow(const QString &title, const QString &category, const QRect &);
  virtual QWaylandSurface * surfaceForId(int) const;
  virtual bool event(QEvent *);
  virtual void surfaceAboutToBeDestroyed(QWaylandSurface *surface);
  virtual void clearUpdateRequest();
  virtual void maybePostUpdateRequest();
  virtual void surfaceMapped();
  virtual void surfaceUnmapped();
  virtual void surfaceSizeChanged();
  virtual void surfaceTitleChanged();
  virtual void surfaceRaised();
  virtual void surfaceLowered();
  virtual void surfaceDamaged(const QRect &);
  virtual void windowSwapped();
  virtual void windowDestroyed();
  virtual void windowPropertyChanged(const QString &);
  virtual void reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState);
}; 

// 2. IMPLEMENT STUB
void LipstickCompositorStub::LipstickCompositorConstructor() {

}
void LipstickCompositorStub::LipstickCompositorDestructor() {

}
LipstickCompositor * LipstickCompositorStub::instance() {
  stubMethodEntered("instance");
  return stubReturnValue<LipstickCompositor *>("instance");
}

void LipstickCompositorStub::classBegin() {
  stubMethodEntered("classBegin");
}

void LipstickCompositorStub::componentComplete() {
  stubMethodEntered("componentComplete");
}

void LipstickCompositorStub::surfaceCreated(QWaylandSurface *surface) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QWaylandSurface * >(surface));
  stubMethodEntered("surfaceCreated",params);
}

void LipstickCompositorStub::openUrl(WaylandClient *client, const QUrl &url) {
    QList<ParameterBase*> params;
    params.append( new Parameter<WaylandClient * >(client));
    params.append( new Parameter<const QUrl &>(url));
    stubMethodEntered("openUrl",params);
}

void LipstickCompositorStub::openUrl(const QUrl &url) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const QUrl &>(url));
    stubMethodEntered("openUrl",params);
}

int LipstickCompositorStub::windowCount() const {
  stubMethodEntered("windowCount");
  return stubReturnValue<int>("windowCount");
}

int LipstickCompositorStub::ghostWindowCount() const {
  stubMethodEntered("ghostWindowCount");
  return stubReturnValue<int>("ghostWindowCount");
}

bool LipstickCompositorStub::homeActive() const {
  stubMethodEntered("homeActive");
  return stubReturnValue<bool>("homeActive");
}

void LipstickCompositorStub::setHomeActive(bool homeActive) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(homeActive));
  stubMethodEntered("setHomeActive",params);
}

void LipstickCompositorStub::setFullscreenSurface(QWaylandSurface *surface) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QWaylandSurface * >(surface));
  stubMethodEntered("setFullscreenSurface",params);
}

void LipstickCompositorStub::setTopmostWindowId(int id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(id));
  stubMethodEntered("setTopmostWindowId",params);
}

void LipstickCompositorStub::setScreenOrientation(Qt::ScreenOrientation screenOrientation) {
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::ScreenOrientation >(screenOrientation));
  stubMethodEntered("setScreenOrientation",params);
}

bool LipstickCompositorStub::debug() const {
  stubMethodEntered("debug");
  return stubReturnValue<bool>("debug");
}

QObject * LipstickCompositorStub::windowForId(int id) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(id));
  stubMethodEntered("windowForId",params);
  return stubReturnValue<QObject *>("windowForId");
}

void LipstickCompositorStub::closeClientForWindowId(int id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(id));
  stubMethodEntered("closeClientForWindowId",params);
}

void LipstickCompositorStub::clearKeyboardFocus() {
  stubMethodEntered("clearKeyboardFocus");
}

void LipstickCompositorStub::setDisplayOff() {
  stubMethodEntered("setDisplayOff");
}

LipstickCompositorProcWindow * LipstickCompositorStub::mapProcWindow(const QString &title, const QString &category, const QRect &rect) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(title));
  params.append( new Parameter<const QString & >(category));
  params.append( new Parameter<const QRect & >(rect));
  stubMethodEntered("mapProcWindow",params);
  return stubReturnValue<LipstickCompositorProcWindow *>("mapProcWindow");
}

QWaylandSurface * LipstickCompositorStub::surfaceForId(int id) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(id));
  stubMethodEntered("surfaceForId",params);
  return stubReturnValue<QWaylandSurface *>("surfaceForId");
}

bool LipstickCompositorStub::event(QEvent *e) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent * >(e));
  stubMethodEntered("event",params);
  return stubReturnValue<QEvent *>("event");
}

void LipstickCompositorStub::surfaceAboutToBeDestroyed(QWaylandSurface *surface) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QWaylandSurface * >(surface));
  stubMethodEntered("surfaceAboutToBeDestroyed",params);
}

void LipstickCompositorStub::clearUpdateRequest() {
  stubMethodEntered("clearUpdateRequest");
}

void LipstickCompositorStub::maybePostUpdateRequest() {
  stubMethodEntered("maybePostUpdateRequest");
}

void LipstickCompositorStub::surfaceMapped() {
  stubMethodEntered("surfaceMapped");
}

void LipstickCompositorStub::surfaceUnmapped() {
  stubMethodEntered("surfaceUnmapped");
}

void LipstickCompositorStub::surfaceSizeChanged() {
  stubMethodEntered("surfaceSizeChanged");
}

void LipstickCompositorStub::surfaceTitleChanged() {
  stubMethodEntered("surfaceTitleChanged");
}

void LipstickCompositorStub::surfaceRaised() {
  stubMethodEntered("surfaceRaised");
}

void LipstickCompositorStub::surfaceLowered() {
  stubMethodEntered("surfaceLowered");
}

void LipstickCompositorStub::surfaceDamaged(const QRect &rect) {
    QList<ParameterBase*> params;
    params.append( new Parameter<QRect>(rect));
    stubMethodEntered("surfaceDamaged",params);
}

void LipstickCompositorStub::windowSwapped() {
  stubMethodEntered("windowSwapped");
}

void LipstickCompositorStub::windowDestroyed() {
  stubMethodEntered("windowDestroyed");
}

void LipstickCompositorStub::windowPropertyChanged(const QString &property) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(property));
  stubMethodEntered("windowPropertyChanged",params);
}

void LipstickCompositorStub::reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmDisplayState::DisplayState >(state));
  stubMethodEntered("reactOnDisplayStateChanges",params);
}



// 3. CREATE A STUB INSTANCE
LipstickCompositorStub gDefaultLipstickCompositorStub;
LipstickCompositorStub* gLipstickCompositorStub = &gDefaultLipstickCompositorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LipstickCompositor::LipstickCompositor() {
  gLipstickCompositorStub->LipstickCompositorConstructor();
}

LipstickCompositor::~LipstickCompositor() {
  gLipstickCompositorStub->LipstickCompositorDestructor();
}

LipstickCompositor * LipstickCompositor::instance() {
  return gLipstickCompositorStub->instance();
}

void LipstickCompositor::classBegin() {
  gLipstickCompositorStub->classBegin();
}

void LipstickCompositor::componentComplete() {
  gLipstickCompositorStub->componentComplete();
}

void LipstickCompositor::surfaceCreated(QWaylandSurface *surface) {
  gLipstickCompositorStub->surfaceCreated(surface);
}

void LipstickCompositor::openUrl(WaylandClient *client, const QUrl &url) {
  gLipstickCompositorStub->openUrl(client, url);
}

void LipstickCompositor::openUrl(const QUrl &url) {
  gLipstickCompositorStub->openUrl(url);
}

int LipstickCompositor::windowCount() const {
  return gLipstickCompositorStub->windowCount();
}

int LipstickCompositor::ghostWindowCount() const {
  return gLipstickCompositorStub->ghostWindowCount();
}

bool LipstickCompositor::homeActive() const {
  return gLipstickCompositorStub->homeActive();
}

void LipstickCompositor::setHomeActive(bool homeActive) {
  gLipstickCompositorStub->setHomeActive(homeActive);
}

void LipstickCompositor::setFullscreenSurface(QWaylandSurface *surface) {
  gLipstickCompositorStub->setFullscreenSurface(surface);
}

void LipstickCompositor::setTopmostWindowId(int id) {
  gLipstickCompositorStub->setTopmostWindowId(id);
}

void LipstickCompositor::setScreenOrientation(Qt::ScreenOrientation screenOrientation) {
  gLipstickCompositorStub->setScreenOrientation(screenOrientation);
}

bool LipstickCompositor::debug() const {
  return gLipstickCompositorStub->debug();
}

QObject * LipstickCompositor::windowForId(int id) const {
  return gLipstickCompositorStub->windowForId(id);
}

void LipstickCompositor::closeClientForWindowId(int id) {
  gLipstickCompositorStub->closeClientForWindowId(id);
}

void LipstickCompositor::clearKeyboardFocus() {
  gLipstickCompositorStub->clearKeyboardFocus();
}

void LipstickCompositor::setDisplayOff() {
  gLipstickCompositorStub->setDisplayOff();
}

LipstickCompositorProcWindow * LipstickCompositor::mapProcWindow(const QString &title, const QString &category, const QRect &rect) {
  return gLipstickCompositorStub->mapProcWindow(title, category, rect);
}

QWaylandSurface * LipstickCompositor::surfaceForId(int id) const {
  return gLipstickCompositorStub->surfaceForId(id);
}

bool LipstickCompositor::event(QEvent *e) {
    return gLipstickCompositorStub->event(e);
}

void LipstickCompositor::surfaceAboutToBeDestroyed(QWaylandSurface *surface) {
  gLipstickCompositorStub->surfaceAboutToBeDestroyed(surface);
}

void LipstickCompositor::clearUpdateRequest() {
    gLipstickCompositorStub->clearUpdateRequest();
}

void LipstickCompositor::maybePostUpdateRequest() {
    gLipstickCompositorStub->maybePostUpdateRequest();
}

void LipstickCompositor::surfaceMapped() {
  gLipstickCompositorStub->surfaceMapped();
}

void LipstickCompositor::surfaceUnmapped() {
  gLipstickCompositorStub->surfaceUnmapped();
}

void LipstickCompositor::surfaceSizeChanged() {
  gLipstickCompositorStub->surfaceSizeChanged();
}

void LipstickCompositor::surfaceTitleChanged() {
  gLipstickCompositorStub->surfaceTitleChanged();
}

void LipstickCompositor::surfaceRaised() {
  gLipstickCompositorStub->surfaceRaised();
}

void LipstickCompositor::surfaceLowered() {
  gLipstickCompositorStub->surfaceLowered();
}

void LipstickCompositor::surfaceDamaged(const QRect &rect) {
  gLipstickCompositorStub->surfaceDamaged(rect);
}

void LipstickCompositor::windowSwapped() {
  gLipstickCompositorStub->windowSwapped();
}

void LipstickCompositor::windowDestroyed() {
  gLipstickCompositorStub->windowDestroyed();
}

void LipstickCompositor::windowPropertyChanged(const QString &property) {
  gLipstickCompositorStub->windowPropertyChanged(property);
}

void LipstickCompositor::reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState state) {
  gLipstickCompositorStub->reactOnDisplayStateChanges(state);
}

QWaylandCompositor::QWaylandCompositor(QWindow *, const char *)
{
}

#endif
