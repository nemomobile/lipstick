#ifndef SWITCHERVIEW_STUB
#define SWITCHERVIEW_STUB

#include "switcherview.h"
#include <stubbase.h>


// 1. DECLARE STUB
class SwitcherViewStub : public StubBase {
public:
    virtual void SwitcherViewConstructor(Switcher *controller);
    virtual void SwitcherViewDestructor();
    virtual bool event(QEvent *e);
    virtual void updateData(const QList<const char *>& modifications);
    virtual void updateFocusedButton(int currentPage);
    virtual void panningStopped();
    virtual void updateButtons();
    virtual void applyPinchGestureTargetMode();
    virtual void runOverviewBounceAnimation();
};

// 2. IMPLEMENT STUB
void SwitcherViewStub::SwitcherViewConstructor(Switcher *controller)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<Switcher*>(controller));
    stubMethodEntered("SwitcherViewConstructor(Switcher*)", params);
}

void SwitcherViewStub::SwitcherViewDestructor()
{
    stubMethodEntered("SwitcherViewDestructor");
}

bool SwitcherViewStub::event(QEvent *e) {
    QList<ParameterBase*> params;
    params.append( new Parameter<QEvent * >(e));
    stubMethodEntered("event",params);
    return stubReturnValue<bool>("event");
}

void SwitcherViewStub::updateData(const QList<const char *>& modifications)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<QList<const char *> >(modifications));
    stubMethodEntered("updateData", params);
}

void SwitcherViewStub::updateFocusedButton(int currentPage)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<int>(currentPage));
    stubMethodEntered("updateFocusedButton", params);
}

void SwitcherViewStub::panningStopped() {
    stubMethodEntered("panningStopped");
}

void SwitcherViewStub::updateButtons() {
    stubMethodEntered("updateButtons");
}

void SwitcherViewStub::applyPinchGestureTargetMode() {
    stubMethodEntered("applyPinchGestureTargetMode");
}

void SwitcherViewStub::runOverviewBounceAnimation() {
    stubMethodEntered("runOverviewBounceAnimation");
}


// 3. CREATE A STUB INSTANCE
SwitcherViewStub gDefaultSwitcherViewStub;
SwitcherViewStub* gSwitcherViewStub = &gDefaultSwitcherViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
SwitcherViewBase::SwitcherViewBase(Switcher */*controller*/)
{
}

SwitcherViewBase::~SwitcherViewBase() {
}

bool SwitcherViewBase::event(QEvent *e) {
    return gSwitcherViewStub->event(e);
}

void SwitcherViewBase::setupModel() {
}

void SwitcherViewBase::updateData(const QList<const char *>& /*modifications*/)
{
}

void SwitcherViewBase::pinchGestureEvent(QGestureEvent*, QPinchGesture*) {
}

bool SwitcherViewBase::sceneEventFilter(QGraphicsItem*, QEvent*) {
    return true;
}

void SwitcherViewBase::applyPinchGestureTargetMode() {
}

void SwitcherViewBase::runOverviewBounceAnimation() {
    gSwitcherViewStub->runOverviewBounceAnimation();
}

void SwitcherViewBase::applySwitcherMode() {
}

int SwitcherViewBase::buttonIndex(const SwitcherButton *) const {
    return 0;
}

void SwitcherViewBase::calculateNearestButtonAt(const QPointF &) {
}

void SwitcherViewBase::removeButtonsFromLayout() {
}

void SwitcherViewBase::startBounceAnimation() {
}

void SwitcherViewBase::setInwardBounceAnimation(bool) {
}

void SwitcherViewBase::pinchBegin(const QPointF &/*centerPoint*/) {
}

void SwitcherViewBase::pinchUpdate(float /*scaleFactor*/) {
}

void SwitcherViewBase::pinchEnd() {
}

void SwitcherViewBase::endTransition() {
}

void SwitcherViewBase::endBounce() {
}

void SwitcherViewBase::updateAnimationStatus()
{
}

SwitcherView::SwitcherView(Switcher *controller) : SwitcherViewBase(controller)
{
    gSwitcherViewStub->SwitcherViewConstructor(controller);
}

SwitcherView::~SwitcherView() {
    gSwitcherViewStub->SwitcherViewDestructor();
}

void SwitcherView::updateData(const QList<const char *>& modifications)
{
    gSwitcherViewStub->updateData(modifications);
}

void SwitcherView::updateFocusedButton(int currentPage)
{
    gSwitcherViewStub->updateFocusedButton(currentPage);
}

void SwitcherView::panningStopped() {
    gSwitcherViewStub->panningStopped();
}

void SwitcherView::updateButtons() {
    gSwitcherViewStub->updateButtons();
}

void SwitcherView::repositionSwitcher() {
}

void SwitcherView::addButtonInOverviewPolicy(QSharedPointer<SwitcherButton>) {
}

void SwitcherView::updateContentsMarginsAndSpacings() {
}

void SwitcherView::updateDetailViewContentsMarginsAndSpacings() {
}

void SwitcherView::updateOverviewContentsMarginsAndSpacings() {
}

void SwitcherView::updateButtonModesAndPageCount() {
}

int SwitcherView::buttonsPerPage() const {
    return 0;
}

void SwitcherView::applySwitcherMode()
{
}

void SwitcherView::applyPinchGestureTargetMode()
{
    gSwitcherViewStub->applyPinchGestureTargetMode();
}

#endif
