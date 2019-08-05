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

#ifndef ArKitPlaneRenderer_H
#define ArKitPlaneRenderer_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit // TODO: internal?
{

// doc: https://developer.apple.com/documentation/arkit/tracking_and_visualizing_planes?language=objc
// https://developer.apple.com/documentation/arkit/arplanegeometry?language=objc
// active detect plane? DetectedPlaneFindingMode, DetectedPlaneType

class ArKitWrapper;

// This class renders the passthrough camera image into the OpenGL frame.
class ArKitPlaneRenderer : public QOpenGLFunctions
{
public:
  ArKitPlaneRenderer(ArKitWrapper* ArKitWrapper);
  ~ArKitPlaneRenderer() = default;

  void init();
  void render();

private:
  ArKitWrapper* m_arKitWrapper = nullptr;

  std::unique_ptr<QOpenGLShaderProgram> m_program;

  GLuint m_attributeVertices = 0;
  GLint m_uniformModelViewProjection = 0;
  GLint m_uniformColor = 0;
  GLint m_uniformPointSize = 0;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ArKitPlaneRenderer_H
