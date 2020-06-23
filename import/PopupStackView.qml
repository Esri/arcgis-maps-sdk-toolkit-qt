/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
import QtQuick.Layouts 1.12

/*!
   \qmltype PopupStackView
   \ingroup ArcGISQtToolkit
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.8
   \brief A view for displaying and editing information of GeoElements,
   including Features and Graphics.
   
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
     \li Create a PopupManager from the Popup and add it to a list of 
     PopupManagers
     \li Assign the list mentioned in the above step to the PopupStackView's
     \c popupManagers property
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
Control {
    id: popupStackView

    /*!
       \brief A list of PopupManagers that controls the information being 
       displayed in a PopupStackView.
       \qmlproperty list<PopupManager> popupManagers
     */
    property var popupManagers: null

    /*!
       \brief This property holds the current top-most item in the stack. I.E.
       the current popup from the list of popup managers.
       \qmlproperty PopupView currentItem
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    property alias currentItem: stack.currentItem

    /*!
       \brief This property holds the number of items currently pushed onto the 
       stack.
       \qmlproperty int depth
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    property alias depth: stack.depth

    /*!
       \brief This property holds whether a transition is running.
       \qmlproperty bool busy
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    property alias busy: stack.busy

    /*!
      \brief This property holds the transition that is applied to the item 
      that enters the stack when another item is popped off of it.
      \qmlproperty Transition popEnter
      \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    property alias popEnter: stack.popEnter

    /*!
       \brief This property holds the transition that is applied to the item 
       that exits the stack when the item is popped off of it.
       \qmlproperty Transition popExit
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    property alias popExit: stack.popExit

    /*!
       \brief This property holds the transition that is applied to the item 
       that enters the stack when the item is pushed onto it.
       \qmlproperty Transition pushEnter
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    property alias pushEnter: stack.pushEnter

    /*!
       \brief This property holds the transition that is applied to the item 
       that exits the stack when another item is pushed onto it.
       \qmlproperty Transition pushExit
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    property alias pushExit: stack.pushExit

    /*!
       \brief Callback function called when the close button is clicked. When
       this property is set to null the close button does not render. When
       the close button is clicked the function in this property is called.
       Defaults to setting visible to false.
     */
    property var closeCallback: function() {
        popupStackView.visible = false;
    }

    /*!
       \qmlsignal PopupStackView::attachmentThumbnailClicked(int index)
       \brief Signal emitted when an attachment thumbnail is clicked.
       The \e index of the PopupAttachment in the PopupAttachmentListModel
       of the currently displayed PopupView.
     */
    signal attachmentThumbnailClicked(var index)

    /*!
       \brief Removes all items from the stack.
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    function clear(...args) {
        return stack.clear(...args);
    }

    /*!
       \brief Search for a specific item inside the stack. The callback function
       is called for each item in the stack (with the item and index as 
       arguments) until the callback function returns true. The return value is
       the item found.
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    function find(...args) {
        return stack.find(...args);
    }

    /*!
       \brief Returns the item at position index in the stack, or null if the 
       index is out of bounds.
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    function get(...args) {
        return stack.get(...args);
    }
    
    /*!
       \brief Pops one or more items off the stack. Returns the last item 
       removed from the stack. If the item argument is specified, all items down
       to (but not including) item will be popped. If item is null, all items 
       down to (but not including) the first item is popped. If not specified, 
       only the current item is popped.
       \sa {https://doc.qt.io/qt-5/qml-qtquick-controls2-stackview.html}{StackView}
     */
    function pop(...args) {
        return stack.pop(...args);
    }

    /*!
       \brief Attempts to show the previous item in the list of 
       PopupManagers.
     */
    function gotoPrevious() {
        stack.pop();
    }

    /*!
       \brief Attempts to show the next item in the list of 
       PopupManagers.
     */
    function gotoNext() {
        // We want a transition on show, so we force the first item to 
        // transition.
        stack.push(popupViewPage, StackView.PushTransition);
    } 

    onVisibleChanged: {
        // Always display with a transition and on page 1.
        if (visible) {
            clear();
            gotoNext();
        }
    }
    
    clip: true

    implicitWidth: 300 + padding

    implicitHeight: 300 + padding

    padding: 5

    background: Rectangle {
        color: palette.base
        border {
            color: palette.shadow
            width: 2
        }
        radius: 2
    }

    contentItem: GridLayout {
        columns: 3
        anchors {
            fill: parent
            margins: 5
        }

        Button {
            text: "Previous"
            onClicked: gotoPrevious();
            Layout.alignment: Qt.AlignLeft
            enabled: popupManagers ? stack.depth > 1 : false
        }

        Text {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            text: popupManagers && popupManagers.length > 0 ? `${stack.depth} of ${popupManagers.length}` : ""
            color: palette.text
        }

        Button {
            text: "Next"
            onClicked: gotoNext();
            Layout.alignment: Qt.AlignRight
            enabled: popupManagers ? stack.depth < popupManagers.length : false
        }

        StackView {
            id: stack
            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Component {
            id: popupViewPage
            PopupView {
                popupManager: popupManagers && popupManagers.length >= StackView.index ? popupManagers[StackView.index] : null
                palette: popupStackView.palette
                background: null
                closeCallback: popupStackView.closeCallback
                onAttachmentThumbnailClicked: {
                    popupStackView.attachmentThumbnailClicked(index);
                }
            }
        }
    }
}
