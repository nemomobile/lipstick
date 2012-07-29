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
import QtQuick 1.1
import Pyro 0.1
import QtMobility.sensors 1.2

Item {
    id: main
    width: initialSize.width
    height: initialSize.height

    OrientationSensor {
        id: orientation
        active: true

        onReadingChanged: {
            console.log("orienation change");
            console.log(reading.orientation);

            if (reading.orientation === OrientationReading.TopUp) {
                // The top of the device is upwards - meaning: portrait
                desktopRotation.angle = -90;
                desktopRotation.origin.x = main.height / 2;
                desktopRotation.origin.y = main.height / 2;
                desktop.width = main.height;
                desktop.height = main.width;
                systemStatusBar.isPortrait = true;
            }
            else if (reading.orientation === OrientationReading.RightUp) {
                // The right side of the device is upwards - meaning: landscape
                desktopRotation.angle = 0;
                desktopRotation.origin.x = 0;
                desktopRotation.origin.y = 0;
                desktop.width = main.width;
                desktop.height = main.height;
                systemStatusBar.isPortrait = false;
            }
        }
    }

    Item {
        id: desktop
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: parent.height
        transform: Rotation {
            id: desktopRotation;
            origin.x: 0;
            origin.y: 0;
            angle: 0
        }

        Image {
            id:background
            anchors.fill:parent
            source:'file:///usr/share/themes/meego/meegotouch/images/MeeGo-People-landscape.png'
        }
        Rectangle {
            id:overlay
            anchors.fill:parent
            opacity:0.6
            color:'black'
        }
        StatusBar {
            id: systemStatusBar
            anchors.top: parent.top
            z: 100
        }
        ListView {
            id:dashboard

            anchors.top: systemStatusBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            snapMode:ListView.SnapOneItem
            orientation:ListView.Horizontal
            boundsBehavior:Flickable.DragOverBounds

            model:VisualItemModel {
                Launcher {
                    id: launcher
                    width: desktop.width
                    height: desktop.height
                }
                Switcher {
                    id: switcher
                    width: desktop.width
                    height: desktop.height
                }
            }
        }
    }
}
