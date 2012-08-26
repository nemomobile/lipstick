import QtQuick 1.1

Rectangle {
    id: lockScreen
    color: "red"

    states: [
        State {
            name: "locked"
            when: LipstickSettings.lockscreenVisible
            PropertyChanges {
                target: lockScreen
                y: 0
            }
        },
        State {
            name: "unlocked"
            when: !LipstickSettings.lockscreenVisible
            PropertyChanges {
                target: lockScreen
                y: -lockScreen.height
            }
        }
    ]
    transitions: Transition {
        PropertyAnimation {
            properties: "y"
            easing.type: Easing.OutBounce
            duration: 400
        }
    }

    MouseArea {
        property int pressY: 0
        anchors.fill: parent

        onPressed: pressY = mouseY
        onPositionChanged: {
            var delta = pressY - mouseY
            pressY = mouseY + delta
            if (parent.y - delta > 0)
                return
            parent.y = parent.y - delta
        }
        onReleased: {
            if (Math.abs(parent.y) > parent.height / 3) {
                console.log("Going away")
    
                LipstickSettings.lockscreenVisible = false
            } else if (LipstickSettings.lockscreenVisible) {
                console.log("No dice")
                parent.state = "unlocked"
                parent.state = "locked"
            }
        }
    }

    Text {
        id: time
        text: Qt.formatDateTime(new Date(), "hh:mm")
        font.pixelSize: 130
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        width: parent.width
    }

    Text {
        horizontalAlignment: Text.AlignHCenter
        anchors.top: time.bottom
        text: Qt.formatDateTime(new Date(), "dd/MM/yyyy")
        color: "white"
        font.pixelSize: 50
        width: parent.width
    }
}

