
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
    property bool ignoreClicked: false

    onPressed: {
         if (desktop.closeApplicationEnabled) {
             desktop.closeApplicationEnabled = false;
             ignoreClicked = true;
         }
    }

    onClicked: {
        if (!desktop.closeApplicationEnabled && !ignoreClicked)
            windowManager.windowToFront(model.object.window);
        if (ignoreClicked)
            ignoreClicked = false;
    }

    onPressAndHold: desktop.closeApplicationEnabled = !desktop.closeApplicationEnabled

    SwitcherPixmapItem {
        id: windowPixmap
        opacity: 0.8
        width: desktop.isPortrait ? parent.height : parent.width
        height: desktop.isPortrait ? parent.width : parent.height
        windowId: model.object.window
        radius: 8
        smooth: true
        transform: Rotation {
            angle: desktop.isPortrait ? 90 : 0
            origin.x: windowPixmap.height / 2
            origin.y: windowPixmap.height / 2
        }
    }

    Image {
        source: "images/icon-close-app.png"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: -(height / 2)
        visible: desktop.closeApplicationEnabled

        MouseArea {
            anchors.fill: parent
            enabled: desktop.closeApplicationEnabled
            onClicked: windowManager.closeWindow(model.object.window)
        }
    }
}
