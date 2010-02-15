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

#include <QApplication>
#include "clock.h"

Clock::Clock(QGraphicsItem *parent) : DuiWidgetController(new ClockModel, parent)
{
    updateTimerId = startTimer(500);
}

void Clock::timerEvent(QTimerEvent *)
{
    model()->setTime(QDateTime::currentDateTime());
}

void Clock::enterDisplayEvent()
{
    setModelUpdatesEnabled(true);
}

void Clock::exitDisplayEvent()
{
    setModelUpdatesEnabled(false);
}

void Clock::setModelUpdatesEnabled(bool enabled)
{
    if (enabled && updateTimerId == 0) {
        // If the updates should be enabled and the update timer is not running, start it
        updateTimerId = startTimer(500);
    } else if (!enabled && updateTimerId != 0) {
        // If the updates should be disabled and the update timer is running, stop it
        killTimer(updateTimerId);
        updateTimerId = 0;
    }
}
