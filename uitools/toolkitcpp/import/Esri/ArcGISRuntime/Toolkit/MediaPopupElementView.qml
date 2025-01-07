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


ColumnLayout {
    id: mediaPopupElementView
    height: childrenRect.height
    clip: true
    focus: true
    Layout.fillWidth: true
    spacing: layoutSpacing

    property real mediaMargin: 10
    property real imageTextMargin: 5
    property real layoutSpacing: 0

    /*!
      \internal
      This is an internal implementation detail and is subject to change.

      \qmlproperty MediaPopupElementView controller
      \brief The Controller handles reading from the MediaPopupElement.

      \sa Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController
    */
    property var controller: null

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
            text: mediaPopupElementView.controller.title !== "" ? controller.title : "Media"
            wrapMode: Text.WordWrap
            font.pixelSize: 20
            font.weight: Font.Black

            Layout.fillWidth: true
            Layout.leftMargin: mediaPopupElementView.mediaMargin
            Layout.rightMargin: mediaPopupElementView.mediaMargin
        }
        Label {
            text: mediaPopupElementView.controller.description
            visible: mediaPopupElementView.controller.description !== ""
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
        model: mediaPopupElementView.controller.popupMediaItems

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
                        // BarSeries will take owership of the QList<QBarSet*> when we call append. This also applies to PieSeries and QPieSlice for PieChartPopupMediaItem.
                        // s.a https://doc.qt.io/qt-6/qbarseries.html#append-1
                        Component.onCompleted: {
                            let sets = listModelData.barSets;
                            if (sets.length > 0)
                                append(sets);
                            else
                                parent.visible = false;
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
                        // BarSeries will take owership of the QList<QBarSet*> when we call append. This also applies to PieSeries and QPieSlice for PieChartPopupMediaItem.
                        // s.a https://doc.qt.io/qt-6/qbarseries.html#append-1
                        Component.onCompleted: {
                            let sets = listModelData.barSets;
                            if (sets.length > 0)
                                append(sets);
                            else
                                parent.visible = false;
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
                        // PieSeries will take owership of the QList<QPieSlice*> when we call append. This also applies to BarSeries and QBarSet for BarChartPopupMediaItem.
                        // s.a https://doc.qt.io/qt-6/qpieseries.html#append-1
                        Component.onCompleted: {
                            let slices = listModelData.pieSlices;
                            if (slices.length > 0)
                                append(slices);
                            else
                                parent.visible = false;
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

                        Component.onCompleted: {
                            let points = listModelData.linePoints;
                            if (listModelData.colorFromJsonFound)
                                color = listModelData.color;
                            if (points.length > 0)
                                append(points);
                            else
                                parent.visible = false;
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
                    height: childrenRect.height
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

