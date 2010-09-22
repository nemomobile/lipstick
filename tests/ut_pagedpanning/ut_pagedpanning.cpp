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
}

void Ut_PagedPanning::cleanup()
{
    delete m_subject;
}

// Test cases
void Ut_PagedPanning::testCreation()
{
    QCOMPARE((int)m_subject->pageCount(), 1);
}

void Ut_PagedPanning::testMovementSmallerThenPageWidthRightToLeft()
{
    qreal currentPosition = 500.0;

    testMovement(m_subject,
                 11,               // Page count
                 currentPosition,  // The position where the movement starts
                 10.0,             // Move amount
                 500.0,            // Where the current position should end up after movement
                 false,            // Left to right
                 5);               // Target page index after move
}

void Ut_PagedPanning::testMovementGreaterThenPageWidthRightToLeft()
{
    qreal currentPosition = 500.0;

    testMovement(m_subject,
                 11,               // Page count
                 currentPosition,  // The position where the movement starts
                 120.0,            // Move amount
                 600.0,            // Where the current position should end up after movement
                 false,            // Left to right
                 6);               // Target page index after move
}

void Ut_PagedPanning::testMovementSmallerThenPageWidthLeftToRight()
{
    qreal currentPosition = 200.0;

    testMovement(m_subject,
                 11,               // Page count
                 currentPosition,  // The position where the movement starts
                 10.0,             // Move amount
                 200.0,            // Where the current position should end up after movement
                 true,             // Left to right
                 2);               // Target page index after move
}

void Ut_PagedPanning::testMovementGreaterThenPageWidthLeftToRight()
{
    qreal currentPosition = 200.0;
    testMovement(m_subject,
                 11,               // Page count
                 currentPosition,  // The position where the movement starts
                 120.0,            // Move amount
                 100.0,            // Where the current position should end up after movement
                 true,             // Left to right
                 1);               // Target page index after move
}

void Ut_PagedPanning::testHugeMovementLeftToRight()
{
    qreal currentPosition = 400.0;
    testMovement(m_subject,
                 11,               // Page count
                 currentPosition,  // The position where the movement starts
                 700.0,            // Move amount
                 0,                // Where the current position should end up after movement
                 true,             // Left to right
                 0);               // Target page index after move
}

void Ut_PagedPanning::testHugeMovementRightToLeft()
{
    qreal currentPosition = 400.0;
    testMovement(m_subject,
                 11,              // Page count
                 currentPosition,  // The position where the movement starts
                 1000.0,           // Move amount
                 1000.0,           // Where the current position should end up after movement
                 false,            // Left to right
                 10);              // Target page index after move
}

void Ut_PagedPanning::testMovementExcatlyPageWidth()
{
    qreal currentPosition = 200.0;
    testMovement(m_subject,
                 11,              // Page count
                 currentPosition,  // The position where the movement starts
                 100.0,            // Move amount
                 100.0 ,           // Where the current position should end up after movement
                 true,             // Left to right
                 1);               // Target page index after move
}

void Ut_PagedPanning::testMovementWithWrappingRightToLeft()
{
    m_subject->setPageWrapMode(true);

    qreal currentPosition = 1000.0;

    testMovement(m_subject,
                 11,               // Page count
                 currentPosition,  // The position where the movement starts
                 100.0,            // Move amount
                 0.0,              // Where the current position should end up after movement
                 false,            // Left to right
                 0);               // Target page index after move
}

void Ut_PagedPanning::testMovementWithWrappingLeftToRight()
{
    m_subject->setPageWrapMode(true);

    qreal currentPosition = 0.0;

    testMovement(m_subject,
                 11,               // Page count
                 currentPosition,  // The position where the movement starts
                 100.0,            // Move amount
                 1000.0,           // Where the current position should end up after movement
                 true,             // Left to right
                 10);              // Target page index after move
}

void Ut_PagedPanning::testAutoPanning()
{
    qreal velocity = 0;
    qreal acceleration = 0;
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    fillDefaultIntegrationParameters(m_subject, 11, 0, 1000);
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
    fillDefaultIntegrationParameters(m_subject, 11, 0, 1000);

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

    fillDefaultIntegrationParameters(m_subject, 11, 0, 1000);
    QCOMPARE(spy.count(), 0);
}

void Ut_PagedPanning::testMovement(PagedPanning* pagedPanning,
                                   int pageCount,
                                   qreal currentPosition,
                                   qreal moveAmount,
                                   qreal targetPosition,
                                   bool leftToRight,
                                   int targetPage,
                                   qreal rangeStart,
                                   qreal rangeEnd)
{
    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;

    fillDefaultIntegrationParameters(pagedPanning, pageCount, rangeStart, rangeEnd);

    qreal pageWidth = (rangeEnd - rangeStart) / qMax(1, pageCount-1);
    pagedPanning->currentPage = currentPosition/pageWidth;
    pagedPanning->pageWidth_ = pageWidth;
    pagedPanning->setPosition(QPointF(currentPosition, 0));

    int pageCrossings;

    if(m_subject->wrapMode) {
        pageCrossings = (qreal)moveAmount / pageWidth;
    } else {
        pageCrossings = qBound((qreal)0.0, (qreal)moveAmount, leftToRight ? currentPosition : rangeEnd - currentPosition) / pageWidth;
    }

    velocity = 0;
    pointerSpring = 0;
    acceleration = 0;
    QSignalSpy spy(pagedPanning, SIGNAL(pageChanged(int)));

    performMovement(pagedPanning,
                    moveAmount, // amount to move
                    leftToRight); // left-to-right

    QCOMPARE(velocity, 0.0);
    QCOMPARE(pagedPanning->position().x(), targetPosition);

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


void Ut_PagedPanning::performMovement(PagedPanning* pagedPanning,
                                      qreal moveAmount,
                                      bool leftToRight,
                                      qreal speed)
{
    Ut_MPhysics2DPanning physicsDriver(pagedPanning);

    int i = 0;
    qreal movePosition = 0;
    bool pointerPressControl = true;

    pagedPanning->pointerPress(QPointF(movePosition, 0));


    //Simulate the movements of mouse
    while (pointerPressControl || pagedPanning->velocity().x() != 0.0) {
        if (i++ < (moveAmount / speed)) {
            //If we have not reached the end position calculate the next mouse position
            movePosition += leftToRight ? speed : -speed;
        } else if (pointerPressControl) {
            //We have reached the end position so release the mouse
            pagedPanning->pointerRelease();
            pointerPressControl = false;
        }

        if (pointerPressControl) {
            //Simulate the swipe by updating mouse location
            pagedPanning->pointerMove(QPointF(movePosition, 0));
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
    pagedPanning->setRange(QRectF(rangeStart, 0, rangeEnd, 0));
    pagedPanning->setDragThreshold(0.5);
    pagedPanning->setVelocityThreshold(100.0);

    pagedPanning->setPageCount(newPageCount);
}


void Ut_PagedPanning::testDragThreshold()
{
    qreal currentPosition = 200.0;

    fillDefaultIntegrationParameters(m_subject, 11, 0, 1000);

    m_subject->currentPage = 2;
    m_subject->setDragThreshold(0.2);
    m_subject->setPosition(QPointF(currentPosition, 0));

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    // Drag the pointer a bit, but don't cross the drag threshold
    performMovement(m_subject,
                    10, // amount to move
                    true); // left-to-right

    QCOMPARE(m_subject->position().x(), 200.0);

    // The page shouldn't have changed yet
    QCOMPARE(spy.count(), 0);

    spy.clear();

    // Drag the pointer over the drag threshold
    performMovement(m_subject,
                    22, // amount to move
                    false); // left-to-right

    QCOMPARE(m_subject->position().x(), 300.0);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 3);
}

void Ut_PagedPanning::testVelocityThreshold()
{
    qreal currentPosition = 100.0;

    fillDefaultIntegrationParameters(m_subject, 11, 0, 1000);

    m_subject->currentPage = 1;
    m_subject->setPosition(QPointF(currentPosition, 0));
    m_subject->setVelocityThreshold(9.0);
    m_subject->setPointerSpringK(1.0);


    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    performMovement(m_subject,
                    1,    // amount to move
                    false, // left-to-right
                    7.0); // speed under threshold

    // Should end up where started
    QCOMPARE(m_subject->position().x(), 100.0);

    performMovement(m_subject,
                    31,    // amount to move
                    false, // left-to-right
                    10.0); // more speed

    // Should end up on the next page now
    QCOMPARE(m_subject->position().x(), 200.0);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 2);
}

void Ut_PagedPanning::testSlide()
{
    qreal currentPosition = 100.0;

    fillDefaultIntegrationParameters(m_subject, 11, 0, 1000);

    m_subject->currentPage = 1;
    m_subject->setPosition(QPointF(currentPosition, 0));
    m_subject->setSlidingFriction(0.02);

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    performMovement(m_subject,
                    70,    // amount to move
                    false, // left-to-right
                    8.0); // more speed

    // Should have slid over three pages
    QCOMPARE(m_subject->position().x(), 400.0);

    QCOMPARE(spy.count(), 3);
    QList<QVariant> arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 4);

    spy.clear();

    performMovement(m_subject,
                    70,    // amount to move
                    true,  // left-to-right
                    10.0); // ridiculous speed

    // The view should slide all the way to right
    QCOMPARE(m_subject->position().x(), 0.0);

    QCOMPARE(spy.count(), 4);
    arguments = spy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 0);

    // Limit the slide to one page
    m_subject->setSlideLimit(1);

    spy.clear();

    performMovement(m_subject,
                    30,    // amount to move
                    false,  // left-to-right
                    20.0); // ludicrous speed

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

    fillDefaultIntegrationParameters(m_subject, 11, 0, 1000);
    // In order for this test to work the setup has to be set up
    // so that the integrators internal state is consistent
    // -> when there is no movement the target page and the 
    // currentPage are the same
    m_subject->currentPage = 2;
    m_subject->targetPage = 2;
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
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    m_subject->setPage(1);
    QCOMPARE(spy.count(), 0);

    m_subject->setRange(QRectF(0, 0, 100, 100));
    m_subject->setPageCount(3);
    m_subject->setPage(1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_subject->currentPage, 1);
    QCOMPARE(m_subject->targetPage, 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 1);
    QCOMPARE(m_subject->position().x(), qreal(50.0f));

    m_subject->setPage(2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_subject->currentPage, 2);
    QCOMPARE(m_subject->targetPage, 2);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 2);
    QCOMPARE(m_subject->position().x(), qreal(100.0f));
}


QTEST_APPLESS_MAIN(Ut_PagedPanning)
