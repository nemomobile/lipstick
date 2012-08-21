
/*
 * Switcher.qml
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

Item {
    property int columnNumber: 3
    id: switcherRoot
    height: gridview.contentHeight

    GridView {
        id: gridview
        width: cellWidth * columnNumber
        cellWidth: (parent.width - 60) / columnNumber
        cellHeight: cellWidth * (desktop.height / desktop.width) + 20
        interactive: false

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

        delegate: Item {
            width: gridview.cellWidth
            height: gridview.cellHeight

            SwitcherItem {
                width: parent.width - 10
                height: parent.height - 10
                anchors.centerIn: parent
            }
        }
    }
}
