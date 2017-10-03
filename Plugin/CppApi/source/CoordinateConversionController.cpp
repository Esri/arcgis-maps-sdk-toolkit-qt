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
#include <QApplication>

#include "ToolManager.h"
#include "ToolResourceProvider.h"
#include "MapQuickView.h"
#include "SceneQuickView.h"
#include <functional>

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionController
  \since Esri::ArcGISRuntime 100.2
  \brief A tool that performs coordinate conversions to and from notations (strings)
  and points.

  This tool abstracts the ArcGIS Runtime SDK class \l CoordinateFormatter and provides a UI
  to allow for combining many sets of options that can all be converted in a single operation.

  \sa CoordinateConversionOptions
 */

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

using CoordinateType = CoordinateConversionOptions::CoordinateType;

/*!
  \brief Default constructor with an optional \a parent.
 */
CoordinateConversionController::CoordinateConversionController(QObject* parent):
  AbstractTool(parent)
{
  ToolManager::instance().addTool(this);

  const auto setupGeoView = [this](GeoView* geoView) -> bool
  {
    if (!geoView)
      return false;

    if (dynamic_cast<Esri::ArcGISRuntime::SceneQuickView*>(geoView))
    {
      auto sceneView = static_cast<Esri::ArcGISRuntime::SceneQuickView*>(geoView);

      setSpatialReference(sceneView->spatialReference());
      connect(sceneView, &Esri::ArcGISRuntime::SceneQuickView::spatialReferenceChanged, this,
      [sceneView, this]()
      {
        setSpatialReference(sceneView->spatialReference());
      });

      connect(sceneView, &Esri::ArcGISRuntime::SceneQuickView::mouseClicked, this,
      [sceneView, this](QMouseEvent& mouseEvent)
      {
        setPointToConvert(sceneView->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
      });
    }
    else if (dynamic_cast<Esri::ArcGISRuntime::MapQuickView*>(geoView))
    {
      auto mapView = static_cast<Esri::ArcGISRuntime::MapQuickView*>(geoView);

      setSpatialReference(mapView->spatialReference());
      connect(mapView, &Esri::ArcGISRuntime::MapQuickView::spatialReferenceChanged, this,
      [mapView, this]()
      {
        setSpatialReference(mapView->spatialReference());
      });

      connect(mapView, &Esri::ArcGISRuntime::MapQuickView::mouseClicked, this,
      [mapView, this](QMouseEvent& mouseEvent)
      {
        setPointToConvert(mapView->screenToLocation(mouseEvent.x(), mouseEvent.y()));
      });
    }

    return true;
  };

  // try to setup the connections now if the geoview is ready, otherwise wait for the view
  if (!setupGeoView(ToolResourceProvider::instance()->geoView()))
  {
    connect(ToolResourceProvider::instance(), &ToolResourceProvider::geoViewChanged, this, [this, setupGeoView]()
    {
      setupGeoView(ToolResourceProvider::instance()->geoView());
    });
  }

  connect(this, &CoordinateConversionController::optionsChanged, this,
  [this]()
  {
    QList<Result> list;
    for (const auto& option : m_options)
      list.append(Result(option->name(), QString(), option->outputMode()));

    results()->setResults(std::move(list));
  });
}

/*!
   \brief Destructor.
 */
CoordinateConversionController::~CoordinateConversionController()
{
}

/*!
  \brief Converts \a notation and updates the results property.

  \note \l inputMode must be set to the appropriate type.
  \note A valid spatial reference must be set before calling this method.

  \warning It is preferable to convert from a Point to avoid any potential
  loss of precision problems. Loss of precision may only be applicable to some formats.
 */
void CoordinateConversionController::convertNotation(const QString& notation)
{
  QList<Result> results;
  const Point convertedPoint = pointFromNotation(notation);

  for (CoordinateConversionOptions* option : m_options)
  {
    const QString name = option->name();
    results.append(Result(name, convertPointInternal(option, convertedPoint), option->outputMode()));
  }

  this->results()->setResults(std::move(results));
  emit resultsChanged();
}

/*!
  \internal
 */
Point CoordinateConversionController::pointFromNotation(const QString& incomingNotation)
{
  if (m_spatialReference.isEmpty())
    qWarning("The spatial reference property is empty: conversions will fail.");

  switch (m_inputMode)
  {
  case CoordinateType::CoordinateTypeGars:
  {
    return CoordinateFormatter::fromGars(incomingNotation,
                                         m_spatialReference,
                                         static_cast<Esri::ArcGISRuntime::GarsConversionMode>(m_inputGarsConversionMode));
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
                                         static_cast<Esri::ArcGISRuntime::MgrsConversionMode>(m_inputMgrsConversionMode));
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
                                        static_cast<Esri::ArcGISRuntime::UtmConversionMode>(m_inputUtmConversionMode));
  }
  default: {}
  }

  return Point();
}

/*!
  \brief Converts the last point assigned with \l setPointToConvert to all the
  options specified.
 */
void CoordinateConversionController::convertPoint()
{
  QList<Result> results;

  for (CoordinateConversionOptions* option : m_options)
  {
    const QString name = option->name();
    results.append(Result(name, convertPointInternal(option, m_pointToConvert), option->outputMode()));
  }

  this->results()->setResults(std::move(results));
  emit resultsChanged();
}

/*!
  \internal
 */
QString CoordinateConversionController::convertPointInternal(CoordinateConversionOptions* option, const Esri::ArcGISRuntime::Point& point)
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
  \qmlproperty CoordinateType CoordinateConversionController::inputMode

  \brief Gets the current input conversion mode.

  \note This is what is used for the \l convertNotation method.
 */
CoordinateType CoordinateConversionController::inputMode() const
{
  return m_inputMode;
}

/*!
  \brief Sets the current input conversion mode to \a inputMode.

  \note This is what is used for the \l convertNotation method.
 */
void CoordinateConversionController::setInputMode(CoordinateType inputMode)
{
  m_inputMode = inputMode;
  emit inputModeChanged();
}

/*!
  \qmlmethod CoordinateConversionResults* CoordinateConversionController::results()

  \brief Gets the results as a list model.

  \note The results are automatically updated as conversions are run.
 */
CoordinateConversionResults* CoordinateConversionController::results()
{
  if (!m_results)
  {
    m_results = new CoordinateConversionResults(this);
    connect(m_results, &CoordinateConversionResults::resultsChanged, this, &CoordinateConversionController::resultsChanged);
  }

  return m_results;
}

/*!
  \qmlmethod list<CoordinateConversionOptions> CoordinateConversionController::options()

  \brief Gets the options as a list.
 */
QQmlListProperty<CoordinateConversionOptions> CoordinateConversionController::options()
{
  return QQmlListProperty<CoordinateConversionOptions>(this, &m_options,
                                                       CoordinateConversionOptions::listAppend,
                                                       CoordinateConversionOptions::listCount,
                                                       CoordinateConversionOptions::listAt,
                                                       CoordinateConversionOptions::listClear);
}

/*!
  \brief Sets the spatial reference to \a spatialReference.

  \note This is required before calling the \l convertNotation method.
 */
void CoordinateConversionController::setSpatialReference(const SpatialReference& spatialReference)
{
  m_spatialReference = spatialReference;
}

/*!
  \qmlproperty GarsConversionMode CoordinateConversionController::inputGarsConversionMode

  \brief Gets the inputGarsConversionMode.

  \note This property is only used if the \l inputMode is set to Gars.
 */
CoordinateConversionOptions::GarsConversionMode CoordinateConversionController::inputGarsConversionMode() const
{
  return m_inputGarsConversionMode;
}

/*!
  \brief Sets the inputGarsConversionMode to \a inputGarsConversionMode.

  \note This property is only used if the \l inputMode is set to Gars.
 */
void CoordinateConversionController::setInputGarsConversionMode(CoordinateConversionOptions::GarsConversionMode inputGarsConversionMode)
{
  m_inputGarsConversionMode = inputGarsConversionMode;
  emit inputGarsConversionModeChanged();
}

/*!
  \qmlproperty MgrsConversionMode CoordinateConversionController::inputMgrsConversionMode

  \brief Gets the inputMgrsConversionMode.

  \note This property is only used if the \l inputMode is set to Mgrs.
 */
CoordinateConversionOptions::MgrsConversionMode CoordinateConversionController::inputMgrsConversionMode() const
{
  return m_inputMgrsConversionMode;
}

/*!
  \brief Sets the inputMgrsConversionMode to \a inputMgrsConversionMode.

  \note This property is only used if the \l inputMode is set to Mgrs.
 */
void CoordinateConversionController::setInputMgrsConversionMode(CoordinateConversionOptions::MgrsConversionMode inputMgrsConversionMode)
{
  m_inputMgrsConversionMode = inputMgrsConversionMode;
  emit inputMgrsConversionModeChanged();
}

/*!
  \qmlproperty UtmConversionMode CoordinateConversionController::inputUtmConversionMode

  \brief Gets the inputUtmConversionMode.

  \note This property is only used if the \l inputMode is set to Utm.
 */
CoordinateConversionOptions::UtmConversionMode CoordinateConversionController::inputUtmConversionMode() const
{
  return m_inputUtmConversionMode;
}

/*!
  \brief Sets the inputUtmConversionMode to \a inputUtmConversionMode.

  \note This property is only used if the \l inputMode is set to Utm.
 */
void CoordinateConversionController::setInputUtmConversionMode(CoordinateConversionOptions::UtmConversionMode inputUtmConversionMode)
{
  m_inputUtmConversionMode = inputUtmConversionMode;
  emit inputUtmConversionModeChanged();
}

/*!
  \brief Sets the next point to be converted via the \l convertPoint method.

  \note If the \l runConversion property is \c true, the conversion will be run immediately.
 */
void CoordinateConversionController::setPointToConvert(const Point& point)
{
  m_pointToConvert = point;

  if (m_runConversion)
    convertPoint();
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

/*!
  \brief Helper method to programtically add a CoordinateConversionOptions \a option
  to the list of options.
 */
void CoordinateConversionController::addOption(CoordinateConversionOptions* option)
{
  m_options.append(option);
  emit optionsChanged();
}

/*!
  \brief Helper method to clear all the options from the list.
 */
void CoordinateConversionController::clearOptions()
{
  m_options.clear();
  emit optionsChanged();
}

/*!
  \brief Gets the name of this tool.
 */
QString CoordinateConversionController::toolName() const
{
  return "CoordinateConversion";
}

/*!
  \qmlmethod void CoordinateConversionController::copyToClipboard(string text)

  \brief Helper method to copy \a text to the clipboard.
 */
void CoordinateConversionController::copyToClipboard(const QString& text)
{
  auto clipboard = QApplication::clipboard();
  if (clipboard)
    clipboard->setText(text);
}

/*!
  \qmlmethod void CoordinateConversionController::clearResults()

  \brief Clears the results strings from the results list model.
 */
void CoordinateConversionController::clearResults()
{
  if (m_results)
    m_results->clearResults();
}

/*!
  \qmlproperty bool CoordinateConversionController::runConversion

  \brief Gets whether the conversion should be run automatically when \l setPointToConvert
  is called.
 */
bool CoordinateConversionController::runConversion() const
{
  return m_runConversion;
}

/*!
  \brief Sets whether the conversion should be run automatically when \l setPointToConvert
  is called to \a runConversion.
 */
void CoordinateConversionController::setRunConversion(bool runConversion)
{
  m_runConversion = runConversion;
  emit runConversionChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
