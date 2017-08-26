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

#include "ArcGISRuntimeSolutions.h"
#include <QtQml>

#include "CoordinateConversionController.h"
#include "CoordinateConversionOptions.h"
#include "CoordinateConversionResults.h"

static constexpr int s_version_major = 1;
static constexpr int s_version_minor = 0;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Solutions
{

ArcGISRuntimeSolutions::ArcGISRuntimeSolutions(QObject* parent) :
  QQmlExtensionPlugin(parent)
{
#ifdef QT_STATICPLUGIN
  Q_INIT_RESOURCE(ArcGISRuntimeSolutions);
#endif
}

void ArcGISRuntimeSolutions::registerTypes(const char* uri)
{
  qmlRegisterType<CoordinateConversionController>(uri, s_version_major, s_version_minor, "CoordinateConversionController");
  qmlRegisterType<CoordinateConversionOptions>(uri, s_version_major, s_version_minor, "CoordinateConversionOptions");
  qmlRegisterType<CoordinateConversionResults>(uri, s_version_major, s_version_minor, "CoordinateConversionResults");
}

} // Solutions
} // ArcGISRuntime
} // Esri
