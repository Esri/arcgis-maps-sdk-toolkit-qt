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

// ArcGISRuntime headers
#include <Loadable.h>

// Qt headers
#include <QObject>

// C++ headers
#include <type_traits>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  /*
    \internal
    \brief Exectues method \a f immediately if \a sender is loaded, otherwise
    attempts to load \a sender and then executes \a f after the load occurs.
    Returns a connection object which may be default-constructed.

    Example:
    \begincode
    doOnLoad(floorManager, this, []
             {
                qDebug() << "FloorManager is guaranteed loaded."
             });
    \endcode
   */
  template <typename Target, typename Func>
  QMetaObject::Connection doOnLoad(Target* sender, QObject* reveiver, Func&& f)
  {
    static_assert(
        std::is_convertible<Target*, ::Esri::ArcGISRuntime::Loadable*>::value,
        "Target type must use Loadable interface");
    static_assert(
        std::is_convertible<Target*, QObject*>::value,
        "Target type must be a QObject");

    if (sender->loadStatus() == LoadStatus::Loaded)
    {
      f();
      return QMetaObject::Connection{};
    }
    else
    {
      auto connection = QObject::connect(
          sender, &Target::loadStatusChanged, reveiver,
          [f = std::forward<Func>(f)](LoadStatus loadStatus)
          {
            if (loadStatus == LoadStatus::Loaded)
              f();
          });

      if (sender->loadStatus() == LoadStatus::NotLoaded)
        sender->load();
      else if (sender->loadStatus() != LoadStatus::Loading)
        sender->retryLoad();

      return connection;
    }
  }


} // Toolkit
} // ArcGISRuntime
} // Esri


#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_DOONLOAD_H
