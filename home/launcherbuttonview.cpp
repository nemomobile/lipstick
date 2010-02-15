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

#include "launcherbuttonview.h"
#include "launcherbutton.h"

#include <QPainter>
#include <QX11Info>
#include <QTimeLine>
#include <DuiTheme>
#include <DuiViewCreator>
#include <DuiScalableImage>
#include <QGraphicsSceneMouseEvent>
#include <QUrl>

#include "windowinfo.h"

LauncherButtonView::LauncherButtonView(LauncherButton *button) :
    DuiButtonIconView(button),
    controller(button),
    thumbnailMaskApplied(false)
{
}

LauncherButtonView::~LauncherButtonView()
{
}

void LauncherButtonView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    DuiButtonIconView::drawBackground(painter, option);

    if (model()->targetType() == "Directory") {
        // Draw the category background image
        drawCategoryBackground(painter);
    } else if (model()->targetType() == "Link" && !thumbnailPixmap.isNull()) {
        if (thumbnailMask.isNull()) {
            // Create a thumbnail mask
            const DuiScalableImage *image = style()->shortcutThumbnailMaskImage();
            if (image != NULL && !image->pixmap()->isNull() && image->pixmap()->width() > 1 && image->pixmap()->height() > 1) {
                QPixmap pixmap(thumbnailPixmap.width(), thumbnailPixmap.height());
                QPainter painter(&pixmap);
                image->draw(QPoint(), thumbnailPixmap.size(), &painter);
                thumbnailMask = pixmap.createMaskFromColor(QColor(255, 255, 255, 255), Qt::MaskOutColor);
                thumbnailMaskApplied = false;
            }
        }

        // Apply thumbnail mask if it exists and has not been applied yet
        if (!thumbnailMaskApplied && !thumbnailMask.isNull()) {
            thumbnailPixmap.setMask(thumbnailMask);
            thumbnailMaskApplied = true;
        }

        // Draw shortcut thumbnail
        painter->drawPixmap(style()->shortcutThumbnailPosition(), thumbnailPixmap);
    }
}

void LauncherButtonView::drawCategoryBackground(QPainter *painter) const
{
    const QPixmap *pixmap = style()->categoryImage();
    if (pixmap != NULL) {
        painter->drawPixmap(0, 0, *pixmap);
    }
}

void LauncherButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    if (model()->targetType() == "Link") {
        drawShortcutContents(painter);
    } else {
        DuiButtonIconView::drawContents(painter, option);
    }
}

void LauncherButtonView::drawShortcutContents(QPainter *painter) const
{
    // Draw the container
    drawShortcutContainer(painter);

    // TODO: We could draw an icon here using drawIcon, if icon would be support in shortcuts.
    // At the moment shortcuts only support thumbnails. This might not be sufficient
    // for content that cannot be thumbnailed (such as audio clips)

    // Draw text
    drawText(painter, style()->shortcutTextPosition(), style()->shortcutTextSize());
}

void LauncherButtonView::drawText(QPainter *painter, const QPointF &pos, const QSize &size) const
{
    const QString text(model()->text());
    if (!text.isEmpty() && controller->isTextVisible()) {
        painter->setFont(style()->font());
        painter->setPen(style()->textColor());
        painter->drawText(QRectF(pos, size), Qt::AlignCenter, text);
    }
}

void LauncherButtonView::drawShortcutContainer(QPainter *painter) const
{
    const DuiScalableImage *container = NULL;

    if (thumbnailPixmap.isNull()) {
        container = style()->invalidShortcutContainerImage();
    } else {
        container = style()->shortcutContainerImage();
    }

    if (container != NULL)
        container->draw(QRect(QPoint(0, 0), size().toSize()), painter);
}

void LauncherButtonView::applyStyle()
{
    updateThumbnail();

    DuiButtonIconView::applyStyle();
}

void LauncherButtonView::setupModel()
{
    DuiButtonIconView::setupModel();

    updateThumbnail();
}

void LauncherButtonView::updateData(const QList<const char *>& modifications)
{
    DuiButtonIconView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == LauncherButtonModel::Thumbnail) {
            updateThumbnail();
        }
    }

    update();
}

void LauncherButtonView::updateThumbnail()
{
    // Reset the thumbnail mask
    thumbnailMask = QPixmap();
    thumbnailMaskApplied = false;

    if (model()->thumbnail().isNull())
        // Thumbnail does not exist: Use a null pixmap
        thumbnailPixmap = QPixmap();
    else {
        QPixmap pixmap;
        if (pixmap.load(QUrl(model()->thumbnail()).path(), "PNG")) {
            // Thumbnail exists: Scale the thumbnail pixmap to a local pixmap
            thumbnailPixmap = QPixmap(style()->shortcutThumbnailSize().width(),
                                      style()->shortcutThumbnailSize().height());


            QPainter p(&thumbnailPixmap);
            p.drawPixmap(QRectF(QPointF(0, 0), style()->shortcutThumbnailSize()),
                         pixmap, QRectF(QPoint(0, 0), QSize(-1, -1)));
        } else {
            // Thumbnail does not exist: Use a null pixmap
            thumbnailPixmap = QPixmap();
        }
    }
}

DUI_REGISTER_VIEW_NEW(LauncherButtonView, LauncherButton)
