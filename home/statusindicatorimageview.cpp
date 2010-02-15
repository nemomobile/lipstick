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

#include "statusindicatorimageview.h"
#include "statusindicator.h"
#include <DuiTheme>
#include <DuiLabel>
#include <DuiViewCreator>
#include <QTimeLine>

StatusIndicatorImageView::StatusIndicatorImageView(StatusIndicator *controller) :
    DuiWidgetView(controller),
    controller(controller),
    animationFrame(0),
    animationTimeline(new QTimeLine(0, this))
{
    animationTimeline->setCurveShape(QTimeLine::LinearCurve);
    animationTimeline->setLoopCount(0);
    connect(animationTimeline, SIGNAL(frameChanged(int)), this, SLOT(setAnimationFrame(int)));
}

StatusIndicatorImageView::~StatusIndicatorImageView()
{
    clearImageList();
}

void StatusIndicatorImageView::setupModel()
{
    DuiWidgetView::setupModel();

    setAnimationFrame(model()->value().toDouble() * images.size());
    if (model()->animate()) {
        startAnimation();
    } else {
        stopAnimation();
    }
}

void StatusIndicatorImageView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == StatusIndicatorModel::Value) {
            // Set the animation frame based on the model value
            setAnimationFrame(model()->value().toDouble() * images.size());
        } else if (member == BatteryStatusIndicatorModel::Animate) {
            if (model()->animate()) {
                startAnimation();
            } else {
                stopAnimation();
            }
        }
    }
}

void StatusIndicatorImageView::applyStyle()
{
    DuiWidgetView::applyStyle();

    clearImageList();

    // Get the list of images to use as animation frames of the indicator
    imageList = style()->imageList().trimmed().split(QChar(' '));
    images = QVector<const QPixmap *>(imageList.length(), NULL);

    // Get the duration of the animation
    animationTimeline->setDuration(style()->animationDuration());
    animationTimeline->setFrameRange(0, images.size());
    animationTimeline->setUpdateInterval(style()->animationDuration() / (images.size() > 0 ? images.size() : 1));
}

void StatusIndicatorImageView::setAnimationFrame(int frame)
{
    frame = qBound(0, frame, images.size() - 1);

    if (animationFrame != frame) {
        animationFrame = frame;
        controller->update();
    }
}

void StatusIndicatorImageView::startAnimation()
{
    if (animationTimeline->state() == QTimeLine::NotRunning) {
        animationTimeline->start();
    }
}

void StatusIndicatorImageView::stopAnimation()
{
    if (animationTimeline->state() == QTimeLine::Running) {
        animationTimeline->stop();
    }
}

void StatusIndicatorImageView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    if (animationFrame < images.size()) {
        if (images[animationFrame] == NULL) {
            // Load the image if it has not been loaded yet
            images[animationFrame] = DuiTheme::pixmapCopy(imageList.at(animationFrame), QSize(size().width(), size().height()));
        }

        if (images[animationFrame] != NULL) {
            // Paint the image
            painter->drawPixmap(QPointF(0, 0), *images[animationFrame]);
        }
    }
}

void StatusIndicatorImageView::clearImageList()
{
    for (int i = 0; i < images.size(); i++) {
        delete images[i];
    }

    imageList.clear();
    images.clear();
}

DUI_REGISTER_VIEW_NEW(StatusIndicatorImageView, StatusIndicator)
