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

import Esri.ArcGISRuntime.Toolkit.Controller

import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

/*!
    \qmltype SearchView
    \inqmlmodule Esri.ArcGISRuntime.Toolkit
    \ingroup ArcGISQtToolkitUiQmlViews
    \since Esri.ArcGISRuntime 100.13
    \brief SearchView enables users to search for one or more locations or features,
    from a variety of sources, with suggestions.
    \image search.png search
    \snippet qml/demos/SearchViewDemoForm.qml Set up Search View QML

    Enter in the text field searches for the typed text. The Down key moves into the list of suggestions
    or results, where Up and Down move between rows and Enter accepts the current one. Escape closes the
    suggestions. Anything typed while on a row goes to the text field. Assistive technologies see the text
    field under its placeholder text, and are told how many suggestions or results arrived.

    The default width is \c 300 at a 16-pixel font size and scales proportionally with the effective Qt
    font size, including changes after creation. Setting \c width explicitly disables the font-based default.

    \note By default, the SearchView will attempt to access the World Geocoding Service, which requires an \l{https://links.esri.com/create-an-api-key}{access token}.
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
      \internal
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
    // Keeps the wheel from reaching the map under the view, also when the list cannot scroll further.
    wheelEnabled: true

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
        id: searchContentItem

        // Set when the user closes the suggestions with Escape, until the text is edited again.
        property bool suggestionsDismissed: false

        TextField {
            id: textField
            activeFocusOnTab: true
            KeyNavigation.tab: clearButton.visible ? clearButton : searchButton
            Layout.fillWidth: true
            // Keeps the 24px buttons inside the field at small font sizes.
            Layout.minimumHeight: 24
            Layout.margins: 0
            placeholderText: controller.defaultPlaceholder
            implicitWidth: 300 * searchView.font.pixelSize / 16

            rightPadding: searchButton.width + searchButton.leftPadding + searchButton.rightPadding
                          + (clearButton.visible ? clearButton.width + clearButton.leftPadding + clearButton.rightPadding : 0)
            // Room above and below the text, so that the field is not dwarfed by the rows under it.
            topPadding: 10
            bottomPadding: 10

            property bool isUserEditing: true

            Accessible.name: placeholderText
            Accessible.searchEdit: true

            Binding {
                target: controller
                property: "currentQuery"
                value: textField.text
            }

            RoundButton {
                id: clearButton
                // Not displayed; gives the button its accessible name.
                text: qsTr("Clear")
                display: AbstractButton.IconOnly
                ToolTip.text: text
                ToolTip.visible: visible && enabled && !toolTipSuppressed && (hovered || visualFocus)
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/x.svg"
                icon.color: textField.color
                // 24 is the WCAG 2.5.8 minimum target size.
                height: Math.max(24, parent.height - 8)
                width: height
                flat: true
                leftPadding: 2
                rightPadding: 2
                topPadding: 0
                bottomPadding: 0
                visible: textField.text.length > 0
                KeyNavigation.tab: searchButton
                KeyNavigation.backtab: textField
                anchors {
                    right: searchButton.left
                    verticalCenter: parent.verticalCenter
                }

                property bool toolTipSuppressed: false
                onPressedChanged: if (pressed) toolTipSuppressed = true
                onHoveredChanged: if (hovered && !pressed) toolTipSuppressed = false
                onVisualFocusChanged: if (visualFocus && !pressed) toolTipSuppressed = false

                function clearSearch() {
                    controller.clearSearch();
                    searchView.state = "";
                    textField.forceActiveFocus();
                }

                Keys.onReturnPressed: event => {
                    clearSearch();
                    event.accepted = true;
                }
                Keys.onEnterPressed: event => {
                    clearSearch();
                    event.accepted = true;
                }
                onClicked: clearSearch()

                Shortcut {
                    sequence: "Escape"
                    enabled: clearButton.ToolTip.visible
                    onActivated: clearButton.ToolTip.hide()
                }
            }

            RoundButton {
                id: searchButton
                // Not displayed; gives the button its accessible name.
                text: qsTr("Search")
                display: AbstractButton.IconOnly
                ToolTip.text: text
                ToolTip.visible: visible && enabled && !toolTipSuppressed && (hovered || visualFocus)
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/search.svg"
                icon.color: textField.color
                // 24 is the WCAG 2.5.8 minimum target size.
                height: Math.max(24, parent.height - 8)
                width: height
                flat: true
                leftPadding: 2
                rightPadding: 2
                topPadding: 0
                bottomPadding: 0
                KeyNavigation.backtab: clearButton.visible ? clearButton : textField
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    rightMargin: 4
                }

                property bool toolTipSuppressed: false
                onPressedChanged: if (pressed) toolTipSuppressed = true
                onHoveredChanged: if (hovered && !pressed) toolTipSuppressed = false
                onVisualFocusChanged: if (visualFocus && !pressed) toolTipSuppressed = false

                onClicked: {
                    textField.accepted();
                }

                Shortcut {
                    sequence: "Escape"
                    enabled: searchButton.ToolTip.visible
                    onActivated: searchButton.ToolTip.hide()
                }
            }

            Keys.onPressed: event => {
                if (event.key === Qt.Key_Down) {
                    // Down moves keyboard focus into the list. Enter in the field always searches the typed text.
                    if (list.visible && list.count > 0) {
                        if (list.currentIndex < 0)
                            list.currentIndex = 0;
                        list.forceActiveFocus(Qt.TabFocusReason);
                    }
                    event.accepted = true;
                } else if (event.key === Qt.Key_Escape && list.visible && searchView.state === "") {
                    searchContentItem.suggestionsDismissed = true;
                    event.accepted = true;
                }
            }
            onAccepted: searchContentItem.acceptDropdownItem(-1);
            onTextChanged: {
                // Bring us back to the user seeing suggestions. This changes the
                // state, but does not unselect any on-screen results.
                if (isUserEditing) {
                    list.currentIndex = 0;
                    searchContentItem.suggestionsDismissed = false;
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
            text: qsTr("Repeat search here.")
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
            text: qsTr("No results found.")
            Layout.fillWidth: true
            // Same gap below the text as the layout leaves above it.
            Layout.bottomMargin: searchContentItem.spacing
            visible: resultListViewEnabled && searchView.state === "searchCommitted" && controller.results.count === 0
                     && !controller.searchInProgress
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            onVisibleChanged: noResultAnnouncement.restart()
        }

        // Selecting a result empties the results just before the state changes, so the label is only
        // announced once it has stayed up.
        Timer {
            id: noResultAnnouncement
            interval: 250
            onTriggered: {
                if (noResultFoundLabel.visible)
                    searchView.Accessible.announce(noResultFoundLabel.text, Accessible.Polite);
            }
        }

        // Tells assistive technologies how many rows arrived, once the list has settled.
        Timer {
            id: countAnnouncement
            interval: 700
            onTriggered: {
                if (!list.visible || list.count === 0)
                    return;
                let message;
                if (searchView.state === "searchCommitted")
                    message = list.count === 1 ? qsTr("1 result") : qsTr("%1 results").arg(list.count);
                else
                    message = list.count === 1 ? qsTr("1 suggestion") : qsTr("%1 suggestions").arg(list.count);
                searchView.Accessible.announce(message, Accessible.Polite);
            }
        }

        ListView {
            id: list
            clip: true
            spacing: searchView.spacing
            cacheBuffer: Math.max(0, Math.ceil(contentHeight))

            // The list is a single tab stop, also reached with Down from the text field. Up and Down move
            // between rows and Enter accepts the current one. Anything typed goes back to the text field.
            activeFocusOnTab: visible && count > 0
            onCountChanged: {
                // Rows arrive after the list is created, which leaves no current item.
                if (count > 0 && currentIndex < 0)
                    currentIndex = 0;
                countAnnouncement.restart();
            }
            onVisibleChanged: countAnnouncement.restart()
            Keys.onPressed: event => {
                if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter) && currentIndex >= 0) {
                    // Handled here, not on the row, because the row may not have been created yet.
                    searchContentItem.acceptDropdownItem(currentIndex);
                    event.accepted = true;
                } else if (event.key === Qt.Key_Up && currentIndex <= 0) {
                    searchContentItem.returnToField();
                    event.accepted = true;
                } else if (event.key === Qt.Key_Escape) {
                    if (searchView.state === "")
                        searchContentItem.suggestionsDismissed = true;
                    searchContentItem.returnToField();
                    event.accepted = true;
                } else if (event.key === Qt.Key_Backspace) {
                    searchContentItem.returnToField();
                    textField.remove(textField.cursorPosition - 1, textField.cursorPosition);
                    event.accepted = true;
                } else if (!(event.modifiers & (Qt.ControlModifier | Qt.AltModifier | Qt.MetaModifier))
                           && event.text.length > 0 && event.text.charCodeAt(0) >= 32 && event.text.charCodeAt(0) !== 127) {
                    searchContentItem.typeInField(event.text);
                    event.accepted = true;
                }
            }

            Accessible.role: Accessible.List
            Accessible.name: searchView.state === "searchCommitted" ? qsTr("Results") : qsTr("Suggestions")

            ScrollBar.vertical: ScrollBar {
                Accessible.ignored: true
            }
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

                return !searchContentItem.suggestionsDismissed && controller.suggestions.count > 0;
            }
            Layout.fillWidth: true
            // A long list scrolls instead of growing past the window.
            Layout.preferredHeight: searchView.Window.window ? Math.min(contentHeight, searchView.Window.height / 2)
                                                             : contentHeight
            Layout.maximumHeight: contentHeight
            Layout.fillHeight: true
            Layout.margins: 0
            // Individual results rendering
            delegate: ItemDelegate {
                id: resultDelegate
                width: list.width
                // 24 is the WCAG 2.5.8 minimum target size.
                height: Math.max(implicitHeight, 24)
                // Set directly because a button's `text` loses "&" to mnemonic handling.
                Accessible.name: {
                    if (displaySubtitle === "")
                        return displayTitle;
                    // Result subtitles usually begin with the title; do not read it twice.
                    if (displaySubtitle.startsWith(displayTitle))
                        return displaySubtitle;
                    return qsTr("%1, %2").arg(displayTitle).arg(displaySubtitle);
                }
                contentItem: GridLayout {
                    flow: GridLayout.TopToBottom
                    rows: 2
                    Image {
                        id: sourceImage
                        Layout.rowSpan: 2
                        source: markerImageUrl
                        sourceSize.height: 32
                        visible: searchView.state !== "searchCommitted"
                        // The suggestion icons are black drawings; give them the text color so they follow the theme.
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            // Colorization keeps the source's lightness, so black is first raised to white.
                            brightness: 1.0
                            colorization: 1.0
                            colorizationColor: textLabel.color
                        }
                        Accessible.ignored: true
                    }
                    Label {
                        id: textLabel
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: displayTitle
                        wrapMode: Text.Wrap
                        font: searchView.font
                        Layout.rowSpan: displaySubtitle === "" ? 2 : 1
                        verticalAlignment: Label.AlignVCenter
                        Accessible.ignored: true
                    }
                    Label {
                        Layout.fillWidth: true
                        text: displaySubtitle
                        visible: text !== ""
                        font: searchView.font
                        wrapMode: Text.Wrap
                        Accessible.ignored: true
                    }
                }
                Accessible.focusable: true
                // A focused list item that is not selected is read out as "not selected".
                Accessible.selectable: true
                Accessible.selected: ListView.isCurrentItem
                // A screen reader can focus a row directly, without moving through the list.
                onActiveFocusChanged: {
                    if (activeFocus)
                        list.currentIndex = index;
                }
                onClicked: searchContentItem.acceptDropdownItem(index)
                // Space belongs to the query, for example "new york"; only Enter accepts a row.
                Keys.onSpacePressed: event => searchContentItem.typeInField(event.text)

                // Keyboard focus indicator. Drawn in the text color so that it contrasts with the row in any style.
                Rectangle {
                    anchors.fill: parent
                    visible: resultDelegate.ListView.isCurrentItem && list.activeFocus
                    color: "transparent"
                    border.width: 2
                    border.color: textLabel.color
                }
            }
            // Group suggestions by the owning source's name.
            // TODO: Re-enable in a later release when we have multiple source capabilities.
//            section {
//                property: "owningSource.displayName"
//                labelPositioning: ViewSection.CurrentLabelAtStart
//                criteria: ViewSection.FullString
//                delegate: Frame {
//                    anchors {
//                        left: parent.left
//                        right: parent.right
//                    }
//                    contentItem: Label {
//                        text: section
//                        horizontalAlignment: Text.AlignHCenter
//                        elide: Text.ElideRight
//                        font.italic: true
//                    }
//                }
//            }
        }

        function returnToField() {
            textField.forceActiveFocus();
        }

        function typeInField(text) {
            returnToField();
            textField.insert(textField.cursorPosition, text);
        }

        function acceptDropdownItem(index) {
            if (index === -1) {
                // With no suggestion, we perform a search with the given text.
                controller.commitSearch(false);
                searchView.state = "searchCommitted";
            } else if (searchView.state === "searchCommitted") {
                acceptResult(index);
                returnToField();
            } else {
                acceptSuggestion(index);
                searchView.state = "searchCommitted";
                returnToField();
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
