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

#include "home.h"
#include "desktop.h"
#include "mainwindow.h"

#include <DuiApplicationPage>
#include <DuiModalSceneWindow>
#include <DuiOverlay>
#include <DuiPannableViewport>
#include <DuiButton>
#include <DuiSceneManager>
#include <DuiApplication>
#include <QGraphicsLinearLayout>
#include "homeapplication.h"

Home::Home(QGraphicsItem *parent) :
    DuiSceneWindow(parent),
    desktop(new Desktop(this))
{
    setObjectName("Home");

    // Use the desktop as the central widget
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addItem(desktop);
    setLayout(layout);
}

Home::~Home()
{
}

QRectF Home::boundingRect() const
{
    return desktop->boundingRect();
}
