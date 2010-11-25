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

PanningWatcher::PanningWatcher(MPannableWidget &widget, QObject *parent) : QObject(parent), pannableWidget(widget), panning(false)
{
    connect(&widget, SIGNAL(positionChanged(QPointF)), this, SLOT(enablePanningIfPositionChanged()));
    connect(&movementDetectorTimer, SIGNAL(timeout()), this, SLOT(disablePanningIfPositionNotChanged()));
    movementDetectorTimer.setSingleShot(true);
    oldViewportPos = widget.position();
}

void PanningWatcher::enablePanningIfPositionChanged()
{
    if(oldViewportPos != pannableWidget.position()) {
        // Position has changed
        if(!panning) {
            // Was not panning before, emit signal
            panning = true;
            emit panningStateChanged(true);
        }

        // Store the current position and start movement detection timer
        oldViewportPos = pannableWidget.position();
        movementDetectorTimer.start(MOVEMENT_DETECTOR_TIMEOUT);
    }
}

void PanningWatcher::disablePanningIfPositionNotChanged()
{
    if (oldViewportPos == pannableWidget.position()) {
        // Position has not changed
        if(panning) {
            // Was panning before, emit signal
            panning = false;
            emit panningStateChanged(false);
        }
    } else {
        // Position has changed
        enablePanningIfPositionChanged();
    }
}

bool PanningWatcher::isPanning()
{
    return panning;
}
