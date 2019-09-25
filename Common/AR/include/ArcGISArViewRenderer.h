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

#include <QQuickFramebufferObject>

class QQuickWindow;

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

namespace Internal {
class ArWrapper;
}

class ArcGISArViewRenderer : public QQuickFramebufferObject::Renderer
{
public:
  ArcGISArViewRenderer();
  ~ArcGISArViewRenderer() override;

  // override methods from QQuickFramebufferObject::Renderer
  QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
  void synchronize(QQuickFramebufferObject* item) override;
  void render() override;

  // AR wrapper
  void setArWrapper(Internal::ArWrapper* arWrapper);

private:
  bool m_isInitialized = false;
  QQuickWindow* m_window = nullptr;
  Internal::ArWrapper* m_arWrapper = nullptr;
};

} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // ArcGISArViewRenderer_H
