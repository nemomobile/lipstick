/*
 * SwitcherItem.qml
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
import QtQuick 1.1
import Pyro 0.1

Item {
    id: switcherItemRoot

    Text {
        id:titleText
        anchors {
            left: parent.left
            right: closeButton.left
        }
        color: 'white'
        smooth: true
        font.pixelSize: 18
        text: model.name
        elide: Text.ElideRight
    }

    Item {
        anchors {
            top: titleText.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            topMargin: 3
        }

        WindowPixmap {
            id: windowPixmap
            width: desktop.isPortrait ? parent.height : parent.width
            height: desktop.isPortrait ? parent.width : parent.height
            windowId: model.windowId
            transform: Rotation {
                angle: desktop.isPortrait ? 90 : 0
                origin.x: windowPixmap.height / 2
                origin.y: windowPixmap.width / 2
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: switcherModel.windowToFront(model.windowId)
    }

    Item {
        id: closeButton
        width: 30
        height: width
        anchors {
            top: parent.top
            right: parent.right
        }

        Rectangle {
            anchors.centerIn: parent
            color: 'red'
            width: 40
            height: 8
            transform: Rotation {
                angle: 45
                origin.x: 20
                origin.y: 4
            }
        }
        Rectangle {
            anchors.centerIn: parent
            color: 'red'
            width: 40
            height: 8
            transform: Rotation {
                angle: -45
                origin.x: 20
                origin.y: 4
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: switcherModel.closeWindow(model.windowId)
        }
    }
}
