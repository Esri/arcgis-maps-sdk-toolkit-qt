/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_DISCONNECTONSIGNAL_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_DISCONNECTONSIGNAL_H

// Toolkit headers
#include "SingleShotConnection.h"

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  /*
   \internal
   \brief When \a signal fires on \a sender, the given \a connection is disconnected.
   This makes the connection's invocation depdendent on \a signal not firing.
   */
  template <typename Sender, typename Signal>
  QMetaObject::Connection disconnectOnSignal(Sender* sender, Signal&& signal, QObject* self, QMetaObject::Connection connection)
  {
    if (!connection)
      return QMetaObject::Connection{};

    return singleShotConnection(sender, signal, self, [c = std::move(connection)]
                                {
                                  QObject::disconnect(c);
                                });
  }

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_DISCONNECTONSIGNAL_H
