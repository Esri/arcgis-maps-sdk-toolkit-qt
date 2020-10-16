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

#include "ArKitPointCloudRenderer.h"
#include "ArKitWrapper.h"
#include <QOpenGLShaderProgram>

using namespace Esri::ArcGISRuntime::Toolkit::Internal;

ArKitPointCloudRenderer::ArKitPointCloudRenderer(ArKitWrapper* ArKitWrapper) :
  m_arKitWrapper(ArKitWrapper)
{
}

ArKitPointCloudRenderer::~ArKitPointCloudRenderer()
{
}

void ArKitPointCloudRenderer::initGL()
{
  initializeOpenGLFunctions();
  // not implemented
}

void ArKitPointCloudRenderer::render()
{
  Q_CHECK_PTR(m_arKitWrapper);
  // not implemented
}

// properties for debug mode
QColor ArKitPointCloudRenderer::pointCloudColor() const
{
  return m_pointCloudColor;
}

void ArKitPointCloudRenderer::setPointCloudColor(const QColor& pointCloudColor)
{
  m_pointCloudColor = pointCloudColor;
}

int ArKitPointCloudRenderer::pointCloudSize() const
{
  return m_pointCloudSize;
}

void ArKitPointCloudRenderer::setPointCloudSize(int pointCloudSize)
{
  m_pointCloudSize = pointCloudSize;
}
