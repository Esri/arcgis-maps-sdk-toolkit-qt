
/*******************************************************************************
 *  Copyright 2012-2025 Esri
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
#ifndef QRT_DISABLE_DEPRECATED_WARNINGS
#define QRT_DISABLE_DEPRECATED_WARNINGS
#endif
#include "register.h"

// Toolkit includes
#include "AttachmentsPopupElementViewController.h"
#include "AuthenticatorController.h"
#include "BarChartPopupMediaItem.h"
#include "BasemapGalleryController.h"
#include "BasemapGalleryItem.h"
#include "BookmarkListItem.h"
#include "BookmarksViewController.h"
#include "CoordinateConversionController.h"
#include "CoordinateConversionOption.h"
#include "CoordinateConversionResult.h"
#include "FieldsPopupElementViewController.h"
#include "FloorFilterController.h"
#include "FloorFilterFacilityItem.h"
#include "FloorFilterLevelItem.h"
#include "FloorFilterSiteItem.h"
#include "ImagePopupMediaItem.h"
#include "LineChartPopupMediaItem.h"
#include "LocatorSearchSource.h"
#include "MediaPopupElementViewController.h"
#include "NorthArrowController.h"
#include "OverviewMapController.h"
#include "PieChartPopupMediaItem.h"
#include "PopupAttachmentItem.h"
#include "PopupElementViewItem.h"
#include "PopupMediaItem.h"
#include "PopupViewController.h"
#include "ScalebarController.h"
#include "SearchResult.h"
#include "SearchSuggestion.h"
#include "SearchViewController.h"
#include "SmartLocatorSearchSource.h"
#include "TextPopupElementViewController.h"
#include "TimeSliderController.h"
#include "UtilityNetworkFunctionTraceResultsModel.h"
#include "UtilityNetworkTraceController.h"
#include "UtilityNetworkTraceStartingPoint.h"
#include "UtilityNetworkTraceStartingPointsModel.h"

// Internal includes
#include "BasemapGalleryImageProvider.h"
#include "PopupAttachmentImageProvider.h"
#include "QmlEnums.h"

// ArcGIS includes
#include <MapQuickView.h>
#include <Point.h>

// Qt Includes
#include <QPointer>
#include <QQmlEngine>
#include <QQmlFileSelector>

// std includes
#include <type_traits>

/*!
  \headerfile Esri/ArcGISRuntime/Toolkit/register
    \inmodule ArcGISRuntimeToolkit

    This file contains the registration function required to register the toolkit
    with the `QQmlEngine`.

    Please refer to
    \c README.md for more information on workflows.
   */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::registerComponents(QQmlEngine& engine)
  \relates Esri/ArcGISRuntime/Toolkit/register
  \brief This registration function must be called after the QML \a engine has been
  declared, but before it is run. This sets up resources and component registration
  with the `QQmlEngine` and the toolkit.
 */

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {

    const QString ESRI_COM_PATH = QStringLiteral(":/esri.com/imports");

    constexpr char const* NAMESPACE = "Esri.ArcGISRuntime.Toolkit.Controller";

    constexpr int VERSION_MAJOR = 3;
    constexpr int VERSION_MINOR = 0;

    QPointer<AuthenticatorController> s_authenticatorController;

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

      [[maybe_unused]] constexpr Creatable_ Creatable = Creatable_{};

      struct Uncreatable_
      {
      };

      template <class T>
      void registerComponentImpl(CreationType::Uncreatable_, int majorVersion, int minorVersion, const char* name)
      {
        qmlRegisterUncreatableType<T>(NAMESPACE, majorVersion, minorVersion, name, "Cannot instantiate type in QML.");
      }

      constexpr Uncreatable_ Uncreatable = Uncreatable_{};

      struct Singleton_
      {
      };

      template <class T>
      void registerComponentImpl(CreationType::Singleton_, int majorVersion, int minorVersion, const char* name)
      {
        qmlRegisterSingletonType<T>(NAMESPACE, majorVersion, minorVersion, name,
                                    [](QQmlEngine* qmlEngine, QJSEngine* jsEngine) -> QObject*
                                    {
                                      if (!s_authenticatorController)
                                      {
                                        s_authenticatorController = T::create(qmlEngine, jsEngine);
                                      }
                                      return s_authenticatorController;
                                    });
      }

      [[maybe_unused]] constexpr Singleton_ Singleton = Singleton_{};
    }

    /*
     \internal
     \brief Function for registration. Registers the C++ type Foo as
     Foo in QML with the 3.0 version and namespace information.

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
      CreationType::registerComponentImpl<T>(creationType, VERSION_MAJOR, VERSION_MINOR, name.toLatin1());
    }

  } // namespace

  void registerComponents(QQmlEngine& appEngine)
  {
    appEngine.addImageProvider(BasemapGalleryImageProvider::PROVIDER_ID, BasemapGalleryImageProvider::instance());
    appEngine.addImageProvider(PopupAttachmentImageProvider::PROVIDER_ID, PopupAttachmentImageProvider::instance());
    appEngine.addImportPath(ESRI_COM_PATH);
    qmlRegisterModule(NAMESPACE, VERSION_MAJOR, VERSION_MINOR);
    registerComponent<AuthenticatorController>(CreationType::Singleton);
    registerComponent<AttachmentsPopupElementViewController>();
    registerComponent<BarChartPopupMediaItem>();
    registerComponent<BasemapGalleryController>();
    registerComponent<BasemapGalleryItem>();
    registerComponent<BookmarksViewController>();
    registerComponent<BookmarkListItem>();
    registerComponent<CoordinateConversionController>();
    registerComponent<CoordinateConversionOption>();
    registerComponent<CoordinateConversionResult>();
    registerComponent<FieldsPopupElementViewController>();
    registerComponent<FloorFilterController>();
    registerComponent<FloorFilterFacilityItem>(CreationType::Uncreatable);
    registerComponent<FloorFilterLevelItem>(CreationType::Uncreatable);
    registerComponent<FloorFilterSiteItem>(CreationType::Uncreatable);
    registerComponent<ImagePopupMediaItem>();
    registerComponent<LineChartPopupMediaItem>();
    registerComponent<LocatorSearchSource>(CreationType::Uncreatable);
    registerComponent<MediaPopupElementViewController>();
    registerComponent<NorthArrowController>();
    registerComponent<OverviewMapController>();
    registerComponent<PieChartPopupMediaItem>();
    registerComponent<PopupAttachmentItem>();
    registerComponent<PopupElementViewItem>();
    registerComponent<PopupMediaItem>();
    registerComponent<PopupViewController>();
    registerComponent<QmlEnums>();
    registerComponent<ScalebarController>();
    registerComponent<SearchResult>();
    registerComponent<SearchSuggestion>();
    registerComponent<SearchViewController>();
    registerComponent<SmartLocatorSearchSource>(CreationType::Uncreatable);
    registerComponent<TextPopupElementViewController>();
    registerComponent<TimeSliderController>();
    registerComponent<UtilityNetworkFunctionTraceResultsModel>();
    registerComponent<UtilityNetworkTraceController>();
    registerComponent<UtilityNetworkTraceStartingPoint>();
    registerComponent<UtilityNetworkTraceStartingPointsModel>();

    // Register ArcGISRuntime types with toolkit.
    qRegisterMetaType<Point>("Esri::ArcGISRuntime::Point");
    qmlRegisterAnonymousType<MapQuickView>(NAMESPACE, VERSION_MAJOR);
  }

} // Esri::ArcGISRuntime::Toolkit
