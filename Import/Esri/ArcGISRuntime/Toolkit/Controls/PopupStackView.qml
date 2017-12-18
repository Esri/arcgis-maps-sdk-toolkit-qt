/*******************************************************************************
 * Copyright 2012-2016 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0

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

    /*! \internal */
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

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
    property real borderWidth: 2 * displayScaleFactor

    /*!
        \brief The radius of the PopupStackView.

        The default radius is \c 2.
    */
    property real radius: 2 * displayScaleFactor

    /*!
        \brief The title text color of the PopupStackView.

        The default color is \c "black".
    */
    property color titleTextColor: "black"

    /*!
        \brief The title text size of the PopupStackView.

        The default size is \c 13.
    */
    property real titleTextSize: 13 * displayScaleFactor

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
        \brief The visibility of the PopupStackView.

        The default visibility is \c false.
    */
    visible: false

    /*!
        \brief The width of the PopupStackView.

        The default width is \c 275.
    */
    width: 275 * displayScaleFactor

    /*!
        \brief The height of the PopupStackView.

        The default height is \c 350.
    */
    height: 350 * displayScaleFactor

    /*!
        \brief Show the PopupStackView.
    */
    function show() {
        currentIndex = 0;
        popupStack.clear();
        if (popupManagers !== null && popupManagers.length > 0) {
            popup1.popupManagerInternal = popupManagers[currentIndex]
            popupStack.push(popup1);
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
        if (currentIndex + 1 === popupManagers.length)
            return;

        currentIndex += 1;

        if (popupStack.currentItem === popup1) {
            popup2.popupManagerInternal = popupManagers[currentIndex]
            popupStack.push(popup2);
        }
        else
        {
            popup1.popupManagerInternal = popupManagers[currentIndex]
            popupStack.push(popup1);
        }

    }

    /*! internal */
    function previousPopup() {
        if (currentIndex === 0)
            return;

        currentIndex -= 1;

        if (popupStack.currentItem === popup2)
            popup1.popupManagerInternal = popupManagers[currentIndex];
        else
            popup2.popupManagerInternal = popupManagers[currentIndex];

        popupStack.pop();
    }

    // Create two PopupView and cycle between the two to act as StackView.
    /*! internal */
    PopupViewBase {
        id: popup1
        backgroundColorInternal: backgroundColor
        borderColorInternal: borderColor
        borderWidthInternal: borderWidth
        radiusInternal: radius
        titleTextSizeInternal: titleTextSize
        attributeNameTextColorInternal: attributeNameTextColor
        attributeValueTextColorInternal: attributeValueTextColor
        titleTextColorInternal: titleTextColor
        visible: false

        onAttachmentThumbnailClickedInternal: attachmentThumbnailClicked(index)
    }

    /*! internal */
    PopupViewBase {
        id: popup2
        backgroundColorInternal: backgroundColor
        borderColorInternal: borderColor
        borderWidthInternal: borderWidth
        radiusInternal: radius
        titleTextSizeInternal: titleTextSize
        attributeNameTextColorInternal: attributeNameTextColor
        attributeValueTextColorInternal: attributeValueTextColor
        titleTextColorInternal: titleTextColor
        visible: false

        onAttachmentThumbnailClickedInternal: attachmentThumbnailClicked(index)
    }

    Column {
        height: parent.height
        width: parent.width

        Rectangle {
            color: backgroundColor
            border {
                color: borderColor
                width: borderWidth
            }
            radius: radius
            width: parent.width
            height: parent.height / 12


            Image {
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 2 * displayScaleFactor
                }
                height: parent.height
                width: height
                source: "images/forward.png"
                rotation: 180

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
                    margins: 5 * displayScaleFactor
                }
                color: attributeNameTextColor
                text: popupManagers !== null && popupManagers.length > 0 ? (currentIndex + 1) + " of " + popupManagers.length : ""

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        previousPopup();
                    }
                }
            }

            Image {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 2 * displayScaleFactor
                }
                height: parent.height
                width: height

                source: "images/forward.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        nextPopup();
                    }
                }
            }
        }

        StackView {
            id: popupStack
            height: parent.height
            width: parent.width
        }
    }
}
