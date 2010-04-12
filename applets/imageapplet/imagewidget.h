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

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <MWidget>

class QPixmap;

class ImageWidget : public MWidget
{
    Q_OBJECT

public:
    ImageWidget(const QString &image_landscape, const QString &image_portrait);
    virtual ~ImageWidget();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private slots:
    /*!
     * \brief A slot for getting information about orientation changes
     */
    void orientationChanged(const M::Orientation &);

private:
    QPixmap *image_landscape;
    QPixmap *image_portrait;
    //!plan is that there are more images for different sizes, this one points to current one
    const QPixmap *currentImage;
    void setCurrentImage(const QPixmap *image);
    QSizeF initialSizeOnPinch;
    bool gestureStarted;
    QVector<QPointF> cornersResizing;
};

#endif // IMAGEWIDGET_H
