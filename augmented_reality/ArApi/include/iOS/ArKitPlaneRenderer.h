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

#ifndef ArKitPlaneRenderer_H
#define ArKitPlaneRenderer_H

// C++ API headers
#include "Deprecated.h"

// Qt headers
#include <QOpenGLFunctions>

namespace Esri::ArcGISRuntime::Toolkit::Internal {

class ArKitWrapper;

class QRT_DEPRECATED ArKitPlaneRenderer : public QOpenGLFunctions
{
public:
  ArKitPlaneRenderer(ArKitWrapper* ArKitWrapper);
  ~ArKitPlaneRenderer();

  void initGL();
  void render();

private:
  ArKitWrapper* m_arKitWrapper = nullptr;
};

} // Esri::ArcGISRuntime::Toolkit::Internal

#endif // ArKitPlaneRenderer_H
