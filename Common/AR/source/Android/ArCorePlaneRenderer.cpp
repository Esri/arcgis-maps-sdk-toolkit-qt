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

#include "ArCorePlaneRenderer.h"
#include "ArCoreWrapper.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

ArCorePlaneRenderer::ArCorePlaneRenderer(ArCoreWrapper* ArCoreWrapper) :
  m_arCoreWrapper(ArCoreWrapper)
{
}

void ArCorePlaneRenderer::initGL()
{
  m_program.reset(new QOpenGLShaderProgram());

  m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                              "uniform mat4 u_modelViewProjection;"
                                              "uniform vec4 u_color;"
                                              "attribute vec2 a_position;"
                                              "varying vec4 v_color;"
                                              "void main() {"
                                              "  v_color = u_color;"
                                              "  vec4 localPosition = vec4(a_position.x, 0.0, a_position.y, 1.0);"
                                              "  vec4 position = u_modelViewProjection * localPosition;"
                                              "  gl_Position = vec4(position.x, -position.y, position.z, position.w);"
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
  m_attributeVertices = m_program->attributeLocation("a_position");

  m_program->release();
}

void ArCorePlaneRenderer::render()
{
  Q_CHECK_PTR(m_arCoreWrapper);

  m_program->bind();

  int32_t size = 0;
  m_arCoreWrapper->planeListData(size);
  if (size == 0)
  {
    m_program->release();
    m_arCoreWrapper->releasePlaneListData();
    return;
  }

  for (int32_t index = 0; index < size; ++index)
  {
    std::vector<float> vertices;
    QMatrix4x4 modelViewProjection;
    if (!m_arCoreWrapper->planeData(modelViewProjection, index, vertices))
      continue;

    glUniformMatrix4fv(m_uniformModelViewProjection, 1, GL_FALSE, modelViewProjection.data());
    glEnableVertexAttribArray(m_attributeVertices);
    glVertexAttribPointer(m_attributeVertices, 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glUniform4f(m_uniformColor, m_planeColor.redF(), m_planeColor.greenF(), m_planeColor.blueF(), m_planeColor.alphaF());
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);

    // release data
    m_arCoreWrapper->releasePlaneData();
  }

  m_program->release();
  m_arCoreWrapper->releasePlaneListData();
}

// properties for debug mode
QColor ArCorePlaneRenderer::planeColor() const
{
  return m_planeColor;
}

void ArCorePlaneRenderer::setPlaneColor(const QColor& planeColor)
{
  m_planeColor = planeColor;
}
