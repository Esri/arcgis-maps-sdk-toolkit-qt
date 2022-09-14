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
#include "Internal/GeoViews.h"

// Qt headers
#include <QtGlobal>

// ArcGISRuntime headers
#include <CoordinateFormatter.h>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

namespace
{
constexpr double DEFAULT_ZOOM_TO_DISTANCE = 1500.0;
}

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionController
  \ingroup ArcGISQtToolkitUiCppControllers
  \inmodule EsriArcGISRuntimeToolkit

  \brief In MVC architecture, this is the controller for the corresponding
  \c CoordinateConversion view.


  A \c CoordinateConversionController stores a single point, and \c 0 or more
  textual representations of this point in a variety of user-defined formats.

  \c CoordinateConversion also accepts a \c MapView or \c SceneView. This allows
  for:

  \list
    \li The controller to zoom to the point's location on the GeoView.
    \li The user to select the point with a mouse-event on the GeoView.
    \li The controller to calculate the screen coordinate of the current
        point relative to the window containing the GeoView.
  \endlist
 */

/*!
  \brief Constructor.
  \list
    \li \a parent owning parent object.
  \endlist
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

/*!
  \brief Destructor
 */
CoordinateConversionController::~CoordinateConversionController()
{
}

/*!
  \brief Returns the \c GeoView as a \c QObject.
 */
QObject* CoordinateConversionController::geoView() const
{
  return m_geoView;
}

/*!
  \brief Set the GeoView object this Controller uses.

  Internally this is cast to a \c MapView or \c SceneView using \c qobject_cast,
  which is why the paremeter is of form \c QObject and not \c GeoView.

  \list
    \li \a geoView \c Object which must inherit from \c{GeoView*} and
        \c{QObject*}.
  \endlist
 */
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
      if (m_inPickingMode && (!m_screenToLocationTask.isValid() ||
                               m_screenToLocationTask.isDone()))
      {
        m_screenToLocationTask = sceneView->screenToLocation(event.pos().x(), event.pos().y());
        event.accept();
      }
    });

    connect(sceneView, &SceneViewToolkit::screenToLocationCompleted, this,
            [this](QUuid taskId, Point point)
    {
      if (taskId != m_screenToLocationTask.taskId())
        return;

      setCurrentPoint(point);
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

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::setCurrentPoint(const Esri::ArcGISRuntime::Point& point)
  \brief Set the current point to \a point. This updates all textual
  representations owned by this controller.

  \list
  \li \a point new point to convert.
  \endlist
 */
void CoordinateConversionController::setCurrentPoint(const Point& point)
{
  if (point == m_currentPoint)
    return;

  m_currentPoint = point;
  emit currentPointChanged(point);
}

/*!
  \brief Set the current point to point. Point is a string which will be
  converted as defined by the formatting hints given in option. This updates
  all textual representations owned by this controller.

  The \c SpatialReference is taken from the \c GeoView.

  If conversion fails this function is treated as a no-op.

  \list
    \li \a point string representation of point.
    \li \a option Option dictating hints on how to convert the string to a
    point.
  \endlist
 */
void CoordinateConversionController::setCurrentPoint(
  const QString& point,
  CoordinateConversionOption* option)
{
  if (auto geoView = qobject_cast<GeoView*>(m_geoView))
    setCurrentPoint(point, geoView->spatialReference(), option);
  else
    setCurrentPoint(point, SpatialReference(), option);
}

/*!
  \brief Set the current point to \a point. \a point is a string which will be
  converted as defined by the formatting hints given in option. This updates
  all textual representations owned by this controller.

  If conversion fails this function is treated as a no-op.

  \list
    \li \a point string representation of point.
    \li \a spatialReference An explicit SpatialReference for conversion.
    \li \a option Option dictating hints on how to convert the string to a
        \c Point.
  \endlist
 */
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

/*!
  \brief Converts the current point held by this controller as a 2D point
  relative to the current window.

  Returns the point returned by currentPoint as a screen coordinate.
 */
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

/*!
  \fn Esri::ArcGISRuntime::Point Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::currentPoint() const
  \brief Returns the current point.
  The current point represents the one point that all elements within
  the conversionResults list-model are tied to.
 */
Point CoordinateConversionController::currentPoint() const
{
  return m_currentPoint;
}

/*!
  \brief Returns the known list of available coordinate conversion formats which
  can be consumed to generate different textual representations of the same
  point.

  Internally, this is a \c GenericListModel with an \c elementType of
  \c CoordinateConversionOption.
 */
GenericListModel* CoordinateConversionController::coordinateFormats() const
{
  return m_coordinateFormats;
}

/*!
  \brief Returns the list of textual representations of the current point in
  different formats.

  Internally, this is a \c GenericListModel with an \c elementType of
  \c CoordinateConversionResult.
 */
GenericListModel* CoordinateConversionController::conversionResults() const
{
  return m_conversionResults;
}

/*!
 \brief Updates the \c GeoView camera to point to the current point's
 location on the map.

 \sa zoomToDistance
 */
void CoordinateConversionController::zoomToCurrentPoint()
{
  if (auto sceneView = qobject_cast<SceneView*>(m_geoView))
  {
    const Camera currentCam = sceneView->currentViewpointCamera();
    const Camera newCam(m_currentPoint, m_zoomToDistance, currentCam.heading(), currentCam.pitch(), currentCam.roll());
    sceneView->setViewpointCamera(newCam, 1.0);
  }
  else if (auto mapView = qobject_cast<MapView*>(m_geoView))
  {
    const Viewpoint currVP = mapView->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewPoint(m_currentPoint, currVP.targetScale());
    mapView->setViewpoint(newViewPoint, 1.0);
  }
}

/*!
  \brief Given option, generates a new result.

  A new \c CoordinateConversionResult is added to the list-model returned by
  coordinateResults. This result is tied to the conversion parameters as
  given by option.

  \list
  \li \a option option to generate result for.
  \endlist
 */
void CoordinateConversionController::addNewCoordinateResultForOption(CoordinateConversionOption* option)
{
  auto result = new CoordinateConversionResult(m_conversionResults);
  result->setType(option);
  result->updateCoordinatePoint(currentPoint());
  m_conversionResults->append(result);
}

/*!
 * \brief Removes a given \c CoordinateConversionResult at index.
 *
 * This function is for deleting results in the list-model returned by
 * coordinateResults.
 *
 * \list
 *   \li \a index index of a given CoordinateConversionResult in the list-model
 *       returned by coordinateResults.
 * \endlist
 */
void CoordinateConversionController::removeCoordinateResultAtIndex(int index)
{
  m_conversionResults->removeRow(index);
}

/*!
  \brief Distance between camera and current point when \c zoomToCurrentPoint
  is invoked.

  If \c geoView is a \c SceneView, then this value represents the distance
  between the camera and the point returned by \c currentPoint when
  \c zoomToCurrentPoint is called.

  If \c geoView is a \c MapView, this value has no effect on zoom calculations.

  The distance is in the units of the \c GeoView \c SpatialReference.

  Defaults to 1500.

  Returns distance between current point and viewpoint camera.
 */
double CoordinateConversionController::zoomToDistance() const
{
  return m_zoomToDistance;
}

/*!
  \brief Set the \c zoomToDistance.
  \sa zoomToDistance
  \list
  \li \a distance distance value in the units of the \c GeoView
      \c SpatialReference.
  \endlist
 */
void CoordinateConversionController::setZoomToDistance(double distance)
{
  if (distance == m_zoomToDistance)
    return;

  m_zoomToDistance = distance;
  emit zoomToDistanceChanged();
}

/*!
  \brief Returns whether this controller is actively listening for mouse
  events.

  When listening for mouse events, this controller will update the
  current point to whatever point is returned by a click event on the
  \c GeoView.

  Returns true if this controller is listening to mouse events on the \c
  GeoView.
 */
bool CoordinateConversionController::inPickingMode() const
{
  return m_inPickingMode;
}

/*!
  \brief Set whether the controller should actively be listening to mouse
  and/or touch events on the GeoView.

  \sa inPickingMode

  \list
    \li \a mode True for listening to mouse events, false for ignoring mouse
        events.
  \endlist
 */
void CoordinateConversionController::setInPickingMode(bool mode)
{
  if (m_inPickingMode == mode)
    return;

  m_inPickingMode = mode;
  emit inPickingModeChanged();
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::geoViewChanged()
  \brief Emitted when the geoView has changed.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::currentPointChanged(const Esri::ArcGISRuntime::Point& point)
  \brief Emitted when the currentPoint has changed. \a point represents the new point.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::zoomToDistanceChanged()
  \brief Emitted when the zoomToDistance property has changed.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::inPickingModeChanged()
  \brief Emitted when the inPickingMode property has changed.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::geoView
  \brief The geoview the controller is utilizing for interactions.
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::geoView()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::zoomToDistance
  \brief Distance between camera and target point when performing a zoom.
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::zoomToDistance()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::formats
  \brief List of all coordinate format objects held by this controller.
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::coordinateFormats()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::results
  \brief List of all conversion result objects held by this controller.
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::conversionResults()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::inPickingMode
  \brief Flag stating whether this controller is listening for input events on
         the geoView.
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionController::inPickingMode()
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
