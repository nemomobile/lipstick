import QtQuick 1.1
import org.freedesktop.contextkit 1.0

Image {
    id: batteryStatusIndicator
    property variant icons
    property int firstIconIndex
    property int currentIconIndex
    property bool animate

    source: "images/" + icons[currentIconIndex] + ".png"
    opacity: LipstickSettings.lockscreenVisible ? 0 : 1
    Behavior on opacity {
        NumberAnimation { duration: 500 }
    }

    ContextProperty {
        id: batteryChargePercentageContextProperty
        key: "Battery.ChargePercentage"
        onValueChanged: batteryStatusIndicator.setFirstIconIndex()
    }
    ContextProperty {
        id: batteryIsChargingContextProperty
        key: "Battery.IsCharging"
        onValueChanged: batteryStatusIndicator.setMode()
    }
    ContextProperty {
        id: systemPowerSaveModeContextProperty
        key: "System.PowerSaveMode"
        onValueChanged: batteryStatusIndicator.setMode()
    }

    function setMode() {
        if (batteryIsChargingContextProperty.value) {
            if (systemPowerSaveModeContextProperty.value) {
                batteryStatusIndicator.icons = [ "icon-s-status-powersave-low", "icon-s-status-powersave1", "icon-s-status-powersave2", "icon-s-status-powersave3", "icon-s-status-powersave4", "icon-s-status-powersave5", "icon-s-status-powersave6", "icon-s-status-powersave7", "icon-s-status-powersave8" ]
            } else {
                batteryStatusIndicator.icons = [ "icon-s-status-battery-low", "icon-s-status-battery1", "icon-s-status-battery2", "icon-s-status-battery3", "icon-s-status-battery4", "icon-s-status-battery5", "icon-s-status-battery6", "icon-s-status-battery7", "icon-s-status-battery8" ]
            }
        } else {
            if (systemPowerSaveModeContextProperty.value) {
                batteryStatusIndicator.icons = [ "icon-s-status-powersave-verylow", "icon-s-status-powersave1", "icon-s-status-powersave2", "icon-s-status-powersave3", "icon-s-status-powersave4", "icon-s-status-powersave5", "icon-s-status-powersave6", "icon-s-status-powersave7", "icon-s-status-powersave8" ]
            } else {
                batteryStatusIndicator.icons = [ "icon-s-status-battery-verylow", "icon-s-status-battery1", "icon-s-status-battery2", "icon-s-status-battery3", "icon-s-status-battery4", "icon-s-status-battery5", "icon-s-status-battery6", "icon-s-status-battery7", "icon-s-status-battery8" ]
            }
        }
        batteryStatusIndicator.animate = batteryIsChargingContextProperty.value
        batteryStatusIndicator.setFirstIconIndex();
    }

    function setFirstIconIndex() {
        // Calculate the image to use for the current charge percentage
        var images = batteryStatusIndicator.icons.length
        var image = Math.floor(images * batteryChargePercentageContextProperty.value / 100)
        if (image >= images) {
            image = images - 1
        }

        // While charging always animate at least one bar
        batteryStatusIndicator.firstIconIndex = (batteryIsChargingContextProperty.value && image >= images - 1) ? (image - 1) : image

        if (!batteryStatusIndicator.animate) {
            // When not animating set the current icon immediately
            batteryStatusIndicator.currentIconIndex = batteryStatusIndicator.firstIconIndex
        }
    }

    Timer {
        id: batteryStatusIndicatorTimer
        interval: 1000
        running: batteryStatusIndicator.animate && !LipstickSettings.lockscreenVisible
        repeat: true
        onTriggered: {
            // Show the next icon or loop to the first one
            batteryStatusIndicator.currentIconIndex = batteryStatusIndicator.currentIconIndex < batteryStatusIndicator.icons.length - 1 ? (batteryStatusIndicator.currentIconIndex + 1) : batteryStatusIndicator.firstIconIndex
        }
    }

    Component.onCompleted: {
        batteryStatusIndicator.setMode()
        batteryStatusIndicator.setFirstIconIndex()
    }
}
