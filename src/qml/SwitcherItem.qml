/*
 * SwitcherItem.qml
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

Item {
    id:root

    Rectangle {
        id:titleBackground
        width:parent.width;height:30
        gradient: Gradient {
            GradientStop {position:0.0;color:'#1f2f2f2f'}
            GradientStop {position:1.0;color:'#ff2f2f2f'}
        }
    }

    Text {
        id:titleText
        anchors {
            left:titleBackground.left
            leftMargin:10
            right:closeButton.left
            verticalCenter:titleBackground.verticalCenter
        }
        color:'white'
        smooth:true
        font.pixelSize:18
        text:model.name
        elide:Text.ElideRight
    }

    Rectangle {
        id:preview
        anchors {bottom:parent.bottom}
        width:parent.width;height:parent.height - titleBackground.height + 1
        color:'#1f1f1f'
    }

    WindowPixmap {
        anchors.fill:preview
        windowId: model.windowId
    }

    MouseArea {
        anchors.fill:parent
        onClicked:switcherModel.windowToFront(model.windowId);
    }

    Rectangle {
        id:closeButton
        width:30;height:width
        color:'red'
        anchors {top:titleBackground.top;right:titleBackground.right}

        MouseArea {
            anchors.fill:parent
            onClicked:switcherModel.closeWindow(model.windowId)
        }
    }
}
