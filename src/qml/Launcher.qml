import Pyro 0.1
import QtQuick 1.1

GridView {
    id: launcherRoot
    height: root.height
    width: root.width
    cellWidth: 130
    cellHeight: 140
    model: MenuModel {
    }

    delegate: Item {
        width: launcherRoot.cellWidth
        height: launcherRoot.cellHeight

        Image {
            id: launcherIcon
            source: model.icon
            anchors.centerIn: parent
            height: 100
            width: 100
            sourceSize.height: 100
            sourceSize.width: 100
            asynchronous: true
        }

        Text {
            text: model.name
            horizontalAlignment: Text.AlignHCenter
            anchors.top: launcherIcon.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: launcherIcon.horizontalCenter
            width: 100
            elide: Text.ElideRight
        }

        MouseArea {
            anchors.fill: parent
            onClicked: model.object.launch()
        }
    }
}

