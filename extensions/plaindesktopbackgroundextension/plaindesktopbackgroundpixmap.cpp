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
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <MTheme>
#include "plaindesktopbackgroundpixmap.h"

PlainDesktopBackgroundPixmap::PlainDesktopBackgroundPixmap(const QString &name, const QString &defaultName, int blurRadius) :
        pixmapFromTheme_(NULL)
{
    if (name.startsWith('/')) {
        // Absolute path: Load from a file
        pixmapFromFile_ = QSharedPointer<QPixmap>(new QPixmap);
        if (pixmapFromFile_->load(name)) {
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

    // Create a blurred version
    blurredPixmap_ = QSharedPointer<QPixmap>(createBlurredPixmap(*pixmap(), blurRadius));
}

PlainDesktopBackgroundPixmap::~PlainDesktopBackgroundPixmap()
{
    if (pixmapFromTheme_ != NULL) {
        MTheme::releasePixmap(pixmapFromTheme_);
    }
}

QPixmap *PlainDesktopBackgroundPixmap::createBlurredPixmap(const QPixmap &pixmap, int blurRadius)
{
    // Create a scene and put the pixmap on it using a blur effect
    QGraphicsScene scene;
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurRadius);
    item->setGraphicsEffect(blur);
    scene.addItem(item);

    // Create a pixmap for the blurred version of the pixmap
    QPixmap *blurredPixmap = new QPixmap(pixmap.width(), pixmap.height());
    blurredPixmap->fill(Qt::black);

    // Paint the scene to the pixmap
    QPainter painter(blurredPixmap);
    scene.render(&painter);

    return blurredPixmap;
}

const QPixmap *PlainDesktopBackgroundPixmap::pixmap() const
{
    return pixmapFromFile_.isNull() ? pixmapFromTheme_ : pixmapFromFile_.data();
}

const QPixmap *PlainDesktopBackgroundPixmap::blurredPixmap() const
{
    return blurredPixmap_.data();
}

QString PlainDesktopBackgroundPixmap::pixmapName() const
{
    return pixmapName_;
}
