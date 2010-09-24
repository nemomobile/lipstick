/*
 * ut_switcherphysicsintegrationstrategy.cpp
 *
 * This file is part of home
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */
#include <QtTest/QtTest>
#include <MApplication>
#include <QSignalSpy>
#include "pagedpanning.h"
#include "ut_pagedpanning.h"
#include <cstdlib>

const int DEFAULT_NUM_PAGES = 11;
const qreal DEFAULT_PAGE_WIDTH = 100.0;

void Ut_PagedPanning::initTestCase()
{
    int argc = 1;
    char *app_name = (char *)"./ut_pagedpanning";
    app = new MApplication(argc, &app_name);
}

void Ut_PagedPanning::cleanupTestCase()
{
    delete app;
}

void Ut_PagedPanning::init()
{
    m_subject = new PagedPanning(NULL);
    QCOMPARE((int)m_subject->pageCount(), 1);

    fillDefaultIntegrationParameters(m_subject, DEFAULT_NUM_PAGES);
}

void Ut_PagedPanning::cleanup()
{
    delete m_subject;
}

// Test cases
void Ut_PagedPanning::testMovementSmallerThenPageWidthRightToLeft()
{
    int currentPage = 5;

    testMovement(currentPage,      // The position where the movement starts
                 10.0,             // Move amount
                 false,            // Left to right
                 5);               // Target page index after move
}

void Ut_PagedPanning::testMovementGreaterThenPageWidthRightToLeft()
{
    int currentPage = 5;

    testMovement(currentPage,      // The position where the movement starts
                 120.0,            // Move amount
                 false,            // Left to right
                 6);               // Target page index after move
}

void Ut_PagedPanning::testMovementSmallerThenPageWidthLeftToRight()
{
    int currentPage = 2;

    testMovement(currentPage,      // The position where the movement starts
                 10.0,             // Move amount
                 true,             // Left to right
                 2);               // Target page index after move
}

void Ut_PagedPanning::testMovementGreaterThenPageWidthLeftToRight()
{
    int currentPage = 2;
    testMovement(currentPage,      // The position where the movement starts
                 120.0,            // Move amount
                 true,             // Left to right
                 1);               // Target page index after move
}

void Ut_PagedPanning::testHugeMovementLeftToRight()
{
    int currentPage = 4;
    testMovement(currentPage,      // The position where the movement starts
                 700.0,            // Move amount
                 true,             // Left to right
                 0);               // Target page index after move
}

void Ut_PagedPanning::testHugeMovementRightToLeft()
{
    int currentPage = 4;
    testMovement(currentPage,      // The position where the movement starts
                 1000.0,           // Move amount
                 false,            // Left to right
                 10);              // Target page index after move
}

void Ut_PagedPanning::testMovementExcatlyPageWidth()
{
    int currentPage = 2;
    testMovement(currentPage,      // The position where the movement starts
                 100.0,            // Move amount
                 true,             // Left to right
                 1);               // Target page index after move
}

void Ut_PagedPanning::testMovementWithWrappingRightToLeft()
{
    m_subject->setPageWrapMode(true);
    fillDefaultIntegrationParameters(m_subject, 3);
    QSignalSpy wrappedSpy(m_subject, SIGNAL(pageWrapped()));

    int currentPage = 2;

    testMovement(currentPage,      // The position where the movement starts
                 100.0,            // Move amount
                 false,            // Left to right
                 0);               // Target page index after move

    QCOMPARE(wrappedSpy.count(), 1);
}

void Ut_PagedPanning::testMovementWithWrappingLeftToRight()
{
    m_subject->setPageWrapMode(true);
    fillDefaultIntegrationParameters(m_subject, 3);
    QSignalSpy wrappedSpy(m_subject, SIGNAL(pageWrapped()));

    int currentPage = 0;

    testMovement(currentPage,      // The position where the movement starts
                 100.0,            // Move amount
                 true,             // Left to right
                 2);               // Target page index after move

    QCOMPARE(wrappedSpy.count(), 1);
}

void Ut_PagedPanning::testAutoPanning()
{
    qreal velocity = 0;
    qreal acceleration = 0;
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    fillDefaultIntegrationParameters(m_subject, DEFAULT_NUM_PAGES, 0, 1000);
    m_subject->currentPage = 3;
    m_subject->setPosition(QPointF(300.0, 0.0));

    m_subject->panToPage(0);

    performIntegration(m_subject);

    QCOMPARE(m_subject->position().x(), 0.0);
    QList<QVariant> arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 0);

    spy.clear();

    velocity = 0;
    acceleration = 0;

    m_subject->panToPage(2);

    performIntegration(m_subject);

    QCOMPARE(m_subject->position().x(), 200.0);
    arguments = spy.takeLast();
    QVERIFY(arguments.at(0).toInt() == 2);

}

void Ut_PagedPanning::testCurrentPageRemainsSameWhenPageCountChanges()
{
    fillDefaultIntegrationParameters(m_subject, DEFAULT_NUM_PAGES, 0, 1000);

    qreal currentPosition = 400.0;
    m_subject->currentPage = 4;
    m_subject->setPosition(QPointF(currentPosition, 0));

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    // Change the page count and set the range accordingly
    m_subject->setPageCount(21);
    m_subject->setRange(QRectF(0, 0, 2000, 0));

    performIntegration(m_subject);

    // Neither the position or the page should have changed
    QCOMPARE(m_subject->position().x(), 400.0);
    QCOMPARE(spy.count(), 0);

    // Set the count to be less than the current page and reduce the range
    m_subject->setPageCount(3);
    m_subject->setRange(QRectF(0, 0, 200, 0));

    performIntegration(m_subject);

    // Now we should have ended on the last page
    QCOMPARE(m_subject->position().x(), 200.0);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 2);
}

void Ut_PagedPanning::testSetPageCount()
{
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    fillDefaultIntegrationParameters(m_subject, DEFAULT_NUM_PAGES, 0, 1000);
    QCOMPARE(spy.count(), 0);
}

void Ut_PagedPanning::testMovement(int currentPage,
                                   qreal moveAmount,
                                   bool leftToRight,
                                   int targetPage,
                                   qreal speed)
{
    int pageCount = m_subject->pageCount();

    qreal rangeStart = 0.0;
    qreal rangeEnd = (pageCount - 1) * DEFAULT_PAGE_WIDTH;

    qreal pageWidth = (rangeEnd - rangeStart) / qMax(1, pageCount - 1);
    qreal currentPosition = currentPage * pageWidth;
    m_subject->currentPage = currentPage;
    m_subject->setPosition(QPointF(currentPosition, 0));

    int pageCrossings = std::abs(targetPage - currentPage);

    if (m_subject->wrapMode) {
        pageCrossings = (qreal)moveAmount / pageWidth;
    }

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    performMovement(moveAmount,
                    leftToRight,
                    targetPage,
                    speed);

    QCOMPARE(m_subject->position().x(), targetPage * DEFAULT_PAGE_WIDTH);

    QCOMPARE(spy.count(), pageCrossings);
    if (pageCrossings > 0) {
        QList<QVariant> arguments = spy.takeLast();
        QCOMPARE(arguments.at(0).toInt(), targetPage);
    }
}

Ut_MPhysics2DPanning::Ut_MPhysics2DPanning(MPhysics2DPanning *physics) :
        physics(physics)
{
    connect(this, SIGNAL(notify(QVariant)), physics, SLOT(_q_integrator(QVariant)));
}

void Ut_MPhysics2DPanning::advancePhysicsCalculation() {
    emit notify(QVariant());
}


void Ut_PagedPanning::performMovement(qreal moveAmount,
                                      bool leftToRight,
                                      int targetPage,
                                      qreal speed)
{
    Ut_MPhysics2DPanning physicsDriver(m_subject);

    int i = 0;
    qreal movePosition = 0;
    bool pointerPressControl = true;

    m_subject->pointerPress(QPointF(movePosition, 0));


    //Simulate the movements of mouse
    while (pointerPressControl || m_subject->velocity().x() != 0.0) {
        if (i++ < (moveAmount / speed)) {
            //If we have not reached the end position calculate the next mouse position
            movePosition += leftToRight ? speed : -speed;
        } else if (pointerPressControl) {
            //We have reached the end position so release the mouse
            m_subject->pointerRelease();
            pointerPressControl = false;
            QCOMPARE(m_subject->targetPage(), targetPage);
        }

        if (pointerPressControl) {
            //Simulate the swipe by updating mouse location
            m_subject->pointerMove(QPointF(movePosition, 0));
        }

        physicsDriver.advancePhysicsCalculation();
    }
}


void Ut_PagedPanning::performIntegration(PagedPanning* pagedPanning)
{
    Ut_MPhysics2DPanning physicsDriver(pagedPanning);
    QSignalSpy stopSpy(pagedPanning, SIGNAL(panningStopped()));

    do {
        physicsDriver.advancePhysicsCalculation();
    } while (stopSpy.count() == 0);

}

void Ut_PagedPanning::fillDefaultIntegrationParameters(PagedPanning* pagedPanning, qreal newPageCount, qreal rangeStart, qreal rangeEnd)
{
    pagedPanning->setPointerSpringK(0.6);
    pagedPanning->setFriction(0.9);
    pagedPanning->setSlidingFriction(0.9);
    pagedPanning->setBorderSpringK(0.9);
    pagedPanning->setBorderFriction(0.9);
    pagedPanning->setDragThreshold(0.5);
    pagedPanning->setVelocityThreshold(100.0);

    if (rangeStart == 0.0 && rangeEnd == 0.0) {
        rangeEnd = (newPageCount - 1) * DEFAULT_PAGE_WIDTH;
    }
    pagedPanning->setRange(QRectF(rangeStart, 0, rangeEnd, 0));
    pagedPanning->setPageCount(newPageCount);
}


void Ut_PagedPanning::testDragThreshold()
{
    m_subject->setDragThreshold(0.2);

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    // Drag the pointer a bit, but don't cross the drag threshold
    testMovement(2,
                 10.0,
                 true,
                 2);

    QCOMPARE(m_subject->position().x(), 200.0);

    // The page shouldn't have changed yet
    QCOMPARE(spy.count(), 0);

    spy.clear();

    // Drag the pointer over the drag threshold
    testMovement(2,
                 22.0,
                 false,
                 3);

    QCOMPARE(m_subject->position().x(), 300.0);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 3);
}

void Ut_PagedPanning::testVelocityThreshold()
{
    m_subject->setVelocityThreshold(9.0);
    m_subject->setPointerSpringK(1.0);


    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    testMovement(1,
                 1.0,
                 false,
                 1,
                 7.0);

    // Should end up where started
    QCOMPARE(m_subject->position().x(), 100.0);

    testMovement(1,
                 31.0,
                 false,
                 2,
                 10.0);

    // Should end up on the next page now
    QCOMPARE(m_subject->position().x(), 200.0);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 2);
}

void Ut_PagedPanning::testSlide()
{
    m_subject->setSlidingFriction(0.02);

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    testMovement(1,
                 70,
                 false,
                 4,
                 8.0);

    // Should have slid over three pages
    QCOMPARE(m_subject->position().x(), 400.0);

    QCOMPARE(spy.count(), 3);
    QList<QVariant> arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 4);

    spy.clear();

    testMovement(4,
                 70,
                 true,
                 0,
                 10.0);

    // The view should slide all the way to right
    QCOMPARE(m_subject->position().x(), 0.0);

    QCOMPARE(spy.count(), 4);
    arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 0);

    // Limit the slide to one page
    m_subject->setSlideLimit(1);

    spy.clear();

    testMovement(0,
                 30,
                 false,
                 1,
                 20.0);

    // With such a huge swing of a gesture,
    // don't mind the number of pages crossed,
    // just check that the final page was correct
    arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 1);
}

void Ut_PagedPanning::testSetRange()
{
    qreal currentPosition = 200.0;
    qreal velocity = 0.0;
    qreal acceleration = 0.0;
    qreal pointerSpring = 0.0;

    fillDefaultIntegrationParameters(m_subject, DEFAULT_NUM_PAGES, 0, 1000);
    // In order for this test to work the setup has to be set up
    // so that the integrators internal state is consistent
    // -> when there is no movement the target page and the
    // currentPage are the same
    m_subject->currentPage = 2;
    m_subject->targetPage_ = 2;
    m_subject->setPosition(QPointF(currentPosition, 0));

    m_subject->setRange(QRectF(0, 0, 550, 0));

    // The position should change immediately in one integration step
    m_subject->integrateAxis(Qt::Horizontal,
            currentPosition,
            velocity,
            acceleration,
            pointerSpring,
            false);

    QCOMPARE(currentPosition, 110.0);
    QCOMPARE(m_subject->currentPage, 2);
}

void Ut_PagedPanning::testSetPage()
{
    delete m_subject;
    m_subject = new PagedPanning(NULL);

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    m_subject->setPage(1);
    QCOMPARE(spy.count(), 0);

    m_subject->setRange(QRectF(0, 0, 100, 100));
    m_subject->setPageCount(3);
    m_subject->setPage(1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_subject->currentPage, 1);
    QCOMPARE(m_subject->targetPage_, 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 1);
    QCOMPARE(m_subject->position().x(), qreal(50.0f));

    m_subject->setPage(2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_subject->currentPage, 2);
    QCOMPARE(m_subject->targetPage_, 2);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 2);
    QCOMPARE(m_subject->position().x(), qreal(100.0f));
}

void Ut_PagedPanning::testWhenPageStartsToPanPanningEnabledSignalIsEmitted()
{
    QSignalSpy panningEnabledSpy(m_subject, SIGNAL(pageIsPanning(bool)));
    m_subject->pointerPress(QPointF(0,0));
    QCOMPARE(panningEnabledSpy.count(), 1);
    QCOMPARE(panningEnabledSpy.takeFirst().at(0).toBool(), true);
}

void Ut_PagedPanning::testWhenPageStopsPanningPanningEnabledSignalIsEmitted()
{
    QSignalSpy panningEnabledSpy(m_subject, SIGNAL(pageIsPanning(bool)));
    // Set up pages so that pan to page does actually happen
    m_subject->previousPosition = 2;
    m_subject->currentPage = 0;
    m_subject->panToCurrentPage();
    QCOMPARE(panningEnabledSpy.count(), 1);
    QCOMPARE(panningEnabledSpy.takeFirst().at(0).toBool(), false);
}

QTEST_APPLESS_MAIN(Ut_PagedPanning)
