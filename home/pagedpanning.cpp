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

PagedPanning::PagedPanning(QObject* parent) : MPhysics2DPanning(parent),
                                              pageCount_(1),
                                              currentPage(0),
                                              autoIntegrateMode(false),
                                              autoIntegrateTargetPage(0)
{
  
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

    qreal pageWidth = (rangeEnd - rangeStart) / qMax(1, pageCount_-1);

    // Damping
    if (position >= rangeStart && position <= rangeEnd) {
        // Inside range
        if (pointerPressed) {
            force = -friction() * velocity;
        } else {
            force = -slidingFriction() * velocity;
        }
    } else {
        // Outside range (in border)
        force = -borderFriction() * velocity;
    }

    // Pointer spring
    if (pointerPressed) {
        force += -pointerSpringK() * pointerDifference;
        // Stop the automatic panning when the pointer comes down
        autoIntegrateMode = false;
    }

    if (!pointerPressed) {

        int targetPage = 0;

        if (previousRange != range()) {
            /*
             Pan the viewport to the correct page
             in case the underlying widget's size and
             therefore page width has changed.
             */
            autoIntegrateMode = true;
            autoIntegrateTargetPage = currentPage;
            previousRange = range();
        }

        if (autoIntegrateMode) {
            targetPage = autoIntegrateTargetPage;
        } else if (pageWidth > 0) {
            /*
              The page that we should be paging to, 
              the 0.5 is simple integer rounding before casting to int
            */ 
            targetPage = (int)((position / (qreal)pageWidth) + 0.5);
        }
        /* 
           The target page must be inside the panning range, this is to make sure 
           that we do not go out side the range
        */ 
        targetPage = qBound(0, targetPage, pageCount_-1);

        force += borderSpringK() * (targetPage * (qreal)pageWidth - position);

        qreal closeEnough = position - (pageWidth * targetPage);

        if (abs(closeEnough) < 1 && abs(force) < 1) {
            // Setting these to zero should stop the integration process
            force = 0;
            velocity = 0;
            acceleration = 0;
            // Make the position exactly the right one
            position = pageWidth * targetPage;

            currentPage = targetPage;
            autoIntegrateMode = false;

            emit pageChanged(currentPage);
        } else {
            acceleration   = force;
            velocity      += acceleration;
            position      += velocity;
            pointerDifference += velocity;     
        }
    } else {
        acceleration   = force;
        velocity      += acceleration;
        position      += velocity;
        pointerDifference += velocity;
    }
}

void PagedPanning::setPageCount(int newPageCount)
{
    pageCount_ = qMax(1, newPageCount);
    panToPage(qMin(newPageCount, currentPage));
}

int PagedPanning::pageCount() const
{
    return pageCount_;
}

void PagedPanning::panToPage(int page)
{
    autoIntegrateTargetPage = page;
    autoIntegrateMode = true;
    start();
}
