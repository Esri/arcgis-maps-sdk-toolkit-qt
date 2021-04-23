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
#include "register_cpp.h"

// Toolkit includes
#include "AuthenticationController.h"
#include "CoordinateConversionController.h"
#include "CoordinateConversionOption.h"
#include "CoordinateConversionResult.h"
#include "NorthArrowController.h"
#include "PopupViewController.h"
#include "TimeSliderController.h"

// ArcGIS includes
#include <Point.h>

// Qt Includes
#include <QQmlEngine>
#include <QQmlFileSelector>

// std includes
#include <type_traits>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

namespace
{

const QString ESRI_COM_PATH = QStringLiteral(":/esri.com/imports");

constexpr char const* NAMESPACE = "Esri.ArcGISRuntime.Toolkit.Controller";

constexpr int VERSION_MAJOR = 100;
constexpr int VERSION_MINOR = 11;

/*
 \internal
 \brief Function for registration. Registers the C++ type Foo as
 FooCPP in QML with the appropriate version and namespace information.
 
 \note In QML, we alias the QML type \c FooCPP to QML type \c Foo using the
 qml files found in the `+cpp_api` folder of our QML directory.
 
 \list
  \li \c T Type to register in QML.
 \endlist
 */
template <typename T>
void registerComponent()
{
  static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
  auto name = QString{T::staticMetaObject.className()};
  name.remove("Esri::ArcGISRuntime::Toolkit::");
  qmlRegisterType<T>(NAMESPACE, VERSION_MAJOR, VERSION_MINOR, name.toLatin1());
}

} // namespace

void registerComponents_cpp_(QQmlEngine& appEngine)
{
  appEngine.addImportPath(ESRI_COM_PATH);
  registerComponent<AuthenticationController>();
  registerComponent<CoordinateConversionController>();
  registerComponent<CoordinateConversionOption>();
  registerComponent<CoordinateConversionResult>();
  registerComponent<NorthArrowController>();
  registerComponent<PopupViewController>();
  registerComponent<TimeSliderController>();

  qRegisterMetaType<Point>("Esri::ArcGISRuntime::Point");
}

} // Toolkit
} // ArcGISRuntime
} // Esri
