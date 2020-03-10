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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONCONTROLLER_H

// Toolkit headers
#include "CoordinateConversionOption.h"
#include "GenericListModel.h"

// Qt headers
#include <QObject>
#include <QString>
#include <QPointF>

// Qt forward declarations
class QAbstractListModel;

// ArcGISRuntime headers
#include <Point.h>
#include <TaskWatcher.h>

namespace Esri
{
namespace ArcGISRuntime
{

class GeoView;

namespace Toolkit
{

/*!
 * \brief In MVC architecture, this is the controller for the corresponding
 * CoordinateConversion view.
 * 
 * A CoordinateConversionController stores a single point, and 0 or more
 * textual representations of this point in a variety of user-defined formats.
 * 
 * CoordinateConversion also accepts a MapView or SceneView. This allows for:
 *   - The controller to zoom to the point's location on the GeoView. 
 *   - The user to select the point with a mouse-event on the GeoView. 
 *   - The controller to calculate the screen coordinate of the current point
 *     relative to the window containing the GeoView.
 * 
 */
class CoordinateConversionController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(double zoomToDistance READ zoomToDistance WRITE setZoomToDistance NOTIFY zoomToDistanceChanged)
  Q_PROPERTY(QAbstractListModel* formats READ coordinateFormats CONSTANT)
  Q_PROPERTY(QAbstractListModel* results READ conversionResults CONSTANT)
  Q_PROPERTY(bool inPickingMode READ inPickingMode WRITE setInPickingMode NOTIFY inPickingModeChanged)
public:
  /*!
   * \brief Constructor.
   * \param parent owning parent object.
   */
  Q_INVOKABLE CoordinateConversionController(QObject* parent = nullptr);

  /*!
   * \brief Destructor
   */
  ~CoordinateConversionController() override;

  /*!
   * \brief Returns the GeoView as a QObject.
   * \return The current GeoView as a QObject.
   */
  QObject* geoView() const;

  /*!
   * \brief Set the GeoView object this Controller uses.
   * 
   * Internally this is cast to a MapView or SceneView using `qobject_cast`,
   * which is why the paremeter is of form `QObject` and not `GeoView`.
   * 
   * \param geoView Object which must inherit from GeoView* and QObject*. 
   */
  void setGeoView(QObject* mapView);

  /*!
   * \brief Known list of available coordinate conversion formats which can be
   * consumed to generate different textual representations of the same point.
   * Internally, this is a GenericListModel with an elementType of
   * CoordinateConversionOption .
   * 
   * \sa GenericListModel
   * 
   * \return GenericListModel of CoordinateConversionOption elements.
   */
  GenericListModel* coordinateFormats() const;

  /*!
   * \brief List of textual representations of the current point in different 
   * formats.
   * Internally, this is a GenericListModel with an elementType of
   * CoordinateConversionResult.
   * 
   * \sa GenericListModel
   * 
   * \return GenericListModel of CoordinateConversionResult elements.
   */
  GenericListModel* conversionResults() const;

  /*!
   * \brief Returns the current point.
   * 
   * The current point represents the one point that all elements within
   * the conversionResults list-model are tied to.
   * 
   * \return Point 
   */
  Point currentPoint() const;

  /*!
   * \brief Converts the current point held by this controller as a 2D point 
   * relative to the current window.
   * 
   * \return The point returned by currentPoint as a screen coordinate. 
   */
  Q_INVOKABLE QPointF screenCoordinate() const;

  /*!
   * \brief Distance between camera and current point when zoomToCurrentPoint
   * is invoked.
   * 
   * If geoView is a SceneView, then this value represents the distance
   * between the camera and the point returned by currentPoint when 
   * zoomToCurrentPoint is called.
   * 
   * If geoView is a MapView, this value has no effect on zoom calculations.
   * 
   * The distance is in the units of the GeoView's SpatialReference.
   * 
   * Defaults to 1500.
   * 
   * \return distance between current point and viewpoint camera.
   */
  double zoomToDistance() const;

  /*!
   * \brief Set the zoomTodistance.
   * \sa zoomToDistance
   * \param distance distance value in the units of the GeoView's
   * SpatialReference.
   */
  void setZoomToDistance(double distance);

  /*!
   * \brief Returns whether this controller is actively listening for mouse
   * events.
   * 
   * When listening for mouse events, the Controller will update the
   * current point to whatever point is returned by a click event on the
   * GeoView.
   * 
   * \return true This controller is listening to mouse events on the GeoView
   * and will actively update the CurrentPoint.
   * \return false This controller is not listening to mouse events on the
   * GeoView.
   */
  bool inPickingMode() const;

  /*!
   * \brief Set whether the controller should actively be listening to mouse
   * and/or touch events on the GeoView.
   * \sa inPickingMode
   * \param mode True for listening to mouse events, false for ignoring mouse
   * events.
   */
  void setInPickingMode(bool mode);

signals:
  /*! \brief Emitted when the geoView has changed. */
  void geoViewChanged();

  /*! \brief Emitted when the currentPoint has changed. */
  void currentPointChanged(const Esri::ArcGISRuntime::Point& point);

  /*!
   * \brief Emitted when the zoomToDistance property has changed.
   */
  void zoomToDistanceChanged();

  /*!
   * \brief Emitted when the inPickingMode property has changed.
   */
  void inPickingModeChanged();

public slots:
  /*!
   * \brief Set the current point to \a point. This updates all textual
   * representations owned by this controller.
   * \param point new point to convert.
   */
  void setCurrentPoint(const Esri::ArcGISRuntime::Point& point);

  /*!
   * \brief Set the current point to point. Point is a string which will be
   * converted as defined by the formatting hints given in option. This updates
   * all textual representations owned by this controller.
   * 
   * The SpatialReference is taken from the GeoView.
   * 
   * If conversion fails this function is treated as a no-op.
   * 
   * \param point string representation of point. 
   * \param option Option dictating hints on how to convert the string to a
   *  point.
   */
  void setCurrentPoint(const QString& point, CoordinateConversionOption* option);

  /*!
   * \brief Set the current point to point. Point is a string which will be
   * converted as defined by the formatting hints given in option. This updates
   * all textual representations owned by this controller.
   * 
   * If conversion fails this function is treated as a no-op.
   * 
   * \param point string representation of point. 
   * \param spatialReference An explicit SpatialReference for conversion.
   * \param option Option dictating hints on how to convert the string to a
   *  point.
   */
  void setCurrentPoint(const QString& point,
                       const SpatialReference& spatialReference,
                       CoordinateConversionOption* option);

  /*!
   * \brief Updates the GeoView's camera to point to the current point's
   * location on the map.
   * \sa zoomToDistance
   */
  void zoomToCurrentPoint();

  /*!
   * \brief Given option, generates a new result.
   * 
   * A new CoordianteConversionResult is added to the list-model returned by
   * coordinateResults. This Result is tied to the conversion parameters as
   * given by option.
   * 
   * \param option option to generate result for.
   */
  void addNewCoordinateResultForOption(CoordinateConversionOption* option);

  /*!
   * \brief Removes a given CoordinateConversionResult at index.
   * 
   * This function is for deleting results in the list-model returned by
   * coordinateResults.
   * 
   * \param index index of a given CoordinateConversionResult in the list-model
   * returned by coordinateResults.
   */
  void removeCoordinateResultAtIndex(int index);

private:
  Point m_currentPoint;
  TaskWatcher m_screenToLocationTask;
  double m_zoomToDistance = 0.0;
  GenericListModel* m_coordinateFormats = nullptr;
  GenericListModel* m_conversionResults = nullptr;
  QObject* m_geoView = nullptr;
  bool m_inPickingMode = false;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONCONTROLLER_H
