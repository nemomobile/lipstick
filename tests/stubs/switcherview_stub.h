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
SwitcherView::SwitcherView(Switcher *controller)
{
    gSwitcherViewStub->SwitcherViewConstructor(controller);
}

SwitcherView::~SwitcherView() {
    gSwitcherViewStub->SwitcherViewDestructor();
}

bool SwitcherView::event(QEvent *e) {
    return gSwitcherViewStub->event(e);
}

void SwitcherView::setupModel() {
}

void SwitcherView::pinchGestureEvent(QGestureEvent*, QPinchGesture*) {
}

bool SwitcherView::sceneEventFilter(QGraphicsItem*, QEvent*) {
    return true;
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

void SwitcherView::applyPinchGestureTargetMode() {
    gSwitcherViewStub->applyPinchGestureTargetMode();
}

void SwitcherView::runOverviewBounceAnimation() {
    gSwitcherViewStub->runOverviewBounceAnimation();
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

void SwitcherView::applySwitcherMode() {
}

int SwitcherView::buttonIndex(const SwitcherButton *) const {
    return 0;
}

SwitcherButton * SwitcherView::buttonAt(QPointF) const {
    return NULL;
}

void SwitcherView::calculateNearestButtonAt(QPointF) {
}

int SwitcherView::buttonsPerPage() const {
    return 0;
}

void SwitcherView::removeButtonsFromLayout() {
}

void SwitcherView::startBounceAnimation() {
}

void SwitcherView::setInwardBounceAnimation(bool) {
}

#endif
