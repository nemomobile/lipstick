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

#include "home.h"
#include "desktop.h"
#include "mainwindow.h"
#include <mstatusbar.h>
#include <QGraphicsLinearLayout>

#include <MWidgetCreator>
M_REGISTER_WIDGET(Home)

Home::Home(QGraphicsItem *parent) :
    MSceneWindow(parent),
    statusBar(new MStatusBar),
    desktop(new Desktop)
{
    setObjectName("Home");

    // Show the status bar
    statusBar->appear();

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
