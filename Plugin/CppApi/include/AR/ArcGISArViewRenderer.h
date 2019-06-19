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

#ifndef ARCGISARVIEWRENDERER_H
#define ARCGISARVIEWRENDERER_H

#include <QQuickWindow>
#include <QPointer>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include "ArWrapper.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit // internal?
{

// This class renders the passthrough camera image into the OpenGL frame.
class ArcGISArViewRenderer : public QQuickFramebufferObject::Renderer, public QOpenGLFunctions
{
public:
  ArcGISArViewRenderer() = default;
  ~ArcGISArViewRenderer() override = default;

  // override methods from QQuickFramebufferObject::Renderer
  QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
  void synchronize(QQuickFramebufferObject* item) override;
  void render() override;

  // renderers
  void setArWrapper(ArWrapper* arWrapper);

private:
  bool m_isInitialized = false;

  QPointer<QQuickWindow> m_window;

  ArWrapper* m_arWrapper = nullptr;

//  //  GLuint shader_program_ = 0;
//  GLuint texture_id_ = 0;

//  GLuint attribute_vertices_ = 0;
//  GLuint attribute_vertices2_ = 0;
//  GLuint attribute_uvs_ = 0;
//  GLuint uniform_texture_ = 0;
//  GLint uniform_mvp_mat_ = 0;
//  GLint uniform_color_ = 0;
//  GLint uniform_point_size_ = 0;

//  static constexpr int kNumVertices = 4;
//  float transformed_uvs_[kNumVertices * 2];

//  QOpenGLShaderProgram *m_program = nullptr;
//  QOpenGLShaderProgram *m_program2 = nullptr;
//  QOpenGLShaderProgram *m_program3 = nullptr;
//  QSize m_viewportSize;

//  int32_t number_of_points = 0;
//  const float* point_cloud_data = nullptr;
//  std::vector<float> point_cloud_data2;


//  QMatrix4x4 m_viewMatrix;
//  QMatrix4x4 m_projectionMatrix;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISARVIEWRENDERER_H
