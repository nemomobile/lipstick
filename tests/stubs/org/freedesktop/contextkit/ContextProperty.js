.pragma library

var batteryChargePercentage
var batteryIsCharging
var systemPowerSaveMode
var contextProperties = new Array

function addContextProperty(contextProperty)
{
    contextProperties.push(contextProperty)
    contextProperty.updateValue()
}

function updateContextProperties()
{
    for (var i = 0; i < contextProperties.length; i++) {
        contextProperties[i].updateValue()
    }
}

function setBatteryChargePercentage(value)
{
    batteryChargePercentage = value
    updateContextProperties()
}

function setBatteryIsCharging(value)
{
    batteryIsCharging = value
    updateContextProperties()
}

function setSystemPowerSaveMode(value)
{
    systemPowerSaveMode = value
    updateContextProperties()
}
