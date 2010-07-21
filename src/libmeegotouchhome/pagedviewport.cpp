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
#include <mwidgetview.h>
#include "pagedviewport.h"
#include "pagedpanning.h"
#include "pagedviewportstyle.h"
#include "mpositionindicator.h"
#include "pagepositionindicator.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET(PagedViewport)

PagedViewport::PagedViewport(QGraphicsItem *parent) : MPannableViewport(parent), pages_(0)
{
    // The strategy will be deleted by the pannable viewport
    pagedPanning = new PagedPanning(this);
    setPhysics(pagedPanning);
    connect(pagedPanning, SIGNAL(pageChanged(int)), this, SIGNAL(pageChanged(int)));
    setPanDirection(Qt::Horizontal);

    PagePositionIndicator *positionIndicator = new PagePositionIndicator();
    connect(this, SIGNAL(pageCountChanged(int)), positionIndicator, SLOT(setPageCount(int)));
    connect(this, SIGNAL(pageChanged(int)), positionIndicator, SLOT(setFocusedPage(int)));
    setPositionIndicator(positionIndicator);

    emit pageChanged(0);
}

PagedViewport::~PagedViewport()
{
}

void PagedViewport::setPanDirection(const Qt::Orientations &)
{
    MPannableViewport::setPanDirection(Qt::Horizontal);
}

void PagedViewport::panToPage(uint page)
{
    pagedPanning->panToPage(page);
}

void PagedViewport::setPage(uint page)
{
    pagedPanning->setPage(page);
}

void PagedViewport::focusFirstPage()
{
    //TODO: add support for setting position to the right-most page
    // when using right-to-left layout
    pagedPanning->setPage(0);
}

void PagedViewport::updatePageCount(int pages)
{
    pages_ = pages;
    pagedPanning->setPageCount(pages);
    emit pageCountChanged(pages);
}

int PagedViewport::currentPage()
{
    return pagedPanning->activePage();
}

float PagedViewport::pageWidth() const
{
    return pagedPanning->pageWidth();
}
