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
#ifndef PAGEDVIEWPORTSTYLE_H
#define PAGEDVIEWPORTSTYLE_H

#include <mpannablewidgetstyle.h>

/*!
    \class PagedViewportStyle
    \brief Style class for PagedViewport.

    The following styling attributes are provided:

      \li drag-threshold - defines a threshold distance to drag the view
          until a page change occurs when the pointer is released - from 1.0,
          meaning a page's width, to zero.
      \li velocity-threshold - defines the minimum velocity of a flick gesture
          that causes a page change, setting this to a smaller value allows the
          page to be changed with smaller gestures
      \li page-snap-spring-k - k constant of the spring model used when the
          view snaps to a page, higher value causes a faster snap.
      \li page-snap-friction-c - friction constant used when the view was snaps
          to a page, affects the damping of the snap.
      \li slide-limit - defines the maximum number of pages to change per gesture,
          value of zero means there's no limit.
      \li pan-threshold - defines the length of swipe before it taken determined
          to be panning

    \sa MPannableWidgetStyle
*/
class PagedViewportStyle : public MPannableWidgetStyle
{
    Q_OBJECT
    M_STYLE(PagedViewportStyle)

    M_STYLE_ATTRIBUTE(qreal, velocityThreshold, VelocityThreshold)
    M_STYLE_ATTRIBUTE(qreal, dragThreshold,     DragThreshold)
    M_STYLE_ATTRIBUTE(int,   slideLimit,        SlideLimit)
    M_STYLE_ATTRIBUTE(qreal, pageSnapSpringK,   PageSnapSpringK)
    M_STYLE_ATTRIBUTE(qreal, pageSnapFrictionC, PageSnapFrictionC)
    M_STYLE_ATTRIBUTE(qreal, panThreshold, PanThreshold)
};

class PagedViewportStyleContainer : public MPannableWidgetStyleContainer
{
    M_STYLE_CONTAINER(PagedViewportStyle)
};

#endif
