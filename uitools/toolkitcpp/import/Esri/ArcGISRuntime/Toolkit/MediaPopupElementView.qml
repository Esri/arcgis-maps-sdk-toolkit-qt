/*******************************************************************************
 *  Copyright 2012-2025 Esri
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

import QtGraphs
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*!
  \internal
  This is an internal implementation detail and is subject to change.
*/

ColumnLayout {
    id: mediaPopupElementView
    clip: true
    focus: true
    Layout.fillWidth: true
    spacing: layoutSpacing

    property real mediaMargin: 10
    property real imageTextMargin: 5
    property real layoutSpacing: 0
    property bool isHoverable: true
    property bool isMobile: (Qt.platform.os === "android" || Qt.platform.os === "ios")

    property var controller: null

    Dialog {
        id: fullScreenImageDialog
        modal: true
        visible: false
        width: Overlay.overlay ? Overlay.overlay.width : 0
        height: Overlay.overlay ? Overlay.overlay.height : 0
        anchors.centerIn: Overlay.overlay

        property string imageTitle: ""
        property string imageCaption: ""
        property var modelData: null
        property var mediaType: null

        ColumnLayout {
            anchors.fill: parent
            Layout.alignment: Qt.AlignHCenter

            Label {
                visible: fullScreenImageDialog.imageTitle !== ""
                text: fullScreenImageDialog.imageTitle
                font.weight: Font.Bold
                elide: Text.ElideRight
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                visible: fullScreenImageDialog.imageCaption !== ""
                text: fullScreenImageDialog.imageCaption
                elide: Text.ElideRight
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Loader {
                id: dialogContentLoader
                Layout.fillWidth: true
                Layout.fillHeight: true

                MouseArea {
                    id: ma
                    anchors.fill: parent

                    onClicked: {
                        if (fullScreenImageDialog.mediaType === QmlEnums.PopupMediaTypeImage) {
                            // emit signal to bubble up url to PopupViewController
                            fullScreenImageDialog.modelData.clickedUrl(fullScreenImageDialog.modelData.linkUrl);
                            if (popupView.openUrlsWithSystemDefaultApplication) {
                                Qt.openUrlExternally(fullScreenImageDialog.modelData.linkUrl);
                            } else {
                                // user disabled default behavior, so we do nothing
                            }
                        }
                    }

                    // Images have additional functionality where charts do not. We indicate the former with the pointing hand cursor.
                    cursorShape: fullScreenImageDialog.mediaType === QmlEnums.PopupMediaTypeImage ? Qt.PointingHandCursor : Qt.ArrowCursor
                }
            }

            Label {
                id: label
                text: isMobile ? qsTr("Tap on the image for more information") : qsTr("Click on the image for more information")
                visible: fullScreenImageDialog.mediaType === QmlEnums.PopupMediaTypeImage
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                id: closeButton
                text: "Close"
                Layout.alignment: Qt.AlignHCenter
                onClicked: fullScreenImageDialog.visible = false;
            }
        }
    }

    MenuSeparator {
        Layout.fillWidth: true
        Layout.leftMargin: mediaPopupElementView.mediaMargin
        Layout.rightMargin: mediaPopupElementView.mediaMargin
    }

    ColumnLayout {
        clip: true
        focus: true
        spacing: mediaPopupElementView.layoutSpacing

        Label {
            text: mediaPopupElementView.controller ? mediaPopupElementView.controller.title : null
            wrapMode: Text.WordWrap
            font.pixelSize: 20
            font.weight: Font.Black

            Layout.fillWidth: true
            Layout.leftMargin: mediaPopupElementView.mediaMargin
            Layout.rightMargin: mediaPopupElementView.mediaMargin
        }
        Label {
            text: mediaPopupElementView.controller ? mediaPopupElementView.controller.description : null
            visible: text !== ""
            wrapMode: Text.WordWrap

            Layout.fillWidth: true
            Layout.leftMargin: mediaPopupElementView.mediaMargin
            Layout.rightMargin: mediaPopupElementView.mediaMargin
        }
    }

    MenuSeparator {
        Layout.fillWidth: true
        Layout.leftMargin: mediaPopupElementView.mediaMargin
        Layout.rightMargin: mediaPopupElementView.mediaMargin
    }

    ListView {
        id: lv
        orientation: ListView.Horizontal
        clip: true
        focus: true
        implicitHeight: 170
        spacing: mediaPopupElementView.mediaMargin
        model: mediaPopupElementView.controller ? mediaPopupElementView.controller.popupMediaItems : null

        Layout.fillWidth: true
        Layout.leftMargin: mediaPopupElementView.mediaMargin
        Layout.rightMargin: mediaPopupElementView.mediaMargin

        delegate: Item {
            id: delegatePopupMedia
            height: 170
            width: 220

            Loader {
                id: mediaLoader
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if(model.popupMediaType === QmlEnums.PopupMediaTypeImage) {
                        model.listModelData.mediaImageSourceUrl(model.listModelData.sourceUrl);
                    }
                    if (model.popupMediaType !== QmlEnums.PopupMediaTypeImage ||
                            (model.popupMediaType === QmlEnums.PopupMediaTypeImage && popupView.openImagesInApp)) {
                        dialogContentLoader.sourceComponent = null;
                        dialogContentLoader.sourceComponent = loader.sourceComponent;
                        fullScreenImageDialog.modelData = model.listModelData;
                        fullScreenImageDialog.imageTitle = model.title;
                        fullScreenImageDialog.imageCaption = model.caption;
                        fullScreenImageDialog.mediaType = model.popupMediaType;
                        fullScreenImageDialog.visible = true;
                    }
                    if (model.popupMediaType === QmlEnums.PopupMediaTypeImage && !popupView.openImagesInApp) {
                        // emit signal to bubble up link to PopupViewController
                        model.listModelData.clickedUrl(model.listModelData.linkUrl);
                    }
                }

                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
            }

            Component.onCompleted: {
                switch (popupMediaType) {
                    case QmlEnums.PopupMediaTypeImage:
                        mediaLoader.sourceComponent = imageComp;
                        break;
                    case QmlEnums.PopupMediaTypeColumnChart:
                        mediaLoader.sourceComponent = columnChartComp;
                        break;
                    case QmlEnums.PopupMediaTypeBarChart:
                        mediaLoader.sourceComponent = barChartComp;
                        break;
                    case QmlEnums.PopupMediaTypePieChart:
                        mediaLoader.sourceComponent = pieChartComp;
                        break;
                    case QmlEnums.PopupMediaTypeLineChart:
                        mediaLoader.sourceComponent = lineChartComp;
                        break;
                }
            }

            Component {
                id: imageComp
                Image {
                    source: listModelData.sourceUrl
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    cache: true
                    height: delegatePopupMedia.height
                    width: delegatePopupMedia.width

                    Layout.leftMargin: mediaPopupElementView.mediaMargin
                }
            }

            Component {
                id: columnChartComp
                GraphsView {
                    id: columnGV
                    height: delegatePopupMedia.height
                    width: delegatePopupMedia.width
                    marginBottom: 0

                    // Setting a default theme if no color information is provided on the BarSeries
                    // For more information on the MixSeries theme see https://doc.qt.io/qt-6/qtgraphs-overview-theme.html
                    theme: GraphsTheme {
                        backgroundColor: "white"
                        plotAreaBackgroundColor: "white"
                        theme: GraphsTheme.Theme.MixSeries
                    }

                    axisY: ValueAxis {
                        max: listModelData.maxValue
                        min: listModelData.minValue
                        tickInterval: Math.abs(max) > Math.abs(min)? max / 3 : Math.abs(min) / 3
                    }

                    BarSeries {
                        hoverable: isHoverable

                        // BarSeries will take owership of the QList<QBarSet*> when we call append. This also applies to PieSeries and QPieSlice for PieChartPopupMediaItem.
                        // s.a https://doc.qt.io/qt-6/qbarseries.html#append-1
                        Component.onCompleted: {
                            let sets = listModelData.barSets;
                            if (sets.length > 0)
                                append(sets);
                            else
                                parent.visible = false;
                        }

                        onHoverEnter: (name, position, value) => {
                            columnPopup.visible = true;
                            // adding in offsets so the popup is above the cursor
                            columnPopup.x = position.x + 40;
                            columnPopup.y = position.y - 20;
                            columnLabelText.text = value.y;
                        }

                        onHover: (name, position, value) => {
                            // adding in offsets so the popup is above the cursor
                            columnPopup.x = position.x + 40;
                            columnPopup.y = position.y - 20;
                            columnLabelText.text = value.y;
                        }

                        onHoverExit: {
                            columnPopup.visible = false;
                        }

                        Popup {
                            id: columnPopup
                            visible: false
                            modal: false
                            topInset: 0
                            bottomInset: 0
                            leftInset: 0
                            rightInset: 0

                            topPadding: 5
                            bottomPadding: 5
                            leftPadding: 5
                            rightPadding: 5

                            contentItem: Label {
                                id: columnLabelText
                                anchors{
                                    horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: barChartComp
                GraphsView {
                    id: barGV
                    height: delegatePopupMedia.height
                    width: delegatePopupMedia.width
                    marginLeft: -25

                    orientation: Qt.Horizontal

                    // Setting a default theme if no color information is provided on the BarSeries
                    // For more information on the MixSeries theme see https://doc.qt.io/qt-6/qtgraphs-overview-theme.html
                    theme: GraphsTheme {
                        backgroundColor: "white"
                        plotAreaBackgroundColor: "white"
                        theme: GraphsTheme.Theme.MixSeries
                    }

                    axisY: ValueAxis {
                        max: listModelData.maxValue
                        min: listModelData.minValue
                        tickInterval: Math.abs(max) > Math.abs(min)? max / 3 : Math.abs(min) / 3
                    }

                    BarSeries {
                        hoverable: isHoverable
                        // BarSeries will take owership of the QList<QBarSet*> when we call append. This also applies to PieSeries and QPieSlice for PieChartPopupMediaItem.
                        // s.a https://doc.qt.io/qt-6/qbarseries.html#append-1
                        Component.onCompleted: {
                            let sets = listModelData.barSets;
                            if (sets.length > 0)
                                append(sets);
                            else
                                parent.visible = false;
                        }

                        onHoverEnter: (name, position, value) => {
                            barPopup.visible = true;
                            // adding in offsets so the popup is above the cursor
                            barPopup.x = position.x;
                            barPopup.y = position.y - 20;
                            barPopupText.text = value.y;
                        }

                        onHover: (name, position, value) => {
                            // adding in offsets so the popup is above the cursor
                            barPopup.x = position.x;
                            barPopup.y = position.y -20;
                            barPopupText.text = value.y;
                        }

                        onHoverExit: {
                            barPopup.visible = false;
                        }

                        Popup {
                            id: barPopup
                            visible: false
                            modal: false
                            topInset: 0
                            bottomInset: 0
                            leftInset: 0
                            rightInset: 0

                            topPadding: 5
                            bottomPadding: 5
                            leftPadding: 5
                            rightPadding: 5

                            contentItem: Label {
                                id: barPopupText
                                anchors{
                                    horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: pieChartComp
                GraphsView {
                    height: delegatePopupMedia.height
                    width: delegatePopupMedia.width
                    marginTop: -25
                    marginBottom: -25
                    marginLeft: -25
                    marginRight: -25

                    // Setting a default theme if no color information is provided on the PieSeries
                    // For more information on the MixSeries theme see https://doc.qt.io/qt-6/qtgraphs-overview-theme.html
                    theme: GraphsTheme {
                        backgroundColor: "white"
                        plotAreaBackgroundColor: "white"
                        theme: GraphsTheme.Theme.MixSeries
                    }

                    PieSeries {
                        hoverable: isHoverable
                        // PieSeries will take owership of the QList<QPieSlice*> when we call append. This also applies to BarSeries and QBarSet for BarChartPopupMediaItem.
                        // s.a https://doc.qt.io/qt-6/qpieseries.html#append-1
                        Component.onCompleted: {
                            let slices = listModelData.pieSlices;
                            if (slices.length > 0)
                                append(slices);
                            else
                                parent.visible = false;
                        }

                        onHoverEnter: (name, position, value) => {
                            piePopup.visible = true;
                            // adding in offsets so the popup is above the cursor
                            piePopup.x = position.x;
                            piePopup.y = position.y - 20;
                            piePopupText.text = value.y;
                        }

                        onHover: (name, position, value) => {
                            // adding in offsets so the popup is above the cursor
                            piePopup.x = position.x;
                            piePopup.y = position.y - 20;
                            piePopupText.text = value.y;
                        }

                        onHoverExit: {
                            piePopup.visible = false;
                        }

                        Popup {
                            id: piePopup
                            visible: false
                            modal: false
                            topInset: 0
                            bottomInset: 0
                            leftInset: 0
                            rightInset: 0

                            topPadding: 5
                            bottomPadding: 5
                            leftPadding: 5
                            rightPadding: 5

                            contentItem: Label {
                                id: piePopupText
                                anchors{
                                    horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: lineChartComp
                GraphsView {
                    height: delegatePopupMedia.height
                    width: delegatePopupMedia.width

                    // Setting a default theme if no color information is provided on the LineSeries
                    // For more information on the MixSeries theme see https://doc.qt.io/qt-6/qtgraphs-overview-theme.html
                    theme: GraphsTheme {
                        backgroundColor: "white"
                        plotAreaBackgroundColor: "white"
                        theme: GraphsTheme.Theme.MixSeries
                    }

                    axisX: ValueAxis {
                        id: xAxis
                        max: listModelData.linePoints.length - 1
                        tickInterval: 1
                        labelsVisible: false
                    }
                    axisY: ValueAxis {
                        id: yAxis
                        max: listModelData.maxValue
                        tickInterval: Math.abs(max) > Math.abs(min)? max / 3 : min / 3
                    }

                    LineSeries {
                        hoverable: isHoverable
                        Component.onCompleted: {
                            let points = listModelData.linePoints;
                            if (!listModelData.chartColorsEmpty)
                                color = listModelData.color;
                            if (points.length > 0)
                                append(points);
                            else
                                parent.visible = false;
                        }

                        onHoverEnter: (name, position, value) => {
                            linePopup.visible = true;
                            // adding in offsets so the popup is above the cursor
                            linePopup.x = position.x;
                            linePopup.y = position.y - 20;
                            linePopupText.text = value.y;
                        }

                        onHover: (name, position, value) => {
                            // adding in offsets so the popup is above the cursor
                            linePopup.x = position.x;
                            linePopup.y = position.y - 20;
                            linePopupText.text = value.y;
                        }

                        onHoverExit: {
                            linePopup.visible = false;
                        }

                        Popup {
                            id: linePopup
                            visible: false
                            modal: false
                            topInset: 0
                            bottomInset: 0
                            leftInset: 0
                            rightInset: 0

                            topPadding: 5
                            bottomPadding: 5
                            leftPadding: 5
                            rightPadding: 5

                            contentItem: Label {
                                id: linePopupText
                                anchors{
                                    horizontalCenter: parent.horizontalCenter
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                    }
                }
            }

            // Overlay text
            Rectangle {
                visible: model.title !== ""
                height: overlayTextLayout.height
                color: "white"
                opacity: 0.7

                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                }

                ColumnLayout {
                    id: overlayTextLayout
                    clip: true
                    focus: true
                    spacing: layoutSpacing

                    anchors.bottom: parent.bottom

                    Label {
                        visible: model.title !== ""
                        text: model.title
                        font.weight: Font.Bold
                        color: "black"
                        elide: Text.ElideRight

                        Layout.alignment: Qt.AlignHCenter
                        Layout.maximumWidth: delegatePopupMedia.width - imageTextMargin
                        Layout.leftMargin: imageTextMargin
                        Layout.rightMargin: imageTextMargin
                    }

                    Label {
                        visible: model.caption !== ""
                        text: model.caption
                        color: "black"
                        elide: Text.ElideRight

                        Layout.alignment: Qt.AlignHCenter
                        Layout.maximumWidth: delegatePopupMedia.width - imageTextMargin
                        Layout.leftMargin: imageTextMargin
                        Layout.rightMargin: imageTextMargin
                    }
                }
            }
        }
    }
}

