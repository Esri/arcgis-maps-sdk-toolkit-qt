/*******************************************************************************
 *  Copyright 2012-2021 Esri
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

import Esri.ArcGISRuntime.Toolkit.Controller 100.13

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

/*!
    \qmltype SearchView
    \inqmlmodule Esri.ArcGISRuntime.Toolkit
    \ingroup ArcGISQtToolkitUiQmlViews
    \since Esri.ArcGISRuntime 100.13
    \brief SearchView enables users to search for one or more locations or features,
    from a variety of sources, with suggestions.
*/
Pane {
    id: searchView

    /*!
      \qmlproperty GeoView geoView
      \brief When connected to a GeoView, SearchView will automatically navigate the view in response to search result changes.
      Additionally, the view's current center and extent will be automatically provided to locators as parameters.
     */
    property var geoView;

    /*!
      \qmlproperty SearchViewController controller
      \brief The Controller which performs the search and manages
      the results/suggestions.

       A default SearchViewController is provided.
    */
    property var controller: SearchViewController { }

    /*!
      \qmlproperty bool automaticConfigurationEnabled
      \brief Determines whether the view will update its configuration based on the attached geoview's geomodel automatically.

      Defaults to true.
     */
    property bool automaticConfigurationEnabled: true;

    /*!
      \qmlproperty bool repeatSearchHereButtonEnabled
      \brief Determines whether a button that allows the user to repeat a search with a spatial constraint is displayed automatically.
      Set to false if you want to use a custom button, for example so that you can place it elsewhere on the map.
      \l SearchViewController has properties and methods you can use to determine when the custom button should be visible and to
      trigger the search repeat behavior.

      Defaults to true.
     */
    property bool repeatSearchHereButtonEnabled: true;

    /*!
        \qmlproperty bool resultListViewEnabled
        \brief Determines whether a built-in result view will be shown.
        If false, the result display/selection list is not shown. Set to false if you want to define a custom result list.
        You might use a custom result list to show results in a separate list, disconnected from the rest of the search view.

        Defaults to true.
     */
    property bool resultListViewEnabled: true;

    /*!
        \qmlproperty string noResultMessage
        \brief Message to show when there are no results or suggestions.

        Defaults to 'No results found'.
     */
    property alias noResultMessage: noResultFoundLabel.text

    padding: 0

    states: [
        // State that is triggered when the user gains a valid selection result.
        State {
            name: "resultSelected"
        },
        // State that is triggered when the user commits to a search.
        State {
            name: "searchCommitted"
        }
    ]

    contentItem: ColumnLayout {
        id: contentItem
        TextField {
            id: textField
            Layout.fillWidth: true
            Layout.margins: 0
            placeholderText: controller.defaultPlaceholder
            implicitWidth: 300

            leftPadding: searchButton.width + searchButton.leftPadding + searchButton.rightPadding
            rightPadding: clearButton.width + clearButton.leftPadding + clearButton.rightPadding

            property bool isUserEditing: true

            Binding {
                target: controller
                property: "currentQuery"
                value: textField.text
            }

            RoundButton {
                id: searchButton
                icon.source: "images/search.png"
                width: height
                flat: true
                padding: 2
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                    margins: 4
                }
                onClicked: {
                    textField.accepted();
                }
            }

            RoundButton {
                id: clearButton
                icon.source: "images/x.png"
                width: height
                flat: true
                padding: 2
                anchors {
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                    margins: 4
                }
                onClicked: {
                    controller.clearSearch();
                    searchView.state = "";
                }
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Up) {
                    if (list.currentIndex === 0) {
                        list.currentIndex = -1;
                    } else {
                        list.decrementCurrentIndex();
                    }

                    event.accepted = true;
                } else if (event.key === Qt.Key_Down) {
                    list.incrementCurrentIndex();
                    event.accepted = true;
                }
            }
            onAccepted: contentItem.acceptDropdownItem(list.currentIndex);
            onTextChanged: {
                // Bring us back to the user seeing suggestions. This changes the
                // state, but does not unselect any on-screen results.
                if (isUserEditing) {
                    list.currentIndex = -1;
                    searchView.state = "";
                }
            }
            function setControllerText(newText) {
                // Allow the controller to update the text without
                // moving us back into show-suggestions mode.
                isUserEditing = false;
                text = newText;
                isUserEditing = true;
            }
        }


        Button {
            text: "Repeat search here."
            Layout.fillWidth: true
            visible: {
                if (!repeatSearchHereButtonEnabled) {
                    return false;
                }

                // No repeat search if no search in progress.
                if (searchView.state === "") {
                    return false;
                }

                // Repeat search if we have some suggestions
                if (searchView.state === "searchCommitted") {
                    return controller.results.count > 0 ? controller.eligableForRequery : false;
                }

                // Repeat search if we have a result.
                if (searchView.state === "resultSelected") {
                    return controller.eligableForRequery;
                }
            }
            onClicked: {
                controller.commitSearch(true);
                searchView.state = "searchCommitted"
            }
        }

        Label {
            id: noResultFoundLabel
            text: "No results found."
            Layout.fillWidth: true
            visible: resultListViewEnabled && searchView.state === "searchCommitted" && controller.results.count === 0
            horizontalAlignment: Text.AlignHCenter
        }

        ListView {
            id: list
            clip: true
            spacing: searchView.spacing
            model: {
                if (searchView.state === "resultSelected") {
                    return null;
                }

                if (searchView.state === "searchCommitted") {
                    return controller.results;
                }

                return controller.suggestions;
            }
            visible: {
                if (!resultListViewEnabled) {
                    return false;
                }

                if (searchView.state === "resultSelected") {
                    return false;
                }

                if (searchView.state === "searchCommitted") {
                    if (controller.results.count === 0) {
                        return false;
                    }

                    if (controller.resultMode === SearchViewController.Single)
                        return false;

                    return true;
                }

                return controller.suggestions.count > 0;
            }
            Layout.fillWidth: true
            Layout.minimumHeight: childrenRect.height
            Layout.margins: 0
            // Individual results rendering
            delegate: Item {
                width: list.width
                height: childrenRect.height
                GridLayout {
                    flow: GridLayout.TopToBottom
                    rows: 2
                    width: parent.width
                    Image {
                        source: markerImageUrl
                        sourceSize.height: 32
                        Layout.rowSpan: 2
                    }
                    Text {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: displayTitle
                        font: searchView.font
                        color: searchView.palette.text
                        elide: Text.ElideRight
                        Layout.rowSpan: displaySubtitle === "" ? 2 : 1
                    }
                    Text {
                        Layout.fillWidth: true
                        text: displaySubtitle
                        font: searchView.font
                        color: searchView.palette.text
                        elide: Text.ElideRight
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPositionChanged: list.currentIndex = index;
                    onClicked: contentItem.acceptDropdownItem(index);
                    onExited: {
                        list.currentIndex = -1;
                    }
                }
            }
            // Group suggestions by the owning source's name.
            section {
                // TODO: Re-enable in a later release when we have multiple source capabilities.
                // property: "owningSource.displayName"
                labelPositioning: ViewSection.CurrentLabelAtStart
                criteria: ViewSection.FullString
                delegate: Control {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    background: Rectangle {
                        color: searchView.palette.alternateBase
                    }
                    contentItem: Text {
                        text: section
                        color: searchView.palette.text
                        horizontalAlignment: Text.AlignHCenter
                        elide: Text.ElideRight
                        font.italic: true
                    }
                }
            }
            // Simple highlight rectangle for selected result.
            highlightFollowsCurrentItem: false
            highlight: Rectangle {
                x: list.x
                width: list.width
                y: list.currentItem ? list.currentItem.y : 0
                height: list.currentItem ? list.currentItem.height : 0
                color: searchView.palette.highlight
            }
        }

        function acceptDropdownItem(index) {
            if (index === -1) {
                // With no suggestion, we perform a search wiith the given text.
                controller.commitSearch(false);
                searchView.state = "searchCommitted";
            } else if (searchView.state === "searchCommitted") {
                acceptResult(index);
            } else {
                acceptSuggestion(index);
                searchView.state = "searchCommitted";
            }
        }

        function acceptSuggestion(index) {
            let s = controller.suggestions;
            let suggestion = s.element(s.index(index, 0));
            textField.setControllerText(suggestion.displayTitle);
            controller.acceptSuggestion(suggestion);
        }

        function acceptResult(index) {
            let r = controller.results;
            let result = r.element(r.index(index, 0));
            textField.setControllerText(result.displayTitle);
            controller.selectedResult = result;
        }
    }

    Binding {
        target: controller
        property: "geoView"
        value: geoView
    }

    onGeoViewChanged: {
        if (controller)
            controller.geoView = geoView;
    }

    Binding {
        target: controller
        property: "automaticConfigurationEnabled"
        value: automaticConfigurationEnabled
    }

    onAutomaticConfigurationEnabledChanged: {
        if (controller)
            controller.automaticConfigurationEnabled = automaticConfigurationEnabled;
    }

    Connections {
        target:controller
        function onCurrentQueryChanged() {
            // Create a two-way binding by listening to any changes to the search box
            // from the controller.
            textField.setControllerText(controller.currentQuery);
        }
        function onSelectedResultChanged() {
            // When a result is selected we set the state to result selected. Note we do not unset the
            // state until the user interacts with the tool.
            if (controller.selectedResult !== null) {
                searchView.state = "resultSelected";
            }
        }
    }
}
