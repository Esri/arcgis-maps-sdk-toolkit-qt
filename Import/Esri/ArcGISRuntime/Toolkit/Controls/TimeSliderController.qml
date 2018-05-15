/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

import QtQuick 2.4
import Esri.ArcGISRuntime 100.3

/*!
    \qmltype TimeSlider
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.3
    \brief
*/
Item {
    id: controller

    property GeoView geoView
    property bool isMapView: geoView && geoView.objectType === "MapView"
    property var operationalLayers: geoView ? (isMapView ? geoView.map.operationalLayers : geoView.scene.operationalLayers) : null
    property var timeAwareLayers: []
    property int numberOfSteps: 20
    property TimeValue timeStepInterval
    property int intervalMS: -1
    property TimeExtent fullExtent
    property TimeExtent currentExtent: geoView && geoView.timeExtent ? geoView.timeExtent : fullExtent
    property int initialStartStep: -1
    property int initialEndStep:  -1

    function setStartAndEndIntervals(startIndex, endIndex) {
        if (!fullExtent)
            return;

        var start = fullExtent.startTime.getTime();
        var newStart = new Date(start + (startIndex * intervalMS));
        var newEnd = new Date(start + (endIndex * intervalMS));

        var newExtent = ArcGISRuntimeEnvironment.createObject("TimeExtent", {"startTime": newStart, "endTime": newEnd});
        geoView.timeExtent = newExtent;
    }

    function setStartInterval(intervalIndex) {
        if (!fullExtent)
            return;

        var start = fullExtent.startTime.getTime();
        var currentStart = new Date(start + (intervalIndex * intervalMS));

        var currentEndTime = currentExtent.endTime;

        var newExtent = ArcGISRuntimeEnvironment.createObject("TimeExtent", {"startTime": currentStart, "endTime": currentEndTime});
        geoView.timeExtent = newExtent;
    }

    function setEndInterval(intervalIndex) {
        if (!fullExtent)
            return;

        var start = fullExtent.startTime.getTime();
        var currentEnd = new Date(start + (intervalIndex * intervalMS));
        var currentStart = currentExtent.startTime;

        var newExtent = ArcGISRuntimeEnvironment.createObject("TimeExtent", {"startTime": currentStart, "endTime": currentEnd});
        geoView.timeExtent = newExtent;
    }

    onOperationalLayersChanged: initializeTimeProperties();

    //    Initializes time-related properties as follows:
    //    FullExtent will be the union of all time extents from ITimeAware layers in the GeoView where IsTimeFilteringEnabled is set to true.
    //    TimeStepInterval will be set to the largest interval specified by the metadata of any ITimeAware layer in the view,
    //    unless that results in ticks being too close together.
    //    If the ticks are too close, then it will be auto-calculated to a value that results in ticks being a reasonable distance apart.
    function initializeTimeProperties() {
        if (!operationalLayers)
            return;

        timeAwareLayers = [];
        // Get all the layers that are visible and are participating in time-based filtering

        for (var i = 0 ; i < operationalLayers.count; i++) {
            var layer = operationalLayers.get(i);
            if (layer.loadStatus !== Enums.LoadStatusLoaded && layer.loadStatus !== Enums.LoadStatusFailedToLoad) {
                layer.loadStatusChanged.connect(initializeTimeProperties);
                continue;
            }

            if (validLayer(layer))
                timeAwareLayers.push(layer);
        }

        handleTimeAwareLayers();

        if (!fullExtent || !currentExtent)
            return;

        var start = fullExtent.startTime.getTime();
        var end = fullExtent.endTime.getTime();

        var stMs = currentExtent.startTime.getTime() - start;
        var endMS = currentExtent.endTime.getTime() - start;

        initialStartStep = stMs / intervalMS;
        initialEndStep = endMS / intervalMS;
    }

    function validLayer(layer) {
        return layer && layer.visible && layer.timeFilteringEnabled;
    }

    function handleTimeAwareLayers() {
        timeStepInterval = null;
        for (var i = 0 ; i < operationalLayers.count; i++) {
            var layer = timeAwareLayers[i];
            if (!layer)
                continue;

            fullExtent = union(layer.fullTimeExtent, fullExtent);

            if (timeStepInterval === null)
                timeStepInterval = layer.timeInterval;
            else if (isGreaterThan(layer.timeInterval, timeStepInterval))
                timeStepInterval = layer.timeInterval;
        }

        calculateNumberOfSteps(fullExtent, timeStepInterval);
    }

    function isGreaterThan(timeValue, otherTimeValue) {
        if (timeValue.unit === otherTimeValue.unit)
            return timeValue.duration > otherTimeValue.duration;
        else
            return toMilliseconds(timeValue) > toMilliseconds(otherTimeValue);
    }

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

    function union(timeExtent, otherTimeExtent) {
        if (otherTimeExtent === null)
            return timeExtent;

        var startTime = timeExtent.startTime < otherTimeExtent.startTime ? timeExtent.startTime : otherTimeExtent.startTime;
        var endTime = timeExtent.endTime > otherTimeExtent.endTime ? timeExtent.endTime : otherTimeExtent.endTime;

        var properties = startTime === endTime ? {"startTime" : startTime}: {"startTime" : startTime, "endTime" : endTime};

        return ArcGISRuntimeEnvironment.createObject("TimeExtent", properties);
    }

    function calculateNumberOfSteps(timeExtent, timeValue) {
        if (!timeExtent)
            return;

        var start = timeExtent.startTime.getTime();
        var end = timeExtent.endTime.getTime();
        var range = end - start;

        intervalMS = toMilliseconds(timeValue);

        numberOfSteps = range / intervalMS;
    }
}
