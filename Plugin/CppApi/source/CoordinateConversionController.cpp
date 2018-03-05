// Copyright 2017 ESRI
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

#include "CoordinateConversionController.h"
#include "CoordinateConversionConstants.h"
#include "CoordinateConversionOptions.h"
#include "CoordinateConversionResults.h"
#include "CoordinateFormatFactory.h"
#include "ToolManager.h"
#include "ToolResourceProvider.h"

#include "CoordinateFormatter.h"
#include "GeometryEngine.h"
#include "GeoView.h"
#include "MapView.h"
#include "PolylineBuilder.h"
#include "SceneView.h"

#include <QClipboard>
#include <QGuiApplication>

#include <functional>

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionController
  \ingroup ToolCoordinateConversion
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.2
  \brief The controller for the coordinate conversion tool.

  This tool converts a notation text string to the selected output notations formats
  defined in your app. The output formats are selected by the user when running the tool,
  from a set of possible formats defined in a \l CoordinateFormatFactory.

  Each \l CoordinateConversionOptions object specifies a
  coordinate notation and options that apply to that notation (decimal places,
  use of spaces, and so on).

  \sa {Coordinate Conversion Tool}
 */

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \brief A constructor that accepts an optional \a parent.
 */
CoordinateConversionController::CoordinateConversionController(QObject* parent):
  AbstractTool(parent),
  m_coordinateFormats{CoordinateConversionConstants::DECIMAL_DEGREES_FORMAT,
                      CoordinateConversionConstants::DEGREES_DECIMAL_MINUTES_FORMAT,
                      CoordinateConversionConstants::DEGREES_MINUTES_SECONDS_FORMAT,
                      CoordinateConversionConstants::MGRS_FORMAT,
                      CoordinateConversionConstants::USNG_FORMAT,
                      CoordinateConversionConstants::UTM_FORMAT,
                      CoordinateConversionConstants::GARS_FORMAT}
{
  ToolManager::instance().addTool(this);

  auto geoView = ToolResourceProvider::instance()->geoView();
  if (geoView)
    setSpatialReference(geoView->spatialReference());

  connect(ToolResourceProvider::instance(), &ToolResourceProvider::spatialReferenceChanged, this, [this]()
  {
    setSpatialReference(ToolResourceProvider::instance()->spatialReference());
  });

  connect(ToolResourceProvider::instance(), &ToolResourceProvider::mouseClickedPoint, this, &CoordinateConversionController::onMouseClicked);

  connect(ToolResourceProvider::instance(), &ToolResourceProvider::locationChanged, this, &CoordinateConversionController::onLocationChanged);

  connect(this, &CoordinateConversionController::optionsChanged, this,
          [this]()
  {
    convertPoint();
  });
}

/*!
   \brief The destructor.
 */
CoordinateConversionController::~CoordinateConversionController()
{
}

/*!
  \brief Converts \a notation and updates the \l results property.

  Before calling this method, set the \l inputFormat property to the
  desired fromat, and call \l setSpatialReference property
  to the spatial reference of the notation's coordinates.

  \note Converting between some notation formats can result in loss
  of precision due to the number of decimal places expressed in the
  notation, and other factors. The best precision is attained by
  converting from a Point object. See \l convertPoint.
 */
void CoordinateConversionController::convertNotation(const QString& notation)
{
  setPointToConvert(pointFromNotation(notation));
}

/*!
  \internal
 */
Point CoordinateConversionController::pointFromNotation(const QString& incomingNotation)
{
  if (m_spatialReference.isEmpty())
    qWarning("The spatial reference property is empty: conversions will fail.");

  CoordinateConversionOptions* inputOption = nullptr;
  for (CoordinateConversionOptions* option : m_options)
  {
    if (isInputFormat(option))
    {
      inputOption = option;
      break;
    }
  }

  if (inputOption == nullptr)
    return Point();

  switch (inputOption->outputMode())
  {
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeGars:
  {
    return CoordinateFormatter::fromGars(incomingNotation,
                                         m_spatialReference,
                                         inputOption->garsConvesrionMode());
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeGeoRef:
  {
    return CoordinateFormatter::fromGeoRef(incomingNotation,
                                           m_spatialReference);
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon:
  {
    return CoordinateFormatter::fromLatitudeLongitude(incomingNotation,
                                                      m_spatialReference);
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeMgrs:
  {
    return CoordinateFormatter::fromMgrs(incomingNotation,
                                         m_spatialReference,
                                         inputOption->mgrsConversionMode());
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeUsng:
  {
    return CoordinateFormatter::fromUsng(incomingNotation,
                                         m_spatialReference);
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeUtm:
  {
    return CoordinateFormatter::fromUtm(incomingNotation,
                                        m_spatialReference,
                                        inputOption->utmConversionMode());
  }
  default: {}
  }

  return Point();
}

/*!
  \brief Converts the last point assigned with \l setPointToConvert to all the
  notations specified in the options.
 */
void CoordinateConversionController::convertPoint()
{
  QList<Result> results;
  for (CoordinateConversionOptions* option : m_options)
  {
    if (isInputFormat(option))
      continue;

    results.append(Result(option->name(), convertPointInternal(option, m_pointToConvert), option->outputMode()));
  }

  if (results.isEmpty())
    resultsInternal()->clearResults();
  else
    resultsInternal()->setResults(std::move(results));

  emit resultsChanged();
}

/*!
  \internal
 */
QString CoordinateConversionController::convertPointInternal(CoordinateConversionOptions* option,
                                                             const Esri::ArcGISRuntime::Point& point) const
{
  switch (option->outputMode())
  {
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeGars:
  {
    return CoordinateFormatter::toGars(point);
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeGeoRef:
  {
    return CoordinateFormatter::toGeoRef(point, option->precision());
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon:
  {
    const auto format = static_cast<Esri::ArcGISRuntime::LatitudeLongitudeFormat>(option->latLonFormat());
    return CoordinateFormatter::toLatitudeLongitude(point, format, option->decimalPlaces());
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeMgrs:
  {
    const auto conversionMode = static_cast<Esri::ArcGISRuntime::MgrsConversionMode>(option->mgrsConversionMode());
    return CoordinateFormatter::toMgrs(point, conversionMode, option->decimalPlaces(), option->addSpaces());
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeUsng:
  {
    return CoordinateFormatter::toUsng(point, option->precision(), option->decimalPlaces());
  }
  case CoordinateConversionOptions::CoordinateType::CoordinateTypeUtm:
  {
    const auto conversionMode = static_cast<Esri::ArcGISRuntime::UtmConversionMode>(option->utmConversionMode());
    return CoordinateFormatter::toUtm(point, conversionMode, option->addSpaces());
  }
  default: {}
  }

  return QString();
}

/*!
  \internal
 */
bool CoordinateConversionController::isInputFormat(CoordinateConversionOptions* option) const
{
  return isFormat(option, m_inputFormat);
}

/*!
  \internal
 */
bool CoordinateConversionController::isFormat(CoordinateConversionOptions *option, const QString& formatName) const
{
  if (option == nullptr)
    return false;

  return option->name().compare(formatName, Qt::CaseInsensitive) == 0;
}

/*!
  \property CoordinateConversionController::results
  \brief The conversion results as a list model.

  The results are automatically updated as conversions are run.

  \sa CoordinateConversionResults
 */
QAbstractListModel* CoordinateConversionController::results()
{
  return resultsInternal();
}

/*!
  \internal
 */
CoordinateConversionResults *CoordinateConversionController::resultsInternal()
{
  if (!m_results)
  {
    m_results = new CoordinateConversionResults(this);
    connect(m_results, &CoordinateConversionResults::resultsChanged, this, &CoordinateConversionController::resultsChanged);
  }

  return m_results;
}

/*!
  \brief Sets the spatial reference to \a spatialReference.
  \note This property must be set before calling the \l convertNotation method.
 */
void CoordinateConversionController::setSpatialReference(const SpatialReference& spatialReference)
{
  m_spatialReference = spatialReference;
}

/*!
  \brief Sets the point to be converted via the \l convertPoint method to \a point.
  \note If the \l runConversion property is \c true, the conversion will be run immediately.
 */
void CoordinateConversionController::setPointToConvert(const Point& point)
{
  if (point == m_pointToConvert)
    return;

  m_pointToConvert = point;

  if (m_runConversion)
    convertPoint();

  emit pointToConvertChanged();
}

/*!
  \brief Returns whether the tool is in capture mode.

  If \c true, the tool will convert a point set via a mouse click.
  If \c false, the too will use the app's current location as the target point.
 */
bool CoordinateConversionController::isCaptureMode() const
{
  return m_captureMode;
}

/*!
  \brief Sets the tool's capture mode to \a captureMode.

  If \c true, the tool will convert a point set via a mouse click.
  If \c false, the too will use the app's current location as the target point.
 */
void CoordinateConversionController::setCaptureMode(bool captureMode)
{
  if (captureMode == m_captureMode)
    return;

  m_captureMode = captureMode;

  setPointToConvert(Point());

  emit captureModeChanged();
}

/*!
  \brief Handles the mouse click at \a clickedPoint.

  If the tool is active and in \l captureMode, this will be used as the input for conversions.
 */
void CoordinateConversionController::onMouseClicked(const Point& clickedPoint)
{
  if (isActive() && isCaptureMode())
    setPointToConvert(clickedPoint);
}

/*!
  \brief Handles the app's location update to \a location.

  If the tool is active and is not in \l captureMode, this will be used as the input for conversions.
 */
void CoordinateConversionController::onLocationChanged(const Point& location)
{
  if (isActive() && !isCaptureMode())
    setPointToConvert(location);
}

/*!
  \brief Returns the input coordinate format of the tool.
 */
QString CoordinateConversionController::inputFormat() const
{
  return m_inputFormat;
}

/*!
  \brief Sets the input coordinate format of the tool to \a inputFormat.
 */
void CoordinateConversionController::setInputFormat(const QString& inputFormat)
{
  if (m_inputFormat == inputFormat)
    return;

  m_inputFormat = inputFormat;

  addCoordinateFormat(m_inputFormat);

  if (m_runConversion)
    convertPoint();

  emit inputFormatChanged();
  emit pointToConvertChanged();
}

/*!
  \brief Returns a list of the avilable coordinate formats for the tool.
 */
QStringList CoordinateConversionController::coordinateFormats() const
{
  return m_coordinateFormats;
}

/*!
  \brief Add the \l CoordinateConversionOptions object \a option to the list of options.
 */
void CoordinateConversionController::addOption(CoordinateConversionOptions* option)
{
  m_options.append(option);
  if (m_options.size() == 1)
    setInputFormat(option->name());

  if (!m_coordinateFormats.contains(option->name()))
  {
    m_coordinateFormats.append(option->name());
    emit coordinateFormatsChanged();
  }

  emit optionsChanged();
}

/*!
  \brief Clear all the \l CoordinateConversionOptions from the list of options.
 */
void CoordinateConversionController::clearOptions()
{
  m_options.clear();
  emit optionsChanged();
}

/*!
  \brief Returns the name of this tool: "CoordinateConversion".
 */
QString CoordinateConversionController::toolName() const
{
  return "CoordinateConversion";
}

/*! \brief Sets any values in \a properties which are relevant for the coordinate conversion controller.
 *
 * This tool will use the following key/value pairs in the \a properties map if they are set:
 *
 * \list
 *  \li CoordinateFormat. The default input coordinate format for the tool.
 * \endList
 */
void CoordinateConversionController::setProperties(const QVariantMap& properties)
{
  auto findFormatIt = properties.find(CoordinateConversionConstants::COORDINATE_FORMAT_PROPERTY);
  if (findFormatIt != properties.end())
    setInputFormat(findFormatIt.value().toString());
}

/*! \brief Returns a string representation of the input point in the input coordinate format.
 */
QString CoordinateConversionController::pointToConvert() const
{
  for (CoordinateConversionOptions* option : m_options)
  {
    if (isInputFormat(option))
      return convertPointInternal(option, m_pointToConvert);
  }

  return QString();
}

/*!
  \brief Copy \a text to the system clipboard.
 */
void CoordinateConversionController::copyToClipboard(const QString& text)
{
  auto clipboard = QGuiApplication::clipboard();
  if (clipboard)
    clipboard->setText(text);
}

/*!
  \brief Clears the results notation strings from the results list model.
 */
void CoordinateConversionController::clearResults()
{
  if (m_results)
    m_results->clearResults();
}

/*!
  \brief Adds the coordinate format \a newFormat to the list of results produced by the tool.
 */
void CoordinateConversionController::addCoordinateFormat(const QString& newFormat)
{
  if (!m_coordinateFormats.contains(newFormat))
    return;

  auto it = m_options.cbegin();
  const auto itEnd = m_options.cend();
  for (; it != itEnd; ++it)
  {
    if (isFormat(*it, newFormat))
      return;
  }

  CoordinateConversionOptions* option = CoordinateFormatFactory::createFormat(newFormat, this);
  if (!option)
    return;

  addOption(option);

  if (m_runConversion)
    convertPoint();
}

/*!
  \brief Removes the coordinate format \a formatToRemove from the list of results produced by the tool.
 */
void CoordinateConversionController::removeCoordinateFormat(const QString& formatToRemove)
{
  if (formatToRemove.compare(m_inputFormat, Qt::CaseInsensitive) == 0)
    return;

  bool removed = false;
  auto it = m_options.begin();
  auto itEnd = m_options.end();
  for (; it != itEnd; ++it)
  {
    const auto& option = *it;
    if (isFormat(option, formatToRemove))
    {
      m_options.erase(it);
      removed = true;
      break;
    }
  }

  if (!removed)
    return;

  if (m_results)
    m_results->removeResult(formatToRemove);

  if(runConversion())
    convertPoint();

  emit optionsChanged();
  emit pointToConvertChanged();
}

QPointF CoordinateConversionController::screenCoordinate(double screenWidth, double screenHeight) const
{
  GeoView* geoView = ToolResourceProvider::instance()->geoView();
  if (!geoView)
    return QPointF();

  // attempt to get the target point as a screen coordinate
  QPointF res;
  SceneView* sceneView = dynamic_cast<SceneView*>(geoView);
  MapView* mapView = dynamic_cast<MapView*>(geoView);
  if (sceneView)
    res = sceneView->locationToScreen(m_pointToConvert).screenPoint();
  else if (mapView)
    res = mapView->locationToScreen(m_pointToConvert);
  else
    return res;

  // if we have a valid screen coordinate, return it
  if (res.x() > 0.0 || res.y() > 0.0)
    return res;

  // otherwise build a polyline describing the extent of the screen
  const Point topLeft = sceneView ? sceneView->screenToBaseSurface(0.01, 0.01) : mapView->screenToLocation(0.01, 0.01);
  const Point topRight = sceneView ? sceneView->screenToBaseSurface(screenWidth, 0.01) : mapView->screenToLocation(screenWidth, 0.01);
  const Point lowerLeft = sceneView ? sceneView->screenToBaseSurface(0.01, screenHeight) : mapView->screenToLocation(0.01, screenHeight);
  const Point lowerRight = sceneView ? sceneView->screenToBaseSurface(screenWidth, screenHeight) : mapView->screenToLocation(screenWidth, screenHeight);
  PolylineBuilder bldr(topLeft.spatialReference());
  bldr.addPoint(topLeft);
  bldr.addPoint(topRight);
  bldr.addPoint(lowerRight);
  bldr.addPoint(lowerLeft);
  bldr.addPoint(topLeft);
  const Polyline viewBoundary = bldr.toPolyline();

  // obtain the point on the view boundary polyline which is closest to the target point
  Point projected = GeometryEngine::instance()->project(m_pointToConvert, topLeft.spatialReference());
  const Point pointOnBoundary = GeometryEngine::instance()->nearestCoordinate(viewBoundary, projected).coordinate();

  // return the point on the boundary as a screen coordinate
  return sceneView ? sceneView->locationToScreen(pointOnBoundary).screenPoint() : mapView->locationToScreen(pointOnBoundary);
}

/*!
  \property CoordinateConversionController::runConversion
  \brief Whether the conversion runs automatically when \l setPointToConvert
  is called.
 */
bool CoordinateConversionController::runConversion() const
{
  return m_runConversion;
}

void CoordinateConversionController::setRunConversion(bool runConversion)
{
  m_runConversion = runConversion;
  emit runConversionChanged();
}

// signals

/*!
  \fn void CoordinateConversionController::optionsChanged();
  \brief Signal emitted when the \l options property changes.
 */

/*!
  \fn void CoordinateConversionController::resultsChanged();
  \brief Signal emitted when the \l results property changes.
 */

/*!
  \fn void CoordinateConversionController::runConversionChanged();
  \brief Signal emitted when the \l runConversion property changes.
 */

/*!
  \fn void CoordinateConversionController::pointToConvertChanged();
  \brief Signal emitted when the \l pointToConvert property changes.
 */

/*!
  \fn void CoordinateConversionController::coordinateFormatsChanged();
  \brief Signal emitted when the list of \l coordinateFormats property changes.
 */

/*!
  \fn void CoordinateConversionController::inputFormatChanged();
  \brief Signal emitted when the list of \l inputFormat property changes.
 */

/*!
  \fn void CoordinateConversionController::captureModeChanged();
  \brief Signal emitted when the list of \l captureMode property changes.
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
