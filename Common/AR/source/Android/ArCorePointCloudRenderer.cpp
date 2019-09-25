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

#include "ArCorePointCloudRenderer.h"
#include "ArCoreWrapper.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \internal
  The implementation is based on the code provide in the ARCore examples:
  - https://github.com/google-ar/arcore-android-sdk/blob/master/samples/hello_ar_c/app/src/main/cpp/hello_ar_application.cc
  - https://github.com/google-ar/arcore-android-sdk/blob/master/samples/hello_ar_c/app/src/main/cpp/plane_renderer.cc
  */

/*!
  \internal
  */
ArCorePointCloudRenderer::ArCorePointCloudRenderer(ArCoreWrapper* arCoreWrapper) :
  m_arCoreWrapper(arCoreWrapper)
{
  Q_CHECK_PTR(m_arCoreWrapper);
}

/*!
  \internal
  */
ArCorePointCloudRenderer::~ArCorePointCloudRenderer() = default;

/*!
  \internal
  This function run in the GL thread.
  */
void ArCorePointCloudRenderer::initGL()
{
  m_program.reset(new QOpenGLShaderProgram());
  m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                              "uniform mat4 u_modelViewProjection;"
                                              "uniform vec4 u_color;"
                                              "uniform float u_pointSize;"
                                              "attribute vec4 a_position;"
                                              "varying vec4 v_color;"
                                              "void main() {"
                                              "  v_color = u_color;"
                                              "  vec4 position = u_modelViewProjection * vec4(a_position.xyz, 1.0);"
                                              "  gl_Position = vec4(position.x, -position.y, position.z, position.w);"
                                              "  gl_PointSize = u_pointSize;"
                                              "}");
  m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                              "precision mediump float;"
                                              "varying vec4 v_color;"
                                              "void main() {"
                                              "  gl_FragColor = v_color;"
                                              "}");

  m_program->link();
  m_program->bind();

  m_uniformModelViewProjection = m_program->uniformLocation("u_modelViewProjection");
  m_uniformColor = m_program->uniformLocation("u_color");
  m_uniformPointSize = m_program->uniformLocation("u_pointSize");
  m_attributeVertices = m_program->attributeLocation("a_position");

  m_program->release();
}
/*!
  \internal
  This function run in the GL thread.
  */
void ArCorePointCloudRenderer::render()
{
  Q_CHECK_PTR(m_arCoreWrapper);


  QMatrix4x4 modelViewProjection;
  std::vector<float> pointCloud;
  m_arCoreWrapper->pointCloudData(modelViewProjection, pointCloud);
  if (pointCloud.empty())
  {
    m_arCoreWrapper->releasePointCouldData();
    return;
  }

  m_program->bind();

  glUniformMatrix4fv(m_uniformModelViewProjection, 1, GL_FALSE, modelViewProjection.data());
  glEnableVertexAttribArray(m_attributeVertices);
  glVertexAttribPointer(m_attributeVertices, 4, GL_FLOAT, GL_FALSE, 0, pointCloud.data());

  glUniform4f(m_uniformColor, m_pointCloudColor.redF(), m_pointCloudColor.greenF(), m_pointCloudColor.blueF(),
              m_pointCloudColor.alphaF());
  glUniform1f(m_uniformPointSize, static_cast<float>(m_pointCloudSize));

  glDrawArrays(GL_POINTS, 0, pointCloud.size() / 4);

  m_program->release();
  m_arCoreWrapper->releasePointCouldData();
}

/*!
  \internal
  Property for debug mode.
  */
QColor ArCorePointCloudRenderer::pointCloudColor() const
{
  return m_pointCloudColor;
}

/*!
  \internal
  Property for debug mode.
  */
void ArCorePointCloudRenderer::setPointCloudColor(const QColor& pointCloudColor)
{
  m_pointCloudColor = pointCloudColor;
}

/*!
  \internal
  Property for debug mode.
  */
int ArCorePointCloudRenderer::pointCloudSize() const
{
  return m_pointCloudSize;
}

/*!
  \internal
  Property for debug mode.
  */
void ArCorePointCloudRenderer::setPointCloudSize(int pointCloudSize)
{
  m_pointCloudSize = pointCloudSize;
}
