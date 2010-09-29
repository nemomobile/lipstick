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
#include "ut_pagedviewport.h"
#include "pagedviewport.h"
#include "pagedviewportview.h"
#include "pagedpanning.h"
#include "pagepositionindicator.h"
#include "layoutvisualizationwrapper_stub.h"

#include <QGraphicsLinearLayout>

static uint checkPageCount = 0;
static uint testPanTargetPage = 0;
static int gPagedPanningTargetPage = 0;
static uint gPagedPanningSetPage = -1;

const int FIRST_PAGE_INDEX = 0;
const int DEFAULT_NUM_PAGES = 6;
const int DEFAULT_LAST_PAGE_INDEX = DEFAULT_NUM_PAGES - 1;


PagedPanning::PagedPanning(QObject* parent) : MPhysics2DPanning(parent),
                                              pageCount_(1),
                                              currentPage(0),
                                              snapMode(false),
                                              velocityThreshold_(7.0),
                                              dragThreshold_(0.5),
                                              pageSnapSpringK_(0.7),
                                              pageSnapFriction_(0.7),
                                              previousPosition(0),
                                              targetPage_(0)
{
}

PagedPanning::~PagedPanning()
{
}

void PagedPanning::panToPage(int page)
{
    testPanTargetPage = page;
    targetPage_ = page;
    snapMode = true;
    emit pageChanged(page);
}

void PagedPanning::panToCurrentPage()
{
    emit pageChanged(gPagedPanningSetPage);
}

void PagedPanning::integrateAxis(Qt::Orientation, qreal &, qreal &, qreal &, qreal &, bool)
{
    emit pageChanged(testPanTargetPage);
}

void PagedPanning::setPageCount(int pageCount)
{
    checkPageCount = pageCount;
}

int PagedPanning::pageCount() const
{
    return checkPageCount;
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

void PagedPanning::setPageWrapMode(bool /*enable*/)
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

int PagedPanning::activePage() const
{
    return gPagedPanningSetPage;
}

int PagedPanning::targetPage() const
{
    return gPagedPanningTargetPage;
}

float PagedPanning::pageWidth() const
{
    return 0;
}

void PagedPanning::setPage(uint page)
{
    gPagedPanningSetPage = page;
}

void PagedPanning::setPanThreshold(qreal value)
{
    Q_UNUSED(value)
}

void Ut_PagedViewport::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_pagedviewport" };
    app = new MApplication(argc, app_name);

    pannedLayout = NULL;
}

void Ut_PagedViewport::cleanupTestCase()
{
    delete app;
}

void Ut_PagedViewport::init()
{
    m_subject = new PagedViewport(NULL);
    connect(this, SIGNAL(panningStopped()), m_subject->physics(), SIGNAL(panningStopped()));
    connect(this, SIGNAL(pageWrapped()), m_subject->physics(), SIGNAL(pageWrapped()));
    connect(this, SIGNAL(pageChanged(int)), m_subject->physics(), SIGNAL(pageChanged(int)));

    gPagedPanningSetPage = -1;
    gPagedPanningTargetPage = FIRST_PAGE_INDEX;

    checkPageCount = DEFAULT_NUM_PAGES;

    gLayoutVisualizationWrapperStub->stubReset();
}

void Ut_PagedViewport::cleanup()
{
    delete m_subject;
    m_subject = NULL;

    pannedLayout = NULL;
}

void Ut_PagedViewport::fillSubjectWithPages(int numPages)
{
    if (m_subject == NULL) {
        return;
    }

    QGraphicsWidget *pannedWidget = new QGraphicsWidget();
    pannedLayout = new QGraphicsLinearLayout(Qt::Horizontal);

    for (int i = 0; i < numPages; ++i) {
        pannedLayout->addItem(new QGraphicsWidget());
    }

    pannedWidget->setLayout(pannedLayout);

    m_subject->setWidget(pannedWidget);
    m_subject->updatePageCount(numPages);
}

void Ut_PagedViewport::test_updatePageCount()
{
    checkPageCount = 0;
    uint target = 12;
    m_subject->updatePageCount(target);

    QCOMPARE(target, checkPageCount);
}

void Ut_PagedViewport::test_panToPage()
{
    fillSubjectWithPages(10);

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    m_subject->panToPage(1);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 1);
}

void Ut_PagedViewport::test_setPage()
{
    m_subject->setPage(5);
    QCOMPARE(gPagedPanningSetPage, uint(5));
}

void Ut_PagedViewport::test_focusFirstPage()
{
    m_subject->focusFirstPage();
    QCOMPARE(gPagedPanningSetPage, uint(0));
}

void Ut_PagedViewport::testWhenPagePansPositionIndicatorsGetToKnow()
{
    bool ret = m_subject->disconnect(m_subject->pagedPanning, SIGNAL(pageIsPanning(bool)), m_subject->positionIndicator(), SLOT(pageIsPanning(bool)));
    QCOMPARE(ret, true);
}

void Ut_PagedViewport::testWhenTargetPageIsCalledThenTheInformationIsFetchedFromThePhysicsObject()
{
    int TARGET_PAGE = 8;
    gPagedPanningTargetPage = TARGET_PAGE;
    QCOMPARE(m_subject->targetPage(), gPagedPanningTargetPage);
}

void Ut_PagedViewport::testWhenThereIsNoPannebleWidgetAndWrappingIsEnabledThenVisualizationWrapperDoesNotGetCalled()
{
    m_subject->setWidget(new QGraphicsWidget());
    m_subject->setPageWrapMode(true);
    emit panningStopped();

    QCOMPARE(gLayoutVisualizationWrapperStub->stubCallCount("setWrappingMode"), 0);
}

void Ut_PagedViewport::testWhenThereIsNoLinearLayoutInThePannebleWidgetAndWrappingIsEnabledThenVisualizationWrapperDoesNotGetCalled()
{
    m_subject->setPageWrapMode(true);
    emit panningStopped();

    QCOMPARE(gLayoutVisualizationWrapperStub->stubCallCount("setWrappingMode"), 0);
}

void Ut_PagedViewport::verifyLayoutWrapper(LayoutVisualizationWrapper::WrappingMode wrapMode, bool verifyConstruction) const
{
    QString errorString;

    if (verifyConstruction) {
        errorString = "Expected: > 0, actual: ";
        errorString += QString::number(gLayoutVisualizationWrapperStub->stubCallCount("Constructor(QGraphicsLinearLayout)"));
        QVERIFY2(gLayoutVisualizationWrapperStub->stubCallCount("Constructor(QGraphicsLinearLayout)") > 0, qPrintable(errorString));
        QCOMPARE(&gLayoutVisualizationWrapperStub->stubLastCallTo("Constructor(QGraphicsLinearLayout)").parameter<QGraphicsLinearLayout&>(0), pannedLayout);
    }

    errorString = "Expected: > 0, actual: ";
    errorString += QString::number(gLayoutVisualizationWrapperStub->stubCallCount("setWrappingMode"));
    QVERIFY2(gLayoutVisualizationWrapperStub->stubCallCount("setWrappingMode") > 0, qPrintable(errorString));
    QCOMPARE(gLayoutVisualizationWrapperStub->stubLastCallTo("setWrappingMode").parameter<LayoutVisualizationWrapper::WrappingMode>(0), wrapMode);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft()
{
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    emit panningStopped();

    verifyLayoutWrapper(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnLastPageThenVisualizationWrapperWrapsLeftEdgeToRight()
{
    gPagedPanningTargetPage = DEFAULT_LAST_PAGE_INDEX;
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);
    emit panningStopped();

    verifyLayoutWrapper(LayoutVisualizationWrapper::WrapLeftEdgeToRight);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnNonEdgePageThenVisualizationWrapperDoesNotWrap()
{
    gPagedPanningTargetPage = DEFAULT_LAST_PAGE_INDEX / 2;
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);
    emit panningStopped();

    verifyLayoutWrapper(LayoutVisualizationWrapper::NoWrap);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight()
{
    gPagedPanningTargetPage = DEFAULT_LAST_PAGE_INDEX;
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    emit pageWrapped();

    verifyLayoutWrapper(LayoutVisualizationWrapper::WrapLeftEdgeToRight);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft()
{
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    emit pageWrapped();

    verifyLayoutWrapper(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft()
{
    const int SECOND_PAGE_INDEX = 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningTargetPage = SECOND_PAGE_INDEX;
    m_subject->setPage(SECOND_PAGE_INDEX);

    gLayoutVisualizationWrapperStub->stubReset();
    gPagedPanningTargetPage = FIRST_PAGE_INDEX;
    emit pageChanged(FIRST_PAGE_INDEX);

    verifyLayoutWrapper(LayoutVisualizationWrapper::WrapRightEdgeToLeft, false);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToSecondPageThenVisualizationWrapperDoesNotWrap()
{
    const int SECOND_PAGE_INDEX = 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningTargetPage = SECOND_PAGE_INDEX;
    emit pageChanged(SECOND_PAGE_INDEX);

    verifyLayoutWrapper(LayoutVisualizationWrapper::NoWrap);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondLastPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight()
{
    const int SECOND_LAST_PAGE_INDEX = DEFAULT_LAST_PAGE_INDEX - 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningTargetPage = SECOND_LAST_PAGE_INDEX;
    m_subject->setPage(SECOND_LAST_PAGE_INDEX);

    gLayoutVisualizationWrapperStub->stubReset();
    gPagedPanningTargetPage = DEFAULT_LAST_PAGE_INDEX;
    emit pageChanged(DEFAULT_LAST_PAGE_INDEX);

    verifyLayoutWrapper(LayoutVisualizationWrapper::WrapLeftEdgeToRight, false);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToSecondLastPageThenVisualizationWrapperDoesNotWrap()
{
    const int SECOND_LAST_PAGE_INDEX = DEFAULT_LAST_PAGE_INDEX - 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningTargetPage = DEFAULT_LAST_PAGE_INDEX;
    m_subject->setPage(DEFAULT_LAST_PAGE_INDEX);

    gLayoutVisualizationWrapperStub->stubReset();
    gPagedPanningTargetPage = SECOND_LAST_PAGE_INDEX;
    emit pageChanged(SECOND_LAST_PAGE_INDEX);

    verifyLayoutWrapper(LayoutVisualizationWrapper::NoWrap, false);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensBetweenNonEndPagesThenVisualizationWrapperDoesNotWrap()
{
    const int SECOND_PAGE = 1;
    const int THIRD_PAGE = 2;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningTargetPage = SECOND_PAGE;
    emit pageChanged(SECOND_PAGE);

    gLayoutVisualizationWrapperStub->stubReset();

    gPagedPanningTargetPage = THIRD_PAGE;
    emit pageChanged(THIRD_PAGE);

    verifyLayoutWrapper(LayoutVisualizationWrapper::NoWrap, false);
}

void Ut_PagedViewport::testWhenPageWrappingGetsDisabledThenVisualizationWrapperDoesNotGetCalled()
{
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);
    gLayoutVisualizationWrapperStub->stubReset();
    m_subject->setPageWrapMode(false);

    emit panningStopped();

    QCOMPARE(gLayoutVisualizationWrapperStub->stubCallCount("setWrappingMode"), 0);
}

QTEST_APPLESS_MAIN(Ut_PagedViewport)
