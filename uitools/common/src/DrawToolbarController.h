/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_DRAWTOOLBARCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_DRAWTOOLBARCONTROLLER_H

// Qt headers
#include <QObject>

// Toolkit headers
#include "Internal/GeoViews.h"

namespace Esri::ArcGISRuntime {
class FreehandTool;
class GeometryEditor;
class Graphic;
class GraphicsOverlay;
class SimpleFillSymbol;
class SimpleLineSymbol;
class SimpleMarkerSymbol;
class VertexTool;
}

namespace Esri::ArcGISRuntime::Toolkit {

class DrawToolbarController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(DrawToolbarController::DrawMode drawMode READ drawMode WRITE setDrawMode NOTIFY drawModeChanged)
  Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
  Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
  Q_PROPERTY(bool isEditing READ isEditing NOTIFY isEditingChanged)
  Q_PROPERTY(DrawToolbarController::SelectedElementType selectedElementType READ selectedElementType NOTIFY selectedElementTypeChanged)

public:
  Q_INVOKABLE explicit DrawToolbarController(QObject* parent = nullptr);

  ~DrawToolbarController();

  enum class DrawMode {
    NoneDrawMode,
    SelectDrawMode,
    PointDrawMode,
    PolylineDrawMode,
    PolygonDrawMode,
    FreehandLineDrawMode,
    FreehandPolygonDrawMode
  };

  Q_ENUM(DrawMode)

  enum class SelectedElementType {
    None,
    Geometry,
    Part,
    Vertex,
    MidVertex
  };

  Q_ENUM(SelectedElementType)

  QObject* mapView() const;
  void setMapView(QObject* geoView);

  Q_INVOKABLE void clearAll();
  Q_INVOKABLE void undo();
  Q_INVOKABLE void redo();
  Q_INVOKABLE void save();
  Q_INVOKABLE void deleteSelected();
  Q_INVOKABLE void addPart(double mouseX, double mouseY);

  Esri::ArcGISRuntime::GeometryEditor* geometryEditor() const { return m_geometryEditor; }

signals:
  void mapViewChanged();
  void drawModeChanged();
  void canUndoChanged();
  void canRedoChanged();
  void isEditingChanged();
  void selectedElementTypeChanged();

private:
  MapViewToolkit* m_mapView{nullptr};
  Esri::ArcGISRuntime::GraphicsOverlay* m_graphicsOverlay{nullptr};
  Esri::ArcGISRuntime::GeometryEditor* m_geometryEditor{nullptr};
  Esri::ArcGISRuntime::VertexTool* m_vertexTool{nullptr};
  Esri::ArcGISRuntime::FreehandTool* m_freehandTool{nullptr};
  Esri::ArcGISRuntime::SimpleFillSymbol* m_simpleFillSymbol{nullptr};
  Esri::ArcGISRuntime::SimpleLineSymbol* m_simpleLineSymbol{nullptr};
  Esri::ArcGISRuntime::SimpleMarkerSymbol* m_simpleMarkerSymbol{nullptr};
  Esri::ArcGISRuntime::Graphic* m_selectedGraphic{nullptr};

  DrawToolbarController::DrawMode drawMode() const;
  void setDrawMode(DrawToolbarController::DrawMode drawMode);
  DrawToolbarController::DrawMode m_drawMode;

  DrawToolbarController::SelectedElementType selectedElementType() const {return m_SelectedElementType;}
  DrawToolbarController::SelectedElementType m_SelectedElementType;

  bool m_canUndo = false;
  bool canUndo() const { return m_canUndo; }
  bool m_canRedo = false;
  bool canRedo() const { return m_canRedo; }
  bool m_isEditing = false;
  bool isEditing() const { return m_isEditing; }
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_DRAWTOOLBARCONTROLLER_H
