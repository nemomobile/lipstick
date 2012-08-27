
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QDeclarativeItem>
#include <QPixmap>
#include "lipstickglobal.h"

class LIPSTICK_EXPORT StatusBar : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool isPortrait READ isPortrait WRITE setIsPortrait NOTIFY isPortraitChanged)

    QPixmap _sharedPixmap;
    bool _isPortrait;

private slots:
    void updateXThings();

public:
    explicit StatusBar(QDeclarativeItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateSharedPixmap();
    bool isPortrait() const;
    void setIsPortrait(bool value);

    // we reimplement pretty much all virtuals, to prevent anything
    // propegating to items (like MouseArea) underneath us.
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);


private slots:
    void initializeStatusBar();

signals:
    void isPortraitChanged();

};

#endif // STATUSBAR_H
