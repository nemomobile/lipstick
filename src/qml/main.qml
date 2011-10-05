/*
 * main.qml
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

Item {
    id:main

    width:800;height:480

    Image {
        id:background
        anchors.fill:parent
        source:'/usr/share/themes/meego/meegotouch/images/MeeGo-People-landscape.png'
    }

    Rectangle {
        id:overlay
        anchors.fill:parent
        opacity:0.6
        color:'black'
    }

    SystemStatusBar {id:systemStatusBar;z:100}

    ListView {
        id:dashboard

        width:parent.width;height:parent.height - systemStatusBar.height
        anchors.top: systemStatusBar.bottom

        snapMode:ListView.SnapOneItem
        orientation:ListView.Horizontal
        boundsBehavior:Flickable.DragOverBounds

        model:VisualItemModel {
            Launcher {id:launcher;width:dashboard.width;height:dashboard.height}
            Switcher {id:switcher;width:dashboard.width;height:dashboard.height}
        }
    }
}
