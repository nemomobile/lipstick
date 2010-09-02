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
