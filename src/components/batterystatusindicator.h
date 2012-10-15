/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef BATTERYSTATUSINDICATOR_H
#define BATTERYSTATUSINDICATOR_H

#include "statusindicator.h"

/*!
 * A status indicator for showing the battery charge level or
 * battery charging animation.
 */
class LIPSTICK_EXPORT BatteryStatusIndicator : public StatusIndicator
{
    Q_OBJECT

public:
    /*!
     * Constructs a BatteryStatusIndicator.
     *
     * \param parent parent object
     */
    explicit BatteryStatusIndicator(QObject *parent = 0);

private slots:
    //! Sets the value of the indicator based on the battery level
    void setValue();

    //! Sets the mode of the indicator based on charging and power saving states
    void setMode();

private:
    //! The average charge level of the currently connected batteries. Expressed as percentage of the maximum charge level.
    ContextProperty *batteryChargePercentageContextProperty;

    //! Whether or not the device is currently charging at least one of its batteries.
    ContextProperty *batteryIsChargingContextProperty;

    //! A boolean indicating whether or not power save mode is enabled.
    ContextProperty *systemPowerSaveModeContextProperty;

#ifdef UNIT_TEST
    friend class Ut_BatteryStatusIndicator;
#endif
};

#endif // BATTERYSTATUSINDICATOR_H
