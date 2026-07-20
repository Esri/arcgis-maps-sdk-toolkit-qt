/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
import QtQuick
import QtQuick.Controls
import Esri.Examples
import Esri.ArcGISRuntime.Toolkit
import DemoApp

DemoPage {
    mapViewContents: Component {
        MapView {
            id: view

            function addTestStartingPoint() {
                const traceTool = traceToolLoader.item
                if (demo.testFeature && traceTool && traceTool.controller.selectedUtilityNetwork) {
                    const terminalName = testCasePicker.currentIndex === 0 ? "XFR:High" : "High"
                    traceTool.controller.addStartingPoint(demo.testFeature, terminalName)
                }
            }

            ComboBox {
                id: testCasePicker
                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 10
                }
                model: [qsTr("Named function labels"), qsTr("Fallback labels")]
                onActivated: {
                    traceToolLoader.active = false
                    demo.loadTestCase(currentIndex)
                    Qt.callLater(function() {
                        traceToolLoader.active = true
                    })
                }
            }

            Loader {
                id: traceToolLoader
                anchors {
                    left: parent.left
                    top: parent.top
                    margins: 10
                }
                active: true

                sourceComponent: UtilityNetworkTrace {
                    geoView: view
                }
            }

            Connections {
                target: traceToolLoader.item ? traceToolLoader.item.controller : null

                function onTraceConfigurationNamesChanged() {
                    const configurationName = testCasePicker.currentIndex === 0
                        ? "Upstream with Function Results"
                        : "Downstream Trace"
                    Qt.callLater(function() {
                        if (traceToolLoader.item)
                            traceToolLoader.item.selectTraceConfiguration(configurationName)
                    })
                }

                function onSelectedUtilityNetworkChanged() {
                    view.addTestStartingPoint()
                }
            }

            Connections {
                target: traceToolLoader.item ? traceToolLoader.item.controller.selectedUtilityNetwork : null

                function onDoneLoading() {
                    view.addTestStartingPoint()
                }
            }

            UtilityNetworkTraceDemo {
                id: demo
                geoView: view

                onTestFeatureChanged: view.addTestStartingPoint()
            }
        }
    }
}
