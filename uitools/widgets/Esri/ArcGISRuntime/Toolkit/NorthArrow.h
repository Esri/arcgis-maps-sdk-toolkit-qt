/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#ifndef ESRI_ARCGISRUNTIME_NORTHARROW_H
#define ESRI_ARCGISRUNTIME_NORTHARROW_H

#include <QLabel>
#include <QPixmap>

namespace Esri
{
namespace ArcGISRuntime
{

class MapGraphicsView;
class SceneGraphicsView;

namespace Toolkit
{

class NorthArrowController;

class NorthArrow : public QLabel
{
  Q_OBJECT
public:
  explicit NorthArrow(QWidget* parent = nullptr);

  ~NorthArrow() override;

  void setMapView(MapGraphicsView* mapView);

  void setSceneView(SceneGraphicsView* sceneView);

  NorthArrowController* controller() const;

protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
  QPixmap m_image;
  NorthArrowController* m_controller = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_NORTHARROW_H
