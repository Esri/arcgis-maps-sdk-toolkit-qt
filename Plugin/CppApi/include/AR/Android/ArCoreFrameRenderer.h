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

//#include "arcore_c_api.h"
//#include <QAndroidJniEnvironment>
//#include <QSize>
//#include <QMatrix4x4>
//#include <QOpenGLFunctions>
//// for ArCoreFrameRenderer
////#include <GLES2/gl2.h>
////#include <GLES2/gl2ext.h>
//#include <QOpenGLShaderProgram>
//#include <QQuickFramebufferObject>
//#include <QPointer>

//#include <vector>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit // internal?
{

class ArCoreWrapper;

// This class renders the passthrough camera image into the OpenGL frame.
class ArCoreFrameRenderer : public QOpenGLFunctions
{
public:
  ArCoreFrameRenderer(ArCoreWrapper* arCoreWrapper);
  ~ArCoreFrameRenderer() = default;

  void init();
  void render();

  GLuint textureId() const;

private:
  ArCoreWrapper* m_arCoreWrapper = nullptr;

  std::unique_ptr<QOpenGLShaderProgram> m_program;

  GLuint m_textureId = 0;
  GLuint m_uniformTexture = 0;
  GLuint m_attributeVertices = 0;
  GLuint m_attributeUvs = 0;

  //  //  GLuint shader_program_ = 0;

  //    GLuint attribute_vertices_ = 0;
  //    GLuint attribute_vertices2_ = 0;
  //    GLuint attribute_uvs_ = 0;
  //    GLint uniform_mvp_mat_ = 0;
  //    GLint uniform_color_ = 0;
  //    GLint uniform_point_size_ = 0;

  //    static constexpr int kNumVertices = 4;
  //    float transformed_uvs_[kNumVertices * 2];

  //    QOpenGLShaderProgram *m_program2 = nullptr;
  //    QOpenGLShaderProgram *m_program3 = nullptr;
  //    QSize m_viewportSize;

  //    int32_t number_of_points = 0;
  //    const float* point_cloud_data = nullptr;
  //    std::vector<float> point_cloud_data2;

  //    ArCoreWrapper* m_arCoreWrapper = nullptr;
  //    QPointer<QQuickWindow> m_window;

  //    QMatrix4x4 m_viewMatrix;
  //    QMatrix4x4 m_projectionMatrix;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ArCoreFrameRenderer_H
