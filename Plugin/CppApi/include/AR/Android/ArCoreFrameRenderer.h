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

#ifndef ArCoreFrameRenderer_H
#define ArCoreFrameRenderer_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class ArCoreWrapper;

class ArCoreFrameRenderer : public QOpenGLFunctions
{
public:
  ArCoreFrameRenderer(ArCoreWrapper* arCoreWrapper);
  ~ArCoreFrameRenderer() = default;

  void initGL();
  void render();

private:
  ArCoreWrapper* m_arCoreWrapper = nullptr;

  std::unique_ptr<QOpenGLShaderProgram> m_program;

  GLuint m_textureId = 0;
  GLuint m_uniformTexture = 0;
  GLuint m_attributeVertices = 0;
  GLuint m_attributeUvs = 0;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ArCoreFrameRenderer_H
