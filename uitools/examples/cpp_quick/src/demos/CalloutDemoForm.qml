// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISRuntime.Toolkit 100.13
import DemoApp 1.0

DemoPage {
    mapViewContents: Component {
        MapView {
            id:view

            Callout {
                id: callout
                calloutData : view.calloutData //binding to parent. Any change is reflected on this
                accessoryButtonVisible: false
            }
            CalloutDemo {
                geoView: view
            }

            Page {
                title: "Leader position"
                header: Label {
                    text: parent.title
                }
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    margins: 5
                }
                contentItem: ColumnLayout {
                    Button {
                        Layout.fillWidth: true
                        text: "Upper left"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.UpperLeft
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Top"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.Top
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Upper right"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.UpperRight
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Right"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.Right
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Lower right"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.LowerRight
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Bottom"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.Bottom
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Lower left"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.LowerLeft
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Left"
                        checkable: true
                        autoExclusive: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.Left
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "Automatic"
                        checkable: true
                        autoExclusive: true
                        checked: true
                        onCheckedChanged: {
                            if (checked)
                                callout.leaderPosition = Callout.LeaderPosition.Automatic
                        }
                    }
                }
            }
        }
    }
}
