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
#ifndef SWITCHERPHYSICSINTEGRATIONSTRATEGY_H
#define SWITCHERPHYSICSINTEGRATIONSTRATEGY_H

#include <QObject>
#include <duiphysics2dpanning.h>

class SwitcherPhysicsIntegrationStrategy : public QObject, public DuiPhysics2DIntegrationStrategy
{
    Q_OBJECT
    
public:
    /*! 
     * Constructs a SwitcherPhysicsIntegrationStrategy
     */
    SwitcherPhysicsIntegrationStrategy();
    
    /*!
     * Destroys a SwitcherPhysicsIntegrationStrategy
     */
    virtual ~SwitcherPhysicsIntegrationStrategy();
    
    /*! \reimp
     * \brief Method used to calculate values for next frame of
     * panning movement.
     */
    virtual void integrate(qreal &position,
                           qreal &velocity,
                           qreal &pointerSpring,
                           qreal &acceleration,
                           qreal rangeStart,
                           qreal rangeEnd,
                           IntegrationData& data);
    //! reimp_end

    /*!
     * Sets the interval where this strategy will snap the motion.
     */
    void setSnapInterval(uint snapInterval);

    /*!
     * Returns the current snap interval
     */
    uint snapInterval() const;

    /*!
     * Pans the view automatically to the given item index
     */
    void panToItem(uint itemIndex);

signals:
    /*!
     * Indecates that the snap index has changed. 
     */
    void snapIndexChanged(int index);

private:
    /*! The interval at which the snapping occurs. */
    uint snapInterval_;

    /*! The current index */
    int currentIndex;

    void snapIntegrate(qreal &position,
                       qreal &velocity,
                       qreal &pointerSpring,
                       qreal &acceleration,
                       qreal rangeStart,
                       qreal rangeEnd,
                       IntegrationData& data);

    bool autoPanMode;

    int autoPanTargetIndex;

    void autoPanIntegrate(qreal &position,
                          qreal &velocity,
                          qreal &pointerSpring,
                          qreal &acceleration,
                          IntegrationData& data);
};

#endif
