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
#include "pagedviewportview.h"
#include "pagedpanning.h"

#include "mviewcreator.h"


PagedViewportView::PagedViewportView(PagedViewport *controller) :
    MPannableWidgetView(controller), controller(controller)
{
}

PagedViewportView::~PagedViewportView()
{
}

void PagedViewportView::applyStyle()
{
    PagedPanning* physics = dynamic_cast<PagedPanning*>(controller->physics());

    if (physics != NULL) {
        physics->setVelocityThreshold(style()->velocityThreshold());
        physics->setDragThreshold(style()->dragThreshold());
        physics->setSlideLimit(style()->slideLimit());
        physics->setVelocityThreshold(style()->velocityThreshold());
        physics->setPageSnapSpringK(style()->pageSnapSpringK());
        physics->setPageSnapFriction(style()->pageSnapFrictionC());
        physics->setPanThreshold(style()->panThreshold());
    }

    MPannableWidgetView::applyStyle();
}

M_REGISTER_VIEW_NEW(PagedViewportView, PagedViewport);
