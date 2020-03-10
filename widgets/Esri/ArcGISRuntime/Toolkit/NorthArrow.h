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

/*!
 * \brief The NorthArrow displays a compass overlaid on the GeoView, with the
 * compass heading matching the current rotation of the MapView, or Camera
 * heading of the SceneView. 
 * Double clicking on the NorthArrow triggers the heading of the connected
 * GeoView to be orientated to 0 (North).
 */
class NorthArrow : public QLabel
{
  Q_OBJECT
public:
  /*!
   * \brief Constructor
   * \param parent Parent widget.
   */
  explicit NorthArrow(QWidget* parent = nullptr);

  /*!
   * \brief Destructor
   */
  ~NorthArrow() override;

  /*!
   * \brief Set the GeoView.
   * \param mapView Sets the GeoView to a MapView.
   */
  void setMapView(MapGraphicsView* mapView);

  /*!
   * \brief Set the GeoView.
   * \param mapView Sets the GeoView to a SceneView.
   */
  void setSceneView(SceneGraphicsView* mapView);

  /*!
   * \brief Returns the controller object driving this widget.
   */
  NorthArrowController* controller() const;

protected:
  /*!
   * \internal
   * \brief When triggered this will orient the GeoView such that this
   * NorthArrow has a heading of 0.
   * \param event MouseEvent to accept.
   */
  void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
  QPixmap m_image;
  NorthArrowController* m_controller = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_NORTHARROW_H
