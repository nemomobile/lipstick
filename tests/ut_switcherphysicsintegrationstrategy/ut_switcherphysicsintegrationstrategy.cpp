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
#include <DuiApplication>
#include <QDebug>
#include <QSignalSpy>
#include "switcherphysicsintegrationstrategy.h"
#include "ut_switcherphysicsintegrationstrategy.h"
#include "duiphysics2dpanning.h"

static void testMovement(SwitcherPhysicsIntegrationStrategy* integrator,
                         int snapInterval,
                         qreal originalPosition,
                         qreal moveAmount,
                         qreal targetPosition,
                         bool leftToRight,
                         int targetSnapIndex,
                         qreal rangeStart = 0.0,
                         qreal rangeEnd = 1000.0);

static void performMovement(SwitcherPhysicsIntegrationStrategy* integrator,
                            qreal moveAmount,
                            bool leftToRight,
                            qreal &position,
                            qreal &velocity,
                            qreal &pointerSpring,
                            qreal &acceleration,
                            qreal rangeStart,
                            qreal rangeEnd,
                            DuiPhysics2DIntegrationStrategy::IntegrationData &integrationData);

static void fillDefaultIntegrationData(DuiPhysics2DIntegrationStrategy::IntegrationData &integrationData);

static void fillIntegrationData(DuiPhysics2DIntegrationStrategy::IntegrationData &integrationData,
                                bool pointer,
                                qreal pointerSpringK,
                                qreal frictionC,
                                qreal slideFrictionC,
                                qreal borderSpringK,
                                qreal borderFrictionC);
                                
void Ut_SwitcherPhysicsIntegrationStrategy::initTestCase()
{
    int argc = 1;
    char *app_name = (char *)"./ut_switcherphysicsintegrationstrategy";
    app = new DuiApplication(argc, &app_name);
}

void Ut_SwitcherPhysicsIntegrationStrategy::cleanupTestCase()
{
    delete app;
}

void Ut_SwitcherPhysicsIntegrationStrategy::init()
{
    m_subject = new SwitcherPhysicsIntegrationStrategy;
}

void Ut_SwitcherPhysicsIntegrationStrategy::cleanup()
{
    delete m_subject;
}

// Test cases
void Ut_SwitcherPhysicsIntegrationStrategy::testCreation()
{
    QCOMPARE((int)m_subject->snapInterval(), 0);
}

void Ut_SwitcherPhysicsIntegrationStrategy::testMovementSmallerThenSnapIntervalRightToLeft()
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

void Ut_SwitcherPhysicsIntegrationStrategy::testMovementGreaterThenSnapIntervalRightToLeft()
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



void Ut_SwitcherPhysicsIntegrationStrategy::testMovementSmallerThenSnapIntervalLeftToRight()
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

void Ut_SwitcherPhysicsIntegrationStrategy::testMovementGreaterThenSnapIntervalLeftToRight()
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

void Ut_SwitcherPhysicsIntegrationStrategy::testHugeMovementLeftToRight()
{
    m_subject->setSnapInterval(100);
    DuiPhysics2DIntegrationStrategy::IntegrationData integrationData;
    fillDefaultIntegrationData(integrationData);

    qreal position = 400;
    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;
    qreal rangeStart = 0;
    qreal rangeEnd = 1000;

    /*
      To fully test this movement direction, specifically the signal emission
      We need to first go to the other direction, to get the internal signal
      position somewhere else then zero
    */    
    performMovement(m_subject,
                    120, // amount to move
                    false, // left-to-right
                    position, velocity, pointerSpring,
                    acceleration, rangeStart, rangeEnd, integrationData);


    integrationData.pointer = true;
    velocity = 0;
    pointerSpring = 0;
    acceleration = 0;

    QSignalSpy spy(m_subject, SIGNAL(snapIndexChanged(int)));

    performMovement(m_subject,
                    700, // amount to move
                    true, // left-to-right
                    position, velocity, pointerSpring,
                    acceleration, rangeStart, rangeEnd, integrationData);
    
    QCOMPARE(position, 0.0);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal
    QVERIFY(arguments.at(0).toInt() == 0); // the snap index should be one
}

void Ut_SwitcherPhysicsIntegrationStrategy::testHugeMovementRightToLeft()
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


void Ut_SwitcherPhysicsIntegrationStrategy::testMovementExcatlySnapInterval()
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

static void testMovement(SwitcherPhysicsIntegrationStrategy* integrator,
                         int snapInterval,
                         qreal currentPosition,
                         qreal moveAmount,
                         qreal targetPosition,
                         bool leftToRight,
                         int targetSnapIndex,
                         qreal rangeStart,
                         qreal rangeEnd)
{
    integrator->setSnapInterval(snapInterval);
    DuiPhysics2DIntegrationStrategy::IntegrationData integrationData;
    fillDefaultIntegrationData(integrationData);

    qreal velocity = 0;
    qreal pointerSpring = 0;
    qreal acceleration = 0;
    QSignalSpy spy(integrator, SIGNAL(snapIndexChanged(int)));

    performMovement(integrator,
                    moveAmount, // amount to move
                    leftToRight, // left-to-right
                    currentPosition, velocity, pointerSpring,
                    acceleration, rangeStart, rangeEnd, integrationData);
    
    QCOMPARE(currentPosition, targetPosition);
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal  
    QVERIFY(arguments.at(0).toInt() == targetSnapIndex);
}


static void performMovement(SwitcherPhysicsIntegrationStrategy* integrator,
                            qreal moveAmount,
                            bool leftToRight,
                            qreal &position,
                            qreal &velocity,
                            qreal &pointerSpring,
                            qreal &acceleration,
                            qreal rangeStart,
                            qreal rangeEnd,
                            DuiPhysics2DIntegrationStrategy::IntegrationData &integrationData)

{
    int i = 0;
    while (integrationData.pointer || velocity != 0.0) {
        if ( i++ < moveAmount) {            
            pointerSpring += leftToRight ? 1 : -1;
            integrationData.pointer = true;
        } else {
            integrationData.pointer = false;
        }
        integrator->integrate(position,
                             velocity,
                             pointerSpring,
                             acceleration,
                             rangeStart,
                             rangeEnd,
                             integrationData);
    }
}

// Helper functions
static void fillDefaultIntegrationData(DuiPhysics2DIntegrationStrategy::IntegrationData &integrationData)
{
    // Fills in the values for the intergration co-efficients
    fillIntegrationData(integrationData, true, 0.6, 0.9, 0.9, 0.9, 0.9);
}


static void fillIntegrationData(DuiPhysics2DIntegrationStrategy::IntegrationData &integrationData,
                                bool pointer,
                                qreal pointerSpringK,
                                qreal frictionC,
                                qreal slideFrictionC,
                                qreal borderSpringK,
                                qreal borderFrictionC)
{
    integrationData.pointer = pointer;
    integrationData.pointerSpringK = pointerSpringK;
    integrationData.frictionC = frictionC;
    integrationData.slideFrictionC = slideFrictionC;
    integrationData.borderSpringK = borderSpringK;
    integrationData.borderFrictionC = borderFrictionC;
}

QTEST_APPLESS_MAIN(Ut_SwitcherPhysicsIntegrationStrategy)
