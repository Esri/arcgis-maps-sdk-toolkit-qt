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
#include "BasemapGalleryController.h"
#include "BasemapGalleryItem.h"
#include "CoordinateConversionController.h"
#include "CoordinateConversionOption.h"
#include "CoordinateConversionResult.h"
#include "NorthArrowController.h"
#include "OverviewMapController.h"
#include "PopupViewController.h"
#include "TimeSliderController.h"

// Internal includes
#include "Internal/BasemapGalleryImageProvider.h"

// ArcGIS includes
#include <Point.h>
#include <MapQuickView.h>

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
constexpr int VERSION_MINOR = 13;

/*
 \internal
 \brief Function for registration. Registers the C++ type Foo as
 FooCPP in QML with the appropriate version and namespace information.
 
 \note In QML, we alias the QML type \c FooCPP to QML type \c Foo using the
 qml files found in the `+cpp_api` folder of our QML directory.
 
 \list
  \li \c T Type to register in QML.
  \li \a minorVersion The version at which the component was created.
 \endlist
 */
template <typename T>
void registerComponent(int minorVersion)
{
  static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
  auto name = QString{T::staticMetaObject.className()};
  name.remove("Esri::ArcGISRuntime::Toolkit::");
  qmlRegisterType<T>(NAMESPACE, VERSION_MAJOR, minorVersion, name.toLatin1());
}

/*
 \internal
 \brief Ensures a Module revision is available from 100.10 onwards 
 to the current version of the Toolkit.
 */
void registerModuleRevisions()
{
  constexpr int START_VERSION = 10;
  for (int i = START_VERSION; i <= VERSION_MINOR; ++i)
    qmlRegisterModule(NAMESPACE, VERSION_MAJOR, i);
}

} // namespace

void registerComponents_cpp_(QQmlEngine& appEngine)
{
  appEngine.addImageProvider(BasemapGalleryImageProvider::PROVIDER_ID, BasemapGalleryImageProvider::instance());
  appEngine.addImportPath(ESRI_COM_PATH);
  registerModuleRevisions();
  registerComponent<AuthenticationController>(10);
  registerComponent<BasemapGalleryController>(12);
  registerComponent<BasemapGalleryItem>(12);
  registerComponent<CoordinateConversionController>(10);
  registerComponent<CoordinateConversionOption>(10);
  registerComponent<CoordinateConversionResult>(10);
  registerComponent<NorthArrowController>(10);
  registerComponent<OverviewMapController>(12);
  registerComponent<PopupViewController>(10);
  registerComponent<TimeSliderController>(10);

  // Register ArcGISRuntime types with toolkit.
  qRegisterMetaType<Point>("Esri::ArcGISRuntime::Point");
  qmlRegisterAnonymousType<MapQuickView>(NAMESPACE, 12);
}

} // Toolkit
} // ArcGISRuntime
} // Esri
