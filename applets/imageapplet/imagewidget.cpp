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

#include "imagewidget.h"

#include <QtGui>
#include <MApplication>
#include <MWindow>
#include <MTheme>

ImageWidget::ImageWidget(const QString &landscape, const QString &portrait):
    image_landscape(0), image_portrait(0), currentImage(0), gestureStarted(false)
{
    cornersResizing << QPointF(0, 0) << QPointF(0, 0) << QPointF(0, 0) << QPointF(0, 0);

    image_landscape = MTheme::pixmapCopy(landscape);
    image_portrait = MTheme::pixmapCopy(portrait);

    // TODO orientation stuff commented out since there is currently no way to get the orientation
    setCurrentImage(image_landscape);
//    setCurrentImage(MApplication::activeWindow()->orientation() == M::Landscape ? image_landscape : image_portrait);

    // Get informed about orientation changes
//    connect(MApplication::activeWindow(), SIGNAL(orientationChanged(const M::Orientation &)), this, SLOT(orientationChanged(const M::Orientation &)));
}

ImageWidget::~ImageWidget()
{
    delete image_landscape;
    delete image_portrait;
}

void ImageWidget::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (currentImage == NULL || currentImage->isNull())
        return;

    QSizeF imageSize = currentImage->size();
    QSizeF diff = size() - imageSize;

    painter->drawPixmap(diff.width() / 2, diff.height() / 2, *currentImage);

    if (gestureStarted) {
        const QPen &pen = painter->pen();
        QRectF rect = QRectF(QPointF(0, 0), size());
        painter->setPen(Qt::yellow);
        for (int i = 0; i < cornersResizing.count(); i++) {
            if ((int) cornersResizing[i].x() | (int) cornersResizing[i].y()) {
                qreal ylength = qMax((qreal)(rect.height() / 3.5), (qreal) 10.0);
                qreal xlength = qMax((qreal)(rect.width() / 3.5), (qreal) 10.0);
                //paint the vertical line from corner
                QPointF p;
                switch (i) {
                case 0:
                    p = rect.topLeft();
                    break;
                case 1:
                    p = rect.topRight();
                    xlength = (-1) * xlength;
                    break;
                case 2:
                    p = rect.bottomLeft();
                    ylength = (-1) * ylength;
                    break;
                case 3:
                    p = rect.bottomRight();
                    xlength = (-1) * xlength;
                    ylength = (-1) * ylength;
                default:
                    break;
                }
                QPolygonF polygon;
                polygon << QPointF(p.x() + xlength, p.y()) << p << QPointF(
                            p.x(), p.y() + ylength);
                painter->drawPolyline(polygon);
            }
        }
        painter->setPen(pen);
    }
}

void ImageWidget::setCurrentImage(const QPixmap *image)
{
    if (image != currentImage) {
        currentImage = image;
        updateGeometry();
    }
}

QSizeF ImageWidget::sizeHint(Qt::SizeHint, const QSizeF &) const
{
    return (currentImage != NULL && !currentImage->isNull()) ? currentImage->size() : QSizeF(50, 50);
}

void ImageWidget::orientationChanged(const M::Orientation &orientation)
{
    setCurrentImage(orientation == M::Landscape ? image_landscape : image_portrait);
}
