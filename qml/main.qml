
/*
 * main.qml
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
import QtMobility.sensors 1.2
import org.nemomobile.lipstick 0.1

Item {
    property bool isPortrait: false;

    id: main
    width: initialSize.width
    height: initialSize.height

    OrientationSensor {
        id: orientation
        active: true

        onReadingChanged: {
            var orientationChanged = false;

            if (reading.orientation === OrientationReading.TopUp && !isPortrait) {
                // The top of the device is upwards - meaning: portrait
                isPortrait = true;
                desktopRotation.angle = -90;
                desktopRotation.origin.x = main.height / 2;
                desktopRotation.origin.y = main.height / 2;
                desktop.width = main.height;
                desktop.height = main.width;
                systemStatusBar.isPortrait = true;
                orientationChanged = true;
            }
            if (reading.orientation === OrientationReading.RightUp && isPortrait) {
                // The right side of the device is upwards - meaning: landscape
                isPortrait = false;
                desktopRotation.angle = 0;
                desktopRotation.origin.x = 0;
                desktopRotation.origin.y = 0;
                desktop.width = main.width;
                desktop.height = main.height;
                systemStatusBar.isPortrait = false;
                orientationChanged = true;
            }

            if (orientationChanged) {
                // TODO: don't set when mouse down
                dashboard.contentY = dashboard.height * dashboard.currentPage
            }
        }
    }

    Item {
        property bool isPortrait : height > width

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
            id: background
            source: 'images/jolla-full-wallpaper.jpeg'
            x: 0
            y: -(dashboard.contentY / 3)
        }

        Rectangle {
            color: "black"
            anchors.fill: parent
            opacity: (Math.abs(-dashboard.contentY) / dashboard.contentHeight)
        }

        Image {
            id: gradient
            source: "images/home-gradient-ramp.png"
            anchors.fill: parent
        }

        StatusBar {
            id: systemStatusBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            z: 100
        }

        Flickable {
            id: dashboard
            anchors.top: systemStatusBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            property int currentPage: 0
            property int maxPages: 0
            property int savedContentY: 0
            contentHeight: dashContent.height

            onContentHeightChanged: {
                dashboard.maxPages = (contentHeight / dashboard.height) - 1

                // TODO: we should recheck our page position and reset contentY if need be
            }

            SpringAnimation on contentY {
                id: yBehavior
                spring: 2
                damping: 0.2
            }

            onMovementStarted: {
                savedContentY = contentY
                yBehavior.stop()
            }

            onFlickStarted: {
                interactive = false
            }


            onFlickEnded: {
                interactive = true
                var delta = dashboard.contentY - dashboard.savedContentY
                delta = delta / dashboard.height
                
                if (delta > 0.10) {
                    // snap down
                    dashboard.currentPage = dashboard.currentPage + 1 
                } else if (delta < -0.10) {
                    // snap up
                    dashboard.currentPage = dashboard.currentPage - 1 
                }   

                // bounce back
                if (dashboard.currentPage < 0)
                    dashboard.currentPage = 0 
                else if (dashboard.currentPage > dashboard.maxPages)
                    dashboard.currentPage = dashboard.maxPages
                 
                var newY = dashboard.currentPage * dashboard.height
                
                yBehavior.from = dashboard.contentY
                yBehavior.to = newY
                yBehavior.start()
            }

            Column {
                id: dashContent
                anchors.left: parent.left
                anchors.right: parent.right

                Switcher {
                    id: switcher
                    width: parent.width
                    height: dashboard.height - launcher.cellWidth
                }

                Launcher {
                    id: launcher
                    width: parent.width
                }
            }
        }
    }
}
