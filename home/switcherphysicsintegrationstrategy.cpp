/*
 * This file is part of duihome
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */
#include <QDebug>
#include "switcherphysicsintegrationstrategy.h"
#include <stdlib.h>
#include <algorithm>

SwitcherPhysicsIntegrationStrategy::SwitcherPhysicsIntegrationStrategy()
{
    snapInterval_ = 0;
    autoPanMode = false;
}

SwitcherPhysicsIntegrationStrategy::~SwitcherPhysicsIntegrationStrategy(){}

void SwitcherPhysicsIntegrationStrategy::integrate(qreal &position,
                           qreal &velocity,
                           qreal &pointerSpring,
                           qreal &acceleration,
                           qreal rangeStart,
                           qreal rangeEnd,
                           IntegrationData& data)
{
    if(autoPanMode) {
        autoPanIntegrate(position,
                         velocity,
                         pointerSpring,
                         acceleration,
                         data);
    } else {
        snapIntegrate(position,
                      velocity,
                      pointerSpring,
                      acceleration,
                      rangeStart,
                      rangeEnd,
                      data);
    }
}

void SwitcherPhysicsIntegrationStrategy::snapIntegrate(qreal &position,
                           qreal &velocity,
                           qreal &pointerSpring,
                           qreal &acceleration,
                           qreal rangeStart,
                           qreal rangeEnd,
                           IntegrationData& data)
{
    qreal force;
    // Damping
    if (position >= rangeStart && position <= rangeEnd) {
        // Inside range
        if (data.pointer) {
            force = -data.frictionC * velocity;
        } else {
            force = -data.slideFrictionC * velocity;
        }
    } else {
        // Outside range (in border)
        force = -data.borderFrictionC * velocity;
    }

    // Pointer spring
    if (data.pointer) {
        force += -data.pointerSpringK * pointerSpring;
    }

    /* 
       We need to make the position snap to the nearest multiple of 
       snapInterval when the finger is lifted from the screen 
       i.e. pointer == false
    */
    if (snapInterval_ > 0 && !data.pointer) {
	/*
	  The window that we should be snapping to, 
	  the 0.5 is simple integer rounding before casting to int
	*/	
	int snapTarget = (int)((position / (qreal)snapInterval_) + 0.5);       
	/* 
	   The snap target must be inside the panning range, this is to make sure 
	   that we do not snap out side the range
	*/
	int maxSnapTarget = (rangeEnd - rangeStart) / snapInterval_;
	snapTarget = std::min(snapTarget, maxSnapTarget);

	force += data.borderSpringK * (snapTarget * (qreal)snapInterval_ - position);

	qreal closeEnough = position - (snapInterval_ * snapTarget);

	if (abs(closeEnough) < 1 && abs(force) < 1) {
	    // Setting these to zero should stop the integration process
	    force = 0;
	    velocity = 0;
	    acceleration = 0;
	    // Make the position exactly the snap position
	    position = snapInterval_ * snapTarget;
	    emit snapIndexChanged(snapTarget);    
	} else {
	    acceleration   = force;
	    velocity      += acceleration;
	    position      += velocity;
	    pointerSpring += velocity;	    
	}
    } else {
	acceleration   = force;
	velocity      += acceleration;
	position      += velocity;
	pointerSpring += velocity;
    }
}

void SwitcherPhysicsIntegrationStrategy::autoPanIntegrate(qreal &position,
                           qreal &velocity,
                           qreal &pointerSpring,
                           qreal &acceleration,
                           IntegrationData &data)
{
    qreal force = 0.0;
    qreal autoPanTargetPosition = autoPanTargetIndex*snapInterval_;

    if ( qAbs(autoPanTargetPosition - position) > 0.5 ) {
        force = data.borderSpringK * (autoPanTargetPosition - position);
        force -= data.borderFrictionC * velocity;

        acceleration   = force;
        velocity      += force;
        position      += velocity;
        pointerSpring += velocity;
    } else {
        acceleration  = 0;
        velocity      = 0;
        position      = autoPanTargetPosition;
        pointerSpring = 0;
        autoPanMode = false;

        currentIndex = autoPanTargetIndex;
        emit snapIndexChanged(autoPanTargetIndex);
    }
}

void SwitcherPhysicsIntegrationStrategy::setSnapInterval(uint newSnapInterval)
{
    snapInterval_ = newSnapInterval;
}

uint SwitcherPhysicsIntegrationStrategy::snapInterval() const
{
    return snapInterval_;
}

void SwitcherPhysicsIntegrationStrategy::panToItem(uint itemIndex)
{
    autoPanTargetIndex = itemIndex;
    autoPanMode = true;
}
