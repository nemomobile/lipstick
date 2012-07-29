/*
 * Launcher.qml
 *
 * Copyright (c) 2011 - Tom Swindell <t.swindell@rubyx.co.uk>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
import QtQuick 1.0
import Pyro 0.1

Item {
    id: launcherRoot
    clip: true

    property alias cellWidth: gridview.cellWidth

    GridView {
        id: gridview
        width: Math.floor(parent.width / cellWidth) * cellWidth
        cellWidth: 80 + 60
        cellHeight: cellWidth
        anchors {
            top: parent.top;
            bottom: parent.bottom;
            horizontalCenter: parent.horizontalCenter;
            topMargin: 20
            bottomMargin: 20
        }

        model: MenuModel { }

        delegate: Item {
            width: gridview.cellWidth
            height: gridview.cellHeight

            Image {
                id:icon
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                    margins: 8
                }
                width: 80
                height: width
                source: model.icon
            }
            Text {
                width: parent.width - 10
                anchors {
                    bottom: parent.bottom
                    horizontalCenter: parent.horizontalCenter
                    margins: 30
                }
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
                font.pixelSize: 18
                color: 'white'
                text: model.name
            }
            MouseArea {
                anchors.fill: parent
                onClicked: model.object.launch();
            }
        }
    }
}
