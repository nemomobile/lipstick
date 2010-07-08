/*
 * This file is part of mhome
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */
#include "pagedpanning.h"
#include <stdlib.h>
#include <QtGlobal>
#include <algorithm>
#include <cmath>

// Velocity at which a slide is considered to be done
#define SLIDE_FINAL_VELOCITY 1.0

PagedPanning::PagedPanning(QObject* parent) : MPhysics2DPanning(parent),
                                              pageCount_(1),
                                              currentPage(0),
                                              snapMode(false),
                                              velocityThreshold_(7.0),
                                              dragThreshold_(0.5),
                                              pageSnapSpringK_(0.7),
                                              pageSnapFriction_(0.7),
                                              previousPosition(0),
                                              targetPage(0),
                                              pageWidth(0)
{
    // Whenever panning stops for reason or the other, make sure the
    // view ends up on top of a page.
    connect(this, SIGNAL(panningStopped()), this, SLOT(panToCurrentPage()));
    setPanDirection(Qt::Horizontal);
}

PagedPanning::~PagedPanning()
{

}

void PagedPanning::integrateAxis(Qt::Orientation orientation,
                                 qreal &position,
                                 qreal &velocity,
                                 qreal &acceleration,
                                 qreal &pointerDifference,
                                 bool pointerPressed)
{
    if (orientation == Qt::Vertical) {
        return;
    }

    qreal rangeStart = range().left();
    qreal rangeEnd = range().right();
    qreal force = 0.0;

    pageWidth = (rangeEnd - rangeStart) / qMax(1, pageCount_-1);

    if (position >= rangeStart && position <= rangeEnd) {
        // Inside range
        if (pointerPressed) {
            // Drag friction
            force = -friction() * velocity;
        } else if (snapMode) {
            force = -pageSnapFriction_ * velocity;
        } else {
            force = -slidingFriction() * velocity;
        }
    } else {
        // Outside range (in border)
        force = -borderFriction() * velocity;
    }

    if (previousRange != range()) {

        previousRange = range();

        if( pageWidth != 0 && previousPageWidth != pageWidth ) {
            /* A change in the page width means the orientation has
               changed - move the view to the correct position immediately */
            position = pageWidth * currentPage;
            force = 0;
            velocity = 0;
            acceleration = 0;
        } else {
            /* If the page width has remained the same, the number of
               pages must have changed */
            snapMode = true;
        }

        targetPage = currentPage;
    }

    if (pointerPressed) {
        // Pointer spring
        force += -pointerSpringK() * pointerDifference;
    } else {
        /* The target page must be inside the panning range, this is to make sure
           that we do not go out side the range */
        targetPage = qBound(0, targetPage, pageCount_-1);

        int nearestPage = 0;

        if (pageWidth > 0) {
            nearestPage = (int)((position / (qreal)pageWidth) + 0.5);
            // nearestPage is the nearest page in the valid range
            nearestPage = qBound(0, nearestPage, pageCount_-1);
        }

        // Cap the nearestPage at targetPage, in case
        // we slide past the target
        if ( initialPage < targetPage )
            nearestPage = qMin(nearestPage, targetPage);
        else
            nearestPage = qMax(targetPage, nearestPage);

        // Emit the signal only when really necessary.
        // The user might have dragged the view over the threshold
        // in which case the page has changed, but nearestPage could
        // still be initialPage.
        if (    nearestPage  != currentPage
             && currentPage != targetPage
             && nearestPage  != initialPage ) {
            emit pageChanged(nearestPage);
            currentPage = nearestPage;
        }

        // Activate the snap when arriving at target
        if ( currentPage == targetPage ) {
            snapMode = true;
        }

        if ( snapMode ) {

            force += pageSnapSpringK_ * (targetPage * (qreal)pageWidth - position);

            qreal closeEnough = position - (pageWidth * targetPage);

            if (abs(closeEnough) < 2 && abs(force) < 2) {
                // Setting these to zero should stop the integration process
                force = 0;
                velocity = 0;
                acceleration = 0;
                // Make the position exactly the right one
                position = pageWidth * targetPage;

                snapMode = false;
            }
        }
    }

    acceleration   = force;
    velocity      += acceleration;
    position      += velocity;
    pointerDifference += velocity;

    previousPosition = position;
    previousPageWidth = pageWidth;
}

void PagedPanning::setPageCount(int newPageCount)
{
    pageCount_ = qMax(1, newPageCount);
    initialPage = currentPage;
    targetPage = qMin(newPageCount - 1, currentPage);
    snapMode = true;
}

int PagedPanning::pageCount() const
{
    return pageCount_;
}

void PagedPanning::panToPage(int page)
{
    initialPage = currentPage;
    targetPage = page;
    snapMode = true;
    start();
}

void PagedPanning::panToCurrentPage()
{
    if ( std::fabs(previousPosition-currentPage*pageWidth) > 1.0 ) {
        targetPage = currentPage;
        snapMode = true;
        start();
    }
}

void PagedPanning::setVelocityThreshold(qreal value)
{
    velocityThreshold_ = value;
}

void PagedPanning::setDragThreshold(qreal value)
{
    dragThreshold_ = qBound((qreal)0.0, (qreal)std::fabs(value), (qreal)1.0);
}

void PagedPanning::setSlideLimit(int value)
{
    slideLimit_ = value;
}

void PagedPanning::setPageSnapSpringK(qreal value)
{
    pageSnapSpringK_ = value;
}

void PagedPanning::setPageSnapFriction(qreal value)
{
    pageSnapFriction_ = value;
}

qreal PagedPanning::slideDistance(qreal initialVelocity, qreal friction)
{
    /*
       Calculate the distance the view slides with the current velocity and
       slide friction factor.

       Velocity is calculated at each iteration as:
       velocity = velocity - friction*velocity
       velocity = (1-friction)*velocity

       Velocity at time t:
       velocity = initialVelocity * (1-friction)^t

       Integrate to get distance:
       distance = initialVelocity * (1-friction)^t / log(1-friction) - initialVelocity
       distance = initialVelocity * ((1-friction)^t - 1) / log(1-friction)

       The time for velocity to drop to finalVelocity is:
                            finalVelocity = initialVelocity * (1-friction)^t
            finalVelocity/initialVelocity = (1-friction)^t
       log(finalVelocity/initialVelocity) = log((1-friction)^t)
       log(finalVelocity/initialVelocity) = t*log((1-friction))
                                        t = log(finalVelocity/initialVelocity)/log(1-friction)
     */
    if (initialVelocity == 0)
        return 0;

    qreal b = 1.0-qMin((qreal)1.0, friction);
    qreal logb = std::log(b);
    qreal t = std::log(SLIDE_FINAL_VELOCITY / std::fabs(initialVelocity)) / logb;

    return initialVelocity * (std::pow(b, t)-1.0) / logb;
}

void PagedPanning::pointerPress(const QPointF &pos)
{
    MPhysics2DPanning::pointerPress(pos);

    pageWidth = (range().right() - range().left()) / qMax(1, pageCount_-1);
    //currentPage = position().x()/pageWidth;
    initialPage = currentPage;

    // Stop the automatic panning when the pointer comes down
    snapMode = false;
}

void PagedPanning::pointerMove(const QPointF &pos)
{
    MPhysics2DPanning::pointerMove(pos);

    /* Target the next page if the view has been dragged over the
       dragThreshold. */
    qreal distanceToInitialPage = position().x() - initialPage*pageWidth;
    int draggedPages;

    if (distanceToInitialPage > 0) {
        draggedPages = (int)(distanceToInitialPage/pageWidth + (1.0-dragThreshold_));
    } else {
        draggedPages = (int)(distanceToInitialPage/pageWidth - (1.0-dragThreshold_));
    }

    targetPage = qBound(0, initialPage + draggedPages, pageCount_-1);

    if( currentPage != targetPage) {
        emit pageChanged(targetPage);
        currentPage = targetPage;
    }
}

void PagedPanning::pointerRelease()
{
    MPhysics2DPanning::pointerRelease();

    // The number of pages to slide
    qreal slidePages = slideDistance(velocity().x(), slidingFriction())/pageWidth;
    // Remove half a page; only slide over the center of a page if really going
    // to the next one
    slidePages -= slidePages > 0 ? 0.5 : -0.5;

    targetPage = currentPage + (int)slidePages;

    /* Pan to the next page if a strong enough flick was performed. */
    if ( targetPage == initialPage
            && std::fabs(velocity().x()) > velocityThreshold_ ) {
        targetPage += velocity().x() > 0 ? 1 : -1;
        snapMode = true;
    }

    if (slideLimit_ > 0)
        targetPage = qBound(initialPage-slideLimit_,
                            targetPage,
                            initialPage+slideLimit_);
}

void PagedPanning::setFirstPagePosition()
{
    //TODO: add support for setting position to the right-most page
    // when using right-to-left layout
    currentPage = 0;
    targetPage = 0;
    setPosition(QPointF(0.0, 0.0));
    emit pageChanged(0);
}
