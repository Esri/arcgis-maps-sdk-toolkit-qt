// Copyright 2016 ESRI
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

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

using CoordinateType = CoordinateConversionOptions::CoordinateType;

CoordinateConversionController::CoordinateConversionController(QObject* parent):
  QObject(parent)
{
  connect(this, &CoordinateConversionController::optionsChanged, this,
  [this]()
  {
    QList<Result> list;
    for (const auto& option : m_options)
      list.append(Result(option->name(), QString(), option->outputMode()));

    results()->setResults(std::move(list));
  });
}

CoordinateConversionController::~CoordinateConversionController()
{
}

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

CoordinateType CoordinateConversionController::inputMode() const
{
  return m_inputMode;
}

void CoordinateConversionController::setInputMode(CoordinateType inputMode)
{
  m_inputMode = inputMode;
  emit inputModeChanged();
}

CoordinateConversionResults* CoordinateConversionController::results()
{
  if (!m_results)
  {
    m_results = new CoordinateConversionResults(this);
    connect(m_results, &CoordinateConversionResults::resultsChanged, this, &CoordinateConversionController::resultsChanged);
  }

  return m_results;
}

QQmlListProperty<CoordinateConversionOptions> CoordinateConversionController::options()
{
  return QQmlListProperty<CoordinateConversionOptions>(this, &m_options,
                                                       CoordinateConversionOptions::listAppend,
                                                       CoordinateConversionOptions::listCount,
                                                       CoordinateConversionOptions::listAt,
                                                       CoordinateConversionOptions::listClear);
}

void CoordinateConversionController::setSpatialReference(const SpatialReference& spatialReference)
{
  m_spatialReference = spatialReference;
}

CoordinateConversionOptions::GarsConversionMode CoordinateConversionController::inputGarsConversionMode() const
{
  return m_inputGarsConversionMode;
}

void CoordinateConversionController::setInputGarsConversionMode(CoordinateConversionOptions::GarsConversionMode inputGarsConversionMode)
{
  m_inputGarsConversionMode = inputGarsConversionMode;
  emit inputGarsConversionModeChanged();
}

CoordinateConversionOptions::MgrsConversionMode CoordinateConversionController::inputMgrsConversionMode() const
{
  return m_inputMgrsConversionMode;
}

void CoordinateConversionController::setInputMgrsConversionMode(CoordinateConversionOptions::MgrsConversionMode inputMgrsConversionMode)
{
  m_inputMgrsConversionMode = inputMgrsConversionMode;
  emit inputMgrsConversionModeChanged();
}

CoordinateConversionOptions::UtmConversionMode CoordinateConversionController::inputUtmConversionMode() const
{
  return m_inputUtmConversionMode;
}

void CoordinateConversionController::setInputUtmConversionMode(CoordinateConversionOptions::UtmConversionMode inputUtmConversionMode)
{
  m_inputUtmConversionMode = inputUtmConversionMode;
  emit inputUtmConversionModeChanged();
}

void CoordinateConversionController::setPointToConvert(const Point& point)
{
  m_pointToConvert = point;

  if (m_runConversion)
    convertPoint();
}

QQmlListProperty<QObject> CoordinateConversionController::objects()
{
  return QQmlListProperty<QObject>(this, nullptr, objectAppend, nullptr, nullptr, nullptr);
}

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

void CoordinateConversionController::addOption(CoordinateConversionOptions* option)
{
  m_options.append(option);
  emit optionsChanged();
}

void CoordinateConversionController::clearOptions()
{
  m_options.clear();
  emit optionsChanged();
}

QString CoordinateConversionController::toolName() const
{
  return "CoordinateConversion";
}

void CoordinateConversionController::copyToClipboard(const QString& text)
{
  QApplication::clipboard()->setText(text);
}

bool CoordinateConversionController::runConversion() const
{
  return m_runConversion;
}

void CoordinateConversionController::setRunConversion(bool runConversion)
{
  m_runConversion = runConversion;
  emit runConversionChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
