/*******************************************************************************
 *  Copyright 2012-2022 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/
import QtQuick 2.15
import QtQuick.Templates 2.15 as T
import QtGraphicalEffects 1.12

T.SpinBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentItem.implicitWidth +
                            up.implicitIndicatorWidth +
                            down.implicitIndicatorWidth)
    implicitHeight: Math.max(implicitContentHeight + topPadding + bottomPadding,
                             implicitBackgroundHeight,
                             up.implicitIndicatorHeight,
                             down.implicitIndicatorHeight)
    topPadding: 5
    bottomPadding: 5

    spacing: 6
//    topPadding: 16
//    bottomPadding: 16
    leftPadding: (control.mirrored ? (up.indicator ? up.indicator.width : 0) : (down.indicator ? down.indicator.width : 0))
    rightPadding: (control.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))

    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    contentItem: TextInput {
        id: textInput
        text: control.displayText

        font: control.font
        //color: enabled ? Calcite.text3 : Calcite.text2
        color : Calcite.text1
        selectionColor: Calcite.brandHover
        selectedTextColor: Calcite.foreground1
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        //cursorDelegate: CursorDelegate { }

        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints
    }

    up.indicator: Item {
        x: control.mirrored ? 0 : parent.width - width
        implicitWidth: 40
        implicitHeight: 40
        height: parent.height
        width: height
        Rectangle {
            //y: parent.y + parent.height - height - control.bottomPadding / 2
            width: parent.width
            height: parent.width - bottomPadding * 2
            y : bottomPadding
            color: enabled ? (up.hovered ? Calcite.foreground2 : Calcite.foreground1) : Calcite.background
            border {
                color: Calcite.border1
            }

//        Ripple {
//            clipRadius: 2
//            x: control.spacing
//            y: control.spacing
//            width: parent.width - 2 * control.spacing
//            height: parent.height - 2 * control.spacing
//            pressed: control.up.pressed
//            active: control.up.pressed || control.up.hovered || control.visualFocus
//            color: control.Material.rippleColor
//        }
            Image {
                id: upIndicatorImage
                fillMode: Image.PreserveAspectFit
                anchors {
                    fill: parent
                }
                source: "images/chevron-right.svg"
                Component.onCompleted: {console.log(parent.width, parent.height);}
            }

            ColorOverlay {
                anchors.fill : upIndicatorImage
                source: upIndicatorImage
                color: up.hovered ? Calcite.text1 : Calcite.text3
            }
        }
    }


    down.indicator: Item {
        x: control.mirrored ? parent.width - width : 0
        implicitWidth: 40
        implicitHeight: 40
        height: parent.height
        width: height

        Rectangle {
            width: parent.width
            height: parent.height - bottomPadding * 2
            y : bottomPadding
            color: enabled ? (down.hovered ? Calcite.foreground2 : Calcite.foreground1) : Calcite.background
            border {
                color: Calcite.border1
            }

            Image {
                id: downIndicatorImage
                fillMode: Image.PreserveAspectFit
                anchors {
                    fill: parent
                }

                source: "images/chevron-left.svg"
                Component.onCompleted: {console.log(parent.width, parent.height);}
            }

            ColorOverlay {
                anchors.fill : downIndicatorImage
                source: downIndicatorImage
                color: down.hovered ? Calcite.text1 : Calcite.text3
            }

//        Ripple {
//            clipRadius: 2
//            x: control.spacing
//            y: control.spacing
//            width: parent.width - 2 * control.spacing
//            height: parent.height - 2 * control.spacing
//            pressed: control.down.pressed
//            active: control.down.pressed || control.down.hovered || control.visualFocus
//            color: control.Material.rippleColor
//        }
        }
    }

    background: Item {
        implicitWidth: 192
        implicitHeight: 40
        //uncomment to see the component real size (no padding on it)
//        Rectangle {
//            width: parent.width
//            height: parent.height

//            border {
//                color : "black"
//            }
//        }

        Rectangle {
            id: backgroundRect
            x: parent.width / 2 - width / 2
            //y: parent.y + parent.height - height - control.bottomPadding / 2
            y : bottomPadding
            width: control.availableWidth
            height: parent.height - control.bottomPadding * 2
            color: enabled ? Calcite.foreground1 : Calcite.background
            border {
                //not sure when an error occurs in a spinbox, acceptable input cannot be used
                //color: !control.acceptableInput ? (control.activeFocus ? Calcite.dangerHover : Calcite.danger) : (control.activeFocus ? Calcite.brand : Calcite.borderInput)
                color: control.activeFocus ? Calcite.brand : Calcite.borderInput
            }
        }
    }
}
