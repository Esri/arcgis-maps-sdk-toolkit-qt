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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_SINGLESHOTCONNECTION_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_SINGLESHOTCONNECTION_H

// Qt heeaders
#include <QObject>

// C++ headers
#include <memory>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  /*
    \internal
    \brief Creates a connection and automatically disconnects it on trigger. Follows same syntax as `QObject::connect`.
    \note This method differs from \c QObject::conenct in that the signal and slot parameters must match.
   */
  template <typename Sender, typename Signal, typename Receiver, typename Slot>
  QMetaObject::Connection singleShotConnection(Sender* sender, Signal&& signal, Receiver* reciever, Slot&& slot)
  {
    auto connection = std::make_shared<QMetaObject::Connection>();
    *connection = QObject::connect(sender, std::forward<Signal>(signal),
                                   reciever, [connection, slot = std::forward<Slot>(slot)](auto&&... args)
                                   {
                                     QObject::disconnect(*connection);
                                     slot(std::forward<decltype(args)>(args)...);
                                   });
    return *connection;
  }

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_SINGLESHOTCONNECTION_H
