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

#include "ArcGISArViewRenderer.h"
#include "ArWrapper.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLContext>
#include <QQuickWindow>

#include <QQuickOpenGLUtils>

using namespace Esri::ArcGISRuntime::Toolkit::Internal;

/*!
  \class ArcGISArViewRenderer
  \internal
  \brief Renders the passthrough camera image into the OpenGL frame.
 */

ArcGISArViewRenderer::ArcGISArViewRenderer(Internal::ArWrapper* arWrapper) :
  m_arWrapper(arWrapper)
{
  Q_CHECK_PTR(arWrapper);
}

ArcGISArViewRenderer::~ArcGISArViewRenderer() = default;

/*!
  \internal
 */
QOpenGLFramebufferObject* ArcGISArViewRenderer::createFramebufferObject(const QSize& size)
{
  QOpenGLFramebufferObjectFormat format;
  format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
  return new QOpenGLFramebufferObject(size, format);
}

/*!
  \internal
 */
void ArcGISArViewRenderer::synchronize(QQuickFramebufferObject* item)
{
  if (!m_window && item)
    m_window = item->window();
}

/*!
  \internal
 */
void ArcGISArViewRenderer::render()
{
  // This function must to run with a valid OpenGL context.
  Q_CHECK_PTR(QOpenGLContext::currentContext());

  Q_CHECK_PTR(m_arWrapper);

  if (!m_isInitialized)
  {
    m_isInitialized = true;
    m_arWrapper->initGL();
  }

  m_arWrapper->render();

  if (m_window)
  {
    QQuickOpenGLUtils::resetOpenGLState();
  }
}
