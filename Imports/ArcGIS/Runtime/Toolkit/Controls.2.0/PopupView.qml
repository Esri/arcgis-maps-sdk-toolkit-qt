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
import Esri.ArcGISExtras 1.1

/*!
    \qmltype PopupView
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since 2.0
    \brief A view for displaying and editing information about a feature.

    A PopupView can be used to display information for any type that
    implements the PopupSource interface. For example, FeatureLayer
    implements PopupSource. This means that it has a PopupDefinition,
    which defines how the Popup should look for any features in that
    layer.

    An example workflow for displaying a PopupView for a feature in a
    Feature Layer would be:

    \list
      \li Declare a PopupView and anchor it to a desired location.
      \li Perform an identify operation on a GeoView and select a
      Feature from the identify result.
      \li Create a Popup from the Feature.
      \li Optionally obtain the Popup's PopupDefinition and set the
      title, whether to show attachments, and so on.
      \li Create a PopupManager from the Popup.
      \li Assign the PopupView's \c popupManager property the PopupManager
      created in the previous step.
      \li Call the \c show() method to display the PopupView.
      \li Call the \c dismiss() method to hide the PopupView.
    \endlist

    The PopupView is a QML Item that can be anchored, given to a dialog,
    or positioned using XY screen coordinates. Transform, Transition, and
    other QML animation types can be used to animate the showing and
    dismissing of the view.

    For more information, please see the Popup and PopupManager
    documentation.

    \note Each time a change is made to the Popup, PopupDefinition,
    PopupManager, or any of their properties, the PopupManager must
    be re-set to the PopupView.
*/
Item {
    id: popupView

    /*========================================
         Configurable properties
    ========================================*/

    /*!
        \brief The PopupManager that controls the information being displayed in the view.

        The PopupManager should be created from a Popup. See the example workflow that is
        highlighted in the description of this type.
    */
    property var popupManager: null

    /*!
        \brief The background color of the PopupView.

        The default color is \c "#f2f3f4".
    */
    property color backgroundColor: "#f2f3f4"

    /*!
        \brief The border color of the PopupView.

        The default color is \c "#4f4f4f".
    */
    property color borderColor: "#4f4f4f"

    /*!
        \brief The border width of the PopupView.

        The default width is \c 2.
    */
    property real borderWidth: 2 * scaleFactor

    /*!
        \brief The radius of the PopupView.

        The default width is \c 2.
    */
    property real radius: 2 * scaleFactor

    /*!
        \brief The title text color of the PopupView.

        The default color is \c "black".
    */
    property color titleTextColor: "black"

    /*!
        \brief The title text size of the PopupView.

        The default size is \c 13.
    */
    property real titleTextSize: 13 * scaleFactor

    /*!
        \brief The attribute name color of the PopupView.

        The default color is \c "gray".
    */
    property color attributeNameTextColor: "gray"

    /*!
        \brief The attribute value color of the PopupView.

        The default color is \c "4f4f4f".
    */
    property color attributeValueTextColor: "#4f4f4f"

    /*!
        \brief The visibility of the PopupView.

        The default visibility is \c false.
    */
    visible: false

    /*!
        \brief The width of the PopupView.

        The default width is \c 275.
    */
    width: 275 * scaleFactor
    /*!
        \brief The height of the PopupView.

        The default height is \c 350.
    */
    height: 350 * scaleFactor

    /*!
        \brief Show the PopupView.
    */
    function show() {
        visible = true;
    }

    /*!
        \brief Hide the PopupView.
    */
    function dismiss() {
        visible = false;
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

    /* internal */
    property real scaleFactor: System.displayScaleFactor   

    /* internal */
    PopupViewBase {
        anchors.fill: parent
        popupManagerInternal: popupManager
        backgroundColorInternal: backgroundColor
        borderColorInternal: borderColor
        borderWidthInternal: borderWidth
        radiusInternal: radius
        titleTextSizeInternal: titleTextSize
        attributeNameTextColorInternal: attributeNameTextColor
        attributeValueTextColorInternal: attributeValueTextColor
        titleTextColorInternal: titleTextColor

        onAttachmentThumbnailClickedInternal: attachmentThumbnailClicked(index)
    }
}
