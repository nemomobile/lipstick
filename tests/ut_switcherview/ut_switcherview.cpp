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
#include "mwindow_stub.h"
#include "ut_switcherview.h"
#include "mainwindow_stub.h"
#include "mscenemanager_stub.h"
#include "switcherview.h"
#include "switcherstyle.h"
#include "switcher_stub.h"
#include "switcherbutton.h"
#include "x11wrapper_stub.h"
#include "pagedpanning.h"

SwitcherModel* g_switcherModel;
QMap<SwitcherButton *, Window> g_windowButtonMap;

class Ut_SwitcherStyle : public SwitcherStyle
{
};

class Ut_SwitcherStyleContainer : public SwitcherStyleContainer
{
};

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
};

// Home stubs
class Home : public MApplicationPage
{
public:
    Home(QGraphicsItem *parent = 0);
};

Home::Home(QGraphicsItem *parent) : MApplicationPage(parent)
{
}

// SwitcherButton stubs
SwitcherButton::SwitcherButton(const QString &title, MWidget *parent, Window window, WindowInfo::WindowPriority windowPriority) :
    MButton(title, parent),
    priority(windowPriority)
{
    Q_UNUSED(title);
    Q_UNUSED(parent);
    Q_UNUSED(window);

    g_windowButtonMap[this] = window;
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

void SwitcherButton::setGeometry(const QRectF &)
{
}


void SwitcherButton::updateIconGeometry()
{
}

void SwitcherButton::resetState()
{
}

WindowInfo::WindowPriority SwitcherButton::windowPriority() const
{
    return priority;
}

void SwitcherButton::setWindowPriority(WindowInfo::WindowPriority windowPriority)
{
    priority = windowPriority;
}

Window SwitcherButton::xWindow()
{
    return g_windowButtonMap[this];
}

bool g_panRequested;
uint g_panRequestIndex;

void PagedPanning::panToPage(uint itemIndex) {
    g_panRequested = true;
    g_panRequestIndex = itemIndex;
    emit pageChanged(itemIndex);
}

PagedPanning::PagedPanning(QObject* parent) : MPhysics2DPanning(parent),
					      pageWidth_(0),
					      currentPage(0),
					      autoIntegrateMode(false),
					      autoIntegrateTargetPage(0)
{

}
PagedPanning::~PagedPanning() {
}

void PagedPanning::integrateAxis(Qt::Orientation, qreal &, qreal &, qreal &, qreal &, bool)
{

}

void PagedPanning::setPageWidth(uint) {
}

uint PagedPanning::pageWidth() const {
    return 0;
}

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

QList< QSharedPointer<SwitcherButton> > Ut_SwitcherView::createButtonList(int buttons)
{
    QList< QSharedPointer<SwitcherButton> > buttonList;
    appendMoreButtonsToList(buttons, buttonList);
    return buttonList;
}

void Ut_SwitcherView::appendMoreButtonsToList(int newButtons, QList< QSharedPointer<SwitcherButton> > &buttonList)
{
    int newCount = buttonList.count() + newButtons;
    for(int i = buttonList.count(); i < newCount; i++){
        QSharedPointer<SwitcherButton> button(new SwitcherButton(QString("Title %1").arg(i), NULL, 1));
        button.data()->setModel(new SwitcherButtonModel());
        buttonList.append(button);
    }
}

void Ut_SwitcherView::init()
{
    g_panRequested = false;
    // Create test switcher
    switcher = new Switcher();
    g_switcherModel = new SwitcherModel;
    switcher->setModel(g_switcherModel);
    m_subject = new TestSwitcherView(switcher);
    switcher->setView(m_subject);
    gSwitcherStub->stubReset();
}

void Ut_SwitcherView::cleanup()
{
    delete m_subject;
    delete g_switcherModel;
}

void Ut_SwitcherView::verifyButtonModesInOverviewMode(QList< QSharedPointer<SwitcherButton> > &buttonList)
{
    int buttons = buttonList.count();
    if (gMSceneManagerStub->orientation() == M::Landscape){
        for(int i = 0; i < buttons; i++){
            if (buttons < 3) {
                QVERIFY(buttonList[i].data()->model()->viewMode() == SwitcherButtonModel::Large);
            } else {
                QVERIFY(buttonList[i].data()->model()->viewMode() == SwitcherButtonModel::Medium);
            }
        }
    } else {
        for(int i = 0; i < buttons; i++){
            if (buttons < 3) {
                QVERIFY(buttonList[i].data()->model()->viewMode() == SwitcherButtonModel::Large);
            } else if (buttons < 5) {
                QVERIFY(buttonList[i].data()->model()->viewMode() == SwitcherButtonModel::Medium);
            } else {
                QVERIFY(buttonList[i].data()->model()->viewMode() == SwitcherButtonModel::Small);
            }
        }
    }
}

void Ut_SwitcherView::testAutoPanningInDetailView()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

    QCOMPARE(g_panRequested, false);

    QList< QSharedPointer<SwitcherButton> > buttonList = createButtonList(4);

    // Change the first button's priority to
    // trigger the panning of the view to show it
    buttonList.first().data()->setWindowPriority(WindowInfo::Call);
    // Update the model with the modified list
    g_switcherModel->setButtons(buttonList);

    // SwitcherView should have called the physics integrator's pan method
    QCOMPARE(g_panRequested, true);
}

void Ut_SwitcherView::testSnapIndexChangedInDetailView()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

    g_switcherModel->setButtons(createButtonList(4));

    // test that snap index changes only when the new
    // snap index is within the button range
    connect(this, SIGNAL(snapIndexChanged(int)),
            m_subject, SLOT(snapIndexChanged(int)));
    emit snapIndexChanged(3);
    QCOMPARE(m_subject->focusedSwitcherButton, 3);
    emit snapIndexChanged(0);
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
    emit snapIndexChanged(-1);
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
    emit snapIndexChanged(9999);
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
}

void Ut_SwitcherView::testPanningStoppedInDetailView()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Detailview);

    g_switcherModel->setButtons(createButtonList(4));

    // test that correct button is emphasized when panning stops
    connect(this, SIGNAL(snapIndexChanged(int)),
            m_subject, SLOT(snapIndexChanged(int)));
    connect(this, SIGNAL(panningStopped()),
            m_subject, SLOT(panningStopped()));
    emit snapIndexChanged(3);
    emit snapIndexChanged(2);
    emit panningStopped();

    QCOMPARE(m_subject->focusedSwitcherButton, 2);
    QCOMPARE(g_switcherModel->buttons().at(0).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(1).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(2).data()->model()->viewMode(), SwitcherButtonModel::Large);
    QCOMPARE(g_switcherModel->buttons().at(3).data()->model()->viewMode(), SwitcherButtonModel::Medium);

    emit snapIndexChanged(1);
    emit snapIndexChanged(2);
    emit snapIndexChanged(3);
    emit panningStopped();

    QCOMPARE(m_subject->focusedSwitcherButton, 3);

    QCOMPARE(g_switcherModel->buttons().at(0).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(1).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(2).data()->model()->viewMode(), SwitcherButtonModel::Medium);
    QCOMPARE(g_switcherModel->buttons().at(3).data()->model()->viewMode(), SwitcherButtonModel::Large);
}

void Ut_SwitcherView::testAutoPanningInOverView()
{
    QCOMPARE(g_panRequested, false);

    QList< QSharedPointer<SwitcherButton> > buttonList = createButtonList(8);
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);

    // Change the first button's priority to
    // trigger the panning of the view to show it
    buttonList.first().data()->setWindowPriority(WindowInfo::Call);
    // Update the model with the modified list
    g_switcherModel->setButtons(buttonList);

    // SwitcherView should have called the physics integrator's pan method
    QCOMPARE(g_panRequested, true);
}

void Ut_SwitcherView::testButtonModesInOverviewMode()
{
    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(3);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    verifyButtonModesInOverviewMode(M::Landscape);

    m_subject->modifiableStyle()->setRowsPerPage(3);
    m_subject->modifiableStyle()->setColumnsPerPage(2);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
    verifyButtonModesInOverviewMode(M::Portrait);
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

    appendMoreButtonsToList(2, buttonList);
    // with 4 buttons
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);

    // with 7 buttons
    appendMoreButtonsToList(3, buttonList);
    g_switcherModel->setButtons(buttonList);
    verifyButtonModesInOverviewMode(buttonList);
}

void Ut_SwitcherView::testPanningStoppedInOverView()
{
    g_switcherModel->setSwitcherMode(SwitcherModel::Overview);
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    m_subject->modifiableStyle()->setRowsPerPage(2);
    m_subject->modifiableStyle()->setColumnsPerPage(3);

    // fill 2 and a half pages
    g_switcherModel->setButtons(createButtonList(15));

    // test that correct button is emphasized when panning stops
    connect(this, SIGNAL(snapIndexChanged(int)),
            m_subject, SLOT(snapIndexChanged(int)));
    connect(this, SIGNAL(panningStopped()),
            m_subject, SLOT(panningStopped()));
    emit snapIndexChanged(1);
    emit snapIndexChanged(2);
    emit panningStopped();
    // Focused on 1st button of last page
    QCOMPARE(m_subject->focusedSwitcherButton, 12);

    emit snapIndexChanged(1);
    emit panningStopped();
    // Focused on 1st button of 2nd page
    QCOMPARE(m_subject->focusedSwitcherButton, 6);

    emit snapIndexChanged(2);
    emit snapIndexChanged(1);
    emit snapIndexChanged(0);
    emit panningStopped();
    // Focused on 1st button of first page
    QCOMPARE(m_subject->focusedSwitcherButton, 0);
}

QTEST_APPLESS_MAIN(Ut_SwitcherView)
