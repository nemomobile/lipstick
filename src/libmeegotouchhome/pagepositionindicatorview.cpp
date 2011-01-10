/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#include "pagepositionindicatorview.h"

#include <MScalableImage>
#include "pagepositionindicator.h"
#include "pagepositionindicatormodel.h"

PagePositionIndicatorView::PagePositionIndicatorView(PagePositionIndicator *controller) :
    MWidgetView(controller),
    controller(controller),
    forceUnfocusedIcon(false)
{
}

PagePositionIndicatorView::~PagePositionIndicatorView()
{
}

void PagePositionIndicatorView::applyStyle()
{
    MWidgetView::applyStyle();

    if (!style()->focusedBetweenPages()) {
        connect(controller, SIGNAL(pageIsPanning(bool)), this, SLOT(setForceUnfocusedIcon(bool)), Qt::UniqueConnection);
    } else {
        disconnect(controller, SIGNAL(pageIsPanning(bool)), this, SLOT(setForceUnfocusedIcon(bool)));
        forceUnfocusedIcon = false;
    }
}

void PagePositionIndicatorView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == PagePositionIndicatorModel::FocusedPage
            || member == PagePositionIndicatorModel::PageCount) {
            update();
        }
    }
}

void PagePositionIndicatorView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    if (!controller->isEnabled()) {
        // Don't do anything if the
        return;
    }

    int pageCount = model()->pageCount();

    if (pageCount > 1) {
        int focusedPage = model()->focusedPage();
        int viewCenter = size().width() / 2;

        const MScalableImage *focusedIcon = style()->focusedIndicatorImage();
        const MScalableImage *unfocusedIcon = style()->unfocusedIndicatorImage();

        int iconWidth = style()->iconSize().width();
        int iconHeight = style()->iconSize().height();

        int indicatorBarWidth = iconWidth * pageCount + style()->spacing() * (pageCount - 1);

        int indicatorBarYpos = 0;
        // center the indicator bar in viewport
        int indicatorBarXpos = viewCenter - (indicatorBarWidth / 2);

        int iconPositionInIndicatorBar = indicatorBarXpos;
        for (int i = 0; i < pageCount; i++) {
            if(forceUnfocusedIcon) {
                if (unfocusedIcon != NULL) {
                    unfocusedIcon->draw(iconPositionInIndicatorBar, indicatorBarYpos, iconWidth, iconHeight, painter);
                }
            } else {
                if (focusedPage == i) {
                    if (focusedIcon != NULL) {
                        focusedIcon->draw(iconPositionInIndicatorBar, indicatorBarYpos, iconWidth, iconHeight, painter);
                    }
                } else {
                    if (unfocusedIcon != NULL) {
                        unfocusedIcon->draw(iconPositionInIndicatorBar, indicatorBarYpos, iconWidth, iconHeight, painter);
                    }
                }
            }
            iconPositionInIndicatorBar += iconWidth + style()->spacing();
        }
    }
}

void PagePositionIndicatorView::setForceUnfocusedIcon(bool enable)
{
    forceUnfocusedIcon = enable;
}

M_REGISTER_VIEW_NEW(PagePositionIndicatorView, PagePositionIndicator)
