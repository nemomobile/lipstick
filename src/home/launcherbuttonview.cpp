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

#include <QGraphicsAnchorLayout>
#include "launcherbuttonview.h"
#include "launcherbutton.h"

LauncherButtonView::LauncherButtonView(LauncherButton *controller) :
    MButtonIconView(controller),
    controller(controller)
{
    progressIndicatorTimeLine.setLoopCount(0);
    progressIndicatorTimeLine.setCurveShape(QTimeLine::LinearCurve);

    connect(&progressIndicatorTimeLine, SIGNAL(frameChanged(int)), this, SLOT(setProgressIndicatorFrame(int)));
}

LauncherButtonView::~LauncherButtonView()
{
}

void LauncherButtonView::setupModel()
{
    MButtonIconView::setupModel();

    QList<const char *> modifications;
    modifications << LauncherButtonModel::ShowProgressIndicator;
    updateData(modifications);
}

void LauncherButtonView::applyStyle()
{
    MButtonIconView::applyStyle();

    // Set the progress indicator timing properties
    controller->setProgressIndicatorTimeout(style()->progressIndicatorTimeout());
    progressIndicatorTimeLine.setDuration(style()->progressIndicatorAnimationDuration());

    if (!progressIndicatorPixmaps.isEmpty()) {
        // Release the old progress indicator pixmaps
        foreach(const QPixmap *pixmap, progressIndicatorPixmaps) {
            MTheme::releasePixmap(pixmap);
        }

        progressIndicatorPixmaps.clear();
    }

    // Load the new progress indicator pixmaps
    QStringList imageList = style()->progressIndicatorImageList().trimmed().split(QChar(' '));
    progressIndicatorPixmaps.fill(NULL, imageList.length());
    for (int i = 0; i < imageList.count(); i++) {
        progressIndicatorPixmaps.replace(i, MTheme::pixmap(imageList.at(i), style()->iconSize()));
    }
    progressIndicatorTimeLine.setFrameRange(0, imageList.count() - 1);

    // Calculate progress indicator rectangle
    int hPadding = style()->paddingLeft() + style()->paddingRight();
    int vPadding = style()->paddingTop() + style()->paddingBottom();
    QRectF contentRect(style()->paddingLeft(), style()->paddingTop(), style()->preferredSize().width() - hPadding, style()->preferredSize().height() - vPadding);
    progressIndicatorRect.setTopLeft(QPointF(contentRect.center().x() - (style()->iconSize().width() / 2), contentRect.top()));
    progressIndicatorRect.setSize(style()->iconSize());
}

void LauncherButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    if (progressIndicatorTimeLine.state() == QTimeLine::Running) {
        // Draw the button icon view first: it messes up with the painter so the state must be saved
        painter->save();
        MButtonIconView::drawContents(painter, option);
        painter->restore();

        // Draw the progress indicator pixmap
        const QPixmap *pixmap = progressIndicatorPixmaps.at(progressIndicatorTimeLine.currentFrame());
        if (pixmap != NULL && !pixmap->isNull()) {
            painter->drawPixmap(progressIndicatorRect, *pixmap, QRectF(pixmap->rect()));
        }
    } else {
        // No progress indicator, so just draw the icon view
        MButtonIconView::drawContents(painter, option);
    }
}

void LauncherButtonView::updateData(const QList<const char *>& modifications)
{
    MButtonIconView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == LauncherButtonModel::ShowProgressIndicator) {
            if (model()->showProgressIndicator()) {
                showProgressIndicator();
            } else {
                hideProgressIndicator();
            }
        }
    }
}

void LauncherButtonView::setProgressIndicatorFrame(int)
{
    update();
}

void LauncherButtonView::showProgressIndicator()
{
    if (progressIndicatorTimeLine.state() != QTimeLine::Running) {
        progressIndicatorTimeLine.start();
        update();
    }
}

void LauncherButtonView::hideProgressIndicator()
{
    if (progressIndicatorTimeLine.state() != QTimeLine::NotRunning) {
        progressIndicatorTimeLine.stop();
        update();
    }
}

M_REGISTER_VIEW_NEW(LauncherButtonView, LauncherButton)
