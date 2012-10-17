
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

// wrapper Item needed because housekeeping graphic and switcher need to both be displayed
// possibly with different opacity.
Item {
    Image {
        source: "images/graphic-housekeeping-mode.png"
        opacity: desktop.closeApplicationEnabled ? 1.0 : 0.0
        Behavior on opacity {
            NumberAnimation {
            }
        }
    }

    Grid {
        id: switcherRoot
        anchors.fill: parent
        anchors.margins: 32

        spacing: 22
        property int windowWidth: columns == 2 ? 197 : 124
        property int windowHeight: columns == 2 ? 296 : 190

        Repeater {
            model: SwitcherModel {
                id:switcherModel
            }

            delegate: SwitcherItem {
                width: switcherRoot.windowWidth
                height: switcherRoot.windowHeight
            }

            onCountChanged: {
                if (count == 0 && desktop.closeApplicationEnabled)
                    desktop.closeApplicationEnabled = false

                if (count <= 4)
                    switcherRoot.columns = 2
                else if (count > 4)
                    switcherRoot.columns = 3
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        z: -1

        onPressAndHold: {
            // don't enter housekeeping mode with no windows open
            if (switcherRoot.count == 0)
                return

            desktop.closeApplicationEnabled = !desktop.closeApplicationEnabled
        }

        onPressed: {
            if (desktop.closeApplicationEnabled)
                desktop.closeApplicationEnabled = false;
        }
    }
}
