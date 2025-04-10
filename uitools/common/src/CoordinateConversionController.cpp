
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
#include "CoordinateConversionController.h"

// Toolkit headers
#include "CoordinateConversionResult.h"
#include "CoordinateOptionDefaults.h"
#include "GeoViews.h"

// Qt headers
#include <QMouseEvent>
#include <QtGlobal>
#include <QUuid>
#include <QPointF>

// ArcGISRuntime headers
#include <Camera.h>
#include <CoordinateFormatter.h>
#include <LocationToScreenResult.h>
#include <MapTypes.h>
#include <SceneViewTypes.h>
#include <Viewpoint.h>

namespace Esri::ArcGISRuntime::Toolkit {

namespace
{
constexpr double DEFAULT_ZOOM_TO_DISTANCE = 1500.0;
}

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionController
  \internal
  This class is an internal implementation detail and is subject to change.
 */

CoordinateConversionController::CoordinateConversionController(QObject* parent):
  QObject(parent),
  m_zoomToDistance(DEFAULT_ZOOM_TO_DISTANCE),
  m_coordinateFormats(new GenericListModel(&CoordinateConversionOption::staticMetaObject, this)),
  m_conversionResults(new GenericListModel(&CoordinateConversionResult::staticMetaObject, this))
{
  {
    // Some default coordinate conversion formats to set us up with.
    m_coordinateFormats->setDisplayPropertyName("name");
    m_coordinateFormats->append(QList<QObject*>()
      << createDecimalDegrees()
      << createDegreesDecimalMinutes()
      << createDegreesMinutesSeconds()
      << createMgrs()
      << createUsng()
      << createUtm()
      << createGars()
    );
  }

  {
    m_conversionResults->setDisplayPropertyName("name");
    connect(m_conversionResults, &QAbstractItemModel::rowsInserted, this,
      [this](const QModelIndex& /*parent*/, int first, int last)
      {
        // Every result owned by the controller needs to listen to the
        // currentPointChanged signal to update its notation.
        for (int i = first; i <= last; ++i)
        {
          auto index = m_conversionResults->index(i);
          auto result = m_conversionResults->element<CoordinateConversionResult>(index);
          if (result)
          {
            connect(this, &CoordinateConversionController::currentPointChanged,
                    result, &CoordinateConversionResult::updateCoordinatePoint);
          }
          else
          {
            qWarning("Empty CoordinateConversionResult in the CoordinateConversionController results model at index %d", i);
          }
        }
      }
     );
  }
}

CoordinateConversionController::~CoordinateConversionController()
{
}

QObject* CoordinateConversionController::geoView() const
{
  return m_geoView;
}

void CoordinateConversionController::setGeoView(QObject* geoView)
{
  if (geoView == m_geoView)
    return;

  if (m_geoView)
    disconnect(this, nullptr, m_geoView, nullptr);

  m_geoView = geoView;

  if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
  {
    connect(sceneView, &SceneViewToolkit::mouseClicked, this,
            [sceneView, this](QMouseEvent& event)
    {
      if (m_inPickingMode && !m_screenToLocationFuture.isRunning())
      {
        m_screenToLocationFuture = sceneView->screenToLocationAsync(event.pos().x(), event.pos().y());
        m_screenToLocationFuture.then(this, [this](const Point& point)
        {
          setCurrentPoint(point);
        });

        event.accept();
      }
    });
  }
  else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
  {
    connect(mapView, &MapViewToolkit::mouseClicked, this,
            [mapView, this](QMouseEvent& event)
    {
      if (m_inPickingMode)
      {
        setCurrentPoint(mapView->screenToLocation(event.pos().x(), event.pos().y()));
        event.accept();
      }
    });
  }

  emit geoViewChanged();
}

void CoordinateConversionController::setCurrentPoint(const Point& point)
{
  if (point == m_currentPoint)
    return;

  m_currentPoint = point;
  emit currentPointChanged(point);
}

void CoordinateConversionController::setCurrentPoint(
  const QString& point,
  CoordinateConversionOption* option)
{
  if (auto geoView = qobject_cast<GeoView*>(m_geoView))
    setCurrentPoint(point, geoView->spatialReference(), option);
  else
    setCurrentPoint(point, SpatialReference(), option);
}

void CoordinateConversionController::setCurrentPoint(
  const QString& point,
  const SpatialReference& spatialReference,
  CoordinateConversionOption* option)
{
  if (!option)
    return;

  auto p = option->pointFromString(point, spatialReference);
  if (p.isValid())
    setCurrentPoint(p);
}

QPointF CoordinateConversionController::screenCoordinate() const
{
  // TODO additional work required here to show a coordinate on the "edge" of
  // the screen if coordinate is not in the current view.
  QPointF res(-1.0, -1.0);
  if (auto sceneView = qobject_cast<SceneView*>(m_geoView))
  {
    const auto location = sceneView->locationToScreen(m_currentPoint);
    const auto lx = location.screenPoint().x();
    const auto ly = location.screenPoint().y();

    if (location.visibility() == SceneLocationVisibility::NotOnScreen)
    {
      // TODO attach to edge of screen.
    }
    else
    {
      res = QPointF(lx, ly);
    }
  }
  else if (auto mapView = qobject_cast<MapView*>(m_geoView))
  {
    res = mapView->locationToScreen(m_currentPoint);
  }

  return res;
}

Point CoordinateConversionController::currentPoint() const
{
  return m_currentPoint;
}

GenericListModel* CoordinateConversionController::coordinateFormats() const
{
  return m_coordinateFormats;
}

CoordinateConversionOption* CoordinateConversionController::getOption(int index) const
{
  return m_coordinateFormats->element<CoordinateConversionOption>(m_coordinateFormats->index(index));
}

GenericListModel* CoordinateConversionController::conversionResults() const
{
  return m_conversionResults;
}

void CoordinateConversionController::zoomToCurrentPoint()
{
  if (auto sceneView = qobject_cast<SceneView*>(m_geoView))
  {
    const Camera currentCam = sceneView->currentViewpointCamera();
    const Camera newCam(m_currentPoint, m_zoomToDistance, currentCam.heading(), currentCam.pitch(), currentCam.roll());
    auto future = sceneView->setViewpointCameraAsync(newCam, 1.0);
    Q_UNUSED(future)
  }
  else if (auto mapView = qobject_cast<MapView*>(m_geoView))
  {
    const Viewpoint currVP = mapView->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewPoint(m_currentPoint, currVP.targetScale());
    auto future = mapView->setViewpointAsync(newViewPoint, 1.0);
    Q_UNUSED(future)
  }
}

void CoordinateConversionController::addNewCoordinateResultForOption(CoordinateConversionOption* option)
{
  auto result = new CoordinateConversionResult(m_conversionResults);
  result->setType(option);
  result->updateCoordinatePoint(currentPoint());
  m_conversionResults->append(result);
}

void CoordinateConversionController::removeCoordinateResultAtIndex(int index)
{
  m_conversionResults->removeRow(index);
}

double CoordinateConversionController::zoomToDistance() const
{
  return m_zoomToDistance;
}

void CoordinateConversionController::setZoomToDistance(double distance)
{
  if (distance == m_zoomToDistance)
    return;

  m_zoomToDistance = distance;
  emit zoomToDistanceChanged();
}

bool CoordinateConversionController::inPickingMode() const
{
  return m_inPickingMode;
}

void CoordinateConversionController::setInPickingMode(bool mode)
{
  if (m_inPickingMode == mode)
    return;

  m_inPickingMode = mode;
  emit inPickingModeChanged();
}

} // Esri::ArcGISRuntime::Toolkit
