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

        Launcher {
        }

        Switcher {
        }
    }
}
