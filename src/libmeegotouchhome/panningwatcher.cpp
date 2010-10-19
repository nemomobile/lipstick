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

#include "panningwatcher.h"
#include <MPannableWidget>

static const int MOVEMENT_DETECTOR_TIMEOUT = 500;

PanningWatcher::PanningWatcher(MPannableWidget &widget, QObject *parent) : QObject(parent), pannableWidget(widget)
{
    connect(&widget, SIGNAL(positionChanged(QPointF)), this, SLOT(updatePanningState()));
    oldViewportPos = widget.position();

    movementDetectorTimer.setSingleShot(true);
    connect(&movementDetectorTimer, SIGNAL(timeout()), this, SLOT(updatePanningState()));
}

void PanningWatcher::updatePanningState()
{
    if(oldViewportPos != pannableWidget.position()) {
        if(!panning) {
            panning = true;
            emit panningStateChanged(true);
        }

        oldViewportPos = pannableWidget.position();

        movementDetectorTimer.start(MOVEMENT_DETECTOR_TIMEOUT);
    } else {
        if(panning) {
            panning = false;
            emit panningStateChanged(false);
        }
    }
}

bool PanningWatcher::isPanning()
{
    return panning;
}
