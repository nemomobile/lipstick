
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

GridView {
    id: switcherRoot
    property int columns: 1
    height: contentHeight
    interactive: false

    cellWidth: width / columns
    cellHeight: Math.floor(height / Math.ceil(count / columns))
    property int displayWidth: 1
    property int displayHeight: 1

	onCellWidthChanged: updateRatio(cellWidth - 16, cellHeight - 16)
	onCellHeightChanged: updateRatio(cellWidth - 16, cellHeight - 16)

    function updateRatio(cw, ch) {
        var nw = ch * (desktop.width / desktop.height)
        var nh = cw * (desktop.height / desktop.width)
        if (nw < cw) {
            displayWidth = nw
            displayHeight = ch
        } else {
            displayWidth = cw
            displayHeight = nh
        }
    }

    model: SwitcherModel {
        id:switcherModel
        onItemCountChanged: {
            if (itemCount == 1)
                switcherRoot.columns = 1
            else if (itemCount <= 4)
                switcherRoot.columns = 2
            else if (itemCount > 4)
                switcherRoot.columns = 3
        }
    }

    delegate: Item {
        width: switcherRoot.cellWidth
        height: switcherRoot.cellHeight

        SwitcherItem {
			width: displayWidth
			height: displayHeight
            anchors.centerIn: parent
        }
    }
}
