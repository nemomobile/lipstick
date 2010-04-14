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
#include <QDebug>
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
    QCOMPARE((int)m_subject->pageWidth(), 0);
}

void Ut_PagedPanning::testMovementSmallerThenPageWidthRightToLeft()
{
    qreal currentPosition = 500.0;
    testMovement(m_subject,
                 100,              // Snap interval
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
                 100,              // Page interval
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
                 100,              // Page interval
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
                 100,              // Page interval
                 currentPosition,  // The position where the movement starts
                 120.0,             // Move amount
                 100.0,            // Where the current position should end up after movement
                 true,             // Left to right
                 1);               // Target page index after move
}

void Ut_PagedPanning::testHugeMovementLeftToRight()
{
    //    m_subject->setPageWidth(100);
    qreal position = 400;
    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;
    qreal rangeStart = 0;
    qreal rangeEnd = 1000;

    fillDefaultIntegrationParameters(m_subject, 100, rangeStart, rangeEnd);

    /*
      To fully test this movement direction, specifically the signal emission
      We need to first go to the other direction, to get the internal signal
      position somewhere else then zero
    */
    performMovement(m_subject,
                    120, // amount to move
                    false, // left-to-right
                    position, velocity, pointerSpring,
                    acceleration);

    velocity = 0;
    pointerSpring = 0;
    acceleration = 0;
    QCOMPARE(position, 500.0);
    
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    performMovement(m_subject,
                    700, // amount to move
                    true, // left-to-right
                    position, velocity, pointerSpring,
                    acceleration);

    QCOMPARE(position, 0.0);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == 0); // the page index should be one
}

void Ut_PagedPanning::testHugeMovementRightToLeft()
{
    qreal currentPosition = 400.0;
    testMovement(m_subject,
                 100,              // Page interval
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
                 100,              // Page interval
                 currentPosition,  // The position where the movement starts
                 100.0,            // Move amount
                 100.0 ,           // Where the current position should end up after movement
                 true,             // Left to right
                 1);               // Target page index after move

}

void Ut_PagedPanning::testAutoPanning()
{
    qreal currentPosition = 300.0;
    // Do the initial movement so that the subject internal state has some meaning. In this
    // case is we want the currentPage to be non zero
    testMovement(m_subject,
                 100.0,            // Page width
                 currentPosition,  // The position where the movement starts
                 0.0,              // Move amount
                 300.0 ,           // Where the current position should end up after movement
                 true,             // Left to right
                 3);               // Target 

    QCOMPARE(currentPosition, 300.0);
    m_subject->panToPage(0);
    
    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));

    // We perform the movement manually here as the timeline is not running 
    performMovement(m_subject,
                    0, // amount to move
                    true, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);

    QCOMPARE(currentPosition, 0.0);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == 0); 
    

    m_subject->panToPage(2);
    // We perform the movement manually here as the timeline is not running
    performMovement(m_subject,
                    0, // amount to move
                    true, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);
    
    QCOMPARE(currentPosition, 200.0);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == 2); 

}

void Ut_PagedPanning::testCurrentPageRemainsSameWhenPageWidthChanges()
{
    qreal currentPosition = 300.0;
    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;

    m_subject->setPageWidth(100);
    fillDefaultIntegrationParameters(m_subject, 100, 0, 1000);

    performMovement(m_subject,
                    0, // amount to move
                    true, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);

    velocity = 0;
    acceleration = 0;
    pointerSpring = 0;
    currentPosition = 300;

    qreal targetPage = 3;

    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));
    performMovement(m_subject,
                    1, // amount to move
                    true, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);


    QCOMPARE(currentPosition , 300.0);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == targetPage); // Make sure that we are on the right page

    velocity = 0;
    acceleration = 0;
    pointerSpring = 0;
    spy.clear();

    m_subject->setPageWidth(50);

    /*
      When the page width is set the pager requests start from its parent class.
      As the QTimeline is not running here we simulate the behaviour be calling the 
      integrator our selves with a zero move amount.
    */
    performMovement(m_subject,
                    0, // amount to move
                    true, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);

    //Now the position should have changed, but the page info signal should reflect the original state
    QCOMPARE(currentPosition , 150.0);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == targetPage);  // Make sure that we are still on the same page
}

void Ut_PagedPanning::testSetPageWidth()
{
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));
    
    fillDefaultIntegrationParameters(m_subject, 100.0, 0, 1000);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == 0);
    
}

void Ut_PagedPanning::testMovement(PagedPanning* pagedPanning,
                                   int pageWidth,
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


    fillDefaultIntegrationParameters(pagedPanning, pageWidth, rangeStart, rangeEnd);

    velocity = 0;
    pointerSpring = 0;
    acceleration = 0;
    QSignalSpy spy(pagedPanning, SIGNAL(pageChanged(int)));

    performMovement(pagedPanning,
                    moveAmount, // amount to move
                    leftToRight, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);

    QCOMPARE(velocity, 0.0);
    QCOMPARE(acceleration, 0.0);
    QCOMPARE(currentPosition, targetPosition);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == targetPage);
}


void Ut_PagedPanning::performMovement(PagedPanning* pagedPanning,
                                      qreal moveAmount,
                                      bool leftToRight,
                                      qreal &position,
                                      qreal &velocity,
                                      qreal &pointerSpring,
                                      qreal &acceleration)

{
    int i = 0;
    bool pointerPressControl = true;
    while (pointerPressControl || velocity != 0.0) {
        if (i++ < moveAmount) {
            pointerSpring += leftToRight ? 1 : -1;
        } else {
            pointerPressControl = false;
        }
        pagedPanning->integrateAxis(Qt::Horizontal,
                                    position,
                                    velocity,
                                    acceleration,
                                    pointerSpring,
                                    pointerPressControl);
    }
}

void Ut_PagedPanning::fillDefaultIntegrationParameters(PagedPanning* pagedPanning, qreal newPageWidth, qreal rangeStart, qreal rangeEnd)
{
    pagedPanning->setPointerSpringK(0.6);
    pagedPanning->setFriction(0.9);
    pagedPanning->setSlidingFriction(0.9);
    pagedPanning->setBorderSpringK(0.9);
    pagedPanning->setBorderFriction(0.9);
    pagedPanning->setRange(QRectF(rangeStart, 0, rangeEnd, 0));

    m_subject->setPageWidth(newPageWidth);

    qreal currentPosition = 0.0;
    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;

    /*
      When the page width is set the paged panning requests start from its parent class.
      As the QTimeline is not running here we simulate the behaviour be calling the 
      integrator our selves with a zero move amount.
    */
    performMovement(m_subject,
                    0, // amount to move
                    true, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);
    QCOMPARE(currentPosition, 0.0);
    
}

QTEST_APPLESS_MAIN(Ut_PagedPanning)
