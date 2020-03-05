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

/*!
 * \brief The user interface for the coordinate conversion tool.
 * 
 * This tool allows a user to select a point on the map or to enter a point by
 * text entry.
 * The point may be highlighted by zooming into that point, or by highlighting
 * the point on the GeoView.
 * A list of different coordinate formats represnenting the same point are
 * available to the user.
 */
class CoordinateConversion : public QFrame 
{
  Q_OBJECT
public:
  /*!
   * \brief Constructor.
   * \param parent Parent widget.
   */
  explicit CoordinateConversion(QWidget* parent = nullptr);

  /*!
   * \brief Destructor.
   */
  ~CoordinateConversion() override;

  /*!
   * \brief Set the GeoView.
   * \param mapView Sets the GeoView to a MapView.
   */
  void setMapView(MapGraphicsView* mapView);

  /*!
   * \brief Set the GeoView.
   * \param mapView Sets the GeoView to a SceneView.
   */
  void setSceneView(SceneGraphicsView* sceneView);

  /*!
   * \brief Returns the controller object driving this widget.
   */
  CoordinateConversionController* controller() const;

private slots:
  /*!
   * \internal
   * \brief Bring up the context menu at the given point.
   * \param point Point to show menu
   */
  void addContextMenu(const QPoint& point);

  /*!
   * \internal
   * \brief Flash the dot on the screen at current location.
   */
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
