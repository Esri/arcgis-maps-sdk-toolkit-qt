/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

#ifndef ArcGISRuntimeToolkit_H
#define ArcGISRuntimeToolkit_H

// toolkit headers
#include "ToolkitCommon.h"

// Qt headers
#include <QQmlExtensionPlugin>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class TOOLKIT_EXPORT ArcGISRuntimeToolkit : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  explicit ArcGISRuntimeToolkit(QObject* parent = nullptr);

  void registerTypes(const char* uri);

  static void registerToolkitTypes(const char* uri = "Esri.ArcGISRuntime.Toolkit.CppApi");

private:
  static constexpr int s_versionMajor100 = 100;
  static constexpr int s_versionMinorUpdate2 = 2;
  static constexpr int s_versionMinorUpdate3 = 3;
  static constexpr int s_versionMinorUpdate6 = 6;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ArcGISRuntimeToolkit_H
