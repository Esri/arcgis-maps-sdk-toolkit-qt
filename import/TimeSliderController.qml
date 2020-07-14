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

import QtQml 2.12

import Esri.ArcGISRuntime 100.9

QtObject {
    id: timeSliderController

    /*!
      \brief The GeoView object this Controller uses.
    */
    property alias geoView: internal.geoView;

    readonly property alias numberOfSteps: internal.nSteps;

    readonly property alias startStep: internal.startStep;

    readonly property alias endStep: internal.endStep;

    signal stepsChanged()

    signal extentsChanged()

    function timeForStep(step) {
        const extent = internal.fullTimeExtent();
        if (!extent) {
            return null;
        }

        const interval = internal.timeInterval();
        if (!interval) {
            return null;
        }

        const intervalMS = internal.toMilliseconds(interval);
        return new Date(extent.startTime.getTime() + step * intervalMS);
    }

    function setSteps(startStep, endStep) {
        internal.startStep = startStep;
        internal.endStep = endStep;

        const extent = internal.fullTimeExtent();
        if (!extent) {
            return;
        }

        const interval = internal.timeInterval();
        if (!interval) {
            return;
        }

        const intervalMS = internal.toMilliseconds(interval);
        const startMS = extent.startTime.getTime();
        const s = new Date(startMS + startStep * intervalMS);
        const e = new Date(startMS + endStep * intervalMS);
        geoView.timeExtent = ArcGISRuntimeEnvironment.createObject(
                    "TimeExtent", { startTime: s, endTime: e });
        stepsChanged();
    }

    // Keep the heading read only for external consumers.
    /*! \internal */
    property QtObject internal: QtObject {
        id: internal
        property var geoView: null;
        property int nSteps: 0;
        property int startStep: 0;
        property int endStep: 0;

        onGeoViewChanged: initializeTimeProperties();

        property Connections geoViewConnection: Connections {
            target: geoView
            ignoreUnknownSignals: true
            onSceneChanged: internal.initializeTimeProperties();
            onMapChanged: internal.initializeTimeProperties();
        }

        property Connections sceneMapConnections: Connections {
            target: geoView ? (geoView.scene ? geoView.scene
                                             : (geoView.map ? geoView.map
                                                            : null))
                            : null
            ignoreUnknownSignals: true
            onOperationalLayersChanged: internal.initializeTimeProperties();
        }

        function initializeTimeProperties() {
            let layers = opLayers();
            if (layers) {
                for (let i = 0; i < layers.count; ++i) {
                    let layer = layers.get(i);
                    if (layer.loadStatus !== Enums.LoadStatusLoaded) {
                        layer.loadStatusChanged.connect(function() {
                            if (this.loadStatus === Enums.LoadStatusLoaded)
                                initializeTimeProperties();
                        }.bind(layer));
                    }
                }
            }

            internal.nSteps = numberOfSteps();
            timeSliderController.extentsChanged();

            const [s, e] = stepsForGeoViewExtent();
            startStep = s;
            endStep = e;
            timeSliderController.stepsChanged();
        }

        function opLayers() {
            if (geoView && geoView.scene) {
                return geoView.scene.operationalLayers;
            } else if (geoView && geoView.map) {
                return geoView.map.operationalLayers;
            } else {
                return null;
            }
        }

        function fullTimeExtent() {
            return accumalateTimeAware(opLayers(), (extent, layer) => {
               let f = layer.fullTimeExtent;
               if (!extent) {
                   return f;
               } else if (!f) {
               return extent;
               } else {
                   return ArcGISRuntimeEnvironment.createObject("TimeExtent", {
                        startTime: min(extent.startTime, f.startTime),
                        endTime: max(extent.endTime, f.endTime)
                    });
               }
            });
        }

        function timeInterval() {
            return accumalateTimeAware(opLayers(), (a, layer) => {
               let b = layer.timeInterval;
               if (!a) {
                   return b;
               } else if (!b) {
                   return a;
               } else {
                   if (a.unit === b.unit) {
                       return a.duration < b.duration ? a : b;
                   } else {
                       return toMilliSeconds(a) < toMilliSeconds(b) ? a : b
                   }
               }
           });
        }

        function numberOfSteps() {
            const extent = fullTimeExtent();
            if (!extent) {
                return 0;
            }

            const interval = timeInterval();
            if (!interval) {
                return 0;
            }

            const range = extent.endTime.getTime() - extent.startTime.getTime();
            const intervalMS = internal.toMilliseconds(interval);
            return Math.ceil(range / intervalMS);
        }

        function accumalateTimeAware(layerList, op) {
            if (!layerList) {
                return null;
            }

            let layers = [];
            for (let i = 0; i < layerList.count; i++) {
                layers.push(layerList.get(i));
            }

            return layers.reduce((val, layer) => {
                 if (!layer) {
                    return val;
                 }

                 if (layer.loadStatus !== Enums.LoadStatusLoaded) {
                    return val;
                 }

                 if (!layer.timeFilteringEnabled) {
                    return val;
                 }

                 // TODO test layer visibility here.

                 return op(val, layer);
            }, null);
        }

        /*!
          \internal
          \brief Converts a TimeValue to the number of milliseconds elapsed.
         */
        function toMilliseconds(timeValue) {
            switch (timeValue.unit)
            {
            case Enums.TimeUnitCenturies:
                return timeValue.duration * 86400000 * 36500.0;
            case Enums.TimeUnitDecades:
                return timeValue.duration * 86400000 * 3650.0;
            case Enums.TimeUnitYears:
                return timeValue.duration * 86400000 * 365.0;
            case Enums.TimeUnitMonths:
                return timeValue.duration * (365.0 / 12) * 86400000;
            case Enums.TimeUnitWeeks:
                return timeValue.duration * 604800000;
            case Enums.TimeUnitDays:
                return timeValue.duration * 86400000;
            case Enums.TimeUnitHours:
                return timeValue.duration * 3600000;
            case Enums.TimeUnitMinutes:
                return timeValue.duration * 60000;
            case Enums.TimeUnitSeconds:
                return timeValue.duration * 1000;
            case Enums.TimeUnitMilliseconds:
                return timeValue.duration;
            default:
                return timeValue.duration;
            }
        }

        function stepsForGeoViewExtent() {
            if (!geoView) {
              return [0, 0];
            }

            const extent = fullTimeExtent();
            if (!extent) {
              return [0, 0];
            }

            const interval = timeInterval();
            if (!interval) {
              return [0, 0];
            }

            const intervalMS = toMilliseconds(interval);
            const geoExtent = geoView.timeExtent;
            const geoS = geoExtent ? geoExtent.startTime.getTime() : NaN;
            const geoE = geoExtent ? geoExtent.endTime.getTime() : NaN;
            if (!geoExtent || isNaN(geoS) || isNaN(geoE)) {
              const r = extent.endTime.getTime() - extent.startTime.getTime();
              return [0, Math.ceil(r / intervalMS)];
            }

            return [
                Math.ceil((geoS - extent.startTime.getTime()) / intervalMS),
                Math.ceil((extent.endTime.getTime() - geoE) / intervalMS)
            ];
        }
    }
}
