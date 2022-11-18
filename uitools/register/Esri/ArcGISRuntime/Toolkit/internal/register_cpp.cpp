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
#include "BookmarksViewController.h"
#include "BookmarkListItem.h"
#include "CoordinateConversionController.h"
#include "CoordinateConversionOption.h"
#include "CoordinateConversionResult.h"
#include "FloorFilterController.h"
#include "FloorFilterFacilityItem.h"
#include "FloorFilterLevelItem.h"
#include "FloorFilterSiteItem.h"
#include "LocatorSearchSource.h"
#include "NorthArrowController.h"
#include "OverviewMapController.h"
#include "PopupViewController.h"
#include "ScalebarController.h"
#include "SearchResult.h"
#include "SearchSourceInterface.h"
#include "SearchSuggestion.h"
#include "SearchViewController.h"
#include "SmartLocatorSearchSource.h"
#include "TimeSliderController.h"
#include "UtilityNetworkFunctionTraceResult.h"
#include "UtilityNetworkFunctionTraceResultsModel.h"
#include "UtilityNetworkListItem.h"
#include "UtilityNetworkTraceController.h"
#include "UtilityNetworkTraceStartingPoint.h"
#include "UtilityNetworkTraceStartingPointsModel.h"

// Internal includes
#include "Internal/BasemapGalleryImageProvider.h"

// ArcGIS includes
#include <MapQuickView.h>
#include <Point.h>

// Qt Includes
#include <QQmlEngine>
#include <QQmlFileSelector>

// std includes
#include <type_traits>

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {

    const QString ESRI_COM_PATH = QStringLiteral(":/esri.com/imports");

    constexpr char const* NAMESPACE = "Esri.ArcGISRuntime.Toolkit.Controller";

    constexpr int VERSION_MAJOR = 200;
    constexpr int VERSION_MINOR = 0;

    /*
      \internal
      \brief This namespace is an implementation detail for how to register types with QML.

      For usage see `registerComponent`.

      Provided is the overloaded method:

      \code
      void registerComponentImpl(<CreationType> creationType, int majorVersion, int minorVersion, const char* name)
      \endcode

      And the three currently accepted values for the \a creationType parameter.

      \value CreationType::Creatable for types that can be created in QML.
      \value CreationType::Uncreatable for types that can be referenced but not used in QML.
      \value CreationType::Interface for types that are interfaces for more concrete QML types.
     */
    namespace CreationType {
      struct Creatable_
      {
      };

      template <class T>
      void registerComponentImpl(CreationType::Creatable_, int majorVersion, int minorVersion, const char* name)
      {
        qmlRegisterType<T>(NAMESPACE, majorVersion, minorVersion, name);
      }

      constexpr Creatable_ Creatable = Creatable_{};

      struct Uncreatable_
      {
      };

      template <class T>
      void registerComponentImpl(CreationType::Uncreatable_, int majorVersion, int minorVersion, const char* name)
      {
        qmlRegisterUncreatableType<T>(NAMESPACE, majorVersion, minorVersion, name, "Cannot instantiate type in QML.");
      }

      constexpr Uncreatable_ Uncreatable = Uncreatable_{};

      struct Interface_
      {
      };

      template <class T>
      void registerComponentImpl(CreationType::Interface_, int majorVersion, int /*minorVersion*/, const char* /*name*/)
      {
        qmlRegisterInterface<T>(NAMESPACE, majorVersion);
      }

      constexpr Interface_ Interface = Interface_{};
    }

    /*
     \internal
     \brief Function for registration. Registers the C++ type Foo as
     Foo in QML with the 100.10, 200.0 version and namespace information.

     \list
      \li \a Determines how the type is instantiated in QML. Choose between CreationType::Creatable, CreationType::Uncreatable and CreationType::Interface.
          CreationType::Creatable is assumed by default if not provided.
     \endlist

      Example call:
      \code
      registerComponent<LocatorSearchSource>(CreationType::Uncreatable);
      \endcode
     */
    template <typename T, typename CType = CreationType::Creatable_>
    void registerComponent(CType creationType = CType{})
    {
      static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
      auto name = QString{T::staticMetaObject.className()};
      name.remove("Esri::ArcGISRuntime::Toolkit::");
      // register component on version 100
      CreationType::registerComponentImpl<T>(creationType, 100, 10, name.toLatin1());
      // register component on version 200
      CreationType::registerComponentImpl<T>(creationType, 200, 0, name.toLatin1());
    }

    /*
     \internal
     \brief Ensures a Module revision is available from 100.10 and 200.0 onwards
     to the current version of the Toolkit.
     */
    void registerModuleRevisions()
    {
      constexpr int MAJOR_VERSION_100 = 100;
      constexpr int START_MINOR_VERSION_100 = 10;
      constexpr int END_MINOR_VERSION_100 = 15;
      // register version 100
      for (int i = START_MINOR_VERSION_100; i <= END_MINOR_VERSION_100; ++i)
      {
        qmlRegisterModule(NAMESPACE, MAJOR_VERSION_100, i);
      }
      // register version 200 onwards
      for (int i = 0; i <= VERSION_MINOR; ++i)
      {
        qmlRegisterModule(NAMESPACE, VERSION_MAJOR, i);
      }
    }

  } // namespace

  void registerComponents_cpp_(QQmlEngine& appEngine)
  {
    appEngine.addImageProvider(BasemapGalleryImageProvider::PROVIDER_ID, BasemapGalleryImageProvider::instance());
    appEngine.addImportPath(ESRI_COM_PATH);
    registerModuleRevisions();
    registerComponent<AuthenticationController>();
    registerComponent<BasemapGalleryController>();
    registerComponent<BasemapGalleryItem>();
    registerComponent<BookmarksViewController>();
    registerComponent<BookmarkListItem>();
    registerComponent<CoordinateConversionController>();
    registerComponent<CoordinateConversionOption>();
    registerComponent<CoordinateConversionResult>();
    registerComponent<FloorFilterController>();
    registerComponent<FloorFilterFacilityItem>(CreationType::Uncreatable);
    registerComponent<FloorFilterLevelItem>(CreationType::Uncreatable);
    registerComponent<FloorFilterSiteItem>(CreationType::Uncreatable);
    registerComponent<LocatorSearchSource>(CreationType::Uncreatable);
    registerComponent<NorthArrowController>();
    registerComponent<OverviewMapController>();
    registerComponent<PopupViewController>();
    registerComponent<ScalebarController>();
    registerComponent<SearchResult>();
    registerComponent<SearchSourceInterface>(CreationType::Interface);
    registerComponent<SearchSuggestion>();
    registerComponent<SearchViewController>(CreationType::Creatable);
    registerComponent<SmartLocatorSearchSource>(CreationType::Uncreatable);
    registerComponent<TimeSliderController>();
    registerComponent<UtilityNetworkFunctionTraceResult>();
    registerComponent<UtilityNetworkFunctionTraceResultsModel>();
    registerComponent<UtilityNetworkListItem>();
    registerComponent<UtilityNetworkTraceController>();
    registerComponent<UtilityNetworkTraceStartingPoint>();
    registerComponent<UtilityNetworkTraceStartingPointsModel>();

    // Register ArcGISRuntime types with toolkit.
    qRegisterMetaType<Point>("Esri::ArcGISRuntime::Point");
    qmlRegisterAnonymousType<MapQuickView>(NAMESPACE, 100);
    qmlRegisterAnonymousType<MapQuickView>(NAMESPACE, 200);
  }

} // Esri::ArcGISRuntime::Toolkit
