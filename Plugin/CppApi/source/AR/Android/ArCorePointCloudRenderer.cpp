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

#include "ArCorePointCloudRenderer.h"
#include "ArCoreWrapper.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

ArCorePointCloudRenderer::ArCorePointCloudRenderer(ArCoreWrapper* ArCoreWrapper) :
  m_arCoreWrapper(ArCoreWrapper)
{
}

void ArCorePointCloudRenderer::init()
{
  m_program.reset(new QOpenGLShaderProgram());
  m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                              "uniform mat4 u_modelViewProjection;"
                                              "uniform vec4 u_color;"
                                              "uniform float u_pointSize;"
                                              "attribute vec4 a_position;"
                                              "varying vec4 v_Color;"
                                              "void main() {"
                                              "  v_Color = u_color;"
                                              "  vec4 position = u_modelViewProjection * vec4(a_position.xyz, 1.0);"
                                              "  gl_Position = vec4(position.x, 1.0 - position.y, position.z, position.w);"
                                              "  gl_PointSize = u_pointSize;"
                                              "}");
  m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                              "precision mediump float;"
                                              "varying vec4 v_Color;"
                                              "void main() {"
                                              "  gl_FragColor = v_Color;"
                                              "}");

  m_program->link();
  m_program->bind();

  m_attributeVertices = m_program->attributeLocation("a_position");
  m_uniformModelViewProjection = m_program->uniformLocation("u_modelViewProjection");
  m_uniformColor = m_program->uniformLocation("u_color");
  m_uniformPointSize = m_program->uniformLocation("u_pointSize");

  m_program->release();
}

void ArCorePointCloudRenderer::render()
{
  Q_CHECK_PTR(m_arCoreWrapper);

  m_program->bind();

  glUniformMatrix4fv(m_uniformModelViewProjection, 1, GL_FALSE, m_arCoreWrapper->modelViewProjectionData());

  glEnableVertexAttribArray(m_attributeVertices);

  glVertexAttribPointer(m_attributeVertices, 4, GL_FLOAT, GL_FALSE, 0, m_arCoreWrapper->pointCloudData());

  // Set cyan color to the point cloud.
  glUniform4f(m_uniformColor, 31.0f / 255.0f, 188.0f / 255.0f, 210.0f / 255.0f, 1.0f);
  glUniform1f(m_uniformPointSize, 50.0f);

  glDrawArrays(GL_POINTS, 0, m_arCoreWrapper->pointCloudSize());

  m_program->release();
}
