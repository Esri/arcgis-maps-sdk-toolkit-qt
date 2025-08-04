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

#ifndef ArKitPointCloudRenderer_H
#define ArKitPointCloudRenderer_H

// C++ API headers
#include "Deprecated.h"

// Qt headers
#include <QColor>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class QOpenGLShaderProgram;

namespace Esri::ArcGISRuntime::Toolkit::Internal {

class ArKitWrapper;

// This class renders the detected point cloud into the view.
class QRT_DEPRECATED ArKitPointCloudRenderer : public QOpenGLFunctions
{
public:
  ArKitPointCloudRenderer(ArKitWrapper* ArKitWrapper);
  ~ArKitPointCloudRenderer();

  void initGL();
  void render();

  // properties for debug mode
  QColor pointCloudColor() const;
  void setPointCloudColor(const QColor& pointCloudColor);

  int pointCloudSize() const;
  void setPointCloudSize(int pointCloudSize);

private:
  ArKitWrapper* m_arKitWrapper = nullptr;

  std::unique_ptr<QOpenGLShaderProgram> m_program;

  // properties for debug mode
  QColor m_pointCloudColor = QColor(Qt::darkBlue);
  int m_pointCloudSize = 10;
};

} // Esri::ArcGISRuntime::Toolkit::Internal

#endif // ArKitPointCloudRenderer_H
