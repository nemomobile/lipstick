/*
 * Switcher.qml
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
    property int columnNumber: 3

    id: switcherRoot
    clip: true

    GridView {
        id: gridview
        width: parent.width * 0.95
        cellWidth: desktop.width / columnNumber - 1
        cellHeight: cellWidth * (desktop.height / desktop.width)
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 35
            bottomMargin: 35
        }

        model: SwitcherModel {
            id:switcherModel
        }

        delegate: SwitcherItem {
            width: gridview.cellWidth - 10
            height: gridview.cellHeight - 10
            anchors.margins: 5
        }
    }
    Text {
        // Empty switcher indicator
        anchors.centerIn: parent
        visible: switcherModel.itemCount === 0
        text: "No apps open"
        color: "white"
        font.pixelSize: 30
    }
}
