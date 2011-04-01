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

#include <MCancelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>
#include <MApplication>
#include <MApplicationPage>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <QGraphicsLayout>
#include <MScene>
#include "mwindow_stub.h"
#include "ut_switcherview.h"
#include "mscenemanager_stub.h"
#include "switcherview.h"
#include "switcherstyle.h"
#include "switcher_stub.h"
#include "switcherbutton.h"
#include "pagedpanning_stub.h"
#include "transformlayoutanimation_stub.h"
#include "pagedviewport_stub.h"
#include <QGestureEvent>
#include <QPinchGesture>
#include <MDeviceProfile>
#include "mainwindow_stub.h"
#include "windowinfo_stub.h"

static void verifyEqualContentMarginValues(qreal first, qreal second, qreal target);
SwitcherModel* g_switcherModel;
QMap<SwitcherButton *, Window> g_windowButtonMap;
QRectF g_switcherGeometry;
struct CallOrderData {
    QObject* callee;
    QString param;
};
//static bool gMPannableViewportSetEnabled = false;
bool operator==(const CallOrderData& d1, const CallOrderData& d2)
{
    return d1.callee == d2.callee && d1.param == d2.param;
}
static QList<CallOrderData>* g_ConstructorCallOrder = NULL;
static QString PANNING_STOPPED_SIGNAL = "2panningStopped()";
static QString MPANNABLEWIDGET_SET_ENABLED = "setEnabled=%1";

QRectF MWidgetView::geometry() const {
    return g_switcherGeometry;
}

class TestSwitcherView : public SwitcherView
{
    M_VIEW(SwitcherModel, SwitcherStyle)

public:
    TestSwitcherView(Switcher *controller) : SwitcherView(controller) {}

    SwitcherStyle *modifiableStyle() {
        SwitcherStyleContainer &sc = style();
        const SwitcherStyle *const_s = sc.operator ->();
        SwitcherStyle *s = const_cast<SwitcherStyle *>(const_s);
        return s;
    }
    SwitcherStyleContainer& styleContainer() {
        return style();
    }

    void setGeometry(const QRectF &rect) {
        g_switcherGeometry = rect;
    }
};

//MPannableWidget stubs
void MPannableWidget::setEnabled(bool enabled)
{
    CallOrderData cd;
    cd.callee = this;
    cd.param = MPANNABLEWIDGET_SET_ENABLED.arg(enabled);
    g_ConstructorCallOrder->append(cd);
}

MPannableWidget::PanningPolicy gMPannableWidgetHorizontalPanningPolicy;
void MPannableWidget::setHorizontalPanningPolicy(PanningPolicy policy)
{
    gMPannableWidgetHorizontalPanningPolicy = policy;
}

bool gMPannableWidgetIsEnabled = true;
bool MPannableWidget::isEnabled()
{
    return gMPannableWidgetIsEnabled;
}


// QObject stubs
void QObject::connectNotify ( const char * signal )
{
    if (!g_ConstructorCallOrder) {
        return;
    }
    CallOrderData cd;
    cd.callee = this;
    cd.param = QString(signal);
    g_ConstructorCallOrder->append(cd);
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
SwitcherButton::SwitcherButton(QGraphicsItem *parent, SwitcherButtonModel *model) :
    MButton(parent, model)
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

void SwitcherButton::setVisibilityPropertyEnabled(bool /*enable*/)
{
}

QList<QGraphicsItem *> items_;

QList<QGraphicsItem *> QGraphicsScene::items(const QRectF &/*rect*/, Qt::ItemSelectionMode /*mode*/) const
{
    return items_;
}

QList<QGraphicsItem *> gSceneItems;
QList<QGraphicsItem *> QGraphicsScene::items(const QPointF &/*pos*/) const
{
    return gSceneItems;
}

QList<QGraphicsItem*> gQGraphicsSceneSendCancelEventItems;
bool QGraphicsScene::sendEvent(QGraphicsItem *item, QEvent *event)
{
    if(dynamic_cast<MCancelEvent*>(event)) {
        gQGraphicsSceneSendCancelEventItems << item;
    }
    return true;
}

QList<QPair<QGraphicsItem*, QGraphicsItem*> > gQGraphicsItem_installSceneEventFilter;
void QGraphicsItem::installSceneEventFilter(QGraphicsItem *filterItem)
{
    gQGraphicsItem_installSceneEventFilter.append(qMakePair(this, filterItem));
}

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
        button->setModel(new SwitcherButtonModel());
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
    QSize s = MDeviceProfile::instance()->resolution();

    mPinch->setHotSpot(QPointF(0, 0));
    mPinch->setCenterPoint(QPointF(s.width() * 3 / 4 / 2 * scaleFactor, 0));

    currentPinchState = state;
    m_subject->pinchGestureEvent(mEvent, mPinch);
}

QSize QScrollBar::sizeHint() const
{
    return QSize();
}

/*
 * Switcher detail view tests
 */

void Ut_SwitcherView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherview";
    g_ConstructorCallOrder = new QList<CallOrderData>;
    app = new MApplication(argc, &app_name);
    mSceneManager = new MSceneManager(NULL, NULL);
    MainWindow::instance(true);
}

void Ut_SwitcherView::cleanupTestCase()
{
    delete app;
    delete g_ConstructorCallOrder;
}

void Ut_SwitcherView::init()
{
    g_ConstructorCallOrder->clear();
    gMPannableWidgetHorizontalPanningPolicy = (MPannableWidget::PanningPolicy)-1;
    // Create test switcher
    switcher = new Switcher();
    g_switcherModel = new SwitcherModel;
    switcher->setModel(g_switcherModel);
    m_subject = new TestSwitcherView(switcher);
    switcher->setView(m_subject);
    switcher->setGeometry(QRectF(0, 0, 100, 100));

    MStyle *style = const_cast<MStyle*>(MTheme::style(SwitcherView::staticStyleType(), QString("DetailviewSwitcher")));
    SwitcherStyle *switcherStyle = static_cast<SwitcherStyle*>(style);
    switcherStyle->setRowsPerPage(1);
    switcherStyle->setColumnsPerPage(1);
    switcherStyle->setPinchLength(1.0f);
    switcherStyle->setPinchCancelThreshold(0.25f);

    style = const_cast<MStyle*>(MTheme::style(SwitcherView::staticStyleType(), QString("OverviewSwitcher")));
    switcherStyle = static_cast<SwitcherStyle*>(style);
    switcherStyle->setRowsPerPage(2);
    switcherStyle->setColumnsPerPage(2);
    switcherStyle->setPinchLength(1.0f);
    switcherStyle->setPinchCancelThreshold(0.25f);

    style = const_cast<MStyle*>(MTheme::style(SwitcherView::staticStyleType(), QString("OverviewSwitcher"), 0, 0, M::Portrait));
    switcherStyle = static_cast<SwitcherStyle*>(style);
    switcherStyle->setRowsPerPage(2);
    switcherStyle->setColumnsPerPage(2);
    switcherStyle->setPinchLength(1.0f);
    switcherStyle->setPinchCancelThreshold(0.25f);

    gSwitcherStub->stubReset();
    gTransformLayoutAnimationStub->stubReset();
    gTransformLayoutAnimationStub->stubSetReturnValue("duration", -1);
    gPagedViewportStub->stubReset();

    mPinch = new QPinchGesture();
    mPinch->setCenterPoint(QPointF(100,100));
    QList<QGesture*> gestures;
    gestures.append(mPinch);
    mEvent = new QGestureEvent(gestures);
    items_.clear();
    gQGraphicsItem_installSceneEventFilter.clear();
    gSceneItems.clear();
    gMPannableWidgetIsEnabled = true;
    gQGraphicsSceneSendCancelEventItems.clear();

    gMWindowStub->stubReset();
    gMWindowStub->stubSetReturnValue("sceneManager", mSceneManager);
    gMWindowStub->stubSetReturnValue("scene", new MScene());
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
void Ut_SwitcherView::testPanningDisabledWhenNoSwitcherButtons_NB186716()
{
    QCOMPARE(gMPannableWidgetHorizontalPanningPolicy, MPannableWidget::PanningAlwaysOff);
    g_switcherModel->setButtons(createButtonList(1));
    QCOMPARE(gMPannableWidgetHorizontalPanningPolicy, MPannableWidget::PanningAlwaysOn);
    g_switcherModel->setButtons(QList< QSharedPointer<SwitcherButton> >());
    QCOMPARE(gMPannableWidgetHorizontalPanningPolicy, MPannableWidget::PanningAlwaysOff);
}

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
    pinchGesture(0.8,Qt::GestureStarted);
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
    pinchGesture(1.5,Qt::GestureStarted);
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

    gTransformLayoutAnimationStub->stubSetReturnValue("manualControl", true);

    // Overview->Detailview gesture cancellation
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    pinchGesture(1.0, Qt::GestureStarted);
    pinchGesture(1.1, Qt::GestureUpdated);
    pinchGesture(1.1, Qt::GestureFinished);
    QCOMPARE(gTransformLayoutAnimationStub->stubCallCount("cancelAnimation"), 1);
    QCOMPARE(m_subject->pinchGestureTargetMode, SwitcherModel::Overview);
    m_subject->layoutAnimation->stop();

    // Detail->Overview gesture cancellation
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);
    pinchGesture(1.0, Qt::GestureStarted);
    pinchGesture(0.9, Qt::GestureUpdated);
    pinchGesture(0.9, Qt::GestureFinished);
    QCOMPARE(gTransformLayoutAnimationStub->stubCallCount("cancelAnimation"), 2);
    QCOMPARE(m_subject->pinchGestureTargetMode, SwitcherModel::Detailview);
    m_subject->layoutAnimation->stop();
}

void Ut_SwitcherView::testTransitionControl()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    g_ConstructorCallOrder->clear();

    gSceneItems.append(m_subject->viewport);

    pinchGesture(1.0, Qt::GestureStarted);

    gTransformLayoutAnimationStub->stubSetReturnValue("manualControl", true);
    pinchGesture(1.1, Qt::GestureUpdated);
    QVERIFY(gTransformLayoutAnimationStub->stubLastCallTo("setManualControl").parameter<bool>(0));
    QCOMPARE(gTransformLayoutAnimationStub->stubLastCallTo("setProgress").parameter<float>(0), 0.1f);

    pinchGesture(1.2, Qt::GestureUpdated);
    QCOMPARE(gTransformLayoutAnimationStub->stubLastCallTo("setProgress").parameter<float>(0), 0.2f);

    pinchGesture(1.3, Qt::GestureFinished);
    QVERIFY(!gTransformLayoutAnimationStub->stubLastCallTo("setManualControl").parameter<bool>(0));
}

void Ut_SwitcherView::testBounceAnimation()
{
    m_subject->modifiableStyle()->setBounceDuration(100);

    m_subject->pinchGestureTargetMode = SwitcherModel::Detailview;
    m_subject->runOverviewBounceAnimation();
    QCOMPARE(m_subject->bounceAnimation->state(), QAbstractAnimation::Stopped);

    m_subject->pinchGestureTargetMode = SwitcherModel::Overview;
    m_subject->runOverviewBounceAnimation();
    QCOMPARE(m_subject->bounceAnimation->state(), QAbstractAnimation::Running);
}

void Ut_SwitcherView::testOverpinchOverview()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    m_subject->modifiableStyle()->setBounceDuration(100);
    m_subject->modifiableStyle()->setBounceScale(0.5);

    float scale = 0.5;
    pinchGesture(1.0, Qt::GestureStarted);
    pinchGesture(1.0 - scale, Qt::GestureUpdated);

    QCOMPARE(m_subject->bounceAnimation->state(), QAbstractAnimation::Paused);
    QVERIFY2(m_subject->bounceAnimation->keyValueAt(0.5).toFloat() < 1.0f, "incorrect zoom direction");
    QCOMPARE(m_subject->bounceAnimation->currentTime(), int(m_subject->bounceAnimation->duration() / 2 * scale));
}

void Ut_SwitcherView::testOverpinchDetailview()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);
    m_subject->modifiableStyle()->setBounceDuration(100);
    m_subject->modifiableStyle()->setBounceScale(0.5);

    float scale = 0.5;
    pinchGesture(1.0, Qt::GestureStarted);
    pinchGesture(1.0 + scale, Qt::GestureUpdated);

    QCOMPARE(m_subject->bounceAnimation->state(), QAbstractAnimation::Paused);
    QVERIFY2(m_subject->bounceAnimation->keyValueAt(0.5).toFloat() > 1.0f, "incorrect zoom direction");
    QCOMPARE(m_subject->bounceAnimation->currentTime(), int(m_subject->bounceAnimation->duration() / 2 * scale));
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

void Ut_SwitcherView::testButtonCounts()
{
    // Set overview mode in portrait orientation
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);

    // Create 4 buttons
    QList< QSharedPointer<SwitcherButton> > buttonList = createButtonList(4);
    g_switcherModel->setButtons(buttonList);

    QCOMPARE(m_subject->overviewPolicy->columnCount(), 2);
    QCOMPARE(m_subject->overviewPolicy->rowCount(), 2);
    QCOMPARE(m_subject->detailPolicy->count(), 4);

    // Set detailview mode in portrait orientation
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);
    m_subject->updateButtons();

    QCOMPARE(m_subject->overviewPolicy->columnCount(), 2);
    QCOMPARE(m_subject->overviewPolicy->rowCount(), 2);
    QCOMPARE(m_subject->detailPolicy->count(), 4);

    // Set landscape orientation
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    m_subject->updateButtons();

    QCOMPARE(m_subject->overviewPolicy->columnCount(), 2);
    QCOMPARE(m_subject->overviewPolicy->rowCount(), 2);
    QCOMPARE(m_subject->detailPolicy->count(), 4);

    // Set overview mode
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    m_subject->updateButtons();

    QCOMPARE(m_subject->overviewPolicy->columnCount(), 2);
    QCOMPARE(m_subject->overviewPolicy->rowCount(), 2);
    QCOMPARE(m_subject->detailPolicy->count(), 4);
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

void verifyPanningResult(QList<QSharedPointer<SwitcherButton> > list, bool panningShouldHappen, uint targetPage)
{
    g_switcherModel->setButtons(list);
    gWindowInfoStub->stubSetReturnValue("window", g_windowButtonMap.value(list.last().data()));
    Window topmost = list.last().data()->xWindow();
    g_switcherModel->setTopmostWindow(topmost);
    QCOMPARE(g_windowButtonMap.count(), list.count());
    QCOMPARE(gPagedViewportStub->stubCallCount("panToPage"), panningShouldHappen ? 1 : 0);
    if (panningShouldHappen) {
        QCOMPARE(gPagedViewportStub->stubLastCallTo("panToPage").parameter<uint>(0), targetPage);
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

    gPagedViewportStub->stubReset();

    int moreButtons = 3;
    appendMoreButtonsToList(moreButtons, list);
    verifyPanningResult(list, true, secondPageIndex);

    gPagedViewportStub->stubReset();

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

    gPagedViewportStub->stubReset();

    int moreButtons = 3;
    appendMoreButtonsToList(moreButtons, list);
    verifyPanningResult(list, true, 6);

    gPagedViewportStub->stubReset();

    // Lets delete something from the first page to see if we pan -> we shouldn't
    // as the top most window does not change.
    list.removeAt(2);
    verifyPanningResult(list, false, -1);
}

void Ut_SwitcherView::testWhenPinchingOnEmptyAreaNearestButtonIsDetected()
{
    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(2);
    QList< QSharedPointer<SwitcherButton> > buttons = createButtonList(3);

    buttons.at(0).data()->setGeometry(QRectF(198, 65, 200, 100));
    buttons.at(1).data()->setGeometry(QRectF(447, 65, 200, 100));
    buttons.at(2).data()->setGeometry(QRectF(198, 216, 200, 100));
    g_switcherModel->setButtons(buttons);

    foreach(QSharedPointer<SwitcherButton> button, buttons) {
        items_.append(button.data());
    }

    // When clicking topLeft area button 1 should be detected
    m_subject->calculateNearestButtonAt(QPointF(140, 124));
    QCOMPARE(m_subject->pinchedButtonPosition, 0);

    // When clicking bottomRight area button 3 should be selected
    m_subject->calculateNearestButtonAt(QPointF(493, 313));
    QCOMPARE(m_subject->pinchedButtonPosition, 2);
}

void Ut_SwitcherView::testWhenPinchGestureHasBeenPerformedOnTopOfPressedSwitcherButtonThenSwitcherButtonIsUp() {
    const qreal BUTTON_WIDTH = 100.0;
    const qreal BUTTON_HEIGHT = 100.0;

    QList<QSharedPointer<SwitcherButton> > buttons = createButtonList(1);

    buttons.at(0)->setGeometry(QRectF(0.0, 0.0, BUTTON_WIDTH, BUTTON_HEIGHT));
    buttons.at(0)->setDown(true);

    g_switcherModel->setButtons(buttons);

    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);
    gTransformLayoutAnimationStub->stubSetReturnValue("manualControl", true);

    // Pinch in the middle of the button
    mPinch->setCenterPoint(QPointF(BUTTON_WIDTH / 2.0, BUTTON_HEIGHT / 2.0));

    pinchGesture(0.5, Qt::GestureStarted);
    pinchGesture(0.5, Qt::GestureUpdated);
    pinchGesture(0.5, Qt::GestureFinished);

    QCOMPARE(buttons.at(0)->isDown(), false);
}

void Ut_SwitcherView::testWhenPinchGestureHasBeenPerformedOnTopOfNonPressedSwitcherButtonThenAllOfTheSwitcherButtonsAreUp()
{
    const qreal BUTTON_WIDTH = 100.0;
    const qreal BUTTON_HEIGHT = 100.0;

    QList<QSharedPointer<SwitcherButton> > buttons = createButtonList(2);

    buttons.at(0)->setGeometry(QRectF(0.0, 0.0, BUTTON_WIDTH, BUTTON_HEIGHT));
    buttons.at(0)->setDown(false);

    buttons.at(1)->setGeometry(QRectF(0.0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT));
    // Set down the bottommost button
    buttons.at(1)->setDown(true);

    g_switcherModel->setButtons(buttons);

    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    gTransformLayoutAnimationStub->stubSetReturnValue("manualControl", true);

    // Pinch in the middle of the upmost button
    mPinch->setCenterPoint(QPointF(BUTTON_WIDTH / 2.0, BUTTON_HEIGHT / 2.0));

    pinchGesture(0.5, Qt::GestureStarted);
    pinchGesture(0.5, Qt::GestureUpdated);
    pinchGesture(0.5, Qt::GestureFinished);

    QCOMPARE(buttons.at(0)->isDown(), false);
    QCOMPARE(buttons.at(1)->isDown(), false);
}

void Ut_SwitcherView::testWhenPinchGestureStartsThenSceneEventFilteringForSwitcherButtonStarts()
{
    QList<QSharedPointer<SwitcherButton> > buttons = createButtonList(1);
    g_switcherModel->setButtons(buttons);

    pinchGesture(0.5, Qt::GestureStarted);

    QCOMPARE(gQGraphicsItem_installSceneEventFilter.count(), 1);
    QCOMPARE(gQGraphicsItem_installSceneEventFilter.at(0).first, buttons.at(0).data());
    QCOMPARE(gQGraphicsItem_installSceneEventFilter.at(0).second, switcher);
}

void Ut_SwitcherView::testGraphicsSceneMouseMoveEventsDoNotGetFilteredForSwitcherButtonsByDefault()
{
    QList<QSharedPointer<SwitcherButton> > buttons = createButtonList(1);
    g_switcherModel->setButtons(buttons);

    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    QCOMPARE(m_subject->sceneEventFilter(buttons.at(0).data(), &mouseMoveEvent), true);
}

void Ut_SwitcherView::testGraphicsSceneMouseMoveEventsGetFilteredForSwitcherButtons()
{
    QList<QSharedPointer<SwitcherButton> > buttons = createButtonList(1);
    g_switcherModel->setButtons(buttons);

    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    QCOMPARE(m_subject->sceneEventFilter(buttons.at(0).data(), &mouseMoveEvent), true);
}

#undef None
void Ut_SwitcherView::testOtherThanGraphicsSceneMouseMoveEventsDoNotGetFilteredForSwitcherButtons()
{
    QList<QSharedPointer<SwitcherButton> > buttons = createButtonList(1);
    g_switcherModel->setButtons(buttons);

    QEvent event(QEvent::None);
    QCOMPARE(m_subject->sceneEventFilter(buttons.at(0).data(), &event), false);
}

void Ut_SwitcherView::testGraphicsSceneMouseMoveEventsDoNotGetFilteredForOtherThanSwitcherButtons()
{
    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    QGraphicsWidget item;
    QCOMPARE(m_subject->sceneEventFilter(&item, &mouseMoveEvent), false);
}

void Ut_SwitcherView::testWhenPinchingStartsOrStopsThenViewportPhysicsEnabledOrDisabled()
{
    MPannableViewport *vp1 = new MPannableViewport;
    MPannableViewport *vp2 = new MPannableViewport;
    gSceneItems << vp1 << vp2;
    m_subject->pinchBegin(QPointF());
    QVERIFY(!vp1->physics()->enabled());
    QVERIFY(!vp2->physics()->enabled());
    m_subject->pinchEnd();
    QVERIFY(vp1->physics()->enabled());
    QVERIFY(vp2->physics()->enabled());
}


QTEST_APPLESS_MAIN(Ut_SwitcherView)
