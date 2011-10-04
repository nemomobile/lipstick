/*
 * SystemStatusBar.qml
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

Item {
    id:root
    width:parent.width;height:42

    Rectangle {id:background;width:parent.width;height:parent.height - 10;color:'black'}

    Rectangle {
        id:shadow
        width:parent.width;height:10
        anchors.bottom:parent.bottom
        gradient: Gradient {
            GradientStop {position:0.0;color:'#8f000000'}
            GradientStop {position:1.0;color:'#00000000'}
        }
    }

    Text {
      anchors {left:parent.left;verticalCenter:background.verticalCenter;margins:10}
      color:'white'
      font.pixelSize:18
      text:'Vodafone UK'
    }

    Text {
      anchors {right:parent.right;verticalCenter:background.verticalCenter;margins:10}
      color:'white'
      font.pixelSize:18
      text:'00:00 <sub>PM</sub>'
    }
}
