import QtQuickTest 1.0
import QtQuick 1.0
import org.freedesktop.contextkit 1.0
import "stubs/org/freedesktop/contextkit/ContextProperty.js" as ContextPropertyStub

Item {
    id: container
    width: 100
    height: 100

    Item {
        id: clock
    }

    BatteryStatusIndicator {
        id: batteryStatusIndicator
    }

    resources: TestCase {
        name: "BatteryStatusIndicator"
        when: windowShown

        function test_setMode_data()
        {
            return [
                { tag: "Battery is not empty, battery is not charging, power save mode disabled", batteryChargePercentage: 5, batteryIsCharging: false, systemPowerSaveMode: false, icon: "images/icon-status-battery.png" },
                { tag: "Battery is not empty, battery is not charging, power save mode enabled", batteryChargePercentage: 5, batteryIsCharging: false, systemPowerSaveMode: true, icon: "images/icon-status-battery-powersave.png" },
                { tag: "Battery is not empty, battery is charging, power save mode disabled", batteryChargePercentage: 5, batteryIsCharging: true, systemPowerSaveMode: false, icon: "images/icon-status-charge.png" },
                { tag: "Battery is not empty, battery is charging, power save mode enabled", batteryChargePercentage: 5, batteryIsCharging: true, systemPowerSaveMode: true, icon: "images/icon-status-charge.png" },
                { tag: "Battery is almost empty, battery is not charging, power save mode disabled", batteryChargePercentage: 4, batteryIsCharging: false, systemPowerSaveMode: false, icon: "images/icon-status-battery-empty.png" },
                { tag: "Battery is almost empty, battery is not charging, power save mode enabled", batteryChargePercentage: 4, batteryIsCharging: false, systemPowerSaveMode: true, icon: "images/icon-status-battery-empty.png" },
                { tag: "Battery is almost empty, battery is charging, power save mode disabled", batteryChargePercentage: 4, batteryIsCharging: true, systemPowerSaveMode: false, icon: "images/icon-status-charge.png" },
                { tag: "Battery is almost empty, battery is charging, power save mode enabled", batteryChargePercentage: 4, batteryIsCharging: true, systemPowerSaveMode: true, icon: "images/icon-status-charge.png" },
            ]
        }

        function test_setMode(data)
        {
            ContextPropertyStub.setBatteryChargePercentage(data.batteryChargePercentage)
            ContextPropertyStub.setBatteryIsCharging(data.batteryIsCharging)
            ContextPropertyStub.setSystemPowerSaveMode(data.systemPowerSaveMode)
            batteryStatusIndicator.setIcon()
            compare(batteryStatusIndicator.icon, Qt.resolvedUrl(data.icon))
            compare(batteryStatusIndicator.text, "" + data.batteryChargePercentage)
        }
    }
}
