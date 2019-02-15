/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

import QtQuick 2.11
import QtQuick.Controls 2.4

/*!
    \qmltype PopupStackView
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitCppApi
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.2
    \brief A view for displaying and editing information of Features.

    A PopupStackView can be used to display information for any type that
    implements the PopupSource interface. For example, FeatureLayer
    implements PopupSource. This means that it has a PopupDefinition,
    which defines how the Popup should look for any features in that
    layer.

    An example workflow for displaying a PopupStackView for features in a
    FeatureLayer would be:

    \list
      \li Declare a PopupStackView and anchor it to a desired location.
      \li Perform an identify operation on a GeoView and select the desired
      Features from the identify result.
      \li Create Popups from the Features.
      \li Optionally obtain the Popup's PopupDefinition and set the
      title, whether to show attachments, and so on.
      \li Create a PopupManager from the Popup and add it to a list of PopupManagers
      \li Assign the list mentioned in the above step to the PopupStackView's \c popupManagers property
      \li Call the \c show() method to display the PopupStackView.
      \li Call the \c dismiss() method to hide the PopupStackView.
    \endlist

    The PopupStackView is a QML Item that can be anchored, given to a dialog,
    or positioned using XY screen coordinates. Transform, Transition, and
    other QML animation types can be used to animate the showing and
    dismissing of the view.

    For more information, please see the Popup and PopupManager
    documentation.

    \note Each time a change is made to the Popup, PopupDefinition,
    PopupManager, or any of their properties, the popupManagers must be
    re-set to the PopupStackView.
*/
Item {
    id: popupStackView

    /*========================================
         Configurable properties
    ========================================*/

    /*!
        \brief A list of PopupManagers that controls the information being displayed in a PopupStackView.

        A PopupManager should be created from a Popup. See the example workflow that is
        highlighted in the description of this type.
    */
    property var popupManagers: null

    /*!
        \brief The background color of the PopupStackView.

        The default color is \c "#f2f3f4".
    */
    property color backgroundColor: "#f2f3f4"

    /*!
        \brief The border color of the PopupStackView.

        The default color is \c "#4f4f4f".
    */
    property color borderColor: "#4f4f4f"

    /*!
        \brief The border width of the PopupStackView.

        The default width is \c 2.
    */
    property real borderWidth: 2

    /*!
        \brief The radius of the PopupStackView.

        The default radius is \c 2.
    */
    property real radius: 2

    /*!
        \brief The title text color of the PopupStackView.

        The default color is \c "black".
    */
    property color titleTextColor: "black"

    /*!
        \brief The title text size of the PopupStackView.

        The default size is \c 13.
    */
    property real titleTextSize: 13

    /*!
        \brief The attribute name color of the PopupStackView.

        The default color is \c "gray".
    */
    property color attributeNameTextColor: "gray"

    /*!
        \brief The attribute value color of the PopupStackView.

        The default color is \c "4f4f4f".
    */
    property color attributeValueTextColor: "#4f4f4f"

    /*!
        \brief The color of the navigation buttons used to change the viewed Popup.

        The default color is \c "gray".
    */
    property color buttonColor: "gray"

    /*!
        \brief The animation duration for the slideHorizontal and slideVertical methods in milliseconds.

        The default duration is \c 250 milliseconds.
    */
    property real animationDuration: 250

    /*!
        \brief The animation easing type for the slideHorizontal and slideVertical methods.

        \sa {http://links.esri.com/qtEasingProperties}{Property Animation QML Type}

        The default animationEasingType is \c Easing.OutQuad.
    */
    property real animationEasingType: Easing.OutQuad

    /*!
        \brief The index of the PopupManager in \c popupManagers that is being displayed
    */
    property int currentIndex: 0

    /*!
        \brief The color used for the close button.

        The default color is \c "gray".
    */
    property color closeButtonColor: "gray"

    /*!
        \brief The visibility of the PopupStackView.

        The default visibility is \c false.
    */
    visible: false

    /*!
        \brief The width of the PopupStackView.

        The default width is \c 275.
    */
    width: 275

    /*!
        \brief The height of the PopupStackView.

        The default height is \c 350.
    */
    height: 350

    /*!
        \brief Show the PopupStackView.
    */
    function show() {
        if (popupStack.busy)
            return;

        currentIndex = 0;
        if (popupManagers !== null && popupManagers.length > 0) {
            swapPopups(popupStack.currentItem === popup1 ? popup1 : popup2,
                       popupStack.currentItem === popup1 ? popup2 : popup1);
        }
        visible = true;
    }

    /*!
        \brief Hide the PopupStackView.
    */
    function dismiss() {
        currentIndex = 0;
        visible = false;
    }

    /*!
        \brief Slide the PopupStackView horizontally with animation.

        \list
          \li fromX - The x-value of the top-left corner to move the PopupStackView from.
          \li toX - The x-value of the top-left corner to move the PopupStackView to.
        \endlist

        If using this method, the left and right anchors cannot be set on the PopupStackView.
        Rather, anchor the top and bottom only, so that the x-value can be changed.

        Set the animationDuration and animationEasingType properties for finer-grained
        control of the animation.
    */
    function slideHorizontal(fromX, toX) {
        show();
        animateHorizontal.from = fromX;
        animateHorizontal.to = toX;
        animateHorizontal.start();
    }

    /*!
        \brief Slide the PopupStackView vertically with animation.

        \list
          \li fromY - The y-value of the top-left corner to move the PopupStackView from.
          \li toY - The y-value of the top-left corner to move the PopupStackView to.
        \endlist

        If using this method, the top and bottom anchors cannot be set on the PopupStackView.
        Rather, anchor the left or right only, so that the y-value can be changed.

        Set the animationDuration and animationEasingType properties for finer-grained
        control of the animation.
    */
    function slideVertical(fromY, toY) {
        show();
        animateVertical.from = fromY;
        animateVertical.to = toY;
        animateVertical.start();
    }

    /*! internal */
    NumberAnimation {
        id: animateHorizontal
        target: popupStackView
        properties: "x"
        duration: animationDuration
        easing.type: animationEasingType
    }

    /*! internal */
    NumberAnimation {
        id: animateVertical
        target: popupStackView
        properties: "y"
        duration: animationDuration
        easing.type: animationEasingType
    }

    /*!
        \brief Signal emitted when an attachment thumbnail is clicked.

        The \a index of the PopupAttachment in the PopupAttachmentListModel
        is passed so that the PopupAttachment can be obtained. An example
        workflow would be to:

        \list
          \li Set up a signal handler for this signal.
          \li When the signal emits, use the index to obtain the PopupAttachment
          from the PopupAttachmentListModel.
          \li Load the PopupAttachment.
          \li Display the full image of the PopupAttachment in a dialog or window using
           the \c fullImageUrl.
        \endlist
    */
    signal attachmentThumbnailClicked(var index)

    /*! internal */
    function nextPopup() {
        if (popupStack.busy)
            return;

        if (currentIndex + 1 === popupManagers.length)
            return;

        currentIndex += 1;

        if (popupStack.currentItem === popup1) {
            swapPopups(popup1, popup2);
        } else {
            swapPopups(popup2, popup1);
        }
    }

    /*! internal */
    function previousPopup() {
        if (popupStack.busy)
            return;

        if (currentIndex === 0)
            return;

        currentIndex -= 1;

        if (popupStack.currentItem === popup1) {
            swapPopups(popup1, popup2);
        }
        else {
            swapPopups(popup2, popup1);
        }
    }

    function swapPopups(frontPopup, backPopup) {
        backPopup.popupManagerInternal = popupManagers[currentIndex];
        popupStack.replace(frontPopup, backPopup);
    }

    /*! internal */
    function drawButton(canvas) {
        var ctx = canvas.getContext("2d");
        ctx.strokeStyle = buttonColor;
        ctx.lineWidth = canvas.height / 10;
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.save();

        // place in middle
        ctx.translate(0.5 * canvas.width - 0.5 * canvas.height / 2, 0.5 * canvas.height - 0.5 * canvas.height / 2);

        // start drawing the triangle
        ctx.beginPath();

        // top point of the triangle
        ctx.moveTo(canvas.height / 4, 0);

        // middle point
        ctx.lineTo(0, canvas.height / 4);

        // bottom point
        ctx.lineTo(canvas.height / 4, canvas.height / 2);

        ctx.stroke();
        ctx.restore();
    }

    // Create two PopupView and cycle between the two to act as StackView.
    /*! internal */
    PopupViewBase {
        id: popup1
        backgroundColorInternal: backgroundColor
        borderColorInternal: borderColor
        borderWidthInternal: 0
        radiusInternal: radius
        titleTextSizeInternal: titleTextSize
        attributeNameTextColorInternal: attributeNameTextColor
        attributeValueTextColorInternal: attributeValueTextColor
        titleTextColorInternal: titleTextColor
        closeButtonColorInternal: closeButtonColor
        visible: false

        onAttachmentThumbnailClickedInternal: attachmentThumbnailClicked(index)
        onPopupViewDismissed: dismiss();
    }

    /*! internal */
    PopupViewBase {
        id: popup2
        backgroundColorInternal: backgroundColor
        borderColorInternal: borderColor
        borderWidthInternal: 0
        radiusInternal: radius
        titleTextSizeInternal: titleTextSize
        attributeNameTextColorInternal: attributeNameTextColor
        attributeValueTextColorInternal: attributeValueTextColor
        titleTextColorInternal: titleTextColor
        closeButtonColorInternal: closeButtonColor
        visible: false

        onAttachmentThumbnailClickedInternal: attachmentThumbnailClicked(index)
        onPopupViewDismissed: dismiss();
    }

    Column {
        height: parent.height
        width: parent.width
        clip: true

        Rectangle {
            id: navButtonsRectangle
            color: backgroundColor
            border {
                color: borderColor
                width: borderWidth
            }
            radius: radius
            width: parent.width
            height: parent.height / 12
            visible: popupManagers !== null && popupManagers.length > 1

            Canvas {
                id: previousButtonCanvas
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 2
                }
                antialiasing: true
                height: parent.height
                width: height

                onPaint: {
                    drawButton(previousButtonCanvas);
                }

                Component.onCompleted: {
                    if (Qt.platform.os === "ios")
                        renderTarget = Canvas.Image;
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        previousPopup();
                    }
                }
            }

            Text {
                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                    margins: 5
                }
                color: attributeNameTextColor
                text: popupManagers !== null && popupManagers.length > 0 ? (currentIndex + 1) + " of " + popupManagers.length : ""
            }

            Canvas {
                id: forwardButtonCanvas
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 2
                }
                antialiasing: true
                height: parent.height
                width: height
                rotation: 180

                onPaint: {
                    drawButton(forwardButtonCanvas);
                }

                Component.onCompleted: {
                    if (Qt.platform.os === "ios")
                        renderTarget = Canvas.Image;
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        nextPopup();
                    }
                }
            }
        }

        Item {
            height: navButtonsRectangle.visible ? parent.height - navButtonsRectangle.height : parent.height
            width: parent.width

            StackView {
                id: popupStack
                anchors.fill: parent

                Component.onCompleted: {
                    push(popup1);
                }
            }

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border {
                    width: borderWidth
                    color: borderColor
                }
            }
        }
    }

    onButtonColorChanged: {
        forwardButtonCanvas.requestPaint();
        previousButtonCanvas.requestPaint();
    }
}
