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

#ifndef ArCorePlaneRenderer_H
#define ArCorePlaneRenderer_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QColor>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

class ArCoreWrapper;

class ArCorePlaneRenderer : public QOpenGLFunctions
{
public:
  ArCorePlaneRenderer(ArCoreWrapper* arCoreWrapper);
  ~ArCorePlaneRenderer() = default;

  void initGL();
  void render();

  // properties for debug mode
  QColor planeColor() const;
  void setPlaneColor(const QColor& planeColor);

private:
  ArCoreWrapper* m_arCoreWrapper = nullptr;

  std::unique_ptr<QOpenGLShaderProgram> m_program;

  GLint m_uniformModelViewProjection = 0;
  GLint m_uniformColor = 0;
  GLuint m_attributeVertices = 0;

  // properties for debug mode
  QColor m_planeColor = QColor(255, 0, 0, 10);
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ArCorePlaneRenderer_H
