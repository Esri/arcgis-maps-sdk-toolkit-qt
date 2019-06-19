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

#include "ArKitFrameRenderer.h"
#include "ArKitWrapper.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

namespace {
// Positions of the quad vertices in clip space (X, Y).
const GLfloat kVertices[] = {
  -1.0f, -1.0f,
  +1.0f, -1.0f,
  -1.0f, +1.0f,
  +1.0f, +1.0f,
};
}  // namespace

ArKitFrameRenderer::ArKitFrameRenderer(ArKitWrapper* arKitWrapper) :
  m_arKitWrapper(arKitWrapper)
{
}

void ArKitFrameRenderer::init()
{
  m_program.reset(new QOpenGLShaderProgram());
  m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                              "attribute vec4 a_position;"
                                              "attribute vec2 a_texCoord;"
                                              "varying vec2 v_texCoord;"
                                              "void main() {"
                                              "  gl_Position = a_position;"
                                              "  v_texCoord = a_texCoord;"
                                              "}");
  m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                              "precision mediump float;"
                                              "varying vec2 v_texCoord;"
                                              "uniform sampler2D u_textureY;"
                                              "uniform sampler2D u_textureCbCr;"
                                              "const mat4 ycbcrToRgbTransform = mat4("
                                              "    vec4(+1.0000, +1.0000, +1.0000, +0.0000),"
                                              "    vec4(+0.0000, -0.3441, +1.7720, +0.0000),"
                                              "    vec4(+1.4020, -0.7141, +0.0000, +0.0000),"
                                              "    vec4(-0.7010, +0.5291, -0.8860, +1.0000)"
                                              ");"
                                              "void main() {"
                                              "  float yValue = texture2D(u_textureY, v_texCoord).r;"
                                              "  vec2 cbcrValue = texture2D(u_textureCbCr, v_texCoord).rg;"
                                              "  vec4 ycbcrValue = vec4(yValue, cbcrValue, 1.0);"
                                              "  gl_FragColor = ycbcrToRgbTransform * ycbcrValue;"
                                              "}");

  m_program->link();
  m_program->bind();

  m_uniformTextureY = m_program->uniformLocation("u_textureY");
  m_uniformTextureCbCr = m_program->uniformLocation("u_textureCbCr");
  m_attributeVertices = m_program->attributeLocation("a_position");
  m_attributeUvs = m_program->attributeLocation("a_texCoord");

  m_program->release();
}

void ArKitFrameRenderer::render(GLuint textureIdY, GLuint textureIdCbCr)
{
  Q_CHECK_PTR(m_arKitWrapper);

  m_program->bind();

  initializeOpenGLFunctions();
  glDepthMask(GL_FALSE);

  qDebug() << "----->> textures:" << textureIdY << textureIdCbCr;

  glUniform1i(m_uniformTextureY, 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureIdY);

  glUniform1i(m_uniformTextureCbCr, 2);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, textureIdCbCr);




//  glBindTexture(CVOpenGLESTextureGetTarget(_chromaTexture), CVOpenGLESTextureGetName(_chromaTexture));
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);




  glEnableVertexAttribArray(m_attributeVertices);
  glVertexAttribPointer(m_attributeVertices, 2, GL_FLOAT, GL_FALSE, 0, kVertices);

  glEnableVertexAttribArray(m_attributeUvs);
  glVertexAttribPointer(m_attributeUvs, 2, GL_FLOAT, GL_FALSE, 0, m_arKitWrapper->transformedUvs());

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDepthMask(GL_TRUE);
  m_program->release();

  for (GLint error = glGetError(); error; error = glGetError()) {
    qDebug() << "==== renderArFrame opengl error:" << error;
  }
}
