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
#include "pagedviewport.h"
#include "switcherphysicsintegrationstrategy.h"
#include "duiwidgetcreator.h"

DUI_REGISTER_WIDGET(PagedViewport)

PagedViewport::PagedViewport(QGraphicsItem *parent) : DuiPannableViewport(parent)
{
    // The strategy will be deleted by the pannable viewport
    integrationStrategy = new SwitcherPhysicsIntegrationStrategy;
    physics()->setIntegrationStrategy(integrationStrategy);
    connect(integrationStrategy, SIGNAL(snapIndexChanged(int)), this, SIGNAL(pageChanged(int)));
    setPanDirection(Qt::Horizontal);
}

PagedViewport::~PagedViewport() {

}

void PagedViewport::setPanDirection(const Qt::Orientations &)
{
    DuiPannableViewport::setPanDirection(Qt::Horizontal);
}

void PagedViewport::panToPage(uint page)
{
    integrationStrategy->panToItem(page);
    // Currently the integrator can not start the movement, so we need
    // to start the movement manually, the integration strategy will then
    // make sure that we end up in the right page
    QPointF currentPosition = physics()->position();
    physics()->setPosition(currentPosition + QPointF(1,0));
}

void PagedViewport::updatePageWidth(int width)
{
    integrationStrategy->setSnapInterval(width);
}
