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
    char *app_name = (char *)"./ut_switcherphysicsintegrationstrategy";
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

void Ut_PagedPanning::testMovementSmallerThenSnapIntervalRightToLeft()
{
    qreal currentPosition = 500.0;
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 10.0,             // Move amount
                 500.0,            // Where the current position should end up after movement
                 false,            // Left to right
                 5);               // Target snap index after move
}

void Ut_PagedPanning::testMovementGreaterThenSnapIntervalRightToLeft()
{
    qreal currentPosition = 500.0;
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 120.0,            // Move amount
                 600.0,            // Where the current position should end up after movement
                 false,            // Left to right
                 6);               // Target snap index after move
}



void Ut_PagedPanning::testMovementSmallerThenSnapIntervalLeftToRight()
{
    qreal currentPosition = 200.0;
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 10.0,             // Move amount
                 200.0,            // Where the current position should end up after movement
                 true,             // Left to right
                 2);               // Target snap index after move
}

void Ut_PagedPanning::testMovementGreaterThenSnapIntervalLeftToRight()
{
    qreal currentPosition = 200.0;
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 120.0,             // Move amount
                 100.0,            // Where the current position should end up after movement
                 true,             // Left to right
                 1);               // Target snap index after move
}

void Ut_PagedPanning::testHugeMovementLeftToRight()
{
    m_subject->setPageWidth(100);
    qreal position = 400;
    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;
    qreal rangeStart = 0;
    qreal rangeEnd = 1000;

    fillDefaultIntegrationParameters(m_subject, rangeStart, rangeEnd);

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
    QVERIFY(arguments.at(0).toInt() == 0); // the snap index should be one
}

void Ut_PagedPanning::testHugeMovementRightToLeft()
{
    qreal currentPosition = 400.0;
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 1000.0,           // Move amount
                 1000.0,           // Where the current position should end up after movement
                 false,            // Left to right
                 10);              // Target snap index after move
}


void Ut_PagedPanning::testMovementExcatlySnapInterval()
{
    qreal currentPosition = 200.0;
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 100.0,            // Move amount
                 100.0 ,           // Where the current position should end up after movement
                 true,             // Left to right
                 1);               // Target snap index after move

}

void Ut_PagedPanning::testAutoPanning()
{
    qreal currentPosition = 300.0;

    m_subject->panToPage(0);
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 0.0,              // Move amount
                 0.0 ,             // Where the current position should end up after movement
                 true,             // Left to right
                 0);               // Target snap index after move

    currentPosition = 0.0;

    m_subject->panToPage(5);
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 0.0,              // Move amount
                 500.0,            // Where the current position should end up after movement
                 true,             // Left to right
                 5);               // Target snap index after move

    currentPosition = 500.0;

    // Test that the snapping still works after an automatic pan
    testMovement(m_subject,
                 100,              // Snap interval
                 currentPosition,  // The position where the movement starts
                 120.0,            // Move amount
                 600.0,            // Where the current position should end up after movement
                 false,            // Left to right
                 6);               // Target snap index after move
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
    pagedPanning->setPageWidth(pageWidth);

    fillDefaultIntegrationParameters(pagedPanning, rangeStart, rangeEnd);


    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;
    QSignalSpy spy(pagedPanning, SIGNAL(pageChanged(int)));

    performMovement(pagedPanning,
                    moveAmount, // amount to move
                    leftToRight, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration);

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

void Ut_PagedPanning::fillDefaultIntegrationParameters(PagedPanning* pagedPanning, qreal rangeStart, qreal rangeEnd)
{
    pagedPanning->setPointerSpringK(0.6);
    pagedPanning->setFriction(0.9);
    pagedPanning->setSlidingFriction(0.9);
    pagedPanning->setBorderSpringK(0.9);
    pagedPanning->setBorderFriction(0.9);
    pagedPanning->setRange(QRectF(rangeStart, 0, rangeEnd, 0));
}

QTEST_APPLESS_MAIN(Ut_PagedPanning)
