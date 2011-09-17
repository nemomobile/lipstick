import Pyro 0.1
import QtQuick 1.1

Item {
    id: root
    width: 500
    height: 500

    ListView {
        anchors.fill: parent
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        model: viewsModel
    }

    VisualItemModel {
        id: viewsModel
        GridView {
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

        GridView {
            id: switcherView
            height: root.height
            width: root.width
            cellHeight: 190
            cellWidth: 300
            model: SwitcherModel {
                id: switcherModel
            }

            delegate: Rectangle {
                id: switcherCell
                height: switcherView.cellHeight
                width: switcherView.cellWidth
                color: "red"

                Image {
                    id: launcherIcon
                    source: "image://windows/" + model.windowId + "/" + Math.random()
                    cache: false
                    anchors.centerIn: parent
                    height: 180
                    width: 280
                    sourceSize.height: 180
                    sourceSize.width: 280
                    asynchronous: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            switcherModel.windowToFront(model.windowId)
                        }
                    }
                }
                Text {
                    id: launcherLabel
                    text: model.name
                    horizontalAlignment: Text.AlignHCenter
                    anchors.top: launcherIcon.bottom
                    anchors.topMargin: 10
                    anchors.horizontalCenter: launcherIcon.horizontalCenter
                    width: launcherIcon.width - 20
                    elide: Text.ElideRight
                }

                Rectangle
                {

                    anchors.top: switcherCell.top
                    anchors.right: switcherCell.right
                    width: 80
                    height: 60
                    color: "red"

                    Text { 
                        anchors.fill: parent
                        font.pixelSize: 50
                        text: "X"
                        color: "blue"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                switcherModel.closeWindow(model.windowId)
                            }
                        }
                    }
                }

            }
        }
    }
}
