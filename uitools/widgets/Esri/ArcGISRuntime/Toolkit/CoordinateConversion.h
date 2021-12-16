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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTECONVERSION_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTECONVERSION_H

#include <QFrame>
#include <QPointer>

class QGraphicsEllipseItem;
class QMenu;

namespace Ui
{
class CoordinateConversion;
}

namespace Esri
{
namespace ArcGISRuntime
{
class MapGraphicsView;
class SceneGraphicsView;

namespace Toolkit
{

class CoordinateConversionController;
class Flash;

class CoordinateConversion : public QFrame
{
  Q_OBJECT
public:
  explicit CoordinateConversion(QWidget* parent = nullptr);

  ~CoordinateConversion() override;

  void setMapView(MapGraphicsView* mapView);

  void setSceneView(SceneGraphicsView* sceneView);

  CoordinateConversionController* controller() const;

private slots:
  void addContextMenu(const QPoint& point);

  void flash();

private:
  CoordinateConversionController* m_controller = nullptr;
  QMenu* m_resultsMenu = nullptr;
  QPointer<Flash> m_flash;
  Ui::CoordinateConversion* m_ui = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTECONVERSION_H
