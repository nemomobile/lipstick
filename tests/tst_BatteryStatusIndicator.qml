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
                { tag: "Battery is not charging, power save mode disabled", batteryIsCharging: false, systemPowerSaveMode: false, icons: [ "icon-s-status-battery-verylow", "icon-s-status-battery1", "icon-s-status-battery2", "icon-s-status-battery3", "icon-s-status-battery4", "icon-s-status-battery5", "icon-s-status-battery6", "icon-s-status-battery7", "icon-s-status-battery8" ], animate: false },
                { tag: "Battery is not charging, power save mode enabled", batteryIsCharging: false, systemPowerSaveMode: true, icons: [ "icon-s-status-powersave-verylow", "icon-s-status-powersave1", "icon-s-status-powersave2", "icon-s-status-powersave3", "icon-s-status-powersave4", "icon-s-status-powersave5", "icon-s-status-powersave6", "icon-s-status-powersave7", "icon-s-status-powersave8" ], animate: false },
                { tag: "Battery is charging, power save mode disabled", batteryIsCharging: true, systemPowerSaveMode: false, icons: [ "icon-s-status-battery-low", "icon-s-status-battery1", "icon-s-status-battery2", "icon-s-status-battery3", "icon-s-status-battery4", "icon-s-status-battery5", "icon-s-status-battery6", "icon-s-status-battery7", "icon-s-status-battery8" ], animate: true },
                { tag: "Battery is charging, power save mode enabled", batteryIsCharging: true, systemPowerSaveMode: true, icons: [ "icon-s-status-powersave-low", "icon-s-status-powersave1", "icon-s-status-powersave2", "icon-s-status-powersave3", "icon-s-status-powersave4", "icon-s-status-powersave5", "icon-s-status-powersave6", "icon-s-status-powersave7", "icon-s-status-powersave8" ], animate: true },
            ]
        }

        function test_setMode(data)
        {
            ContextPropertyStub.setBatteryIsCharging(data.batteryIsCharging)
            ContextPropertyStub.setSystemPowerSaveMode(data.systemPowerSaveMode)
            batteryStatusIndicator.setMode()
            compare(batteryStatusIndicator.icons, data.icons)
            compare(batteryStatusIndicator.animate, data.animate)
        }

        function test_setFirstIconIndex_data()
        {
            return [
                { tag: "Battery not charging, remaining equal to maximum", batteryChargePercentage: 100, batteryIsCharging: false, icons: [ "empty", "value1", "value2", "value3" ], icon: "value3" },
                { tag: "Battery not charging, remaining less than maximum", batteryChargePercentage: 50, batteryIsCharging: false, icons: [ "empty", "value1", "value2", "value3" ], icon: "value2" },
                { tag: "Battery not charging, remaining 0", batteryChargePercentage: 0, batteryIsCharging: false, icons: [ "empty", "value1", "value2", "value3" ], icon: "empty" },
                { tag: "Battery charging, remaining equal to maximum", batteryChargePercentage: 100, batteryIsCharging: true, icons: [ "value0", "value1", "value2", "value3" ], icon: "value2" },
                { tag: "Battery charging, remaining less than maximum", batteryChargePercentage: 50, batteryIsCharging: true, icons: [ "value0", "value1", "value2", "value3" ], icon: "value2" },
                { tag: "Battery charging, remaining 0", batteryChargePercentage: 0, batteryIsCharging: true, icons: [ "value0", "value1", "value2", "value3" ], icon: "value0" },
            ]
        }

        function test_setFirstIconIndex(data)
        {
            ContextPropertyStub.setBatteryChargePercentage(data.batteryChargePercentage)
            ContextPropertyStub.setBatteryIsCharging(data.batteryIsCharging)
            batteryStatusIndicator.icons = data.icons
            batteryStatusIndicator.setFirstIconIndex()
            batteryStatusIndicator.currentIconIndex = batteryStatusIndicator.firstIconIndex
            compare(batteryStatusIndicator.source, Qt.resolvedUrl("images/" + data.icon + ".png"))
        }
    }
}
