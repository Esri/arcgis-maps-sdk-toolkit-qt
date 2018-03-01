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
#include "CoordinateConversionResults.h"
#include "CoordinateFormatter.h"
#include <QClipboard>
#include <QGuiApplication>

#include "ToolManager.h"
#include "ToolResourceProvider.h"
#include "GeoView.h"
#include <functional>

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionController
  \ingroup ToolCoordinateConversion
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.2
  \brief The controller for the coordinate conversion tool.

  This tool converts a notation text string to all output notations formats
  defined in your app. Those output formats are defined using a list of
  \l CoordinateConversionOptions objects that your app builds
  using \l addOption. Each \l CoordinateConversionOptions object specifies a
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

using CoordinateType = CoordinateConversionOptions::CoordinateType;

const QString CoordinateConversionController::DECIMAL_DEGREES_FORMAT = QStringLiteral("DD");
const QString CoordinateConversionController::DEGREES_DECIMAL_MINUTES_FORMAT = QStringLiteral("DDM");
const QString CoordinateConversionController::DEGREES_MINUTES_SECONDS_FORMAT = QStringLiteral("DMS");
const QString CoordinateConversionController::MGRS_FORMAT = QStringLiteral("MGRS");
const QString CoordinateConversionController::USNG_FORMAT = QStringLiteral("USGS");
const QString CoordinateConversionController::UTM_FORMAT = QStringLiteral("UTM");
const QString CoordinateConversionController::COORDINATE_FORMAT_PROPERTY = QStringLiteral("CoordinateFormat");

/*!
  \brief A constructor that accepts an optional \a parent.
 */
CoordinateConversionController::CoordinateConversionController(QObject* parent):
  AbstractTool(parent),
  m_coordinateFormats{DECIMAL_DEGREES_FORMAT,
                      DEGREES_DECIMAL_MINUTES_FORMAT,
                      DEGREES_MINUTES_SECONDS_FORMAT,
                      MGRS_FORMAT,
                      USNG_FORMAT,
                      UTM_FORMAT }
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

  Before calling this method, set the \l inputMode property to the
  notation type of \a notation, and call \l setSpatialReference property
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
    const QString name = option->name();
    if (name.compare(m_inputFormat) == 0)
    {
      inputOption = option;
      break;
    }
  }

  if (inputOption == nullptr)
    return Point();

  switch (inputOption->outputMode())
  {
  case CoordinateType::CoordinateTypeGars:
  {
    return CoordinateFormatter::fromGars(incomingNotation,
                                         m_spatialReference,
                                         Esri::ArcGISRuntime::GarsConversionMode::Center);
  }
  case CoordinateType::CoordinateTypeGeoRef:
  {
    return CoordinateFormatter::fromGeoRef(incomingNotation,
                                           m_spatialReference);
  }
  case CoordinateType::CoordinateTypeLatLon:
  {
    return CoordinateFormatter::fromLatitudeLongitude(incomingNotation,
                                                      m_spatialReference);
  }
  case CoordinateType::CoordinateTypeMgrs:
  {
    return CoordinateFormatter::fromMgrs(incomingNotation,
                                         m_spatialReference,
                                         static_cast<Esri::ArcGISRuntime::MgrsConversionMode>(inputOption->mgrsConversionMode()));
  }
  case CoordinateType::CoordinateTypeUsng:
  {
    return CoordinateFormatter::fromUsng(incomingNotation,
                                         m_spatialReference);
  }
  case CoordinateType::CoordinateTypeUtm:
  {
    return CoordinateFormatter::fromUtm(incomingNotation,
                                        m_spatialReference,
                                        static_cast<Esri::ArcGISRuntime::UtmConversionMode>(inputOption->utmConversionMode()));
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
    const QString name = option->name();
    if (name.compare(m_inputFormat) == 0)
      continue;

    results.append(Result(name, convertPointInternal(option, m_pointToConvert), option->outputMode()));
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
QString CoordinateConversionController::convertPointInternal(CoordinateConversionOptions* option, const Esri::ArcGISRuntime::Point& point) const
{
  switch (option->outputMode())
  {
  case CoordinateType::CoordinateTypeGars:
  {
    return CoordinateFormatter::toGars(point);
  }
  case CoordinateType::CoordinateTypeGeoRef:
  {
    return CoordinateFormatter::toGeoRef(point, option->precision());
  }
  case CoordinateType::CoordinateTypeLatLon:
  {
    const auto format = static_cast<Esri::ArcGISRuntime::LatitudeLongitudeFormat>(option->latLonFormat());
    return CoordinateFormatter::toLatitudeLongitude(point, format, option->decimalPlaces());
  }
  case CoordinateType::CoordinateTypeMgrs:
  {
    const auto conversionMode = static_cast<Esri::ArcGISRuntime::MgrsConversionMode>(option->mgrsConversionMode());
    return CoordinateFormatter::toMgrs(point, conversionMode, option->decimalPlaces(), option->addSpaces());
  }
  case CoordinateType::CoordinateTypeUsng:
  {
    return CoordinateFormatter::toUsng(point, option->precision(),option->decimalPlaces());
  }
  case CoordinateType::CoordinateTypeUtm:
  {
    const auto conversionMode = static_cast<Esri::ArcGISRuntime::UtmConversionMode>(option->utmConversionMode());
    return CoordinateFormatter::toUtm(point, conversionMode, option->addSpaces());
  }
  default: {}
  }

  return QString();
}

/*!
  \property CoordinateConversionController::results
  \brief The conversion results as a list model.

  The results are automatically updated as conversions are run.
 */
QAbstractListModel* CoordinateConversionController::results()
{
  return resultsInternal();
}

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
  \internal
 */
QQmlListProperty<QObject> CoordinateConversionController::objects()
{
  return QQmlListProperty<QObject>(this, nullptr, objectAppend, nullptr, nullptr, nullptr);
}

/*!
  \internal
 */
void CoordinateConversionController::objectAppend(QQmlListProperty<QObject>* property, QObject* value)
{
  auto engine = qobject_cast<CoordinateConversionController*>(property->object);

  if (!engine)
    return;

  auto* option = qobject_cast<CoordinateConversionOptions*>(value);

  if (!option)
    return;

  engine->addOption(option);
}

bool CoordinateConversionController::isCaptureMode() const
{
  return m_captureMode;
}

void CoordinateConversionController::setCaptureMode(bool captureMode)
{
  if (captureMode == m_captureMode)
    return;

  m_captureMode = captureMode;

  setPointToConvert(Point());

  emit captureModeChanged();
}

void CoordinateConversionController::onMouseClicked(const Point& clickedPoint)
{
  if (!isActive())
    return;

  if (isCaptureMode())
     setPointToConvert(clickedPoint);
}

void CoordinateConversionController::onLocationChanged(const Point& location)
{
  if (!isActive())
    return;

  if (!isCaptureMode())
    setPointToConvert(location);
}

QString CoordinateConversionController::inputFormat() const
{
  return m_inputFormat;
}

void CoordinateConversionController::setInputFormat(const QString& inputFormat)
{
  if (m_inputFormat == inputFormat)
    return;

  if (!m_coordinateFormats.contains(inputFormat))
    return;

  m_inputFormat = inputFormat;

  addCoordinateFormat(m_inputFormat);

  if (m_runConversion)
    convertPoint();

  emit inputFormatChanged();
  emit pointToConvertChanged();
}

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

void CoordinateConversionController::setProperties(const QVariantMap& properties)
{
  auto findFormatIt = properties.find(COORDINATE_FORMAT_PROPERTY);
  if (findFormatIt != properties.end())
    setInputFormat(findFormatIt.value().toString());
}

QString CoordinateConversionController::pointToConvert() const
{
  for (CoordinateConversionOptions* option : m_options)
  {
    if (option->name().compare(m_inputFormat) == 0)
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

void CoordinateConversionController::addCoordinateFormat(const QString& newFormat)
{
  if (!m_coordinateFormats.contains(newFormat))
    return;

  auto it = m_options.cbegin();
  auto itEnd = m_options.cend();
  for (; it != itEnd; ++it)
  {
    const auto& option = *it;
    if (option->name().compare(newFormat) == 0)
      return;
  }

  CoordinateConversionOptions* option = new CoordinateConversionOptions(this);
  option->setName(newFormat);

  if (newFormat == DEGREES_DECIMAL_MINUTES_FORMAT)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(CoordinateConversionOptions::LatitudeLongitudeFormat::LatitudeLongitudeFormatDegreesDecimalMinutes);
  }
  else if (newFormat == USNG_FORMAT)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeUsng);
    option->setPrecision(7);
    option->setAddSpaces(true);
  }
  else if (newFormat == UTM_FORMAT)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeUtm);
    option->setUtmConversionMode(CoordinateConversionOptions::UtmConversionMode::UtmConversionModeNorthSouthIndicators);
    option->setAddSpaces(true);
  }
  else if (newFormat == DEGREES_MINUTES_SECONDS_FORMAT)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(CoordinateConversionOptions::LatitudeLongitudeFormat::LatitudeLongitudeFormatDegreesMinutesSeconds);
    option->setDecimalPlaces(6);
  }
  else if (newFormat == MGRS_FORMAT)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeMgrs);
    option->setMgrsConversionMode(CoordinateConversionOptions::MgrsConversionMode::MgrsConversionModeAutomatic);
  }
  else if (newFormat == DECIMAL_DEGREES_FORMAT)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(CoordinateConversionOptions::LatitudeLongitudeFormat::LatitudeLongitudeFormatDecimalDegrees);
  }
  else
  {
    delete option;
    return;
  }

  addOption(option);

  if (m_runConversion)
    convertPoint();
}

void CoordinateConversionController::removeCoordinateFormat(const QString& formatToRemove)
{
  if (formatToRemove.compare(m_inputFormat) == 0)
    return;

  bool removed = false;
  auto it = m_options.begin();
  auto itEnd = m_options.end();
  for (; it != itEnd; ++it)
  {
    const auto& option = *it;
    if (option->name().compare(formatToRemove) == 0)
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
  \fn void CoordinateConversionController::inputGarsConversionModeChanged();
  \brief Signal emitted when the \l inputGarsConversionMode property changes.
 */

/*!
  \fn void CoordinateConversionController::inputMgrsConversionModeChanged();
  \brief Signal emitted when the \l inputMgrsConversionMode property changes.
 */

/*!
  \fn void CoordinateConversionController::inputUtmConversionModeChanged();
  \brief Signal emitted when the \l inputUtmConversionMode property changes.
 */

/*!
  \fn void CoordinateConversionController::runConversionChanged();
  \brief Signal emitted when the \l runConversion property changes.
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
