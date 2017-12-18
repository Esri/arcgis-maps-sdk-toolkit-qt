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
/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISRuntimeToolkit
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.2
  \brief The main plugin class for the ArcGISRuntimeToolkit.

  You must call the static \l registerToolkitTypes function
  to ensure all the types are accessible in the QML environment.
  You may alternately do this manually if you only need specific types.
 */

static CoordinateConversionOptions* m_optionsProvider = nullptr;

QObject* optionsProvider(QQmlEngine* engine, QJSEngine*)
{
  if (!m_optionsProvider)
  {
    m_optionsProvider = new CoordinateConversionOptions(engine);
  }

  return m_optionsProvider;
}

/*!
   \brief Constructor that accepts an optional \a parent object.
 */
ArcGISRuntimeToolkit::ArcGISRuntimeToolkit(QObject* parent) :
  QQmlExtensionPlugin(parent)
{
#ifdef QT_STATICPLUGIN
  Q_INIT_RESOURCE(ArcGISRuntimeToolkit);
#endif
}

/*!
  \brief Type registration function to ensure the types are accessible
  in the QML environment.

  You can use this method if you want to customize the import path. For ease
  of use, \l registerToolkitTypes can be used instead without needing to instantiate
  this class.

  \list
  \li \a uri - The namespace in which to register the types.
  \endlist
 */
void ArcGISRuntimeToolkit::registerTypes(const char* uri)
{
  registerToolkitTypes(uri);
}

/*!
  \brief Static type registration function to ensure the types are accessible
  in the QML environment.

  This is the starting point for most uses of the ArcGISRuntimeToolkit. If you
  want to use the ArcGISRuntimeToolkit out of the box, then this method (with
  the default argument) will register all the types with the default import
  path.

  \list
  \li \a uri - The namespace in which to register the types.
  \endlist
 */
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
