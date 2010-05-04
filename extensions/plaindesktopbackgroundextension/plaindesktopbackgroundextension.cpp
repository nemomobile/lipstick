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

#include <QPainter>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <MTheme>
#include <MLibrary>
#include "plaindesktopbackgroundextension.h"
#include "plaindesktopbackgroundpixmap.h"
#include "plaindesktopbackgroundstyle.h"

Q_EXPORT_PLUGIN2(plaindesktopbackgroundextension, PlainDesktopBackgroundExtension)

PlainDesktopBackgroundExtension::PlainDesktopBackgroundExtension() :
    desktop(NULL),
    landscapeGConfItem("/desktop/meego/background/landscape/picture_filename"),
    portraitGConfItem("/desktop/meego/background/portrait/picture_filename"),
    blurFactor(0),
    blurRadius(0),
    pixmapBeingUpdated(false)
{
    // Set up the blurring timeline
    connect(&blurTimeLine, SIGNAL(valueChanged(qreal)), this, SLOT(setBlurFactor(qreal)));

    // Connect to the windowListUpdated signal of the HomeApplication to get information about window list changes
    connect(qApp, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), this, SLOT(setBlurTimeLineDirection(const QList<WindowInfo> &)));

    // Connect the GConf signals
    connect(&landscapeGConfItem, SIGNAL(valueChanged()), this, SLOT(updateLandscapePixmap()));
    connect(&portraitGConfItem, SIGNAL(valueChanged()), this, SLOT(updatePortraitPixmap()));
}

PlainDesktopBackgroundExtension::~PlainDesktopBackgroundExtension()
{
}

void PlainDesktopBackgroundExtension::setDesktopInterface(MDesktopInterface &desktopInterface)
{
    desktop = &desktopInterface;
}

const PlainDesktopBackgroundStyle *plainDesktopBackgroundStyle(M::Orientation orientation)
{
    return static_cast<const PlainDesktopBackgroundStyle *> (MTheme::style("PlainDesktopBackgroundStyle", "", "", "", orientation, NULL));
}

bool PlainDesktopBackgroundExtension::initialize(const QString &)
{
    // Get the defaults from the style
    const PlainDesktopBackgroundStyle *landscapeStyle = plainDesktopBackgroundStyle(M::Landscape);
    blurRadius = landscapeStyle->blurRadius();
    blurTimeLine.setDuration(landscapeStyle->blurDuration());
    blurTimeLine.setUpdateInterval(landscapeStyle->blurUpdateInterval());
    landscapeDefaultBackgroundImage = landscapeStyle->defaultBackgroundImage();
    MTheme::releaseStyle(landscapeStyle);

    const PlainDesktopBackgroundStyle *portraitStyle = plainDesktopBackgroundStyle(M::Portrait);
    portraitDefaultBackgroundImage = portraitStyle->defaultBackgroundImage();
    MTheme::releaseStyle(portraitStyle);

    // Update pixmaps
    updateLandscapePixmap();
    updatePortraitPixmap();

    return true;
}

MWidget *PlainDesktopBackgroundExtension::widget()
{
    return NULL;
}

void drawPixmap(QPainter *painter, const QPixmap &pixmap, const QRectF &boundingRect)
{
    painter->drawPixmap(boundingRect, pixmap, QRectF(0, 0, pixmap.width(), pixmap.height()));
}

void PlainDesktopBackgroundExtension::drawBackground(QPainter *painter, const QRectF &boundingRect) const
{
    // Get the pixmaps for the current orientation
    M::OrientationAngle angle = desktop != NULL ? desktop->orientationAngle() : M::Angle0;
    PlainDesktopBackgroundPixmap *pixmap = (angle == M::Angle0 || angle == M::Angle180) ? landscapePixmap.data() : portraitPixmap.data();

    if (pixmap != NULL) {
        if (pixmap->pixmap() != NULL && blurFactor < 1) {
            // The normal background pixmap needs to be drawn unless the background should be completely blurred
            drawPixmap(painter, *pixmap->pixmap(), boundingRect);
        }

        if (pixmap->blurredPixmap() != NULL && blurFactor > 0) {
            // The blurred background pixmap needs to be drawn if the background should be blurred at all
            painter->setOpacity(blurFactor);
            drawPixmap(painter, *pixmap->blurredPixmap(), boundingRect);
        }
    }
}

void PlainDesktopBackgroundExtension::setBlurTimeLineDirection(const QList<WindowInfo> &windowList)
{
    // If there are windows in the window list the background should be blurred (blurFactor should be animated from 0 to 1).
    // If there are no windows in the window list the background should not be blurred (blurFactor should be animated from 1 to 0).
    QTimeLine::Direction direction = windowList.length() > 0 ? QTimeLine::Forward : QTimeLine::Backward;
    if (blurTimeLine.direction() != direction) {
        blurTimeLine.toggleDirection();
    }

    if (blurTimeLine.state() == QTimeLine::NotRunning) {
        blurTimeLine.resume();
    }
}

void PlainDesktopBackgroundExtension::setBlurFactor(qreal blurFactor)
{
    if (this->blurFactor != blurFactor) {
        this->blurFactor = blurFactor;
        desktop->update();
        // Set a property with the blur factor to make functional testing possible
        qApp->setProperty("plainDesktopBackgroundExtensionBlurFactor", QVariant(blurFactor));
    }
}

void PlainDesktopBackgroundExtension::updateLandscapePixmap()
{
    if (!pixmapBeingUpdated) {
        updatePixmap(&landscapePixmap, landscapeGConfItem, landscapeDefaultBackgroundImage);
    }
}

void PlainDesktopBackgroundExtension::updatePortraitPixmap()
{
    if (!pixmapBeingUpdated) {
        updatePixmap(&portraitPixmap, portraitGConfItem, portraitDefaultBackgroundImage);
    }
}

void PlainDesktopBackgroundExtension::updateDesktop()
{
    if (desktop != NULL) {
        desktop->update();
    }
}

void PlainDesktopBackgroundExtension::updatePixmap(QSharedPointer<PlainDesktopBackgroundPixmap> *pixmap, MGConfItem &gConfItem, const QString &defaultName)
{
    if (pixmap != NULL) {
        bool previousPixmapExists = !(*pixmap).isNull();
        QString oldName = previousPixmapExists ? (*pixmap)->pixmapName() : "";
        QString newName = gConfItem.value().toString();
        *pixmap = QSharedPointer<PlainDesktopBackgroundPixmap>(new PlainDesktopBackgroundPixmap(newName, defaultName, blurRadius));
        QString actualName = (*pixmap)->pixmapName();

        if (previousPixmapExists && !(newName.isEmpty() && actualName == defaultName) && (actualName != newName && !oldName.isEmpty())) {
            // The loaded pixmap is not the requested one so something went wrong. Use the old pixmap name instead.
            pixmapBeingUpdated = true;
            gConfItem.set(oldName);
            *pixmap = QSharedPointer<PlainDesktopBackgroundPixmap>(new PlainDesktopBackgroundPixmap(oldName, defaultName, blurRadius));
            pixmapBeingUpdated = false;

            // Redraw the desktop if the blurred pixmap becomes available later
            connect((*pixmap).data(), SIGNAL(pixmapUpdated()), this, SLOT(updateDesktop()));
        }

        // Redraw the desktop
        updateDesktop();
    }
}

M_LIBRARY
