import QtQuick 1.1
import org.freedesktop.contextkit 1.0

Item {
    id: batteryStatusIndicator
    property alias icon: batteryStatusIndicatorImage.source
    property alias text: batteryStatusIndicatorText.text

    opacity: LipstickSettings.lockscreenVisible ? 0 : 1
    Behavior on opacity {
        NumberAnimation { duration: 500 }
    }

    ContextProperty {
        id: batteryChargePercentageContextProperty
        key: "Battery.ChargePercentage"
        onValueChanged: batteryStatusIndicator.setIcon()
    }

    ContextProperty {
        id: batteryIsChargingContextProperty
        key: "Battery.IsCharging"
        onValueChanged: batteryStatusIndicator.setIcon()
    }

    ContextProperty {
        id: systemPowerSaveModeContextProperty
        key: "System.PowerSaveMode"
        onValueChanged: batteryStatusIndicator.setIcon()
    }

    Image {
        id: batteryStatusIndicatorImage
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 9
        source: "images/icon-status-battery.png"
    }

    Text {
        id: batteryStatusIndicatorText
        anchors.left: batteryStatusIndicatorImage.right
        anchors.bottom: parent.bottom
        text: "" + (0 + batteryChargePercentageContextProperty.value)
        color: "white"
        font.family: headerFont.name
        font.pixelSize: 46
    }

    Text {
        id: batteryStatusIndicatorPercent
        anchors.left: batteryStatusIndicatorText.right
        anchors.leftMargin: 7
        anchors.bottom: parent.bottom
        text: "%"
        color: "white"
        font.family: headerFont.name
        font.pixelSize: 33
    }

    function setIcon() {
        if (batteryIsChargingContextProperty.value) {
            batteryStatusIndicatorImage.source = "images/icon-status-charge.png"
        } else {
            if (batteryChargePercentageContextProperty.value < 5) {
                batteryStatusIndicatorImage.source = "images/icon-status-battery-empty.png"
            } else {
                batteryStatusIndicatorImage.source = systemPowerSaveModeContextProperty.value ? "images/icon-status-battery-powersave.png" : "images/icon-status-battery.png"
            }
        }
    }

    Connections {
        target: LipstickSettings;
        onLockscreenVisibleChanged: {
            if (LipstickSettings.lockscreenVisible) {
                batteryChargePercentageContextProperty.unsubscribe()
                batteryIsChargingContextProperty.unsubscribe()
                systemPowerSaveModeContextProperty.unsubscribe()
            } else {
                batteryChargePercentageContextProperty.subscribe()
                batteryIsChargingContextProperty.subscribe()
                systemPowerSaveModeContextProperty.subscribe()
            }
        }
    }

    Component.onCompleted: {
        batteryChargePercentageContextProperty.subscribe()
        batteryIsChargingContextProperty.subscribe()
        systemPowerSaveModeContextProperty.subscribe()
        batteryStatusIndicator.setIcon()
    }
}
