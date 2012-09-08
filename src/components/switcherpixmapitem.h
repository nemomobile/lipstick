
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
// Copyright (c) 2011, Robin Burchell
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (c) 2010, Nokia Corporation and/or its subsidiary(-ies) <directui@nokia.com>

#ifndef SWITCHERPIXMAPITEM_H
#define SWITCHERPIXMAPITEM_H

#include <QDeclarativeItem>

#include "lipstickglobal.h"
#include "xtools/xeventlistener.h"

class LIPSTICK_EXPORT SwitcherPixmapItem : public QDeclarativeItem, XEventListener
{
    Q_OBJECT
    Q_DISABLE_COPY(SwitcherPixmapItem)
    Q_PROPERTY(int windowId READ windowId WRITE setWindowId)

    struct Private;
    Private * const d;

    void createDamage();
    void destroyDamage();
    void updateXWindowPixmap();

public:
    explicit SwitcherPixmapItem(QDeclarativeItem *parent = 0);
    virtual ~SwitcherPixmapItem();

    int windowId() const;
    void setWindowId(int windowId);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:
    void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);

private:
    virtual bool handleXEvent(const XEvent &event);
};

#endif // SWITCHERPIXMAPITEM_H
