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

/*!
   \qmltype TimeSliderController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.9
   \brief In MVC architecture, this is the controller for the corresponding
   TimeSlider.
   
  This controller calculates interval steps and the range of the full extent for
  the TimeSlider. This is based on the combined extents of time-aware layers
  in the given \c GeoView.

  The time-extent of the GeoView itself can be manipulated using steps with
  calls to \l setSteps. 

  Here is an example of how to use the TimeSlider from QML.
    \code
        import "qrc:///Esri/ArcGISRuntime/Toolkit" as Toolkit
        // add a mapView component (the geoView)
        MapView {
            anchors.fill: parent
            id: mapView
            Map {
                ...
            }
            // declare a TimeSlider and bind it to the geoView
            Toolkit.TimeSlider {
                id: timeSlider
                anchors {
                    left: mapView.left
                    right: mapView.right
                    bottom: mapView.attributionTop
                }
                geoView: mapView
            }
        }
    \endcode
 */
QtObject {
    id: timeSliderController

    /*!
      \brief The GeoView object this Controller uses.
    */
    property alias geoView: internal.geoView;

    /*!
    \brief The number of steps the TimeSlider should display based on 
    on the fullTimeExtent dividied by smallest timeInterval.
    */
    readonly property alias numberOfSteps: internal.nSteps;

    /*!
    \brief The current start step.
    */
    readonly property alias startStep: internal.startStep;

    /*!
    \brief The current end step.
    */
    readonly property alias endStep: internal.endStep;

    /*!
    \brief Emitted when either the start or end step changes.
    */
    signal stepsChanged()

    /*!
    \brief Emitted when the extents of any \c TimeAware layer changes.
    */
    signal extentsChanged()

    /*!
    \brief Calculates a \c date from a step.

    Given "step" and \l numberOfSteps, we can calculate the date-time for
    an arbitrary step interpolated between the start and end times of
    the full time extent.
    
    \list
    \li step Step to calculate a time for.
    \endlist

    Returns a \c date that is the time calculated for the step.
    */
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

    /*!
      \brief Sets the current steps.
  
      Setting steps changes the the current time-extent of the \c GeoView to a 
      \c TimeExtent range calculated by the current steps using \l timeForStep.
      
      \list
      \li startStep start-step to set.
      \li endStep end-step to set.
      \endlist
    */
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

    /*! \internal */
    property QtObject internal: QtObject {
        id: internal
        property var geoView: null;
        property int nSteps: 0;
        property int startStep: 0;
        property int endStep: 0;

        // Recalculate on any geoview changes.
        onGeoViewChanged: initializeTimeProperties();

        // Recalculate on any scene/map changes
        property Connections geoViewConnection: Connections {
            target: geoView
            ignoreUnknownSignals: true
            function onSceneChanged() {
                internal.initializeTimeProperties();
            }
            function onMapChanged() {
                internal.initializeTimeProperties();
            }
        }

        // Recalculate on any operational layer changes.
        property Connections sceneMapConnections: Connections {
            target: geoView ? (geoView.scene ? geoView.scene
                                             : (geoView.map ? geoView.map
                                                            : null))
                            : null
            ignoreUnknownSignals: true
            function onOperationalLayersChanged() {
                internal.initializeTimeProperties();
            }
        }

        // Grabs the operational layers from the geoview.
        function opLayers() {
            if (geoView && geoView.scene) {
                return geoView.scene.operationalLayers;
            } else if (geoView && geoView.map) {
                return geoView.map.operationalLayers;
            } else {
                return null;
            }
        }

        /*
        \internal
        \brief Starts tracking the 
        layers in the current geoView, which are now used for calculating 
        full-time-extent and smallest time-interval.
        */
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

        /*!
        \internal
        \brief Calculates the union of all extents of all \c TimeAware layers in
        the current \c GeoView.

        Returns a \c TimeExtent covering the combined range of all extents.
        */
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

        /*!
        \internal
        \brief Calculates the time-interval as a \c TimeValue, which is the 
        smallest possible interval of all \c TimeAware layers in the current 
        \c GeoView.
        
        Returns a \c TimeValue which is the minimum time-interval of all 
        intervals.
        */
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

        /*!
        \internal
        \brief Calculates the number of steps the TimeSlider should display 
        based on \l fullTimeExtent dividied by \l timeInterval.

        Returns number of steps for TimeSlider.
        */
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

        /*
        \internal
        \brief Calls `reduce` on a \c LayerListModel, but filters out
                layers in the list-model that are not TimeAware.
        \list
        \li \a layerList ListModel to accummalate.
        \li \a op Accumulation function used for reduce.
        \endlist 
        Returns the accumulation.
        */
        function accumalateTimeAware(layerList, op) {
            if (!layerList) {
                return null;
            }

            let layers = [];
            layerList.forEach(opLayer => layers.push(opLayer));

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

            const millisecondsPerDay = 86400000.0;
            const daysPerCentury = 36500.0;
            const daysPerDecade = 3650.0;
            const daysPerYear = 365.0;
            const mothsPerYear = 12;
            const millisecondsPerHour = 3600000.0;
            const millisecondsPerMinute = 60000.0;
            const millisecondsPerSecond = 1000.0;
            const millisecondsPerWeek = 604800000;

            switch (timeValue.unit)
            {
            case Enums.TimeUnitMilliseconds:
                return timeValue.duration;
            case Enums.TimeUnitCenturies:
                return timeValue.duration * millisecondsPerDay * daysPerCentury;
            case Enums.TimeUnitDecades:
                return timeValue.duration * millisecondsPerDay * daysPerDecade;
            case Enums.TimeUnitYears:
                return timeValue.duration * millisecondsPerDay * daysPerYear;
            case Enums.TimeUnitMonths:
                return timeValue.duration * (daysPerYear / mothsPerYear) * millisecondsPerDay;
            case Enums.TimeUnitWeeks:
                return timeValue.duration * millisecondsPerWeek;
            case Enums.TimeUnitDays:
                return timeValue.duration * millisecondsPerDay;
            case Enums.TimeUnitHours:
                return timeValue.duration * millisecondsPerHour;
            case Enums.TimeUnitMinutes:
                return timeValue.duration * millisecondsPerMinute;
            case Enums.TimeUnitSeconds:
                return timeValue.duration * millisecondsPerSecond;
            default:
                return timeValue.duration;
            }
        }

        /*!
        \internal
        \brief Calculates the start and end steps derived from the \c GeoView 
        object's current \c TimeExtent.

        When \c GeoView has an empty extent, this is equivalent to 
        "show everything" and the steps returned are 
        \c{[0, numberOfSteps]} (inclusive).

        Returns a pair of steps derived from the current \c GeoView time-extent.
        \sa TimeSliderController::timeForStep
        */
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
