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

#ifndef ArcGISArViewRenderer_H
#define ArcGISArViewRenderer_H

// C++ API headers
#include "Deprecated.h"

// Qt headers
#include <QQuickFramebufferObject>

class QQuickWindow;

namespace Esri::ArcGISRuntime::Toolkit::Internal {

class ArWrapper;

class QRT_DEPRECATED ArcGISArViewRenderer : public QQuickFramebufferObject::Renderer
{
public:
  ArcGISArViewRenderer(Internal::ArWrapper* arWrapper = nullptr);
  ~ArcGISArViewRenderer() override;

  // override methods from QQuickFramebufferObject::Renderer
  QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
  void synchronize(QQuickFramebufferObject* item) override;
  void render() override;

private:
  bool m_isInitialized = false;
  QQuickWindow* m_window = nullptr;
  Internal::ArWrapper* m_arWrapper = nullptr;
};

} // Esri::ArcGISRuntime::Toolkit::Internal

#endif // ArcGISArViewRenderer_H
