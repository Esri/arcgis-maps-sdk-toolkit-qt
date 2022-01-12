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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_DOONLOAD_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_DOONLOAD_H

// Qt headers
#include <QObject>

// C++ headers
#include <type_traits>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  /*
    \internal
    \brief Exectues method \a f immediately if \a target is loaded, otherwise
    attempts to load \a target and then executes \a f after the load occurs.
    Returns the connection object where applicable, which may be default-constructed.
   */
  template <typename Target, typename Func>
  QMetaObject::Connection doOnLoad(Target* target, QObject* self, Func&& f)
  {
    static_assert(
        std::is_convertible<Target*, ::Esri::ArcGISRuntime::Loadable*>::value,
        "Target type must use Loadable interface");
    static_assert(
        std::is_convertible<Target*, QObject*>::value,
        "Target type must be a QObject");

    if (target->loadStatus() == LoadStatus::Loaded)
    {
      f();
      return QMetaObject::Connection{};
    }
    else
    {
      auto connection = QObject::connect(
          target, &Target::loadStatusChanged, self,
          [f = std::forward<Func>(f)](LoadStatus loadStatus)
          {
            if (loadStatus == LoadStatus::Loaded)
              f();
          });

      if (target->loadStatus() == LoadStatus::NotLoaded)
        target->load();
      else if (target->loadStatus() != LoadStatus::Loading)
        target->retryLoad();

      return connection;
    }
  }


} // Toolkit
} // ArcGISRuntime
} // Esri


#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_DOONLOAD_H