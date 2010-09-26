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

#include <QPainter>
#include <QImageReader>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <MTheme>
#include "plaindesktopbackgroundpixmap.h"

static const int PIXMAP_WIDTH_MAX = 2048;
static const int PIXMAP_HEIGHT_MAX = 2048;

PlainDesktopBackgroundPixmap::PlainDesktopBackgroundPixmap(const QString &name, const QString &defaultName, int blurRadius, qreal brightness) :
        blurRadius_(blurRadius),
        brightness_(brightness),
        pixmapFromTheme_(NULL)
{
    if (name.startsWith('/')) {
        // Absolute path: Load from a file
        bool success = false;

        QImageReader imageReader(name);
        if (imageReader.canRead()) {
            QSize size = imageReader.size();
            if (size.height() <= PIXMAP_HEIGHT_MAX && size.width() <= PIXMAP_WIDTH_MAX) {
                // Only load the pixmap if it's not too big
                pixmapFromFile_ = QSharedPointer<QPixmap>(new QPixmap);
                success = pixmapFromFile_->load(name);
            }
        }

        if (success) {
            // Loading succeeded
            pixmapName_ = name;
        } else {
            // Loading failed: destroy the pixmap
            pixmapFromFile_.clear();

            // Load the default from the theme instead
            pixmapFromTheme_ = MTheme::pixmap(defaultName);
            pixmapName_ = defaultName;
        }
    } else if (!name.isEmpty()) {
        // Not a path: Load from the theme by pixmap ID
        pixmapFromTheme_ = MTheme::pixmap(name);
        pixmapName_ = name;
    } else {
        // Empty name: Load the default from the theme
        pixmapFromTheme_ = MTheme::pixmap(defaultName);
        pixmapName_ = defaultName;
    }

    // Create a defocused version
    createDefocusedPixmap();
}

PlainDesktopBackgroundPixmap::~PlainDesktopBackgroundPixmap()
{
    if (pixmapFromTheme_ != NULL) {
        MTheme::releasePixmap(pixmapFromTheme_);
    }
}

void PlainDesktopBackgroundPixmap::createDefocusedPixmap()
{
    // Disconnect previous pixmapRequestsFinished() connections if any
    disconnect(MTheme::instance(), SIGNAL(pixmapRequestsFinished()), this, SLOT(createDefocusedPixmap()));

    if (pixmapFromFile_.isNull() && pixmapFromTheme_ != NULL && pixmapFromTheme_->isNull()) {
        // A pixmap from the theme is being used but it is not available yet: listen to pixmapRequestsFinished() signals
        connect(MTheme::instance(), SIGNAL(pixmapRequestsFinished()), this, SLOT(createDefocusedPixmap()));
    } else {
        // A pixmap from a file is being used or a pixmap from the theme is being used and it is available
        defocusedPixmap_ = QSharedPointer<QPixmap>(createDefocusedPixmap(*pixmap(), blurRadius_, brightness_));
        emit pixmapUpdated();
    }
}

QPixmap *PlainDesktopBackgroundPixmap::createDefocusedPixmap(const QPixmap &pixmap, int blurRadius, qreal brightness)
{
    // Create a scene and put the pixmap on it using a blur effect
    QGraphicsScene scene;
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurRadius);
    item->setGraphicsEffect(blur);
    item->setOpacity(brightness);
    scene.addItem(item);

    // Create a pixmap for the defocused version of the pixmap
    QPixmap *defocusedPixmap = new QPixmap(pixmap.width(), pixmap.height());
    defocusedPixmap->fill(Qt::black);

    // Paint the scene to the pixmap
    QPainter painter(defocusedPixmap);
    scene.render(&painter);

    return defocusedPixmap;
}

const QPixmap *PlainDesktopBackgroundPixmap::pixmap() const
{
    return pixmapFromFile_.isNull() ? pixmapFromTheme_ : pixmapFromFile_.data();
}

const QPixmap *PlainDesktopBackgroundPixmap::defocusedPixmap() const
{
    return defocusedPixmap_.data();
}

QString PlainDesktopBackgroundPixmap::pixmapName() const
{
    return pixmapName_;
}
