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
  virtual bool openUrl(WaylandClient *, const QUrl &);
  virtual bool openUrl(const QUrl &);
  virtual void retainedSelectionReceived(QMimeData *mimeData);
  virtual int windowCount() const;
  virtual int ghostWindowCount() const;
  virtual bool homeActive() const;
  virtual void setHomeActive(bool);
  virtual void setFullscreenSurface(QWaylandSurface *surface);
  virtual void setTopmostWindowId(int id);
  virtual void setScreenOrientation(Qt::ScreenOrientation screenOrientation);
  virtual QObject *clipboard() const;
  virtual bool debug() const;
  virtual QObject * windowForId(int) const;
  virtual void closeClientForWindowId(int);
  virtual void clearKeyboardFocus();
  virtual void setDisplayOff();
  virtual LipstickCompositorProcWindow * mapProcWindow(const QString &title, const QString &category, const QRect &);
  virtual QWaylandSurface * surfaceForId(int) const;
  virtual void surfaceMapped();
  virtual void surfaceUnmapped();
  virtual void surfaceSizeChanged();
  virtual void surfaceTitleChanged();
  virtual void surfaceRaised();
  virtual void surfaceLowered();
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
  virtual void surfaceDamaged(const QRegion &);
#else
  virtual void surfaceDamaged(const QRect &);
#endif
  virtual void windowSwapped();
  virtual void windowDestroyed();
  virtual void windowPropertyChanged(const QString &);
  virtual void reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState);
  virtual void setScreenOrientationFromSensor();
  virtual void clipboardDataChanged();
  virtual void onVisibleChanged(bool visible);
  virtual QWaylandSurfaceView *createView(QWaylandSurface *surf);
  virtual void onSurfaceDying();
  virtual void readContent();
  virtual void initialize();
  virtual bool completed();
  virtual void timerEvent(QTimerEvent *e);
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

bool LipstickCompositorStub::openUrl(WaylandClient *client, const QUrl &url) {
    QList<ParameterBase*> params;
    params.append( new Parameter<WaylandClient * >(client));
    params.append( new Parameter<const QUrl &>(url));
    stubMethodEntered("openUrl", params);
    return stubReturnValue<bool>("openUrl");
}

bool LipstickCompositorStub::openUrl(const QUrl &url) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const QUrl &>(url));
    stubMethodEntered("openUrl",params);
    return stubReturnValue<bool>("openUrl");
}

void LipstickCompositorStub::retainedSelectionReceived(QMimeData *mimeData) {
  QList<ParameterBase*> params;
  params.append(new Parameter<QMimeData *>(mimeData));
  stubMethodEntered("retainedSelectionReceived", params);
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

QObject *LipstickCompositorStub::clipboard() const {
  stubMethodEntered("clipboard");
  return stubReturnValue<QObject *>("clipboard");
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

void LipstickCompositorStub::onSurfaceDying() {
    stubMethodEntered("onSurfaceDying");
}

void LipstickCompositorStub::readContent() {
    stubMethodEntered("readContent");
}

void LipstickCompositorStub::initialize() {
    stubMethodEntered("initialize");
}

bool LipstickCompositorStub::completed() {
    stubMethodEntered("completed");
    return true;
}

void LipstickCompositorStub::timerEvent(QTimerEvent *e)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<QTimerEvent *>(e));
    stubMethodEntered("timerEvent", params);
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
void LipstickCompositorStub::surfaceDamaged(const QRegion &rect) {
    QList<ParameterBase*> params;
    params.append( new Parameter<QRegion>(rect));
    stubMethodEntered("surfaceDamaged",params);
}
#else
void LipstickCompositorStub::surfaceDamaged(const QRect &rect) {
    QList<ParameterBase*> params;
    params.append( new Parameter<QRect>(rect));
    stubMethodEntered("surfaceDamaged",params);
}
#endif

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

void LipstickCompositorStub::setScreenOrientationFromSensor( ) {
  stubMethodEntered("setScreenOrientationFromSensor");
}

void LipstickCompositorStub::clipboardDataChanged() {
  stubMethodEntered("clipboardDataChanged");
}

void LipstickCompositorStub::onVisibleChanged(bool v) {
    QList<ParameterBase*> params;
    params.append( new Parameter<bool>(v));
    stubMethodEntered("onVisibleChanged", params);
}

QWaylandSurfaceView *LipstickCompositorStub::createView(QWaylandSurface *surf)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<QWaylandSurface *>(surf));
    stubMethodEntered("createView", params);
    return stubReturnValue<QWaylandSurfaceView *>("createView");
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

bool LipstickCompositor::openUrl(WaylandClient *client, const QUrl &url) {
  return gLipstickCompositorStub->openUrl(client, url);
}

bool LipstickCompositor::openUrl(const QUrl &url) {
  return gLipstickCompositorStub->openUrl(url);
}

void LipstickCompositor::retainedSelectionReceived(QMimeData *mimeData)
{
  gLipstickCompositorStub->retainedSelectionReceived(mimeData);
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

QObject *LipstickCompositor::clipboard() const {
  return gLipstickCompositorStub->clipboard();
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

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
void LipstickCompositor::surfaceDamaged(const QRegion &rect) {
  gLipstickCompositorStub->surfaceDamaged(rect);
}
#else
void LipstickCompositor::surfaceDamaged(const QRect &rect) {
  gLipstickCompositorStub->surfaceDamaged(rect);
}
#endif

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

void LipstickCompositor::homeApplicationAboutToDestroy() {
}

void LipstickCompositor::setScreenOrientationFromSensor() {
  gLipstickCompositorStub->setScreenOrientationFromSensor();
}

void LipstickCompositor::clipboardDataChanged() {
  gLipstickCompositorStub->clipboardDataChanged();
}

void LipstickCompositor::onVisibleChanged(bool v) {
  gLipstickCompositorStub->onVisibleChanged(v);
}

QWaylandSurfaceView *LipstickCompositor::createView(QWaylandSurface *surf) {
  return gLipstickCompositorStub->createView(surf);
}

void LipstickCompositor::onSurfaceDying() {
    gLipstickCompositorStub->onSurfaceDying();
}

void LipstickCompositor::readContent() {
    gLipstickCompositorStub->readContent();
}

void LipstickCompositor::initialize() {
    gLipstickCompositorStub->initialize();
}

bool LipstickCompositor::completed() {
    return gLipstickCompositorStub->completed();
}

void LipstickCompositor::timerEvent(QTimerEvent *e)
{
    gLipstickCompositorStub->timerEvent(e);
}

void LipstickCompositor::setKeyboardLayout(const QString &) {
}

QString LipstickCompositor::keyboardLayout() const {
    return QString();
}

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
QWaylandCompositor::QWaylandCompositor(QWindow *, const char *)
#else
QWaylandCompositor::QWaylandCompositor(QWindow *, const char *, QWaylandCompositor::ExtensionFlags)
#endif
{
}

QWaylandQuickCompositor::QWaylandQuickCompositor(QQuickWindow *, const char *, QWaylandCompositor::ExtensionFlags)
{
}

#endif
