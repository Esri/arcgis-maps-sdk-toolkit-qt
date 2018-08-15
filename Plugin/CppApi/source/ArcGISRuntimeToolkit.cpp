
/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

#include "ArcGISRuntimeToolkit.h"
#include <QtQml>

#include "ArcGISCompassController.h"
#include "CoordinateConversionController.h"
#include "TimeSliderController.h"

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

/*!
   \brief Constructor that accepts an optional \a parent object.
 */
ArcGISRuntimeToolkit::ArcGISRuntimeToolkit(QObject* parent) :
  QQmlExtensionPlugin(parent)
{
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
  // types
  qmlRegisterType<CoordinateConversionController>(uri, s_versionMajor100, s_versionMinorUpdate2, "CoordinateConversionController");
  qmlRegisterType<ArcGISCompassController>(uri, s_versionMajor100, s_versionMinorUpdate2, "ArcGISCompassController");
  qmlRegisterType<TimeSliderController>(uri, s_versionMajor100, s_versionMinorUpdate3, "TimeSliderController");
}

} // Toolkit
} // ArcGISRuntime
} // Esri
