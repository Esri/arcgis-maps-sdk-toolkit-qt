/*******************************************************************************
 *  Copyright 2012-2019 Esri
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

#ifndef ArEnums_H
#define ArEnums_H

#include <QObject>

namespace Esri::ArcGISRuntime::Toolkit {

class ArEnums
{
  Q_GADGET

public:
  enum class LocationTrackingMode
  {
    Ignore = 0,
    Initial = 1,
    Continuous = 2
  };
  Q_ENUM(LocationTrackingMode)

  enum class SensorStatus
  {
    Stopped = 0,
    Starting = 1,
    Started = 2
  };
  Q_ENUM(SensorStatus)

  ArEnums();
  ~ArEnums();
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ArEnums_H
