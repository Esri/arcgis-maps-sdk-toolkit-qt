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

#ifndef LocationTrackingMode_H
#define LocationTrackingMode_H

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

/// Controls how the locations generated from the location data source are used during AR tracking.
///
/// - ignore: Ignore all location data source locations.
/// - initial: Use only the initial location from the location data source and ignore all subsequent locations.
/// - continuous: Use all locations from the location data source.

enum class LocationTrackingMode
{
  Ignore = 0,
  Initial = 1,
  Continuous = 2
};

} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // LocationTrackingMode_H
