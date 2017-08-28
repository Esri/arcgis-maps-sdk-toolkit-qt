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

#include "ArcGISRuntimeToolkit.h"
#include <QtQml>

#include "CoordinateConversionController.h"
#include "CoordinateConversionOptions.h"
#include "CoordinateConversionResults.h"

static constexpr int s_version_major = 100;
static constexpr int s_version_minor = 2;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

static CoordinateConversionOptions* m_optionsProvider = nullptr;

QObject* optionsProvider(QQmlEngine* engine, QJSEngine*)
{
  if (!m_optionsProvider)
  {
    m_optionsProvider = new CoordinateConversionOptions(engine);
  }

  return m_optionsProvider;
}

ArcGISRuntimeSolutions::ArcGISRuntimeSolutions(QObject* parent) :
  QQmlExtensionPlugin(parent)
{
#ifdef QT_STATICPLUGIN
  Q_INIT_RESOURCE(ArcGISRuntimeSolutions);
#endif
}

void ArcGISRuntimeSolutions::registerTypes(const char* uri)
{
  registerSolutionsTypes(uri);
}

void ArcGISRuntimeSolutions::registerSolutionsTypes(const char* uri)
{
  // singletons
  qmlRegisterSingletonType<CoordinateConversionOptions>(uri, s_version_major, s_version_minor, "CoordinateConversionOptions", optionsProvider);

  // types
  qmlRegisterType<CoordinateConversionOptions>(uri, s_version_major, s_version_minor, "CoordinateConversionOptions");
  qmlRegisterType<CoordinateConversionResults>(uri, s_version_major, s_version_minor, "CoordinateConversionResults");
  qmlRegisterType<CoordinateConversionController>(uri, s_version_major, s_version_minor, "CoordinateConversionController");
  qRegisterMetaType<CoordinateConversionResults*>("CoordinateConversionResults*");

  // enums
  qRegisterMetaType<CoordinateConversionOptions::CoordinateType>("CoordinateConversionOptions::CoordinateType");
  qRegisterMetaType<CoordinateConversionOptions::GarsConversionMode>("CoordinateConversionOptions::GarsConversionMode");
  qRegisterMetaType<CoordinateConversionOptions::MgrsConversionMode>("CoordinateConversionOptions::MgrsConversionMode");
  qRegisterMetaType<CoordinateConversionOptions::UtmConversionMode>("CoordinateConversionOptions::UtmConversionMode");
  qRegisterMetaType<CoordinateConversionOptions::LatitudeLongitudeFormat>("CoordinateConversionOptions::LatitudeLongitudeFormat");
}

} // Toolkit
} // ArcGISRuntime
} // Esri
