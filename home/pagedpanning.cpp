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
                                              previousPointerPressed(false),
                                              previousPosition(0),
                                              targetPage(0),
                                              pageWidth(0)
{
    // Whenever panning stops for reason or the other, make sure the
    // view ends up on top of a page.
    connect(this, SIGNAL(panningStopped()), this, SLOT(panToCurrentPage()));
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
    qreal force;

    pageWidth = (rangeEnd - rangeStart) / qMax(1, pageCount_-1);

    // Damping
    if (position >= rangeStart && position <= rangeEnd) {
        // Inside range
        if (pointerPressed) {
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

    if (pointerPressed == true && previousPointerPressed == false) {
        initialPage = currentPage;
    }

    if (pointerPressed) {
        // Pointer spring
        force += -pointerSpringK() * pointerDifference;
        // Stop the automatic panning when the pointer comes down
        snapMode = false;

        /* Target the next page if the view has been dragged over the
           dragThreshold. */
        qreal distanceToInitialPage = position - initialPage*pageWidth;
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

    if (previousRange != range()) {
        /*
           Pan the viewport to the correct page
           in case the underlying widget's size and
           therefore the page width has changed.
         */
        snapMode = true;
        targetPage = currentPage;
        previousRange = range();
    }

    // Check if pointer was just lifted
    if (pointerPressed == false && previousPointerPressed == true) {
        // The number of pages to slide
        qreal slidePages = slideDistance(velocity, slidingFriction())/pageWidth;
        // Remove half a page; only slide over the center of a page if really going
        // to the next one
        slidePages -= slidePages > 0 ? 0.5 : -0.5;

        targetPage = currentPage + (int)slidePages;

        /* Pan to the next page if a strong enough flick was performed. */
        if ( targetPage == initialPage
             && std::fabs(velocity) > velocityThreshold_ ) {
            targetPage += velocity > 0 ? 1 : -1;
            snapMode = true;
        }

        if (slideLimit_ > 0)
            targetPage = qBound(initialPage-slideLimit_,
                                targetPage,
                                initialPage+slideLimit_);

    }

    /* The target page must be inside the panning range, this is to make sure
       that we do not go out side the range */
    targetPage = qBound(0, targetPage, pageCount_-1);

    if ( !pointerPressed ) {
        int nearestPage = 0;

        if (pageWidth > 0) {
            nearestPage = (int)((position / (qreal)pageWidth) + 0.5);
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

            if (abs(closeEnough) < 1 && abs(force) < 1) {
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

    previousPointerPressed = pointerPressed;
    previousPosition = position;
}

void PagedPanning::setPageCount(int newPageCount)
{
    pageCount_ = qMax(1, newPageCount);
    panToPage(qMin(newPageCount - 1, currentPage));
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
    qreal b = 1.0-friction;
    qreal logb = std::log(b);
    qreal t = std::log(SLIDE_FINAL_VELOCITY / std::fabs(initialVelocity)) / logb;

    return initialVelocity * (std::pow(b, t)-1.0) / logb;
}
