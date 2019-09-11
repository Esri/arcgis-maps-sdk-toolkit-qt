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

#include "ArKitPlaneRenderer.h"
#include "ArKitWrapper.h"
#include <QOpenGLShaderProgram>

using namespace Esri::ArcGISRuntime::Toolkit::Internal;

// This class renders the passthrough camera image into the OpenGL frame.
// doc: https://developer.apple.com/documentation/arkit/tracking_and_visualizing_planes?language=objc
// https://developer.apple.com/documentation/arkit/arplanegeometry?language=objc
// active detect plane? DetectedPlaneFindingMode, DetectedPlaneType

ArKitPlaneRenderer::ArKitPlaneRenderer(ArKitWrapper* ArKitWrapper) :
  m_arKitWrapper(ArKitWrapper)
{
}

ArKitPlaneRenderer::~ArKitPlaneRenderer()
{
}

void ArKitPlaneRenderer::initGL()
{
  initializeOpenGLFunctions();
  // not implemented
}

void ArKitPlaneRenderer::render()
{
  Q_CHECK_PTR(m_arKitWrapper);
  // not implemented
}
