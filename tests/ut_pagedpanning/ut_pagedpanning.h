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
class MPhysics2DPanning;

/*
 * A helper class to drive the physics simulation.
 * There is no easy way to drive the MPhysics2DPanning class's physics
 * integration. A class named Ut_MPhysics2DPanning is marked as friend
 * class of that class however and that gives access to the private parts
 * of the class and hence to the physics integration machinery.
 */
class Ut_MPhysics2DPanning : public QObject
{
    Q_OBJECT

    MPhysics2DPanning *physics;
public:
    Ut_MPhysics2DPanning(MPhysics2DPanning *physics);

    void advancePhysicsCalculation();

signals:
    void notify(QVariant);
};


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
     * Tests movent that is exactly the threshold and it should advance to the next
     * snap position
     */
    void testMovementExactlyPageWidth();

    /*!
     * Tests page wrapping at last page
     */
    void testMovementWithWrappingRightToLeft();

    /*!
     * Tests page wrapping at first page
     */
    void testMovementWithWrappingLeftToRight();

    /*!
     * Tests movement that is exctly the threshold and it should advance to the next
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
    void testWhenSlideIsLimitedToOnePageAndPanningOverOnePageThenTheSlideContinuesOneMorePage();

    /*!
     * Tests that when the page width changes the view is immediately moved
     * to the correct position determined by the current page and the new page width.
     */
    void testSetRange();

    /*!
     * Tests setting the page immediately
     */
    void testSetPage();

    void testWhenPageStartsToPanThenPageIsPanningSignalIsEmitted();
    void testWhenPageStopsPanningThenPageIsPanningSignalIsEmitted();

    void testWhenRangeChangesWhileActivelyPanningThenPanningStoppedSignalGetsEmitted();
    void testWhenRangeChangesWhilePhysicsPanningThenPanningStoppedSignalGetsEmitted();

    void testWhenTriedToPanThePagePositionWillNotChangeWhenPanningIsDisabled();
    void testCurrentPageRemainsSameWhenPageCountChangesWhenPanningIsDisabled();

    void testWhenPhysicsDisabledWhileActivelyPanningThenPositionIsPannedToNearestPage();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    PagedPanning* m_subject;

    void testMovement(int currentPage,
                      qreal moveAmount,
                      bool leftToRight,
                      int targetPage,
                      qreal speed = 1.0);

    void performMovement(qreal moveAmount,
                         bool leftToRight,
                         int targetPage,
                         qreal speed = 1.0);

    void fillDefaultIntegrationParameters(PagedPanning* pagedPanning, qreal newPageWidth, qreal rangeStart = 0.0, qreal rangeEnd = 0.0);

    void performIntegration(PagedPanning* pagedPanning);
};

#endif
