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

#include <contextproperty.h>
#include "batterystatusindicator.h"

static const char *BATTERY_MODE_NORMAL = "";
static const char *BATTERY_MODE_CHARGING = "Charging";
static const char *BATTERY_MODE_POWERSAVE = "PowerSave";
static const char *BATTERY_MODE_POWERSAVE_AND_CHARGING = "PowerSaveCharging";

BatteryStatusIndicator::BatteryStatusIndicator(QObject *parent) :
    StatusIndicator(parent),
    batteryChargePercentageContextProperty(createContextProperty("Battery.ChargePercentage")),
    batteryIsChargingContextProperty(createContextProperty("Battery.IsCharging")),
    systemPowerSaveModeContextProperty(createContextProperty("System.PowerSaveMode"))
{
    connect(batteryChargePercentageContextProperty, SIGNAL(valueChanged()), this, SLOT(setValue()));
    connect(batteryIsChargingContextProperty, SIGNAL(valueChanged()), this, SLOT(setMode()));
    connect(systemPowerSaveModeContextProperty, SIGNAL(valueChanged()), this, SLOT(setMode()));

    setMode();
    setValue();
}

void BatteryStatusIndicator::setValue()
{
    // valueList must contain a "battery empty" image and images for different battery levels
    int images = valueList().count() - 1;

    // Calculate the amount of bars for the current charge percentage
    int percentage = qMax(qMin(batteryChargePercentageContextProperty->value().toInt(), 100), 0);
    int image = qMin(images * percentage / 100, images - 1);

    if (batteryIsChargingContextProperty->value().toBool()) {
        // While charging don't use the "battery empty" image, so skip it with image + 1, but always animate at least one bar, so don't add 1 if the battery is almost full
        setInitialValueIndex(image < images - 1 ? (image + 1) : image);
    } else {
        // While not charging use the "battery empty" icon when 0 bars remaining, otherwise skip it with remainingBars + 1
        setInitialValueIndex(image == 0 ? 0 : (image + 1));
    }
}

void BatteryStatusIndicator::setMode()
{
    if (batteryIsChargingContextProperty->value().toBool()) {
        StatusIndicator::setMode(systemPowerSaveModeContextProperty->value().toBool() ? BATTERY_MODE_POWERSAVE_AND_CHARGING : BATTERY_MODE_CHARGING);
    } else {
        StatusIndicator::setMode(systemPowerSaveModeContextProperty->value().toBool() ? BATTERY_MODE_POWERSAVE : BATTERY_MODE_NORMAL);
    }
}
