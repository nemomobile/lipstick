/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include <MApplication>
#include <MApplicationPage>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <QGraphicsLayout>
#include "mwindow_stub.h"
#include "ut_switcherview.h"
#include "mainwindow_stub.h"
#include "mscenemanager_stub.h"
#include "switcherview.h"
#include "switcherstyle.h"
#include "switcher_stub.h"
#include "switcherbutton.h"
#include "x11wrapper_stub.h"
#include "windowinfo_stub.h"
#include "pagedpanning.h"
#include "pagedviewport.h"
#include <QGestureEvent>
#include <QPinchGesture>

static void setSwitcherButtonSize(QList< QSharedPointer<SwitcherButton> > &buttonList, const QSizeF &size);
static void verifyEqualContentMarginValues(qreal first, qreal second, qreal target);
static void verifyPanningResult(QList<QSharedPointer<SwitcherButton> > list, bool panningShouldHappen, int targetPage);
SwitcherModel* g_switcherModel;
QMap<SwitcherButton *, Window> g_windowButtonMap;
QRectF g_switcherGeometry;
bool g_panRequested;
uint g_panRequestIndex;

class Ut_SwitcherStyle : public SwitcherStyle
{
};

class Ut_SwitcherStyleContainer : public SwitcherStyleContainer
{
};

QRectF MWidgetView::geometry() const {
    return g_switcherGeometry;
}

class TestSwitcherView : public SwitcherView
{
    M_VIEW(SwitcherModel, Ut_SwitcherStyle)

public:
    TestSwitcherView(Switcher *controller) : SwitcherView(controller) {}


    SwitcherStyle *modifiableStyle() {
        SwitcherStyleContainer &sc = style();
        const SwitcherStyle *const_s = sc.operator ->();
        SwitcherStyle *s = const_cast<SwitcherStyle *>(const_s);
        return s;
    }
    Ut_SwitcherStyleContainer& styleContainer() {
        return style();
    }

    void setGeometry(const QRectF &rect) {
        g_switcherGeometry = rect;
    }
};

//PagedViewport stubs
PagedViewport::PagedViewport(QGraphicsItem *parent)
{
    Q_UNUSED(parent)
}

PagedViewport::~PagedViewport() { }

void PagedViewport::setPanDirection(const Qt::Orientations &panDirection)
{
    Q_UNUSED(panDirection)
}

void PagedViewport::updatePageCount(int pages)
{
    Q_UNUSED(pages)
}

int PagedViewport::currentPage()
{
    return 0;
}

void PagedViewport::panToPage(uint page)
{
    Q_UNUSED(page)
    g_panRequested = true;
    g_panRequestIndex = page;
}

void PagedViewport::focusFirstPage()
{

}

// Home stubs
class Home : public MApplicationPage
{
public:
    Home(QGraphicsItem *parent = 0);
};

Home::Home(QGraphicsItem *parent) : MApplicationPage(parent)
{
}

// Gesture stubs
Qt::GestureState currentPinchState = Qt::NoGesture;
Qt::GestureState QGesture::state() const
{
    return currentPinchState;
}

// SwitcherButton stubs
SwitcherButton::SwitcherButton(QGraphicsItem *parent) :
    MButton(parent)
{
    Q_UNUSED(parent);
}

SwitcherButton::~SwitcherButton()
{
    g_windowButtonMap.remove(this);
}

void SwitcherButton::switchToWindow()
{
}

void SwitcherButton::close()
{
}

void SwitcherButton::prepareGeometryChange()
{
}

void SwitcherButton::resetState()
{
}

void SwitcherButton::enterDisplayEvent()
{
}

void SwitcherButton::exitDisplayEvent()
{
}

void SwitcherButton::setXWindow(Window window)
{
    g_windowButtonMap[this] = window;
}

Window SwitcherButton::xWindow()
{
    return g_windowButtonMap[this];
}

PagedPanning::PagedPanning(QObject* parent) : MPhysics2DPanning(parent),
                                              pageCount_(1),
                                              currentPage(0),
                                              snapMode(false),
                                              velocityThreshold_(7.0),
                                              dragThreshold_(0.5),
                                              pageSnapSpringK_(0.7),
                                              pageSnapFriction_(0.7),
                                              previousPosition(0),
                                              targetPage(0),
                                              pageWidth(0)
{
}

PagedPanning::~PagedPanning()
{
}

void PagedPanning::panToPage(int itemIndex) {
    g_panRequested = true;
    g_panRequestIndex = itemIndex;
    emit pageChanged(itemIndex);
}

void PagedPanning::setFirstPagePosition() {
}

void PagedPanning::panToCurrentPage()
{
    emit pageChanged(currentPage);
}

void PagedPanning::integrateAxis(Qt::Orientation, qreal &, qreal &, qreal &, qreal &, bool)
{
}

void PagedPanning::setPageCount(int) {
}

int PagedPanning::pageCount() const {
    return 1;
}

void PagedPanning::setVelocityThreshold(qreal)
{
}

void PagedPanning::setDragThreshold(qreal)
{
}

void PagedPanning::setSlideLimit(int)
{
}

void PagedPanning::setPageSnapSpringK(qreal)
{
}

void PagedPanning::setPageSnapFriction(qreal)
{
}

void PagedPanning::pointerPress(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PagedPanning::pointerMove(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PagedPanning::pointerRelease()
{
}

int PagedPanning::activePage()
{
    return currentPage;
}

QList<QGraphicsItem *> items_;
QList<QGraphicsItem *> QGraphicsView::items(const QPoint &/*pos*/) const
{ return items_;}

QList< QSharedPointer<SwitcherButton> > Ut_SwitcherView::createButtonList(int buttons)
{
    QList< QSharedPointer<SwitcherButton> > buttonList;
    appendMoreButtonsToList(buttons, buttonList);
    return buttonList;
}

void Ut_SwitcherView::appendMoreButtonsToList(int newButtons, QList< QSharedPointer<SwitcherButton> > &buttonList)
{
    int newCount = buttonList.count() + newButtons;
    for(int i = buttonList.count(); i < newCount; i++) {
        QSharedPointer<SwitcherButton> button(new SwitcherButton);
        button->setText(QString("Title %1").arg(i));
        button->setXWindow(i);
        button.data()->setModel(new SwitcherButtonModel());
        buttonList.append(button);
    }
}

void Ut_SwitcherView::verifyButtonModesInOverviewMode(QList< QSharedPointer<SwitcherButton> > &buttonList)
{
    int buttons = buttonList.count();
    for(int i = 0; i < buttons; i++){
        if (buttons < 3) {
            QCOMPARE(buttonList[i].data()->model()->viewMode(), SwitcherButtonModel::Large);
        } else {
            QCOMPARE(buttonList[i].data()->model()->viewMode(), SwitcherButtonModel::Medium);
        }
    }
}

void Ut_SwitcherView::verifyButtonModesInOverviewMode(M::Orientation orientation)
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    gMSceneManagerStub->stubSetReturnValue("orientation", orientation);

    QList< QSharedPointer<SwitcherButton> > buttonList = createButtonList(1);

    // with 1 button
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);

    appendMoreButtonsToList(1, buttonList);
    // with 2 buttons
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);

    appendMoreButtonsToList(1, buttonList);
    // with 3 buttons
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);

    appendMoreButtonsToList(2, buttonList);
    // with 4 buttons
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);
}

void Ut_SwitcherView::verifyLayoutPolicyContentMargins(const QSizeF &buttonSize)
{
    qreal left, right, top, bottom;
    if (g_switcherModel->buttons().size() == 0) {
        // Test the overview policy margins
        m_subject->overviewPolicy->getContentsMargins(&left, &top, &right, &bottom);
        verifyEqualContentMarginValues(top, bottom, 0.0);
        verifyEqualContentMarginValues(left, right, 0.0);

        // Test the detail view policy margins
        m_subject->detailPolicy->getContentsMargins(&left, &top, &right, &bottom);
        verifyEqualContentMarginValues(top, bottom, 0.0);
        verifyEqualContentMarginValues(left, right, 0.0);
    } else {
        qreal horizontalMargin, verticalMargin;
        if (g_switcherModel->buttons().size() < 3) {
            // Test the overview policy vertical margins
            m_subject->overviewPolicy->getContentsMargins(&left, &top, &right, &bottom);
            qreal numberOfRowSpacings = qMax(0, m_subject->overviewPolicy->rowCount() - 1);
            qreal heightTakenByRowSpacings =  numberOfRowSpacings * m_subject->modifiableStyle()->buttonVerticalSpacing();
            verticalMargin = (m_subject->geometry().height() - (buttonSize.height() * m_subject->overviewPolicy->rowCount() + heightTakenByRowSpacings)) / 2;

            verifyEqualContentMarginValues(top, bottom, verticalMargin);
        } else {
            // Overview policy vertical margins are determined by the style
            // TODO this should be tested.
        }

        // TODO test the overview policy horizontal margins!
        // TODO test the overview policy spacings!

        // Test the detail view policy margins and horizontal spacing
        m_subject->detailPolicy->getContentsMargins(&left, &top, &right, &bottom);
        horizontalMargin = (m_subject->geometry().width() - buttonSize.width()) / 2;
        verticalMargin = (m_subject->geometry().height() - buttonSize.height()) / 2;
        verifyEqualContentMarginValues(top, bottom, verticalMargin);
        verifyEqualContentMarginValues(left, right, horizontalMargin);
        QCOMPARE(m_subject->detailPolicy->horizontalSpacing(), horizontalMargin / 2);
    }
}

void setSwitcherButtonSize(QList< QSharedPointer<SwitcherButton> > &buttonList, const QSizeF &size)
{
    for(int i = 0; i < buttonList.count(); i++) {
        buttonList[i].data()->setPreferredSize(size);
    }
}

void verifyEqualContentMarginValues(qreal first, qreal second, qreal target)
{
    QCOMPARE(first, second);
    QCOMPARE(first, target);
}

void Ut_SwitcherView::pinchGesture(qreal scaleFactor, Qt::GestureState state)
{
    currentPinchState = state;
    mPinch->setScaleFactor(scaleFactor);
    m_subject->pinchGestureEvent(mEvent, mPinch);
}

/*
 * Switcher detail view tests
 */

void Ut_SwitcherView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherview";
    app = new MApplication(argc, &app_name);
    mSceneManager = new MSceneManager(NULL, NULL);
    gMWindowStub->stubSetReturnValue("sceneManager", mSceneManager);
}

void Ut_SwitcherView::cleanupTestCase()
{
    delete app;
}

void Ut_SwitcherView::init()
{
    g_panRequested = false;
    // Create test switcher
    switcher = Switcher::instance();
    g_switcherModel = new SwitcherModel;
    switcher->setModel(g_switcherModel);
    m_subject = new TestSwitcherView(switcher);
    switcher->setView(m_subject);
    gSwitcherStub->stubReset();
    mPinch = new QPinchGesture();
    mPinch->setCenterPoint(QPointF(100,100));
    QList<QGesture*> gestures;
    gestures.append(mPinch);
    mEvent = new QGestureEvent(gestures);
    items_.clear();
}

void Ut_SwitcherView::cleanup()
{
    delete switcher;
    delete mPinch;
    delete mEvent;
}

void Ut_SwitcherView::testSnapIndexChangedInDetailView()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

    g_switcherModel->setButtons(createButtonList(4));

    // test that snap index changes only when the new
    // snap index is within the button range
    connect(this, SIGNAL(pageChanged(int)),
            m_subject, SLOT(updateFocusedButton(int)));
    emit pageChanged(3);
    QCOMPARE(m_subject->focusedSwitcherButton, 3);
    emit pageChanged(0);
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
    emit pageChanged(-1);
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
    emit pageChanged(9999);
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
}

void Ut_SwitcherView::testPanningStoppedInDetailView()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

    g_switcherModel->setButtons(createButtonList(4));

    // test that correct button is emphasized when panning stops
    connect(this, SIGNAL(pageChanged(int)),
            m_subject, SLOT(updateFocusedButton(int)));
    connect(this, SIGNAL(panningStopped()),
            m_subject, SLOT(panningStopped()));
    emit pageChanged(3);
    emit pageChanged(2);
    emit panningStopped();

    QCOMPARE(m_subject->focusedSwitcherButton, 2);
    QCOMPARE(g_switcherModel->buttons().at(0).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(1).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(2).data()->model()->viewMode(), SwitcherButtonModel::Large);
    QCOMPARE(g_switcherModel->buttons().at(3).data()->model()->viewMode(), SwitcherButtonModel::Medium);

    emit pageChanged(1);
    emit pageChanged(2);
    emit pageChanged(3);
    emit panningStopped();

    QCOMPARE(m_subject->focusedSwitcherButton, 3);

    QCOMPARE(g_switcherModel->buttons().at(0).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(1).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(2).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(3).data()->model()->viewMode(), SwitcherButtonModel::Large);
}

/*
 * Switcher overview tests
 */

void Ut_SwitcherView::testButtonModesInOverviewMode()
{
    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(2);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    verifyButtonModesInOverviewMode(M::Landscape);

    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(2);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
    verifyButtonModesInOverviewMode(M::Portrait);
}

void Ut_SwitcherView::testPanningStoppedInOverView()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(2);

    // fill 2 and a half pages
    g_switcherModel->setButtons(createButtonList(10));

    // test that correct button is emphasized when panning stops
    connect(this, SIGNAL(pageChanged(int)),
            m_subject, SLOT(updateFocusedButton(int)));
    connect(this, SIGNAL(panningStopped()),
            m_subject, SLOT(panningStopped()));
    emit pageChanged(1);
    emit pageChanged(2);
    emit panningStopped();
    // Focused on 1st button of last page
    QCOMPARE(m_subject->focusedSwitcherButton, 8);

    emit pageChanged(1);
    emit panningStopped();
    // Focused on 1st button of 2nd page
    QCOMPARE(m_subject->focusedSwitcherButton, 4);

    emit pageChanged(2);
    emit pageChanged(1);
    emit pageChanged(0);
    emit panningStopped();
    // Focused on 1st button of first page
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
}

/*
 * Mode change tests
 */
void Ut_SwitcherView::testDetailToOverviewModeChange()
{
    SwitcherButton item1;
    item1.setText("button");
    item1.setXWindow(1);
    items_.append(&item1);
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);
    g_switcherModel->setButtons(createButtonList(4));
    currentPinchState = Qt::GestureStarted;

    // Scale factor <1 is transition to Overview mode.
    mPinch->setLastScaleFactor(0.8);

    pinchGesture(0.5,Qt::GestureStarted);
    pinchGesture(0.5,Qt::GestureUpdated);
    pinchGesture(0.5,Qt::GestureFinished);

    QCOMPARE (g_switcherModel->switcherMode(), SwitcherModel::Overview);

    // Check that doing the pinch gesture twice in the same direction doesn't switch the mode
    // Ie, the gesture direction should determine whether transition happens
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);

    pinchGesture(0.5,Qt::GestureStarted);
    pinchGesture(0.5,Qt::GestureUpdated);
    pinchGesture(0.5,Qt::GestureFinished);

   QCOMPARE (g_switcherModel->switcherMode(), SwitcherModel::Overview);
}

void Ut_SwitcherView::testOverviewToDetailModeChange()
{
    SwitcherButton item1;
    item1.setText("button");
    item1.setXWindow(1);
    items_.append(&item1);
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);

   // Scale factor >1 is transition to Detailview mode.
   mPinch->setLastScaleFactor(1.5);

   pinchGesture(2.0,Qt::GestureStarted);
   pinchGesture(2.0,Qt::GestureUpdated);
   pinchGesture(2.0,Qt::GestureFinished);
   QCOMPARE (g_switcherModel->switcherMode(), SwitcherModel::Detailview);

   // Check that doing the pinch gesture twice in the same direction doesn't switch the mode
   // Ie, the gesture direction should determine whether transition happens
   g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

   pinchGesture(2.0,Qt::GestureStarted);
   pinchGesture(2.0,Qt::GestureUpdated);
   pinchGesture(2.0,Qt::GestureFinished);
   QCOMPARE (g_switcherModel->switcherMode(), SwitcherModel::Detailview);
}

void Ut_SwitcherView::testModeChangeCancel()
{
    SwitcherButton item1;
    item1.setText("button");
    item1.setXWindow(1);
    items_.append(&item1);
    // Overview->Detailview gesture cancellation
   g_switcherModel->setSwitcherMode(SwitcherModel::Overview);

   mPinch->setLastScaleFactor(1.0);

   // Start with outward pinch and change to inward pinch
   pinchGesture(1.5,Qt::GestureStarted);
   pinchGesture(1.5,Qt::GestureUpdated);
   pinchGesture(0.5,Qt::GestureUpdated);
   pinchGesture(0.5,Qt::GestureFinished);
   QCOMPARE (g_switcherModel->switcherMode(), SwitcherModel::Overview);

   // Detail->Overview gesture cancellation
   g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

   mPinch->setLastScaleFactor(0.8);

   // Start with inward pinch and change to outward pinch
   pinchGesture(0.5,Qt::GestureStarted);
   pinchGesture(0.5,Qt::GestureUpdated);
   pinchGesture(1.5,Qt::GestureUpdated);
   pinchGesture(1.5,Qt::GestureFinished);
   QCOMPARE (g_switcherModel->switcherMode(), SwitcherModel::Detailview);
}

void Ut_SwitcherView::testEvent()
{
    QEvent e(QEvent::TouchBegin);
    e.setAccepted(false);
    m_subject->event(&e);
    QCOMPARE(e.isAccepted(), true);
}

void Ut_SwitcherView::testSwitcherButtonAlignment()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(2);

    qreal verticalSpacing = 10;
    m_subject->modifiableStyle()->setButtonVerticalSpacing(verticalSpacing);

    QList< QSharedPointer<SwitcherButton> > buttonList = createButtonList(1);
    qreal buttonSize = 100;
    QSizeF size(buttonSize, buttonSize);
    setSwitcherButtonSize(buttonList, size);

    qreal switcherHeight = 400.0;
    qreal switcherWidth = 600.0;
    m_subject->setGeometry(QRectF(0, 0, switcherWidth, switcherHeight));

    // with 1 button
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);
    verifyLayoutPolicyContentMargins(size);

    //with 3 buttons
    appendMoreButtonsToList(2, buttonList);
    setSwitcherButtonSize(buttonList, size);
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);
    verifyLayoutPolicyContentMargins(size);

    //with 4 buttons
    appendMoreButtonsToList(1, buttonList);
    setSwitcherButtonSize(buttonList, size);
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);
    verifyLayoutPolicyContentMargins(size);

    // Change the orientation to portrait
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);

    switcherHeight = 600.0;
    switcherWidth = 400.0;
    m_subject->setGeometry(QRectF(0, 0, switcherWidth, switcherHeight));

    // Called by the SceneManager after orientation has changed
    m_subject->updateButtons();

    verifyLayoutPolicyContentMargins(size);

    // with 0 buttons
    QList< QSharedPointer<SwitcherButton> > emptyButtonList;
    g_switcherModel->setButtons(emptyButtonList);
    verifyButtonModesInOverviewMode(emptyButtonList);
    QSize notSizeAtAll;
    verifyLayoutPolicyContentMargins(notSizeAtAll);
}

void Ut_SwitcherView::testRemovingButtons()
{
    QList< QSharedPointer<SwitcherButton> > list(createButtonList(2));
    QSharedPointer<SwitcherButton> removedButton = list.at(0);
    g_switcherModel->setButtons(list);
    QGraphicsLayout *layout = dynamic_cast<PagedViewport *>(switcher->layout()->itemAt(0))->widget()->layout();
    QCOMPARE(layout->count(), 2);

    list.removeAt(0);
    g_switcherModel->setButtons(list);

    QCOMPARE(layout->count(), 1);
    // verify that removed button was not deleted while there is still ref in QSharedPointer
    QVERIFY(!removedButton.isNull());
}

void verifyPanningResult(QList<QSharedPointer<SwitcherButton> > list, bool panningShouldHappen, int targetPage)
{
    g_switcherModel->setButtons(list);
    gWindowInfoStub->stubSetReturnValue("window", g_windowButtonMap.value(list.last().data()));
    Window topmost = list.last().data()->xWindow();
    g_switcherModel->setTopmostWindow(topmost);
    QCOMPARE(g_windowButtonMap.count(), list.count());
    QCOMPARE(g_panRequested, panningShouldHappen);
    if (panningShouldHappen) {
        QCOMPARE(g_panRequestIndex, uint(targetPage));
    }
}

void Ut_SwitcherView::testPanToSwitcherPageInOverviewMode()
{
    int firstPageIndex = 0;
    int secondPageIndex = 1;

    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(2);
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);

    g_windowButtonMap.clear();
    int buttons = 4;
    QList< QSharedPointer<SwitcherButton> > list(createButtonList(buttons));
    verifyPanningResult(list, true, firstPageIndex);

    g_panRequested = false;

    int moreButtons = 3;
    appendMoreButtonsToList(moreButtons, list);
    verifyPanningResult(list, true, secondPageIndex);

    g_panRequested = false;

    // Lets delete something from the first page to see if we pan -> we shouldn't 
    // as the top most window does not change.
    list.removeAt(2);
    verifyPanningResult(list, false, -1);
}

void Ut_SwitcherView::testPanToSwitcherPageInDetailviewMode()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

    g_windowButtonMap.clear();
    int buttons = 4;
    QList< QSharedPointer<SwitcherButton> > list(createButtonList(buttons));

    verifyPanningResult(list, true, 3);

    g_panRequested = false;

    int moreButtons = 3;
    appendMoreButtonsToList(moreButtons, list);
    verifyPanningResult(list, true, 6);

    g_panRequested = false;

    // Lets delete something from the first page to see if we pan -> we shouldn't 
    // as the top most window does not change.
    list.removeAt(2);
    verifyPanningResult(list, false, -1);
}

void Ut_SwitcherView::testWhenPinchingOnSwitcherButtonExactPinchedButtonIsDetected()
{
    QList< QSharedPointer<SwitcherButton> > buttons = createButtonList(2);
    g_switcherModel->setButtons(buttons);
    items_.append(buttons.at(1).data());
    m_subject->buttonAt(QPointF(100,100));
    QCOMPARE(m_subject->pinchedButtonPosition, qint16(1));
}

void Ut_SwitcherView::testWhenPinchingOnEmptyAreaNearestButtonIsDetected()
{
    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(2);
    QList< QSharedPointer<SwitcherButton> > buttons = createButtonList(3);
    gMWindowStub->stubSetReturnValue("orientation", M::Landscape);
    buttons.at(0).data()->setPos(198,65);
    buttons.at(1).data()->setPos(447,65);
    buttons.at(2).data()->setPos(198,216);
    g_switcherModel->setButtons(buttons);
    // When clicking topLeft area button 1 should be detected
    m_subject->nearestButtonFrom(QPointF(140,124));
    QCOMPARE(m_subject->pinchedButtonPosition, qint16(0));
    // When clicking bottomRight area button 3 should be selected
    m_subject->nearestButtonFrom(QPointF(593,313));
    QCOMPARE(m_subject->pinchedButtonPosition, qint16(2));

    gMWindowStub->stubSetReturnValue("orientation", M::Portrait);
    m_subject->viewport->setGeometry(QRectF(0,0,480,748));
    buttons.at(0).data()->setPos(51,65);
    buttons.at(1).data()->setPos(255,65);
    buttons.at(2).data()->setPos(50,408);
    g_switcherModel->setButtons(buttons);
    // When clicking topRight area button 2 should be detected
    m_subject->nearestButtonFrom(QPointF(60,72));
    QCOMPARE(m_subject->pinchedButtonPosition, qint16(1));
}

QTEST_APPLESS_MAIN(Ut_SwitcherView)
