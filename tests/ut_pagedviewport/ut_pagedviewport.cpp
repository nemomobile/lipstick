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
#include "pagedpanning_stub.h"
#include "pagepositionindicator.h"
#include "layoutvisualizationwrapper_stub.h"

#include <QGraphicsLinearLayout>

const int FIRST_PAGE_INDEX = 0;
const int DEFAULT_NUM_PAGES = 6;
const int DEFAULT_LAST_PAGE_INDEX = DEFAULT_NUM_PAGES - 1;

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

    gPagedPanningStub->stubReset();
    gPagedPanningStub->stubSetReturnValue("targetPage", FIRST_PAGE_INDEX);
    gPagedPanningStub->stubSetReturnValue("pageCount", DEFAULT_NUM_PAGES);

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
    gPagedPanningStub->stubSetReturnValue("pageCount", numPages);
}

void Ut_PagedViewport::testInitialState()
{
    QVERIFY(disconnect(m_subject->pagedPanning, SIGNAL(pageIsPanning(bool)), m_subject, SIGNAL(pageIsPanning(bool))));
}

void Ut_PagedViewport::test_updatePageCount()
{
    const int TARGET_PAGE = 12;
    m_subject->updatePageCount(TARGET_PAGE);

    QCOMPARE(gPagedPanningStub->stubCallCount("setPageCount"), 1);
    QCOMPARE(gPagedPanningStub->stubLastCallTo("setPageCount").parameter<int>(0), TARGET_PAGE);
}

void Ut_PagedViewport::test_panToPage()
{
    fillSubjectWithPages(10);

    const int TARGET_PAGE = 1;

    m_subject->panToPage(TARGET_PAGE);

    QCOMPARE(gPagedPanningStub->stubCallCount("panToPage"), 1);
    QCOMPARE(gPagedPanningStub->stubLastCallTo("panToPage").parameter<int>(0), TARGET_PAGE);
}

void Ut_PagedViewport::test_setPage()
{
    const uint TARGET_PAGE = 5;

    m_subject->setPage(TARGET_PAGE);

    QCOMPARE(gPagedPanningStub->stubCallCount("setPage"), 1);
    QCOMPARE(gPagedPanningStub->stubLastCallTo("setPage").parameter<uint>(0), TARGET_PAGE);
}

void Ut_PagedViewport::test_focusFirstPage()
{
    m_subject->focusFirstPage();

    QCOMPARE(gPagedPanningStub->stubCallCount("setPage"), 1);
    QCOMPARE(gPagedPanningStub->stubLastCallTo("setPage").parameter<uint>(0), (uint)0);
}

void Ut_PagedViewport::testWhenPagePansPositionIndicatorsGetToKnow()
{
    bool ret = m_subject->disconnect(m_subject->pagedPanning, SIGNAL(pageIsPanning(bool)), m_subject->positionIndicator(), SLOT(pageIsPanning(bool)));
    QCOMPARE(ret, true);
}

void Ut_PagedViewport::testWhenTargetPageIsCalledThenTheInformationIsFetchedFromThePhysicsObject()
{
    int TARGET_PAGE = 8;
    gPagedPanningStub->stubSetReturnValue("targetPage", TARGET_PAGE);

    QCOMPARE(m_subject->targetPage(), TARGET_PAGE);
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

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft_data()
{
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft()
{
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    emit panningStopped();

    verifyLayoutWrapper(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnLastPageThenVisualizationWrapperWrapsLeftEdgeToRight_data()
{
    QTest::addColumn<int>("layoutDirection");
    QTest::addColumn<int>("wrappingMode");

    QTest::newRow("Left to right") << (int)Qt::LeftToRight << (int)LayoutVisualizationWrapper::WrapLeftEdgeToRight;
    QTest::newRow("Right to left") << (int)Qt::RightToLeft << (int)LayoutVisualizationWrapper::WrapRightEdgeToLeft;
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnLastPageThenVisualizationWrapperWrapsLeftEdgeToRight()
{
    QFETCH(int, layoutDirection);
    QFETCH(int, wrappingMode);

    m_subject->setLayoutDirection((Qt::LayoutDirection)layoutDirection);

    gPagedPanningStub->stubSetReturnValue("targetPage", DEFAULT_LAST_PAGE_INDEX);
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);
    emit panningStopped();

    verifyLayoutWrapper((LayoutVisualizationWrapper::WrappingMode)wrappingMode);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndPanningStopsOnNonEdgePageThenVisualizationWrapperDoesNotWrap()
{
    gPagedPanningStub->stubSetReturnValue("targetPage", DEFAULT_LAST_PAGE_INDEX / 2);
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);
    emit panningStopped();

    verifyLayoutWrapper(LayoutVisualizationWrapper::NoWrap);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight_data()
{
    QTest::addColumn<int>("layoutDirection");
    QTest::addColumn<int>("wrappingMode");

    QTest::newRow("Left to right") << (int)Qt::LeftToRight << (int)LayoutVisualizationWrapper::WrapLeftEdgeToRight;
    QTest::newRow("Right to left") << (int)Qt::RightToLeft << (int)LayoutVisualizationWrapper::WrapRightEdgeToLeft;
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight()
{
    QFETCH(int, layoutDirection);
    QFETCH(int, wrappingMode);

    m_subject->setLayoutDirection((Qt::LayoutDirection)layoutDirection);

    gPagedPanningStub->stubSetReturnValue("targetPage", DEFAULT_LAST_PAGE_INDEX);
    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    emit pageWrapped();

    verifyLayoutWrapper((LayoutVisualizationWrapper::WrappingMode)wrappingMode);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft_data()
{
    QTest::addColumn<int>("layoutDirection");
    QTest::addColumn<int>("wrappingMode");

    QTest::newRow("Left to right") << (int)Qt::LeftToRight << (int)LayoutVisualizationWrapper::WrapRightEdgeToLeft;
    QTest::newRow("Right to left") << (int)Qt::RightToLeft << (int)LayoutVisualizationWrapper::WrapLeftEdgeToRight;
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft()
{
    QFETCH(int, layoutDirection);
    QFETCH(int, wrappingMode);

    m_subject->setLayoutDirection((Qt::LayoutDirection)layoutDirection);

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    emit pageWrapped();

    verifyLayoutWrapper((LayoutVisualizationWrapper::WrappingMode)wrappingMode);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft_data()
{
    QTest::addColumn<int>("layoutDirection");
    QTest::addColumn<int>("wrappingMode");

    QTest::newRow("Left to right") << (int)Qt::LeftToRight << (int)LayoutVisualizationWrapper::WrapRightEdgeToLeft;
    QTest::newRow("Right to left") << (int)Qt::RightToLeft << (int)LayoutVisualizationWrapper::WrapLeftEdgeToRight;
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft()
{
    QFETCH(int, layoutDirection);
    QFETCH(int, wrappingMode);

    m_subject->setLayoutDirection((Qt::LayoutDirection)layoutDirection);

    const int SECOND_PAGE_INDEX = 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningStub->stubSetReturnValue("activePage", SECOND_PAGE_INDEX);
    m_subject->setPage(SECOND_PAGE_INDEX);

    gPagedPanningStub->stubSetReturnValue("targetPage", FIRST_PAGE_INDEX);
    emit pageChanged(FIRST_PAGE_INDEX);

    verifyLayoutWrapper((LayoutVisualizationWrapper::WrappingMode)wrappingMode, false);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToSecondPageThenVisualizationWrapperDoesNotWrap()
{
    const int SECOND_PAGE_INDEX = 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningStub->stubSetReturnValue("targetPage", SECOND_PAGE_INDEX);
    emit pageChanged(SECOND_PAGE_INDEX);

    verifyLayoutWrapper(LayoutVisualizationWrapper::NoWrap);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondLastPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight_data()
{
    QTest::addColumn<int>("layoutDirection");
    QTest::addColumn<int>("wrappingMode");

    QTest::newRow("Left to right") << (int)Qt::LeftToRight << (int)LayoutVisualizationWrapper::WrapLeftEdgeToRight;
    QTest::newRow("Right to left") << (int)Qt::RightToLeft << (int)LayoutVisualizationWrapper::WrapRightEdgeToLeft;
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondLastPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight()
{
    QFETCH(int, layoutDirection);
    QFETCH(int, wrappingMode);

    m_subject->setLayoutDirection((Qt::LayoutDirection)layoutDirection);

    const int SECOND_LAST_PAGE_INDEX = DEFAULT_LAST_PAGE_INDEX - 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningStub->stubSetReturnValue("activePage", SECOND_LAST_PAGE_INDEX);
    m_subject->setPage(SECOND_LAST_PAGE_INDEX);

    gPagedPanningStub->stubSetReturnValue("targetPage", DEFAULT_LAST_PAGE_INDEX);
    emit pageChanged(DEFAULT_LAST_PAGE_INDEX);

    verifyLayoutWrapper((LayoutVisualizationWrapper::WrappingMode)wrappingMode, false);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToSecondLastPageThenVisualizationWrapperDoesNotWrap()
{
    const int SECOND_LAST_PAGE_INDEX = DEFAULT_LAST_PAGE_INDEX - 1;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningStub->stubSetReturnValue("targetPage", DEFAULT_LAST_PAGE_INDEX);
    m_subject->setPage(DEFAULT_LAST_PAGE_INDEX);

    gLayoutVisualizationWrapperStub->stubReset();
    gPagedPanningStub->stubSetReturnValue("targetPage", SECOND_LAST_PAGE_INDEX);
    emit pageChanged(SECOND_LAST_PAGE_INDEX);

    verifyLayoutWrapper(LayoutVisualizationWrapper::NoWrap, false);
}

void Ut_PagedViewport::testWhenPageWrappingIsEnabledAndWrappingHappensBetweenNonEndPagesThenVisualizationWrapperDoesNotWrap()
{
    const int SECOND_PAGE = 1;
    const int THIRD_PAGE = 2;

    fillSubjectWithPages(DEFAULT_NUM_PAGES);
    m_subject->setPageWrapMode(true);

    gPagedPanningStub->stubSetReturnValue("targetPage", SECOND_PAGE);
    emit pageChanged(SECOND_PAGE);

    gLayoutVisualizationWrapperStub->stubReset();

    gPagedPanningStub->stubSetReturnValue("targetPage", THIRD_PAGE);
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
