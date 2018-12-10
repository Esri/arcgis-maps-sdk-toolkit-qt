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
    \qmltype TimeSliderController
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.3
    \brief The controller for the Time Slider tool.

    The controller presents the temporal range of the data as a number of steps.
    These steps allow the temporal extent to be set and animated by stepping through
    the range.

    \note The controller will be automatically created by a TimeSlider
    so you do not need to create this type.
*/
Item {
    id: controller

    signal currentTimeExtentChanged

    /*!
      \qmlproperty GeoView geoView
      \brief the GeoView for this tool to \a geoView.

      The view should be either a MapView or a SceneView.

      \note This property will be provided by the TimeSlider so you do not need
      to set this.
      */
    property GeoView geoView

    /*!
     \internal
     */
    property bool isMapView: geoView && geoView.objectType === "MapView"

    /*!
     \internal
     */
    property var operationalLayers: geoView ? (isMapView ? geoView.map.operationalLayers : geoView.scene.operationalLayers) : null

    /*!
     \internal
     */
    property var timeAwareLayers: []

    /*!
     \qmlproperty int numberOfSteps
     \brief The total number of required steps to cover the full time extent (read-only).

     This figure is based on the full temporal range of the data in the geoView
     and the time intervals used by the data.
     */
    property int numberOfSteps: -1

    /*!
     \internal
     */
    property TimeValue timeStepInterval

    /*!
     \internal
     */
    property int intervalMS: -1

    /*!
     \qmlproperty TimeExtent fullExtent
     \brief The full time extent of the data in the current geoView (read-only).
     */
    property TimeExtent fullExtent

    /*!
     \qmlproperty date fullExtentStart
     \brief The start time of the data in the current geoView. (read-only).
     */
    property var fullExtentStart: fullExtent ? fullExtent.startTime : null

    /*!
     \qmlproperty date fullExtentEnd
     \brief The end time of the data in the current geoView (read-only).
     */
    property var fullExtentEnd: fullExtent ? fullExtent.endTime : null

    /*!
     \qmlproperty TimeExtent currentExtent
     \brief The current time extent of the data in the current geoView (read-only).
     */
    property TimeExtent currentExtent: geoView && geoView.timeExtent ? geoView.timeExtent : fullExtent

    /*!
     \qmlproperty date currentExtentStart
     \brief The start time of the current temporal extent of the geoView (read-only).
     */
    property var currentExtentStart: currentExtent ? currentExtent.startTime : null

    /*!
     \qmlproperty date currentExtentEnd
     \brief The end time of the current temporal extent of the geoView (read-only).
     */
    property var currentExtentEnd: currentExtent ? currentExtent.endTime : null

    /*!
     \qmlproperty int startStep
     \brief The start step of the current time extent (read-only).
     */
    property int startStep: currentExtent && fullExtent ? (currentExtent.startTime.getTime() - fullExtent.startTime.getTime()) / intervalMS : -1

    /*!
     \qmlproperty int endStep
     \brief The end step of the current time extent (read-only).
     */
    property int endStep: currentExtent && fullExtent ? (currentExtent.endTime.getTime() - fullExtent.startTime.getTime()) / intervalMS : -1

    property var stepTimes: []

    function setStartAndEndIntervals(startIndex, endIndex) {
        if (!fullExtent)
            return;

        var start = fullExtent.startTime.getTime();
        var newStart = new Date(start + (startIndex * intervalMS));
        var newEnd = new Date(start + (endIndex * intervalMS));

        var newExtent = ArcGISRuntimeEnvironment.createObject("TimeExtent", {"startTime": newStart, "endTime": newEnd});
        geoView.timeExtent = newExtent;

        currentTimeExtentChanged();
    }

    function setStartInterval(intervalIndex) {
        if (!fullExtent)
            return;

        var start = fullExtent.startTime.getTime();
        var currentStart = new Date(start + (intervalIndex * intervalMS));

        var currentEndTime = currentExtent.endTime;

        var newExtent = ArcGISRuntimeEnvironment.createObject("TimeExtent", {"startTime": currentStart, "endTime": currentEndTime});
        geoView.timeExtent = newExtent;

        currentTimeExtentChanged();
    }

    function setEndInterval(intervalIndex) {
        if (!fullExtent)
            return;

        var start = fullExtent.startTime.getTime();
        var currentEnd = new Date(start + (intervalIndex * intervalMS));
        var currentStart = currentExtent.startTime;

        var newExtent = ArcGISRuntimeEnvironment.createObject("TimeExtent", {"startTime": currentStart, "endTime": currentEnd});
        geoView.timeExtent = newExtent;

        currentTimeExtentChanged();
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

        // Get all the layers that are visible and are participating in time-based filtering
        timeAwareLayers = [];
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
    }

    /*! \internal */
    function validLayer(layer) {
        return layer && layer.visible && layer.timeFilteringEnabled;
    }

    /*! \internal */
    function handleTimeAwareLayers() {
        timeStepInterval = null;
        for (var i = 0 ; i < timeAwareLayers.length; i++) {
            var layer = timeAwareLayers[i];
            if (!layer)
                continue;

            fullExtent = union(layer.fullTimeExtent, fullExtent);

            if (timeStepInterval === null)
                timeStepInterval = layer.timeInterval;
            else if (isGreaterThan(layer.timeInterval, timeStepInterval))
                timeStepInterval = layer.timeInterval;
        }

        if (!fullExtent)
            return;

        if (timeStepInterval === null || timeStepInterval.unit === Enums.TimeUnitUnknown) {
            var start = fullExtent.startTime.getTime();
            var end = fullExtent.endTime.getTime();
            var range = end - start;

            var estimatedUnit = toTimeUnit(range);
            timeStepInterval = ArcGISRuntimeEnvironment.createObject("TimeValue", {"unit": estimatedUnit, "duration": 1.0});
        }

        calculateNumberOfSteps(fullExtent, timeStepInterval);
        setStepTimes();
    }

    /*! \internal */
    function isGreaterThan(timeValue, otherTimeValue) {
        if (timeValue.unit === otherTimeValue.unit)
            return timeValue.duration > otherTimeValue.duration;
        else
            return toMilliseconds(timeValue) > toMilliseconds(otherTimeValue);
    }

    /*! \internal */
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

    /*! \internal */
    function union(timeExtent, otherTimeExtent) {
        if (otherTimeExtent === null)
            return timeExtent;

        var startTime = timeExtent.startTime < otherTimeExtent.startTime ? timeExtent.startTime : otherTimeExtent.startTime;
        var endTime = timeExtent.endTime > otherTimeExtent.endTime ? timeExtent.endTime : otherTimeExtent.endTime;

        var properties = startTime === endTime ? {"startTime" : startTime}: {"startTime" : startTime, "endTime" : endTime};

        return ArcGISRuntimeEnvironment.createObject("TimeExtent", properties);
    }

    /*! \internal */
    function calculateNumberOfSteps(timeExtent, timeValue) {
        if (!timeExtent)
            return;

        var start = timeExtent.startTime.getTime();
        var end = timeExtent.endTime.getTime();
        var range = end - start;

        intervalMS = toMilliseconds(timeValue);

        numberOfSteps = (range / intervalMS) + 1;
    }

    /*! \internal */
    function setStepTimes() {
        var tempStepTimes = [];

        var startMs = fullExtentStart.getTime();
        for (var i = 0; i < numberOfSteps; ++i)
          tempStepTimes.push( new Date(startMs + (i * intervalMS)) );

        stepTimes = tempStepTimes;
    }

    /*! \internal */
    function toTimeUnit() {
        var daysPerYear = 365.0;
        var millisecondsPerDay = 86400000.0;
        var millisecondsPerHour = 3600000.0;
        var millisecondsPerMinute = 60000.0;

        if (milisecondsRange < millisecondsPerMinute)
          return Enums.TimeUnitSeconds;
        else if (milisecondsRange < millisecondsPerHour)
          return Enums.TimeUnitMinutes;
        else if (milisecondsRange < millisecondsPerDay)
          return Enums.TimeUnitHours;
        else if (milisecondsRange < (millisecondsPerDay * daysPerYear))
          return Enums.TimeUnitDays;
        else if (milisecondsRange > (millisecondsPerDay * daysPerYear * 100))
          return Enums.TimeUnitCenturies;
        else
          return Enums.TimeUnitYears;
    }
}
