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

#include "launcherbuttonprogressindicatorview.h"
#include <QPropertyAnimation>

LauncherButtonProgressIndicatorView::LauncherButtonProgressIndicatorView(LauncherButtonProgressIndicator *controller)
        : MWidgetView(controller),
       positionAnimation(new QPropertyAnimation(this, "currentFrame", this)),
       currentFrameIndex(-1),
       controller(controller)
{
    positionAnimation->setLoopCount(-1);

    connect(controller, SIGNAL(visibleChanged()), this, SLOT(pauseOrResumeAnimation()));
    connect(controller, SIGNAL(displayEntered()), this, SLOT(resumeAnimation()));
    connect(controller, SIGNAL(displayExited()), this, SLOT(pauseAnimation()));
}

LauncherButtonProgressIndicatorView::~LauncherButtonProgressIndicatorView()
{
    releaseUsedPixmaps();
}

void LauncherButtonProgressIndicatorView::setupModel()
{
    MWidgetView::setupModel();

    // Set the default style mode
    applyStyle();

    pauseOrResumeAnimation();
}

void LauncherButtonProgressIndicatorView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == LauncherButtonProgressIndicatorModel::IndicatorState) {
            applyStyle();
        } else if (member == LauncherButtonProgressIndicatorModel::Value) {
            // Count the frame from the value and the range and add one as frame index starts from 1
            setCurrentFrame(frameFromValue(model()->value()));
        } else if (member == MProgressIndicatorModel::UnknownDuration) {
            pauseOrResumeAnimation();
        }
    }
}

void LauncherButtonProgressIndicatorView::updateStyleMode()
{
    switch(model()->indicatorState()) {
        case LauncherButtonModel::Installing:
            style().setModeInstalling();
        break;
        case LauncherButtonModel::Downloading:
            style().setModeDownloading();
            setCurrentFrame(0);
        break;
        case LauncherButtonModel::Launching:
            style().setModeLaunching();
        break;
        default:
        break;
    }
}

void LauncherButtonProgressIndicatorView::applyStyle()
{
    // Style mode needs to be checked when applyStyle() is called as MWidgetView can change
    // the style mode to disabled when parent widget is disabled
    updateStyleMode();

    MWidgetView::applyStyle();

    positionAnimation->setDuration(style()->period());

    positionAnimation->setStartValue(0);
    positionAnimation->setEndValue(style()->numberOfFrames());
    reloadFrames();
}

void LauncherButtonProgressIndicatorView::reloadFrames()
{
    releaseUsedPixmaps();

    // Only load new frames if there actually is a base image in style
    if (!style()->baseImageName().isEmpty()) {
        // Starts from 1 because that's the agreed convention
        // for the image file names
        for (int i = 1; i <= style()->numberOfFrames(); i++) {
            QString frame_name = QString("%1_%2_%3").arg(style()->baseImageName())\
                                                    .arg(style()->baseImageSize())\
                                                    .arg(i);
            const QPixmap *p = MTheme::pixmap(frame_name);

            // If the size is different in style and for pixmap, consider the pixmap invalid (MTheme::pixmap() returns default
            // pixmap when icon id is not found so this is the only way to know whether icon was found from theme or not).
            // If pixmap is invalid then try the different naming convention. (This is a workaround for bug NB#258902.)
            if (i < 10 && (p->size().height() != style()->baseImageSize() || p->size().width() != style()->baseImageSize())) {
                // Try naming convention where there is 0 before a single digit index
                frame_name = QString("%1_%2_0%3").arg(style()->baseImageName())\
                                                    .arg(style()->baseImageSize())\
                                                    .arg(i);

                if (p) {
                    MTheme::releasePixmap(p);
                }

                p = MTheme::pixmap(frame_name);
            }
            animationPixmaps << p;
        }
    }
}

void LauncherButtonProgressIndicatorView::releaseUsedPixmaps()
{
    foreach(const QPixmap *p, animationPixmaps)
        MTheme::releasePixmap(p);
    animationPixmaps.clear();
}

void LauncherButtonProgressIndicatorView::resumeAnimation()
{
    if (model()->unknownDuration()) {
        if (positionAnimation->state() == QPropertyAnimation::Paused) {
            positionAnimation->resume();
        }
        else if (positionAnimation->state() == QPropertyAnimation::Stopped)
            positionAnimation->start();
    }
}

void LauncherButtonProgressIndicatorView::pauseAnimation()
{
    if (positionAnimation->state() == QPropertyAnimation::Running)
        positionAnimation->pause();
}

void LauncherButtonProgressIndicatorView::pauseOrResumeAnimation()
{
    if (controller->isVisible() && controller->isOnDisplay() && model()->unknownDuration()) {
        resumeAnimation();
    } else {
        pauseAnimation();
    }
}

int LauncherButtonProgressIndicatorView::currentFrame() const
{
    return  currentFrameIndex;
}

void LauncherButtonProgressIndicatorView::setCurrentFrame(int newCurrentFrame)
{
    if ( currentFrameIndex == newCurrentFrame || newCurrentFrame >= animationPixmaps.count() || newCurrentFrame < 0) {
        return;
    }

    currentFrameIndex = newCurrentFrame;
    update();
}

void LauncherButtonProgressIndicatorView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    if (animationPixmaps.isEmpty() ||  currentFrameIndex < 0)
        return;

    int minimumFrame;
    if (model()->unknownDuration()) {
        minimumFrame =  currentFrameIndex;
    } else {
        // When "known duration" then draw pixmaps from minimum to current progress
        minimumFrame = frameFromValue(model()->minimum());
    }

    for (int frameIndex = minimumFrame; minimumFrame >=  0 && frameIndex <=  currentFrameIndex; frameIndex++) {
        const QPixmap *pixmap = animationPixmaps[frameIndex];
        if (pixmap && !pixmap->isNull()) {
            painter->drawPixmap(QRectF(QPointF(0, 0), size()), *pixmap, QRectF(pixmap->rect()));
        }
    }
}

int LauncherButtonProgressIndicatorView::frameFromValue(const int &value) const
{
    if (value < 0  || value > model()->maximum() || model()->maximum() < model()->minimum()) {
        return -1;
    }

    // Calculate the frame from the value, range and amount of frames.
    return (value * (style()->numberOfFrames() - 1)) / (model()->maximum() - model()->minimum());
}

M_REGISTER_VIEW_NEW(LauncherButtonProgressIndicatorView, LauncherButtonProgressIndicator)
