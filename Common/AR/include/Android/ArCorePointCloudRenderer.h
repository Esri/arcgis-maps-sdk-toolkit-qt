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

#ifndef ArCorePointCloudRenderer_H
#define ArCorePointCloudRenderer_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QColor>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
namespace Internal {

class ArCoreWrapper;

class ArCorePointCloudRenderer : public QOpenGLFunctions
{
public:
  ArCorePointCloudRenderer(ArCoreWrapper* arCoreWrapper);
  ~ArCorePointCloudRenderer();

  void initGL();
  void render();

  // properties for debug mode
  QColor pointCloudColor() const;
  void setPointCloudColor(const QColor& pointCloudColor);

  int pointCloudSize() const;
  void setPointCloudSize(int pointCloudSize);

private:
  ArCoreWrapper* m_arCoreWrapper = nullptr;

  std::unique_ptr<QOpenGLShaderProgram> m_program;

  GLuint m_attributeVertices = 0;
  GLint m_uniformModelViewProjection = 0;
  GLint m_uniformColor = 0;
  GLint m_uniformPointSize = 0;

  // properties for debug mode
  QColor m_pointCloudColor = QColor(50, 50, 255);
  int m_pointCloudSize = 10;
};

} // Internal namespace
} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // ArCorePointCloudRenderer_H
