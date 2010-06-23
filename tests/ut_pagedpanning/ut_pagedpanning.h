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

#ifndef UT_PAGEDPANNING_H
#define UT_PAGEDPANNING_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class PagedPanning;

class Ut_PagedPanning : public QObject
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
    void testMovementSmallerThenPageWidthLeftToRight();

    /*! 
     * Tests big enough movement from left to right that should snap to the previous 
     * snap position
     */
    void testMovementGreaterThenPageWidthLeftToRight();

    /*! 
     * Tests big enough movement from right to left that should snap to the previous 
     * snap position
     */
    void testMovementGreaterThenPageWidthRightToLeft();

    /*! 
     * Tests small movement from right to left that should snap back to
     * its original position
     */
    void testMovementSmallerThenPageWidthRightToLeft();

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
    void testMovementExcatlyPageWidth();

    /*!
     * Tests movent that is exctly the threshold and it should advance to the next
     * snap position
     */
    void testAutoPanning();

    /*!
     * Tests that the current page stays the same when the page width is changed. 
     * Note this test also tests the case where a rotation occurs.
     */
    void testCurrentPageRemainsSameWhenPageCountChanges();

    /*!
     * Tests the setting of the page count.
     */
    void testSetPageCount();

    /*!
     * Tests dragging under and over the drag threshold.
     */
    void testDragThreshold();

    /*!
     * Tests small flicks with different velocities.
     */
    void testVelocityThreshold();

    /*!
     * Tests sliding over multiple pages.
     */
    void testSlide();

    /*!
     * Tests that when the page width changes the view is immediately moved
     * to the correct position determined by the current page and the new page width.
     */
    void testSetRange();

    /*!
     * Test setting page position to first page
     */
    void testSetFirstPagePosition();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    PagedPanning* m_subject;

    void testMovement(PagedPanning* integrator,
		      int pageWidth,
		      qreal originalPosition,
		      qreal moveAmount,
		      qreal targetPosition,
		      bool leftToRight,
		      int targetPage,
		      qreal rangeStart = 0.0,
		      qreal rangeEnd = 1000.0);
    
    void performMovement(PagedPanning* pagedPanning,
			 qreal moveAmount,
			 bool leftToRight,
                         qreal speed = 1.0);

    void fillDefaultIntegrationParameters(PagedPanning* pagedPanning, qreal newPageWidth, qreal rangeStart, qreal rangeEnd);

    void performIntegration(PagedPanning* pagedPanning);
};

#endif
