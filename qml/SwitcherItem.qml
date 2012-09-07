
/*
 * SwitcherItem.qml
 *
 * Copyright (c) 2011 - Tom Swindell <t.swindell@rubyx.co.uk>
 * Copyright (c) 2012 - Timur Kristóf <venemo@fedoraproject.org>
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
import org.nemomobile.lipstick 0.1

MouseArea {
    id: switcherItemRoot
    onClicked: windowManager.windowToFront(model.object.window)
    onPressAndHold: closeButton.enabled = !closeButton.enabled

    SwitcherPixmapItem {
        id: windowPixmap
        opacity: 0.5
        width: desktop.isPortrait ? parent.height : parent.width
        height: desktop.isPortrait ? parent.width : parent.height
        windowId: model.object.window
        transform: Rotation {
            angle: desktop.isPortrait ? 90 : 0
            origin.x: windowPixmap.height / 2
            origin.y: windowPixmap.height / 2
        }
    }

    MouseArea {
        id: closeButton
        enabled: false
        width: 30
        height: width
        anchors {
            top: parent.top
            right: parent.right
        }

        Rectangle {
            visible: closeButton.enabled
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
            visible: closeButton.enabled
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

        onClicked: windowManager.closeWindow(model.object.window)
    }
}
