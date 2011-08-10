/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "pagedpanning.h"
#include <stdlib.h>
#include <QtGlobal>
#include <algorithm>
#include <cmath>

// Velocity at which a slide is considered to be done
#define SLIDE_FINAL_VELOCITY 1

PagedPanning::PagedPanning(QObject* parent) : MPhysics2DPanning(parent),
                                              pageCount_(1),
                                              currentPage(0),
                                              snapMode(false),
                                              previousRange(),
                                              velocityThreshold_(7),
                                              panThreshold(30),
                                              dragThreshold_(0.5),
                                              slideLimit_(0),
                                              pageSnapSpringK_(0.7),
                                              pageSnapFriction_(0.7),
                                              previousPosition(0),
                                              targetPage_(0),
                                              initialPage(0),
                                              previousPageWidth(0),
                                              distanceToPressPosition(0),
                                              initialTargetPageVelocityThreshold(0),
                                              wrapMode(false),
                                              pageWrapping(false)
{
    // Whenever panning stops for reason or the other, make sure the
    // view ends up on top of a page.
    connect(this, SIGNAL(panningStopped()), this, SLOT(panToCurrentPage()));
    setPanDirection(Qt::Horizontal);
}

PagedPanning::~PagedPanning()
{
}

// Wraps x into the range [0,length)
static int wrap(int x, int length)
{
    int a = x / length;

    x += (x < 0 ? (-a + 1) : -a) * length;

    return x;
}

void PagedPanning::integrateAxis(Qt::Orientation orientation, qreal &position, qreal &velocity, qreal &acceleration, qreal &pointerDifference, bool pointerPressed)
{
    if (orientation == Qt::Vertical) {
        return;
    }

    if (!enabled()) {
        snapMode = true;
    }

    qreal force = 0;
    qreal currentPageWidth = pageWidth();

    if ((position >= range().left() && position <= range().right()) || wrapMode) {
        // Inside range
        if (snapMode) {
            force = -pageSnapFriction_ * velocity;
        } else if (pointerPressed) {
            // Drag friction
            force = -friction() * velocity;
        } else {
            force = -slidingFriction() * velocity;
        }
    } else {
        // Outside range (in border)
        force = -borderFriction() * velocity;
    }

    if (currentPageWidth != 0 && previousPageWidth != currentPageWidth) {
        /*
           A change in the page width means the orientation has
           changed - move the view to the correct position immediately
           otherwise there can be some visible 'sliding' effects.
        */
        position = currentPageWidth * currentPage;
        force = 0;
        velocity = 0;
        acceleration = 0;
        if (pointerPressed) {
            emit panningStopped();
        }
    }

    if (pointerPressed && enabled()) {
        // Pointer spring
        force += -pointerSpringK() * pointerDifference;
    } else {
        /* The target page must be inside the panning range, this is to make sure
           that we do not go out side the range */
        targetPage_ = qBound(0, targetPage_, pageCount_-1);

        int nearestPage = 0;
        if (currentPageWidth > 0) {
            nearestPage = (int)((position / (qreal)currentPageWidth) + 0.5);
            // nearestPage is the nearest page in the valid range
            nearestPage = qBound(0, nearestPage, pageCount_-1);
        }

        // Cap the nearestPage at targetPage, in case we slide past the target
        if (initialPage < targetPage_) {
            nearestPage = qMin(nearestPage, targetPage_);
        } else {
            nearestPage = qMax(targetPage_, nearestPage);
        }

        // Emit the signal only when really necessary.
        // The user might have dragged the view over the threshold
        // in which case the page has changed, but nearestPage could
        // still be initialPage.
        if (nearestPage  != currentPage && currentPage != targetPage_ && nearestPage  != initialPage ) {
            emit pageChanged(nearestPage);
            currentPage = nearestPage;
        }

        // Activate the snap when arriving at target
        if (currentPage == targetPage_) {
            snapMode = true;
        }

        if (snapMode) {
            force += pageSnapSpringK_ * (targetPage_ * (qreal)currentPageWidth - position);

            qreal closeEnough = position - (currentPageWidth * targetPage_);

            if (abs(closeEnough) < 2 && abs(force) < 2) {
                // Setting these to zero and releasing the pointer should stop the integration process
                force = 0;
                velocity = 0;
                acceleration = 0;

                if (pointerPressed) {
                    MPhysics2DPanning::pointerRelease();
                }

                // Make the position exactly the right one
                position = currentPageWidth * targetPage_;

                snapMode = false;
            }
        }
    }

    acceleration = force;
    velocity += acceleration;
    position += velocity;
    pointerDifference += velocity;

    previousPosition = position;
    previousPageWidth = currentPageWidth;

    if (snapMode && std::fabs(velocity) < 1) {
        // Keep the velocity over 1 or under -1 to keep the animation going
        velocity = velocity < 0.0f ? -1 : 1;
    }
}

void PagedPanning::setPageCount(int newPageCount)
{
    pageCount_ = qMax(1, newPageCount);
    initialPage = currentPage;
    targetPage_ = qMin(newPageCount - 1, currentPage);
    snapMode = true;
}

int PagedPanning::pageCount() const
{
    return pageCount_;
}

void PagedPanning::panToPage(int page)
{
    initialPage = currentPage;
    targetPage_ = page;
    snapMode = true;
    previousPageWidth = pageWidth();
    start();
}

void PagedPanning::panToCurrentPage()
{
    if (std::fabs(previousPosition - currentPage * pageWidth()) > 1) {
        targetPage_ = currentPage;
        snapMode = true;
        start();
    }
    emit pageIsPanning(false);
}

void PagedPanning::setVelocityThreshold(qreal value)
{
    velocityThreshold_ = value;
}

void PagedPanning::setPanThreshold(qreal value)
{
    panThreshold = value;
}

void PagedPanning::setDragThreshold(qreal value)
{
    dragThreshold_ = qBound((qreal)0, (qreal)std::fabs(value), (qreal)1);
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

void PagedPanning::setPageWrapMode(bool enable)
{
    wrapMode = enable;
}

bool PagedPanning::pageWrapMode() const
{
    return wrapMode;
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
    if (initialVelocity == 0) {
        return 0;
    }

    qreal b = 1 - qMin((qreal)1, friction);
    qreal logb = std::log(b);
    qreal t = std::log(SLIDE_FINAL_VELOCITY / std::fabs(initialVelocity)) / logb;

    return initialVelocity * (std::pow(b, t) - 1) / logb;
}

void PagedPanning::pointerPress(const QPointF &pos)
{
    if(!enabled()) {
        return;
    }

    MPhysics2DPanning::pointerPress(pos);

    initialPage = currentPage;
    pressPosition = pos;
    initialTargetPageVelocityThreshold = 0;

    // Stop the automatic panning when the pointer comes down
    snapMode = false;
    pageWrapping = false;

    emit pageIsPanning(true);
}

void PagedPanning::pointerMove(const QPointF &pos)
{
    if(!enabled()) {
        return;
    }

    MPhysics2DPanning::pointerMove(pos);

    distanceToPressPosition = pressPosition.x() - pos.x();

    // Target the next page if the view has been dragged over the dragThreshold
    qreal pageChange = distanceToPressPosition > 0 ? (1 - dragThreshold_) : (-1 + dragThreshold_);
    targetPage_ = initialPage + (int)(distanceToPressPosition / pageWidth() + pageChange);

    if (wrapMode) {
        int wrappedPage = wrap(targetPage_, pageCount_);
        pageWrapping = targetPage_ != wrappedPage;
        targetPage_ = wrappedPage;
    } else {
        targetPage_ = qBound(0, targetPage_, pageCount_ - 1);
    }

    // Store the velocity threshold needed to get to the page which was targeted after the pan threshold was first exceeded
    if (initialTargetPageVelocityThreshold == 0 && std::fabs(distanceToPressPosition) > panThreshold) {
        initialTargetPageVelocityThreshold = distanceToPressPosition > 0 ? velocityThreshold_ : -velocityThreshold_;
    }

    if (currentPage != targetPage_) {
        emit pageChanged(targetPage_);
        currentPage = targetPage_;
    }
}

void PagedPanning::pointerRelease()
{
    if(!enabled()) {
        return;
    }

    MPhysics2DPanning::pointerRelease();

    if (std::fabs(distanceToPressPosition) > panThreshold) {
        // Movement exceeds panning threshold: check whether the current velocity is to the initial target page's direction
        if ((initialTargetPageVelocityThreshold > 0 && velocity().x() >= initialTargetPageVelocityThreshold) || (initialTargetPageVelocityThreshold < 0 && velocity().x() <= initialTargetPageVelocityThreshold)) {
            // Current velocity is to the correct direction and over velocity limit: Calculate the target page based on the current velocity
            qreal slidePages = slideDistance(velocity().x(), slidingFriction()) / pageWidth();
            // Remove half a page; only slide over the center of a page if really going to the next one
            slidePages -= slidePages > 0 ? 0.5 : -0.5;
            targetPage_ = currentPage + (int) slidePages;

            if (targetPage_ == initialPage) {
                // If the velocity was not enough to switch between pages, still pan at least one page
                targetPage_ += velocity().x() > 0 ? 1 : -1;
                snapMode = true;
            }
        } else if ((initialTargetPageVelocityThreshold > 0 && velocity().x() < 0) || (initialTargetPageVelocityThreshold < 0 && velocity().x() > 0) || std::fabs(distanceToPressPosition) / pageWidth() < dragThreshold_) {
            // Velocity was to the wrong direction away from the initial target page or movement didn't exceed the drag threshold: return to the initial page, wrapping if necessary
            if (currentPage == pageCount_ - 1 && initialPage == 0 && velocity().x() > 0) {
                setPosition(QPointF(position().x() + pageWidth() * pageCount(), position().y()));
            } else if (currentPage == 0 && initialPage == pageCount_ - 1 && velocity().x() < 0) {
                setPosition(QPointF(position().x() - pageWidth() * pageCount(), position().y()));
            }

            targetPage_ = initialPage;
            initialPage = currentPage;
        }
    }

    if (slideLimit_ > 0) {
        targetPage_ = qBound(currentPage - slideLimit_, targetPage_, currentPage + slideLimit_);
    }

    // If wrap mode is on, and the pointer movement or the above logic caused page wrapping,
    // move the current position to the opposite side of the pannable area
    if(wrapMode && (pageWrapping || targetPage_ >= pageCount_ || targetPage_ < 0)) {
        targetPage_ = wrap(targetPage_, pageCount_);
        setPosition(QPointF(position().x() + (initialPage > targetPage_ ? -1 : 1) * pageWidth() * pageCount() , position().y()));
        pageWrapping = false;
        emit pageWrapped();
    }
}

void PagedPanning::setPage(uint page)
{
    if (page >= uint(pageCount())) {
        return;
    }

    currentPage = page;
    targetPage_ = page;
    snapMode = true;
    setPosition(QPointF(pageWidth() * page, 0));
    emit pageChanged(page);
}

int PagedPanning::activePage() const
{
    return currentPage;
}

int PagedPanning::targetPage() const
{
    return qBound(0, targetPage_, pageCount() - 1);
}

float PagedPanning::pageWidth() const
{
    return (range().right() - range().left()) / qMax(1, pageCount_ - 1);
}
