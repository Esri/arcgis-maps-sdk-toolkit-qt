/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

#ifndef ARCGISCOMPASSCONTROLLER_H
#define ARCGISCOMPASSCONTROLLER_H

#include "AbstractTool.h"

namespace Esri
{
namespace ArcGISRuntime
{

class MapQuickView;
class SceneQuickView;
class GeoView;

namespace Toolkit
{
class TOOLKIT_EXPORT ArcGISCompassController : public AbstractTool
{
  Q_OBJECT

  // whether compass should be visible if rotation/heading is 0
  Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide NOTIFY autoHideChanged)

  // rotation or heading of the MapView/SceneView (respectively). Used to determine Compass rotation
  Q_PROPERTY(double heading READ heading WRITE setHeading NOTIFY headingChanged)

signals:
  void headingChanged();
  void autoHideChanged();

public:
  ArcGISCompassController(QObject *parent = nullptr);
  ~ArcGISCompassController() override;

  bool setView(Esri::ArcGISRuntime::GeoView* geoView);
  /*! \internal */
  bool setView(Esri::ArcGISRuntime::MapQuickView* mapView);
  /*! \internal */
  bool setView(Esri::ArcGISRuntime::SceneQuickView* sceneView);

  double heading() const;
  void setHeading(double value);

  bool autoHide() const;
  void setAutoHide(bool value);

  QString toolName() const override;

private:
  double m_heading = 0.0;
  bool m_autoHide = true;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::SceneQuickView* m_sceneView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISCOMPASSCONTROLLER_H
