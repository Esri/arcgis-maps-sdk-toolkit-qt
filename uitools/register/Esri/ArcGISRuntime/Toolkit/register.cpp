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
#include "register.h"

/*!
  \headerfile Esri/ArcGISRuntime/Toolkit/register

  This file contains the registration function required to register the toolkit
  with the `QQmlEngine`.

  Please refer to
  \c README.md for more information on workflows.
*/

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::registerComponents(QmlEngine& engine)
  \relates Esri/ArcGISRuntime/Toolkit/register
  \brief This registration function must be called after the QmlEngine has been
  declared, but before it is run. This sets up resources and component registration
  with the `QQmlEngine` and the toolkit.
 */


#if defined(CPP_ARCGISRUNTIME_TOOLKIT)
#  include "internal/register_cpp.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

void registerComponents(QQmlEngine& engine)
{
  registerComponents_cpp_(engine);
}

} // Toolkit
} // ArcGISRuntime
} // Esri

#elif defined(QML_ARCGISRUNTIME_TOOLKIT)
#  include "internal/register_qml.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

void registerComponents(QQmlEngine& engine)
{
  registerComponents_qml_(engine);
}

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif
