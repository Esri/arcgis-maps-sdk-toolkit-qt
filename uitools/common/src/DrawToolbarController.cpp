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
#include "DrawToolbarController.h"

// Qt headers
#include <QUuid>

// ArcGISRuntime headers
#include <Error.h>
#include <FreehandTool.h>
#include <Geometry.h>
#include <GeometryEditor.h>
#include <GeometryEditorElement.h>
#include <GeometryEditorGeometry.h>
#include <GeometryEditorMidVertex.h>
#include <GeometryEditorPart.h>
#include <GeometryEditorTypes.h>
#include <GeometryEditorVertex.h>
#include <GeometryTypes.h>
#include <Graphic.h>
#include <GraphicListModel.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>
#include <IdentifyGraphicsOverlayResult.h>
#include <ImmutablePartCollection.h>
#include <InteractionConfiguration.h>
#include <MapQuickView.h>
#include <Part.h>
#include <PartCollection.h>
#include <Point.h>
#include <Polygon.h>
#include <PolygonBuilder.h>
#include <Polyline.h>
#include <PolylineBuilder.h>
#include <SimpleFillSymbol.h>
#include <SimpleLineSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <SpatialReference.h>
#include <SymbolTypes.h>
#include <TaskWatcher.h>
#include <VertexTool.h>

namespace Esri::ArcGISRuntime::Toolkit {


/*!
  \class Esri::ArcGISRuntime::Toolkit::DrawToolbarController
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief TODO
 */

/*!
  \brief Constructor
  \list
    \li \a parent Parent owning \c QObject.
  \endlist
 */
DrawToolbarController::DrawToolbarController(QObject* parent) :
  QObject(parent),
  m_graphicsOverlay(new GraphicsOverlay(this)),
  m_geometryEditor(new GeometryEditor(this)),
  m_vertexTool(new VertexTool(this)),
  m_freehandTool(new FreehandTool(this)),
  m_simpleFillSymbol(new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor("orange"), this)),
  m_simpleLineSymbol(new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor("blue"), 5, this)),
  m_simpleMarkerSymbol(new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("purple"), 20, this))
{
  connect(m_geometryEditor, &GeometryEditor::selectedElementChanged, this, [this]()
  {
    auto selectedElement = m_geometryEditor->selectedElement();
    if (!selectedElement)
    {
      m_SelectedElementType = SelectedElementType::None;
      emit selectedElementTypeChanged();
      return;
    }
    switch (selectedElement->geometryEditorElementType()) {
    case GeometryEditorElementType::GeometryEditorGeometry:
      m_SelectedElementType = SelectedElementType::Geometry;
      break;
    case GeometryEditorElementType::GeometryEditorPart:
      m_SelectedElementType = SelectedElementType::Part;
      break;
    case GeometryEditorElementType::GeometryEditorVertex:
      m_SelectedElementType = SelectedElementType::Vertex;
      break;
    case GeometryEditorElementType::GeometryEditorMidVertex:
      m_SelectedElementType = SelectedElementType::MidVertex;
      break;
    }
    emit selectedElementTypeChanged();
  });

  connect(m_geometryEditor, &GeometryEditor::canUndoChanged, this, [this]()
  {
    m_canUndo = m_geometryEditor->canUndo();
    emit canUndoChanged();
  });

  connect(m_geometryEditor, &GeometryEditor::canRedoChanged, this, [this]()
  {
    m_canRedo = m_geometryEditor->canRedo();
    emit canRedoChanged();
  });

  connect(m_geometryEditor, &GeometryEditor::isStartedChanged, this, [this]()
  {
    m_isEditing = m_geometryEditor->isStarted();
    emit isEditingChanged();
  });
}

/*!
  \brief Destructor.
 */
DrawToolbarController::~DrawToolbarController()
{
}

/*!
    \brief Returns the \c MapView as a \c QObject.
  */
QObject* DrawToolbarController::mapView() const
{
  return m_mapView;
}

/*!
    \brief Set the \c MapView object this Controller uses.

    Internally this is cast to a \c MapView using \c qobject_cast.

    \list
    \li \a mapView MapView Object.
    \endlist
   */
void DrawToolbarController::setMapView(QObject* mapView)
{
  auto mapView2 = qobject_cast<MapViewToolkit*>(mapView);

  if (m_mapView == mapView2)
    return;

  m_mapView = mapView2;

  m_mapView->setGeometryEditor(m_geometryEditor);
  m_geometryEditor->setParent(m_mapView);

  m_mapView->graphicsOverlays()->append(m_graphicsOverlay);


  connect(m_mapView, &MapQuickView::mouseMoved, this, [this](const QMouseEvent& event)
  {
    //qDebug() << "MOUSE MOVED!!" << event.position().x() << event.position().y();
  });

  connect(m_mapView, &MapQuickView::mouseClicked, this, [this](const QMouseEvent& event)
  {
    if (m_drawMode == DrawMode::SelectDrawMode)
      m_mapView->identifyGraphicsOverlay(m_graphicsOverlay, event.position().x(), event.position().y(), 4, false);
  });

  connect(m_mapView, &MapQuickView::mouseDoubleClicked, this, [this](QMouseEvent& event)
  {
    if (m_geometryEditor->isStarted())
    {
      m_geometryEditor->insertVertex(m_mapView->screenToLocation(event.position().x(), event.position().y()));
      save();
      event.accept();
    }
    else
      event.ignore();
  });

  connect(m_mapView, &MapQuickView::identifyGraphicsOverlayCompleted, this, [this](QUuid, IdentifyGraphicsOverlayResult* result)
  {
    if (result->graphics().count() < 1)
      return;

    m_selectedGraphic = result->graphics().at(0);
    Geometry geom = m_selectedGraphic->geometry();
    m_geometryEditor->start(geom);
    m_geometryEditor->setTool(m_vertexTool);

    switch (geom.geometryType()) {
    case GeometryType::Point:
    case GeometryType::Multipoint:
      m_drawMode = DrawMode::PointDrawMode;
      break;
    case GeometryType::Polyline:
      m_drawMode = DrawMode::PolylineDrawMode;
      break;
    case GeometryType::Polygon:
    case GeometryType::Envelope:
      m_drawMode = DrawMode::PolygonDrawMode;
      break;
    case GeometryType::Unknown:
      break;
    default:
      break;
    }

    m_graphicsOverlay->graphics()->removeOne(m_selectedGraphic);
    delete result;

    emit drawModeChanged();
  });

  emit mapViewChanged();
}

DrawToolbarController::DrawMode DrawToolbarController::drawMode() const
{
  return m_drawMode;
}

void DrawToolbarController::setDrawMode(DrawMode drawMode)
{
  if (!m_geometryEditor)
    return;

  if (!m_mapView)
    return;

  m_drawMode = drawMode;

  switch (drawMode) {
  case DrawMode::SelectDrawMode:
    if (m_geometryEditor->isStarted())
      save();
    break;
  case DrawMode::PointDrawMode:
    if (m_geometryEditor->isStarted())
      return;
    m_geometryEditor->setTool(m_vertexTool);
    m_geometryEditor->start(GeometryType::Point);
    break;
  case DrawMode::PolylineDrawMode:
    if (m_geometryEditor->isStarted())
      return;
    m_geometryEditor->setTool(m_vertexTool);
    m_geometryEditor->start(GeometryType::Polyline);
    break;
  case DrawMode::PolygonDrawMode:
    if (m_geometryEditor->isStarted())
      return;
    m_geometryEditor->setTool(m_vertexTool);
    m_geometryEditor->start(GeometryType::Polygon);
    break;
  case DrawMode::FreehandPolygonDrawMode:
    if (m_geometryEditor->isStarted())
      return;
    m_geometryEditor->setTool(m_freehandTool);
    m_geometryEditor->start(GeometryType::Polygon);
    break;
  case DrawMode::FreehandLineDrawMode:
    if (m_geometryEditor->isStarted())
      return;
    m_geometryEditor->setTool(m_freehandTool);
    m_geometryEditor->start(GeometryType::Polyline);
    break;
  case DrawMode::NoneDrawMode:
  default:
    qDebug() << "none";
    break;
  }

  m_drawMode = drawMode;
  emit drawModeChanged();
}

void DrawToolbarController::clearAll()
{
  if (m_geometryEditor)
  {
    m_geometryEditor->clearGeometry();
    m_geometryEditor->clearSelection();
  }
}

void DrawToolbarController::undo()
{
  if (m_geometryEditor)
  {
    if (m_geometryEditor->canUndo())
    {
      m_geometryEditor->undo();
    }
  }
}

void DrawToolbarController::redo()
{
  if (m_geometryEditor)
  {
    if (m_geometryEditor->canRedo())
    {
      m_geometryEditor->redo();
    }
  }
}

void DrawToolbarController::save()
{
  if (!m_geometryEditor || !m_graphicsOverlay)
    return;

  if (!m_geometryEditor->isStarted())
    return;

  Geometry geom = m_geometryEditor->stop();
  auto graphic = new Graphic(geom, this);

  switch (geom.geometryType()) {
  case GeometryType::Point:
  case GeometryType::Multipoint:
    graphic->setSymbol(m_simpleMarkerSymbol);
    break;
  case GeometryType::Polyline:
    graphic->setSymbol(m_simpleLineSymbol);
    break;
  case GeometryType::Polygon:
  case GeometryType::Envelope:
    graphic->setSymbol(m_simpleFillSymbol);
    break;
  case GeometryType::Unknown:
    qDebug() << "unknown";
    break;
  }

  m_graphicsOverlay->graphics()->append(graphic);

  setDrawMode(m_drawMode);
}

void DrawToolbarController::deleteSelected()
{
  m_geometryEditor->deleteSelectedElement();
}

void DrawToolbarController::addPart(double mouseX, double mouseY)
{
  QObject localParent;
  if (m_drawMode == DrawMode::PolygonDrawMode)
  {
    PolygonBuilder builder(geometry_cast<Polygon>(m_geometryEditor->geometry()), &localParent);
    Part part(builder.spatialReference(), &localParent);
    builder.parts()->addPart(&part);
    m_geometryEditor->replaceGeometry(builder.toGeometry());
    Point newPoint = m_mapView->screenToLocation(mouseX, mouseY);
    m_geometryEditor->insertVertex(newPoint);
  }
  else if (m_drawMode == DrawMode::PolylineDrawMode)
  {
    PolylineBuilder builder2(geometry_cast<Polyline>(m_geometryEditor->geometry()), &localParent);
    Part part(builder2.spatialReference(), &localParent);
    builder2.parts()->addPart(&part);
    m_geometryEditor->replaceGeometry(builder2.toGeometry());
    Point newPoint = m_mapView->screenToLocation(mouseX, mouseY);
    m_geometryEditor->insertVertex(newPoint);
  }
}

} // Esri::ArcGISRuntime::Toolkit
