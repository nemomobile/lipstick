import Pyro 0.1
import QtQuick 1.1

GridView {
    id: launcherRoot
    height: root.height
    width: root.width
    model: MenuModel {
    }

    delegate: Item {
        width: 100
        height: 110

        Image {
            id: launcherIcon
            source: model.icon
            anchors.centerIn: parent
            height: 64
            width: 64
            sourceSize.height: 64
            sourceSize.width: 64
            asynchronous: true
        }

        Text {
            text: model.name
            anchors.top: launcherIcon.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: launcherIcon.horizontalCenter
            width: 80
            elide: Text.ElideRight
        }

        MouseArea {
            anchors.fill: parent
            onClicked: model.object.launch()
        }
    }
}

