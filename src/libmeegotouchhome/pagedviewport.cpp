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
#include <QGraphicsLinearLayout>
#include <QGestureEvent>
#include <QPanGesture>

#include <MWidgetCreator>
M_REGISTER_WIDGET(PagedViewport)

PagedViewport::PagedViewport(QGraphicsItem *parent) :
        MPannableViewport(parent),
        previousPage(0),
        layoutVisualizationWrapper(NULL),
        explicitlyStopped(false),
        beingPanned(false)
{
    // The strategy will be deleted by the pannable viewport
    pagedPanning = new PagedPanning(this);
    setPhysics(pagedPanning);
    connect(pagedPanning, SIGNAL(pageChanged(int)), this, SIGNAL(pageChanged(int)));
    connect(pagedPanning, SIGNAL(pageIsPanning(bool)), this, SIGNAL(pageIsPanning(bool)));
    setPanDirection(Qt::Horizontal);
    setAcceptGesturesFromAnyDirection(true);

    PagePositionIndicator *positionIndicator = new PagePositionIndicator();
    connect(this, SIGNAL(pageCountChanged(int)), positionIndicator, SLOT(setPageCount(int)));
    connect(this, SIGNAL(pageChanged(int)), positionIndicator, SLOT(setFocusedPage(int)));
    setPositionIndicator(positionIndicator);

    connect(pagedPanning, SIGNAL(pageIsPanning(bool)), positionIndicator, SIGNAL(pageIsPanning(bool)));

    emit pageChanged(0);

    grabGesture(Qt::PinchGesture);
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
    previousPage = pagedPanning->activePage();
}

void PagedViewport::focusFirstPage()
{
    //TODO: add support for setting position to the right-most page
    // when using right-to-left layout
    pagedPanning->setPage(0);
}

void PagedViewport::updatePageCount(int pages)
{
    pagedPanning->setPageCount(pages);
    emit pageCountChanged(pages);
}

int PagedViewport::currentPage() const
{
    return pagedPanning->activePage();
}

int PagedViewport::targetPage() const
{
    return pagedPanning->targetPage();
}

float PagedViewport::pageWidth() const
{
    return pagedPanning->pageWidth();
}

void PagedViewport::setPageWrapMode(bool enable)
{
    pagedPanning->setPageWrapMode(enable);

    if (enable) {
        QGraphicsWidget *pannableWidget = widget();
        if (pannableWidget) {
            QGraphicsLinearLayout *pannableLayout = dynamic_cast<QGraphicsLinearLayout*>(pannableWidget->layout());
            if (pannableLayout) {
                layoutVisualizationWrapper = QSharedPointer<LayoutVisualizationWrapper>(new LayoutVisualizationWrapper(*pannableLayout));
                updateVisualizationWrapper();
                // Multiple signals are monitored to control the visualization wrapper
                // - panningStopped signal when rotation happens while panning
                connect(this, SIGNAL(panningStopped()), SLOT(updateVisualizationWrapper()));
                // - pageChanged signal when panning between pages without wrapping
                connect(this, SIGNAL(pageChanged(int)), SLOT(newPageSet(int)));
                // - pageWrapped signal when panning from first page to last or vice-versa via page wrapping
                connect(physics(), SIGNAL(pageWrapped()), SLOT(updateVisualizationWrapper()));
            }
        }
    } else {
        layoutVisualizationWrapper = QSharedPointer<LayoutVisualizationWrapper>(NULL);
        disconnect(SIGNAL(panningStopped()), this, SLOT(updateVisualizationWrapper()));
        disconnect(SIGNAL(pageChanged(int)), this, SLOT(newPageSet(int)));
        disconnect(physics(), SIGNAL(pageWrapped()), this, SLOT(updateVisualizationWrapper()));
    }
}

void PagedViewport::newPageSet(int newPage)
{
    const int firstPageIndex = 0;
    const int lastPageIndex = pagedPanning->pageCount() - 1;
    if (!((previousPage == firstPageIndex && newPage == lastPageIndex) ||
          (previousPage == lastPageIndex && newPage == firstPageIndex))) {
        updateVisualizationWrapper();
    }

    previousPage = newPage;
}

void PagedViewport::updateVisualizationWrapper()
{
    if (layoutVisualizationWrapper.isNull()) {
        return;
    }

    int currentTargetPage = targetPage();

    if (currentTargetPage == 0) {
        // First page
        layoutVisualizationWrapper->setWrappingMode(layoutDirection() == Qt::LeftToRight ? LayoutVisualizationWrapper::WrapRightEdgeToLeft : LayoutVisualizationWrapper::WrapLeftEdgeToRight);
    } else if (currentTargetPage == pagedPanning->pageCount() - 1) {
        // Last page
        layoutVisualizationWrapper->setWrappingMode(layoutDirection() == Qt::LeftToRight ? LayoutVisualizationWrapper::WrapLeftEdgeToRight : LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    } else {
        // Middle page
        layoutVisualizationWrapper->setWrappingMode(LayoutVisualizationWrapper::NoWrap);
    }
}

void PagedViewport::stopPanning()
{
    pagedPanning->pointerRelease();
    pagedPanning->stop();
    explicitlyStopped = true;
    beingPanned = false;
}

void PagedViewport::panGestureEvent(QGestureEvent *event, QPanGesture* panGesture)
{
    // In case pan has been explicitly stopped but gesture has not been canceled yet (no mouse release or cancel)
    // then instead of reacting to gesture updates we need to cancel the gesture
    if (explicitlyStopped && panGesture->state() == Qt::GestureUpdated) {
        panGesture->setGestureCancelPolicy(QGesture::CancelAllInContext);
        event->accept(panGesture);
    } else {
        // Not explicitly stopped anymore after getting gesture start, finish or cancel events
        explicitlyStopped = false;

        beingPanned = !(panGesture->state() == Qt::GestureFinished || panGesture->state() == Qt::GestureCanceled);
        MPannableViewport::panGestureEvent(event, panGesture);
    }
}

bool PagedViewport::event(QEvent *event)
{
    if (beingPanned && event->type() == QEvent::GestureOverride) {
        event->setAccepted(true);

        return true;
    }

    return MPannableViewport::event(event);
}
