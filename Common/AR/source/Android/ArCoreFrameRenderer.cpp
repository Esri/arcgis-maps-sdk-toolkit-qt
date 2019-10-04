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

#include "ArCoreFrameRenderer.h"
#include "ArCoreWrapper.h"

/*!
  \internal

  Internal class to render the camera frame in the QML item "ArcGISArView".

  The AR core gets the data of the AR frames directly from the camera and copies them
  to an OpenGL texture. The texture is rendered in a quad (2 triangles with 4 vertices)
  which represent all the QML item surface.

  The image size received from the camera is constant for a specific device but the surface
  size of the QML item can be changed, for example when the device is switched between the
  portrait and landscape modes, or if the menu bar is displayed or not.

  The texture coordinate must be adapted to respect the camera image ratio. These
  coordinates are calculated by the AR core function "transformedUvs".
 */

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit::Internal;

namespace {
// positions of the quad vertices in GL space
const GLfloat kVerticesPortrait[] = {
  -1.0f, -1.0f,
  +1.0f, -1.0f,
  -1.0f, +1.0f,
  +1.0f, +1.0f
};
} // anonymous namespace

ArCoreFrameRenderer::ArCoreFrameRenderer(ArCoreWrapper* arCoreWrapper) :
  m_arCoreWrapper(arCoreWrapper)
{
  Q_CHECK_PTR(m_arCoreWrapper);
}

ArCoreFrameRenderer::~ArCoreFrameRenderer() = default;

/*!
  \internal
  this function run in the GL thread.
 */
void ArCoreFrameRenderer::initGL()
{
  // This function must to run with a valid OpenGL context.
  Q_CHECK_PTR(QOpenGLContext::currentContext());

  initializeOpenGLFunctions();

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
                                              "#extension GL_OES_EGL_image_external : require\n"
                                              "precision mediump float;"
                                              "varying vec2 v_texCoord;"
                                              "uniform samplerExternalOES u_texture;"
                                              "void main() {"
                                              "  gl_FragColor = texture2D(u_texture, vec2(1.0 - v_texCoord.x, v_texCoord.y));"
                                              "}");

  m_program->link();
  m_program->bind();

  glGenTextures(1, &m_textureId);
  glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  m_uniformTexture = m_program->uniformLocation("u_texture");
  m_attributeVertices = m_program->attributeLocation("a_position");
  m_attributeUvs = m_program->attributeLocation("a_texCoord");

  m_program->release();

  // set the id to AR core then the camera frames will be copied in the texture.
  Q_CHECK_PTR(m_arCoreWrapper);
  m_arCoreWrapper->setTextureId(m_textureId);
}

/*!
  \internal
  this function run in the GL thread.
 */
void ArCoreFrameRenderer::render()
{
  // This function must to run with a valid OpenGL context.
  Q_CHECK_PTR(QOpenGLContext::currentContext());

  // Init the program if necessary.
  if (!m_program)
    initGL();

  // Render the frame as texture.
  Q_CHECK_PTR(m_program);
  m_program->bind();

  glClear(GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);

  glUniform1i(m_uniformTexture, 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);

  glEnableVertexAttribArray(m_attributeVertices);
  glVertexAttribPointer(m_attributeVertices, 2, GL_FLOAT, GL_FALSE, 0, kVerticesPortrait);

  glEnableVertexAttribArray(m_attributeUvs);
  Q_CHECK_PTR(m_arCoreWrapper);
  glVertexAttribPointer(m_attributeUvs, 2, GL_FLOAT, GL_FALSE, 0, m_arCoreWrapper->transformedUvs().data());

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDepthMask(GL_TRUE);
  m_program->release();
}
