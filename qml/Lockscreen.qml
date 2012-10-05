import QtQuick 1.1
import org.nemomobile.lipstick 0.1
import com.jolla.components 1.0

Item {
    property alias clockRunning: clock.running

    Connections {
        target: LipstickSettings

        // TODO: turn clock off when screen turns off to save power
        onLockscreenVisibleChanged: {
            if (!LipstickSettings.lockscreenVisible) {
                if (dashboard.currentPage == 0) {
                    // lockscreen visible, but we don't want to see it.
                    // reset view to switcher/launcher
                    dashboard.currentPage = 1
                    dashboard.snapToCurrentPage();
                    stopClock()
                }
                return
            } else {
                // lockscreen enabled. make sure we're already there.
                startClock()
                dashboard.currentPage = 0
                dashboard.snapToCurrentPage();
            }
        }
    }

    Clock {
        id: clock
        y: LipstickSettings.lockscreenVisible ? (parent.height / 2 - height / 2) : parent.height - height

        /* Hide the clock when the notification text area is visible */
        opacity: notificationArea.contentX <= 0 ? 1 : 0
        Behavior on opacity {
            NumberAnimation { duration: 250 }
        }

        Behavior on y {
            SpringAnimation {
                spring: 3
                damping: 0.2
                mass: 0.3
            }
        }
    }

    function startClock() {
        console.log("Starting clock")
        clock.start()
    }

    function stopClock() {
        console.log("Stopping clock")
        clock.stop()
    }

    Flickable {
        property int notificationHeight: 100
        property int notificationMargin: 24
        property int notificationIconSize: 60
        id: notificationArea
        anchors.fill: parent

        /* Ensure notification icon visiblity by having the flickable area twice the lock screen's width minus notification icon size (including margins) */
        contentWidth: 2 * parent.width - notificationIconSize - 2 * notificationMargin

        /* Hide the notification text area when the lock screen appears */
        Connections {
            target: LipstickSettings;
            onLockscreenVisibleChanged: {
                if (LipstickSettings.lockscreenVisible) {
                    notificationArea.contentX = 0
                }
            }
        }

        /* Snap the notification area to either edge */
        SpringAnimation on contentX {
            id: xBehavior
            spring: 3
            damping: 0.2
            mass: 0.3
        }

        onMovementEnded: {
            xBehavior.stop()
            xBehavior.from = notificationArea.contentX
            xBehavior.to = notificationArea.contentX < notificationArea.width / 2 ? 0 : (notificationArea.contentWidth - notificationArea.width)
            xBehavior.start()
        }

        /* The dimming area to make the notification texts more visible */
        Rectangle {
            color: "black"
            anchors.fill: parent
            anchors.leftMargin: parent.parent.width
            anchors.rightMargin: -parent.parent.width
            opacity: LipstickSettings.lockscreenVisible ? 0.5 : 0
            Behavior on opacity {
                NumberAnimation { duration: 500 }
            }
        }

        /* The notification list contains notification icons and texts for them. Only the texts are tappable */
        ListView {
            property int listHeight: 0
            id: notificationList
            anchors.left: parent.left
            anchors.top: parent.top

            /* Begin the list from the right edge of the screen minus notification icon size (including margins) */
            anchors.leftMargin: parent.parent.width - notificationArea.notificationIconSize - 2 * notificationArea.notificationMargin

            /* Center the list vertically. Only the top margin needs to be set; setting the bottom margin would change the height of the list which would break animations. */
            anchors.topMargin: (parent.height - listHeight) / 2

            /* List width is the lock screen's width */
            width: parent.parent.width

            /* List height needs to be static so that the animations work. Set it to lock screen height plus notification height so that delegates don't get removed when panning them out of the screen. */
            height: parent.parent.height + notificationArea.notificationHeight

            opacity: LipstickSettings.lockscreenVisible ? 1 : 0
            Behavior on opacity {
                NumberAnimation { duration: 500 }
            }

            model: NotificationListModel { }

            Component.onCompleted: listHeight = notificationArea.notificationHeight * notificationList.count

            delegate: Item {
                id: delegateItem
                width: parent.width
                height: notificationArea.notificationHeight

                Image {
                    id: icon
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: notificationArea.notificationMargin
                    anchors.topMargin: notificationArea.notificationMargin + 6
                    width: object.icon ? notificationArea.notificationIconSize : 0
                    height: width
                    source: object.icon ? "image://theme/" + object.icon : ""
                }

                MouseArea {
                    anchors.top: parent.top
                    anchors.left: icon.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: notificationArea.notificationMargin
                    anchors.leftMargin: 2 * notificationArea.notificationMargin

                    onClicked: {
                        object.actionInvoked("clicked")
                    }

                    Text {
                        id: summary
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        text: object.summary
                        color: "white"
                        font.family: headerFont.name
                        font.pixelSize: 28
                        clip: true
                        elide: Text.ElideRight
                    }

                    Text {
                        id: timestamp
                        anchors.top: summary.bottom
                        anchors.right: parent.right
                        anchors.topMargin: 6
                        text: object.localizedTimestamp
                        color: "gray"
                        font.family: headerFont.name
                        font.pixelSize: 28
                    }

                    Text {
                        id: body
                        anchors.top: summary.bottom
                        anchors.left: parent.left
                        anchors.right: timestamp.right
                        anchors.topMargin: 6
                        text: object.body
                        color: "white"
                        font.family: headerFont.name
                        font.pixelSize: 28
                        clip: true
                        elide: Text.ElideRight
                    }
                }

                ListView.onAdd: SequentialAnimation {
                    PropertyAction {
                        target: delegateItem
                        property: "scale"
                        value: 0
                    }
                    PropertyAction {
                        target: delegateItem
                        property: "height"
                        value: 0
                    }
                    ParallelAnimation {
                        NumberAnimation {
                            target: delegateItem
                            property: "scale"
                            to: 1
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                        NumberAnimation {
                            target: delegateItem
                            property: "height"
                            to: notificationArea.notificationHeight
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                        NumberAnimation {
                            target: notificationList
                            property: "listHeight"
                            to: notificationArea.notificationHeight * notificationList.count
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                    }
                }

                ListView.onRemove: SequentialAnimation {
                    PropertyAction {
                        target: delegateItem
                        property: "ListView.delayRemove"
                        value: true
                    }
                    ParallelAnimation {
                        NumberAnimation {
                            target: delegateItem
                            property: "scale"
                            to: 0
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                        NumberAnimation {
                            target: delegateItem
                            property: "height"
                            to: 0
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                        NumberAnimation {
                            target: notificationList
                            property: "listHeight"
                            to: notificationArea.notificationHeight * (notificationList.count - 1)
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                    }
                    PropertyAction {
                        target: delegateItem
                        property: "ListView.delayRemove"
                        value: false
                    }
                }
            }
        }
    }
}
