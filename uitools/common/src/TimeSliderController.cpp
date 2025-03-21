
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
#include "TimeSliderController.h"

// ArcGISRuntime Toolkit headers
#include "GeoViews.h"

// ArcGISRuntime headers
#include <GeoView.h>
#include <Layer.h>
#include <LayerListModel.h>
#include <Map.h>
#include <MapTypes.h>
#include <Scene.h>
#include <ServiceTypes.h>
#include <TimeAware.h>
#include <TimeValue.h>

// std headers
#include <cmath>

// Qt headers
#include <QDateTime>
#include <QDebug>

namespace Esri::ArcGISRuntime::Toolkit {

namespace
{
  /*
   \internal
   \brief Calls `std::accumlate` on a \c LayerListModel, but filters out
          layers in the list-model that are not TimeAware.
          The type \c T must be default-constructible.
   \list
   \li \a listModel ListModel to accummalate.
   \li \a f Accumulation function that takes the current \c T and a \c TimeAware
       object.
   \endlist
   Returns the accumulation of \c T.
   */
  template <class T, class BinaryOperation>
  T accumulateTimeAware(LayerListModel* listModel, BinaryOperation f)
  {
    if (!listModel)
      return T{};

    return std::accumulate(
      listModel->cbegin(),
      listModel->cend(),
      T{},
      [f = std::move(f)](const T& val, Layer* layer)
      {
        if (!layer || layer->loadStatus() != LoadStatus::Loaded)
          return val;

        auto timeAware = dynamic_cast<TimeAware*>(layer);
        if (!timeAware || !timeAware->isTimeFilteringEnabled())
          return val;

        // TODO test for visible here.

        return f(val, timeAware);
      }
    );
  }

  /*
   \internal
   \brief Converts a \c TimeValue to a double in milliseconds.
   \list
   \li \a timeValue \c TimeValue to convert.
   \endlist
   Returns TimeValue as double in milliseconds.
   */
  double toMilliseconds(const TimeValue& timeValue)
  {
    constexpr double millisecondsPerDay = 86400000.0;
    constexpr double daysPerCentury = 36500.0;
    constexpr double daysPerDecade = 3650.0;
    constexpr double daysPerYear = 365.0;
    constexpr int mothsPerYear = 12;
    constexpr double millisecondsPerHour = 3600000.0;
    constexpr double millisecondsPerMinute = 60000.0;
    constexpr double millisecondsPerSecond = 1000.0;
    constexpr double millisecondsPerWeek = 604800000;

    switch (timeValue.unit())
    {
    case TimeUnit::Milliseconds:
        return timeValue.duration();
    case TimeUnit::Centuries:
        return timeValue.duration() * millisecondsPerDay * daysPerCentury;
    case TimeUnit::Decades:
        return timeValue.duration() * millisecondsPerDay * daysPerDecade;
    case TimeUnit::Years:
        return timeValue.duration() * millisecondsPerDay * daysPerYear;
    case TimeUnit::Months:
        return timeValue.duration() * (daysPerYear / mothsPerYear) * millisecondsPerDay;
    case TimeUnit::Weeks:
        return timeValue.duration() * millisecondsPerWeek;
    case TimeUnit::Days:
        return timeValue.duration() * millisecondsPerDay;
    case TimeUnit::Hours:
        return timeValue.duration() * millisecondsPerHour;
    case TimeUnit::Minutes:
        return timeValue.duration() * millisecondsPerMinute;
    case TimeUnit::Seconds:
        return timeValue.duration() * millisecondsPerSecond;
    default:
        return timeValue.duration();
    }
  }

  /*
   \internal
   \brief Compares two \c TimeValue objects and returns the smallest one.
   \list
     \li \a a First \c TimeValue.
     \li \a b Second \c TimeValue.
   \endlist
   Returns smallest \c TimeValue.
   */
  TimeValue minTimeValue(const TimeValue& a, const TimeValue& b)
  {
    if (a.unit() == b.unit())
      return a.duration() < b.duration() ? a : b;
    else
      return toMilliseconds(a) < toMilliseconds(b) ? a : b;
  }
}

/*!
  \class Esri::ArcGISRuntime::Toolkit::TimeSliderController
  \internal
  This class is an internal implementation detail and is subject to change.
 */

TimeSliderController::TimeSliderController(QObject* parent) :
  QObject(parent)
{
}

TimeSliderController::~TimeSliderController()
{
}

QObject* TimeSliderController::geoView() const
{
  return m_geoView.data();
}

void TimeSliderController::setGeoView(QObject* geoView)
{
  if (geoView == m_geoView.data())
    return;

  disconnect(this, nullptr, m_geoView.data(), nullptr);
  disconnectAllLayers();

  m_geoView = geoView;

  if (!m_geoView)
  {
    emit geoViewChanged();
    return;
  }

  if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView.data()))
  {
    connect(mapView, &MapViewToolkit::mapChanged,
            this, qOverload<>(&TimeSliderController::initializeTimeProperties));
  }
  else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView.data()))
  {
    connect(sceneView, &SceneViewToolkit::sceneChanged,
            this, qOverload<>(&TimeSliderController::initializeTimeProperties));
  }

  emit geoViewChanged();

  initializeTimeProperties();
}

void TimeSliderController::disconnectAllLayers()
{
  if (!m_operationalLayers)
    return;

  disconnect(m_operationalLayers, nullptr, this, nullptr);
  for (const auto* layer : *m_operationalLayers)
    disconnect(layer, nullptr, this, nullptr);
}

void TimeSliderController::initializeTimeProperties()
{
  LayerListModel* model = nullptr;

  if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView.data()))
  {
    if (auto map = mapView->map())
      model = map->operationalLayers();
  }
  else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView.data()))
  {
    if (auto scene = sceneView->arcGISScene())
      model = scene->operationalLayers();
  }
  initializeTimeProperties(model);
}

void TimeSliderController::initializeTimeProperties(LayerListModel* opLayers)
{
  disconnectAllLayers();
  m_operationalLayers = opLayers;

  if (!m_operationalLayers)
    return;

  connect(m_operationalLayers.data(), &LayerListModel::itemAdded,
          this, qOverload<>(&TimeSliderController::initializeTimeProperties));

  connect(m_operationalLayers.data(), &LayerListModel::itemRemoved,
          this, qOverload<>(&TimeSliderController::initializeTimeProperties));

  for (auto layer : *m_operationalLayers)
  {
    if (dynamic_cast<TimeAware*>(layer))
    {
      connect(layer, &Layer::loadStatusChanged,
              this, qOverload<>(&TimeSliderController::initializeTimeProperties));
    }
  }

  m_steps = stepsForGeoViewExtent();
  emit extentsChanged();
  emit stepsChanged();
}

TimeExtent TimeSliderController::fullTimeExtent() const
{
  return accumulateTimeAware<TimeExtent>(
    m_operationalLayers,
    [](const TimeExtent& t, TimeAware* tLayer)
    {
      const auto f = tLayer->fullTimeExtent();
      if (t.isEmpty())
        return f;
      else if (f.isEmpty())
        return t;
      else
        return TimeExtent{std::min(t.startTime(), f.startTime()),
                          std::max(t.endTime(), f.endTime())};
    }
  );
}

TimeValue TimeSliderController::timeInterval() const
{
  return accumulateTimeAware<TimeValue>(
    m_operationalLayers,
    [](const TimeValue& t, TimeAware* tLayer)
    {
      const auto f = tLayer->timeInterval();
      if (t.isEmpty())
        return f;
      else if (f.isEmpty())
        return t;
      else
        return minTimeValue(t, f);
    }
  );
}

int TimeSliderController::numberOfSteps() const
{
  const auto extent = fullTimeExtent();
  if (extent.isEmpty())
    return 0;

  const auto interval = timeInterval();
  if (interval.isEmpty())
    return 0;

  const auto range = extent.startTime().msecsTo(extent.endTime());
  const auto intervalMS = toMilliseconds(interval);

  return std::ceil(range / intervalMS);
}

int TimeSliderController::startStep() const
{
  return std::get<0>(m_steps);
}

int TimeSliderController::endStep() const
{
  return std::get<1>(m_steps);
}

void TimeSliderController::setSteps(int s, int e)
{
  setSteps(std::make_pair(s, e));
}

void TimeSliderController::setSteps(std::pair<int, int> steps)
{
  if (steps == m_steps)
    return;

  m_steps = std::move(steps);

  if (auto geoView = qobject_cast<GeoView*>(m_geoView))
  {
    TimeExtent extent{timeForStep(startStep()), timeForStep(endStep())};
    geoView->setTimeExtent(std::move(extent));
  }

  emit stepsChanged();
}

QDateTime TimeSliderController::timeForStep(int step) const
{
  const auto extent = fullTimeExtent();
  if (extent.isEmpty())
    return QDateTime{};

  const auto interval = timeInterval();
  if (interval.isEmpty())
    return QDateTime{};

  const auto intervalMS = toMilliseconds(interval);
  return extent.startTime().addMSecs(step * intervalMS);
}

std::pair<int, int> TimeSliderController::stepsForGeoViewExtent() const
{
  auto geoView = qobject_cast<GeoView*>(m_geoView);

  if (!geoView)
    return std::make_pair(0, 0);

  const auto fullExtent = fullTimeExtent();
  if (fullExtent.isEmpty())
    return std::make_pair(0, 0);

  const auto interval = timeInterval();
  if (interval.isEmpty())
    return std::make_pair(0, 0);

  const auto intervalMS = toMilliseconds(interval);

  const auto geoExtent = geoView->timeExtent();
  if (geoExtent.isEmpty())
  {
    auto range = fullExtent.startTime().msecsTo(fullExtent.endTime());
    return std::make_pair(0, std::ceil(range / intervalMS));
  }

  const int s = std::ceil(fullExtent.startTime().msecsTo(
                          geoExtent.startTime())  / intervalMS);
  const int e = std::ceil(geoExtent.endTime().msecsTo(
                          fullExtent.endTime()) / intervalMS);

  return std::make_pair(s, e);
}

} // Esri::ArcGISRuntime::Toolkit
