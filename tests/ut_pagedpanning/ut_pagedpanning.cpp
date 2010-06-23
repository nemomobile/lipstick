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
    char *app_name = (char *)"./ut_pagepanning";
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
    pagedPanning->pageWidth = pageWidth;
    pagedPanning->setPosition(QPointF(currentPosition, 0));

    int pageCrossings;

    if(leftToRight) {
        pageCrossings = qBound((qreal)0.0,
                               (qreal)moveAmount,
                               (qreal)currentPosition)/pageWidth;
    } else {
        pageCrossings = qBound((qreal)0.0,
                               (qreal)moveAmount,
                               (qreal)(rangeEnd-currentPosition))/pageWidth;
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
        QVERIFY(arguments.at(0).toInt() == targetPage);
    }
}


void Ut_PagedPanning::performMovement(PagedPanning* pagedPanning,
                                      qreal moveAmount,
                                      bool leftToRight,
                                      qreal speed)

{
    int i = 0;
    qreal movePosition = 0;
    bool pointerPressControl = true;

    pagedPanning->pointerPress(QPointF(movePosition, 0));

    while (pointerPressControl || pagedPanning->velocity().x() != 0.0) {
        if (i++ < moveAmount/speed) {
            movePosition += leftToRight ? speed : -speed;
        } else if (pointerPressControl) {
            pagedPanning->pointerRelease();
            pointerPressControl = false;
        }

        if (pointerPressControl) {
            pagedPanning->pointerMove(QPointF(movePosition, 0));
        }

        // Wait for the timer event
        while(!QCoreApplication::hasPendingEvents())
            QTest::qSleep(100);

        QCoreApplication::processEvents();
    }
}


void Ut_PagedPanning::performIntegration(PagedPanning* pagedPanning)
{
    QSignalSpy stopSpy(pagedPanning, SIGNAL(panningStopped()));

    do {
        while (!QCoreApplication::hasPendingEvents())
            QTest::qSleep(100);

        QCoreApplication::processEvents();
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
    m_subject->setVelocityThreshold(10.0);
    m_subject->setPointerSpringK(1.0);

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    performMovement(m_subject,
                    1,    // amount to move
                    false, // left-to-right
                    7.0); // speed under threshold

    // Should end up where started
    QCOMPARE(m_subject->position().x(), 100.0);

    performMovement(m_subject,
                    1,    // amount to move
                    false, // left-to-right
                    11.0); // more speed

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
                    4.0); // more speed

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

    m_subject->currentPage = 2;
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

void Ut_PagedPanning::testSetFirstPagePosition()
{
    //TODO: add testing of setting position to the right-most page
    // when using right-to-left layout

    // first set position to be something else than 0
    m_subject->setPosition(QPointF(200.0, 0.0));

    m_subject->setFirstPagePosition();

    QCOMPARE(m_subject->position().x(), 0.0);
    QCOMPARE(m_subject->currentPage, 0);
    QCOMPARE(m_subject->targetPage, 0);
}

QTEST_APPLESS_MAIN(Ut_PagedPanning)
