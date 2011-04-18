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
