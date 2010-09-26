/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome.
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

#include <QApplication>
#include <QPainter>
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
    defocusFactor(0),
    blurRadius(0),
    brightness(1),
    pixmapBeingUpdated(false)
{
    // Set up the defocusing timeline
    connect(&defocusTimeLine, SIGNAL(valueChanged(qreal)), this, SLOT(setDefocusFactor(qreal)));

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
    brightness = landscapeStyle->brightness();
    defocusTimeLine.setDuration(landscapeStyle->defocusDuration());
    defocusTimeLine.setUpdateInterval(landscapeStyle->defocusUpdateInterval());
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

QGraphicsWidget *PlainDesktopBackgroundExtension::widget()
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
        if (pixmap->pixmap() != NULL && defocusFactor < 1) {
            // The normal background pixmap needs to be drawn unless the background should be completely defocused
            drawPixmap(painter, *pixmap->pixmap(), boundingRect);
        }

        if (pixmap->defocusedPixmap() != NULL && defocusFactor > 0) {
            // The defocused background pixmap needs to be drawn if the background should be defocused at all
            painter->setOpacity(defocusFactor);
            drawPixmap(painter, *pixmap->defocusedPixmap(), boundingRect);
        }
    }
}

void PlainDesktopBackgroundExtension::setDefocused(bool defocused)
{
    QTimeLine::Direction direction = defocused ? QTimeLine::Forward : QTimeLine::Backward;
    if (defocusTimeLine.direction() != direction) {
        defocusTimeLine.toggleDirection();
    }

    if (defocusTimeLine.state() == QTimeLine::NotRunning) {
        defocusTimeLine.resume();
    }
}

void PlainDesktopBackgroundExtension::setDefocusFactor(qreal defocusFactor)
{
    if (this->defocusFactor != defocusFactor) {
        this->defocusFactor = defocusFactor;
        if (desktop != NULL) {
            desktop->update();
        }
        // Set a property with the defocus factor to make functional testing possible
        qApp->setProperty("plainDesktopBackgroundExtensionDefocusFactor", QVariant(defocusFactor));
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
        *pixmap = QSharedPointer<PlainDesktopBackgroundPixmap>(new PlainDesktopBackgroundPixmap(newName, defaultName, blurRadius, brightness));
        QString actualName = (*pixmap)->pixmapName();

        if (previousPixmapExists && !(newName.isEmpty() && actualName == defaultName) && (actualName != newName && !oldName.isEmpty())) {
            // The loaded pixmap is not the requested one so something went wrong. Use the old pixmap name instead.
            pixmapBeingUpdated = true;
            gConfItem.set(oldName);
            *pixmap = QSharedPointer<PlainDesktopBackgroundPixmap>(new PlainDesktopBackgroundPixmap(oldName, defaultName, blurRadius, brightness));
            pixmapBeingUpdated = false;

            // Redraw the desktop if the defocused pixmap becomes available later
            connect((*pixmap).data(), SIGNAL(pixmapUpdated()), this, SLOT(updateDesktop()));
        }

        // Redraw the desktop
        updateDesktop();
    }
}

M_LIBRARY
