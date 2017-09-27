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
#include "ArcGISCompassController.h"

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

ArcGISRuntimeToolkit::ArcGISRuntimeToolkit(QObject* parent) :
  QQmlExtensionPlugin(parent)
{
#ifdef QT_STATICPLUGIN
  Q_INIT_RESOURCE(ArcGISRuntimeToolkit);
#endif
}

void ArcGISRuntimeToolkit::registerTypes(const char* uri)
{
  registerToolkitTypes(uri);
}

void ArcGISRuntimeToolkit::registerToolkitTypes(const char* uri)
{
  // singletons
  qmlRegisterSingletonType<CoordinateConversionOptions>(uri, s_versionMajor, s_versionMinor, "CoordinateConversionOptions", optionsProvider);

  // types
  qmlRegisterType<CoordinateConversionOptions>(uri, s_versionMajor, s_versionMinor, "CoordinateConversionOptions");
  qmlRegisterType<CoordinateConversionResults>(uri, s_versionMajor, s_versionMinor, "CoordinateConversionResults");
  qmlRegisterType<CoordinateConversionController>(uri, s_versionMajor, s_versionMinor, "CoordinateConversionController");
  qRegisterMetaType<CoordinateConversionResults*>("CoordinateConversionResults*");
  qmlRegisterType<ArcGISCompassController>(uri, s_versionMajor, s_versionMinor, "ArcGISCompassController");

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
