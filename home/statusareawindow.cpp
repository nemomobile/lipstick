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

#include <DuiOrientationChangeEvent>
#include <DuiOnDisplayChangeEvent>
#include <DuiDeviceProfile>
#include "statusareawindow.h"
#include "statusarea.h"
#include "mainwindow.h"

StatusAreaWindow::StatusAreaWindow(QWidget *parent) :
    DuiWindow(parent),
    scene(new QGraphicsScene),
    statusArea_(new StatusArea)
{
    // Set the window type to _NET_WM_WINDOW_TYPE_DOCK
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);

    // Set the scene and add the status area to the scene
    setScene(scene);
    scene->addItem(statusArea_);

    // Rotate to current orientation
    rotate(MainWindow::instance()->orientation());
    connect(MainWindow::instance(), SIGNAL(orientationChangeFinished(const Dui::Orientation &)), this, SLOT(rotate(const Dui::Orientation &)));
}

StatusAreaWindow::~StatusAreaWindow()
{
    scene->removeItem(statusArea_);
    delete statusArea_;
    delete scene;
}

StatusArea *StatusAreaWindow::statusArea() const
{
    return statusArea_;
}

void StatusAreaWindow::rotate(const Dui::Orientation &orientation)
{
    // Notify each item on the notification view's scene about the new orientation
    DuiOrientationChangeEvent event(orientation);
    Q_FOREACH(QGraphicsItem * item, scene->items()) {
        scene->sendEvent(item, &event);
    }

    QTransform transform;
    transform.rotate(MainWindow::instance()->orientationAngle());
    setTransform(transform);
    QSizeF size = statusArea_->preferredSize();
    if (orientation == Dui::Portrait) {
        size.transpose();
    }
    setFixedSize(size.toSize());
    if (MainWindow::instance()->orientationAngle() == Dui::Angle0 ||
            MainWindow::instance()->orientationAngle() == Dui::Angle270) {
        move(0, 0);
    } else if (MainWindow::instance()->orientationAngle() == Dui::Angle90) {
        move(DuiDeviceProfile::instance()->resolution().width() - width(), 0);
    } else {
        move(0, DuiDeviceProfile::instance()->resolution().height() - height());
    }
    centerOn(statusArea_);
}
