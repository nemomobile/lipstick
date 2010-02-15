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

#include "statusarea.h"
#include <DuiApplicationIfProxy>
#include <QGraphicsSceneMouseEvent>

const QString StatusArea::STATUS_INDICATOR_MENU_SERVICE_NAME = "com.nokia.duistatusindicatormenu";
// To prevent swipe inside the status bar. Must end swiping atlease some distance away from status bar
const int SWIPE_THRESHOLD = 30;

StatusArea::StatusArea(DuiWidget *parent) :
    DuiWidgetController(parent)
{
}

bool StatusArea::sceneEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress: {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
        if (mouseEvent) {
            firstPos = mouseEvent->pos();
        }
        return true;
    }
    case QEvent::GraphicsSceneMouseMove: {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
        if (mouseEvent) {
            lastPos = mouseEvent->pos();
            return true;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseRelease: {
        if (firstPos.y() + SWIPE_THRESHOLD < lastPos.y()) {
            showStatusIndicatorMenu();
            lastPos = QPointF();
            return true;
        }
        break;
    }
    default:
        break;
    }
    //Event not handled here, so return false
    return false;
}

void StatusArea::showStatusIndicatorMenu()
{
    DuiApplicationIfProxy duiApplicationIfProxy(STATUS_INDICATOR_MENU_SERVICE_NAME, this);
    if (duiApplicationIfProxy.connection().isConnected()) {
        duiApplicationIfProxy.launch();
    } else {
        qWarning() << "Could not launch" << STATUS_INDICATOR_MENU_SERVICE_NAME << "- DBus not connected?";
    }
}
