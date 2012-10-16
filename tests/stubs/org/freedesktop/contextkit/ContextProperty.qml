import QtQuick 1.1
import "ContextProperty.js" as ContextPropertyStub

Item {
    id: contextProperty
    property variant value: undefined
    property string key

    function updateValue() {
        switch (key) {
        case "Battery.ChargePercentage":
            value = ContextPropertyStub.batteryChargePercentage
            break;
        case "Battery.IsCharging":
            value = ContextPropertyStub.batteryIsCharging
            break;
        case "System.PowerSaveMode":
            value = ContextPropertyStub.systemPowerSaveMode
            break;
        }
    }

    onKeyChanged: updateValue()
    Component.onCompleted: ContextPropertyStub.addContextProperty(contextProperty)
}
