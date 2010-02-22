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

#include "switcherbuttonswview.h"
#include <QRectF>
#include <QPointF>
#include <QSizeF>
#include <QPainter>
#include <DuiScalableImage>

SwitcherButtonSWView::SwitcherButtonSWView(SwitcherButton *button) :
    SwitcherButtonView(button)
{
}

SwitcherButtonSWView::~SwitcherButtonSWView()
{
}

void SwitcherButtonSWView::backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    // Draw window thumbnail
    painter->drawPixmap(QRect(style()->iconPosition().toPoint(),
                              style()->iconSize()),
                        qWindowPixmap);
}

void SwitcherButtonSWView::backendSpecificUpdateXWindowPixmap()
{
    qWindowPixmap = QPixmap::fromX11Pixmap(xWindowPixmap, QPixmap::ExplicitlyShared);
}

DUI_REGISTER_VIEW_NEW(SwitcherButtonSWView, SwitcherButton)
