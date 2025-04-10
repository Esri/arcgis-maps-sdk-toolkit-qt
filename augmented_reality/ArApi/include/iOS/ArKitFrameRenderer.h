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

#ifndef ArKitFrameRenderer_H
#define ArKitFrameRenderer_H

// C++ API headers
#include "Deprecated.h"

// Qt headers
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class QOpenGLShaderProgram;

namespace Esri::ArcGISRuntime::Toolkit::Internal {

class QRT_DEPRECATED ArKitFrameRenderer : public QOpenGLFunctions
{
public:
  ArKitFrameRenderer();
  ~ArKitFrameRenderer();

  // update the GL frame size
  void setSize(const QSizeF& size);

  // render the GL frames
  void initGL();
  void render(float verticesRatioX, float verticesRatioY);

  // functions for GL textures
  void updateTextreDataY(int width, int height, const void* data);
  void updateTextreDataCbCr(int width, int height, const void* data);

private:
  std::unique_ptr<QOpenGLShaderProgram> m_program;

  QSizeF m_size;
  GLuint m_uniformTextureY = 0;
  GLuint m_uniformTextureCbCr = 0;
  GLuint m_uniformVerticesRatio = 0;
  GLuint m_attributeVertices = 0;
  GLuint m_attributeUvs = 0;

  QOpenGLTexture m_textureY;
  QOpenGLTexture m_textureCbCr;
};

} // Esri::ArcGISRuntime::Toolkit::Internal

#endif // ArKitFrameRenderer_H
