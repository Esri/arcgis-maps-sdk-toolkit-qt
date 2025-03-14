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

    property var controller: null

    Dialog {
        id: fullScreenImageDialog
        modal: true
        visible: false
        width: Overlay.overlay ? Overlay.overlay.width : 0
        height: Overlay.overlay ? Overlay.overlay.height : 0
        anchors.centerIn: Overlay.overlay

        Loader {
            id: chartsLoader
            anchors{
                top: parent.top
                bottom: closeButton.top
                left: parent.left
                right: parent.right
            }
        }

        Button {
            id: closeButton
            text: "Close"
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            onClicked: fullScreenImageDialog.visible = false;
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
                id: loader
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    // if (popupMediaType === QmlEnums.PopupMediaTypeImage) {
                        fullScreenImageDialog.visible = true;
                        chartsLoader.sourceComponent = null;
                        chartsLoader.sourceComponent = loader.sourceComponent;
                    // }
                }
            }

            Component.onCompleted: {
                switch (popupMediaType) {
                    case QmlEnums.PopupMediaTypeImage:
                        loader.sourceComponent = imageComp;
                        break;
                    case QmlEnums.PopupMediaTypeColumnChart:
                        loader.sourceComponent = columnChartComp;
                        break;
                    case QmlEnums.PopupMediaTypeBarChart:
                        loader.sourceComponent = barChartComp;
                        break;
                    case QmlEnums.PopupMediaTypePieChart:
                        loader.sourceComponent = pieChartComp;
                        break;
                    case QmlEnums.PopupMediaTypeLineChart:
                        loader.sourceComponent = lineChartComp;
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

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // saving this as a placeholder. Should fire an event the user could take otherwise
                            // we define our own default behavior to open the image in full screen mode.
                            // Qt.openUrlExternally(model.linkUrl);
                        }
                    }
                }
            }

            Component {
                id: columnChartComp
                GraphsView {
                    height: delegatePopupMedia.height
                    width: delegatePopupMedia.width
                    marginTop: 10
                    marginBottom: 10
                    marginLeft: 10
                    marginRight: 10

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

                        onHoverEnter: {
                            columnPopup.visible = true;
                            columnPopup.x = position.x + 10;
                            columnPopup.y = position.y + 10;
                        }

                        onHover: {
                            columnPopup.x = position.x + 10;
                            columnPopup.y = position.y + 10;
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
                    height: delegatePopupMedia.height
                    width: delegatePopupMedia.width
                    marginTop: 10
                    marginBottom: 10
                    marginLeft: -50

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

                        onHoverEnter: {
                            barPopup.visible = true;
                            barPopup.x = position.x + 20;
                            barPopup.y = position.y + 20;
                        }

                        onHover: {
                            barPopup.x = position.x + 20;
                            barPopup.y = position.y + 20;
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
                        name: "PieSeries"
                        hoverable: isHoverable
                        // PieSeries will take owership of the QList<QPieSlice*> when we call append. This also applies to BarSeries and QBarSet for BarChartPopupMediaItem.
                        // s.a https://doc.qt.io/qt-6/qpieseries.html#append-1

                        PieSlice {
                            value: 14179
                        }
                        PieSlice {
                            value: 9832
                        }

                        // Component.onCompleted: {
                        //     let slices = listModelData.pieSlices;
                        //     if (slices.length > 0)
                        //         append(slices);
                        //     else
                        //         parent.visible = false;
                        // }

                        onHoverEnter: {
                            popup.visible = true;
                            popup.x = position.x + 20;
                            popup.y = position.y + 20;
                        }

                        onHover: {
                            popup.x = position.x + 20;
                            popup.y = position.y + 20;
                            dynamicText.text = value.y;
                        }

                        onHoverExit: {
                            popup.visible = false;
                        }

                        Popup {
                            id: popup
                            visible: false
                            modal: false
                            height: dynamicText.height
                            width: dynamicText.width

                            Rectangle {
                                anchors.fill: parent
                                color: "white"
                                border.color: "black"
                                height: dynamicText.height
                                width: dynamicText.width
                                Label {
                                    id: dynamicText
                                    anchors.centerIn: parent
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
                    marginTop: 10
                    marginBottom: 10
                    marginLeft: 10
                    marginRight: 10

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
                        name: "LineSeries"
                        Component.onCompleted: {
                            let points = listModelData.linePoints;
                            if (!listModelData.chartColorsEmpty)
                                color = listModelData.color;
                            if (points.length > 0)
                                append(points);
                            else
                                parent.visible = false;
                        }

                        onHoverEnter: {
                            linePopup.visible = true;
                            linePopup.x = position.x + 20;
                            linePopup.y = position.y + 20;
                        }

                        onHover: {
                            linePopup.x = position.x + 20;
                            linePopup.y = position.y + 20;
                            linePopupText.text = value.y;
                        }

                        onHoverExit: {
                            linePopup.x = position.x + 20;
                            linePopup.y = position.y + 20;
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

