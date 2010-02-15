/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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

#include "switcherbuttonswview.h"
#include <QRectF>
#include <QPointF>
#include <QSizeF>
#include <QPainter>
#include <DuiScalableImage>

SwitcherButtonSWView::SwitcherButtonSWView(SwitcherButton *button) :
    SwitcherButtonView(button),
    thumbnailMaskApplied(false)
{
}

SwitcherButtonSWView::~SwitcherButtonSWView()
{
}

void SwitcherButtonSWView::applyStyle()
{
    DuiWidgetView::applyStyle();

    thumbnailPixmap = QPixmap(style()->iconSize());
    thumbnailMask = QPixmap();

    updateThumbnail();
}

void SwitcherButtonSWView::updateThumbnail()
{
    if (!thumbnailPixmap.isNull() && !qWindowPixmap.isNull()) {
        QPainter painter(&thumbnailPixmap);
        painter.drawPixmap(QRectF(QPointF(0, 0), style()->iconSize()), qWindowPixmap,
                           QRectF(QPointF(0, 0), QSizeF(-1, -1)));

        // The mask needs to be reapplied
        thumbnailMaskApplied = false;
    }
}

void SwitcherButtonSWView::backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    if (thumbnailMask.isNull()) {
        // Create a thumbnail mask
        // TODO: the mask isn't scaled properly when the button size is increased
        // when it's being dragged.
        const DuiScalableImage *mask = style()->maskImage();
        if (mask != NULL && !mask->pixmap()->isNull() &&
                mask->pixmap()->width() > 1 &&
                mask->pixmap()->height() > 1) {
            thumbnailMask = mask->pixmap()->createMaskFromColor(QColor(255, 255, 255, 255), Qt::MaskOutColor);
            thumbnailMaskApplied = false;
        }
    }

    // Set thumbnail mask
    if (!thumbnailMaskApplied && !thumbnailMask.isNull()) {
        thumbnailPixmap.setMask(thumbnailMask);
        thumbnailMaskApplied = true;
    }

    // Draw window thumbnail
    painter->drawPixmap(QRectF(style()->iconPosition(), style()->iconSize()),
                        thumbnailPixmap, QRectF(QPointF(0, 0), QSizeF(-1, -1)));
}

void SwitcherButtonSWView::backendSpecificUpdateXWindowPixmap()
{
    qWindowPixmap = QPixmap::fromX11Pixmap(xWindowPixmap, QPixmap::ExplicitlyShared);
}

DUI_REGISTER_VIEW_NEW(SwitcherButtonSWView, SwitcherButton)
