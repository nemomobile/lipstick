/*
 * ut_switcherphysicsintegrationstrategy.h
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

#ifndef UT_SWITCHERPHYSICSINTEGRATIONSTRATEGY_H
#define UT_SWITCHERPHYSICSINTEGRATIONSTRATEGY_H

#include <QtTest/QtTest>
#include <QObject>

class DuiApplication;
class SwitcherPhysicsIntegrationStrategy;

class Ut_SwitcherPhysicsIntegrationStrategy : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases    
    // Test the creation.
    void testCreation();

    /*! 
     * Tests small movement from left to right that should snap back to
     * its original position
     */
    void testMovementSmallerThenSnapIntervalLeftToRight();

    /*! 
     * Tests big enough movement from left to right that should snap to the previous 
     * snap position
     */
    void testMovementGreaterThenSnapIntervalLeftToRight();

    /*! 
     * Tests big enough movement from right to left that should snap to the previous 
     * snap position
     */
    void testMovementGreaterThenSnapIntervalRightToLeft();

    /*! 
     * Tests small movement from right to left that should snap back to
     * its original position
     */
    void testMovementSmallerThenSnapIntervalRightToLeft();

    /*!
     * Tests huge movement that should snap to the first snap position
     */
    void testHugeMovementLeftToRight();

    /*!
     * Tests huge movement that should snap to the last snap position
     */
    void testHugeMovementRightToLeft();

    /*!
     * Tests movent that is exctly the threshold and it should advance to the next
     * snap position
     */
    void testMovementExcatlySnapInterval();

private:
    // DuiApplication
    DuiApplication *app;
    // The object being tested
    SwitcherPhysicsIntegrationStrategy* m_subject;
};

#endif
