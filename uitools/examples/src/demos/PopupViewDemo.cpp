
/*******************************************************************************
 *  Copyright 2012-2022 Esri
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

#include "PopupViewDemo.h"

#include "Envelope.h"
#include "Error.h"
#include "Feature.h"
#include "IdentifyLayerResult.h"
#include "LayerListModel.h"
#include "AttributeListModel.h"
#include "Map.h"
#include "MapQuickView.h"
#include "MapTypes.h"
#include "Popup.h"
#include "PopupDefinition.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "ServiceFeatureTable.h"
#include "SpatialReference.h"
#include "Viewpoint.h"
#include "DynamicEntity.h"
#include "DynamicEntityLayer.h"

#include "DynamicEntityObservation.h"

#include <QFuture>
#include <QList>
#include <QMouseEvent>
#include <QUuid>

using namespace Esri::ArcGISRuntime;

PopupViewDemo::PopupViewDemo(QObject* parent) :
  BaseDemo(parent)
{
  connect(this, &BaseDemo::geoViewChanged, this, &PopupViewDemo::setUp);
}

PopupViewDemo::~PopupViewDemo()
{
}

Esri::ArcGISRuntime::Map* PopupViewDemo::initMap_(QObject* parent) const
{
  return new Map(QUrl("https://arcgisruntime.maps.arcgis.com/home/item.html?id=07f8e6e5145542ca8cbeda17797c6a4c"), parent);
}

Scene* PopupViewDemo::initScene_(QObject* parent) const
{
  Scene* scene = BaseDemo::initScene_(parent);
  Viewpoint viewPoint(Envelope(-122.5277, 37.7204, -122.3511, 37.7956, SpatialReference(4326)));
  scene->setInitialViewpoint(viewPoint);
  FeatureLayer* fl = new FeatureLayer(new ServiceFeatureTable(QUrl("https://sampleserver6.arcgisonline.com/arcgis/rest/services/"
                                                                   "SF311/FeatureServer/0"),
                                                              parent),
                                      parent);
  scene->operationalLayers()->append(fl);
  return scene;
}

Popup* PopupViewDemo::popup()
{
  return m_popup;
}

void PopupViewDemo::setPopup(Popup* popup)
{
  if (m_popup == popup)
  {
    return;
  }

  if (m_popup)
  {
    m_popup->deleteLater();
  }

  if (!popup)
  {
    auto layers = geoModel()->operationalLayers();
    if (auto* deLayer = dynamic_cast<DynamicEntityLayer*>(layers->at(1)))
    {
      deLayer->clearSelection();
    }
  }

  m_popup = popup;
  emit popupChanged();
}

void PopupViewDemo::setUp()
{
  // Disconnect any previous connection
  if (m_mouseClickedConnection)
  {
    disconnect(m_mouseClickedConnection);
  }

  apply([this](auto geoView)
        {
          using ViewType = std::remove_pointer_t<decltype(geoView)>;
          m_mouseClickedConnection = connect(geoView, &ViewType::mouseClicked, this, [this, geoView](QMouseEvent& mouse)
                                             {
                                               auto layers = geoModel()->operationalLayers();
                                               if (!layers || layers->size() < 2)
                                               {
                                                 return;
                                               }

                                               auto layer = layers->at(1);
                                               geoView->identifyLayerAsync(layer, mouse.position(), 12, false)
                                                   .then(this, [layer, this](IdentifyLayerResult* rawIdentifyResult)
                                                         {
                                                           auto identifyResult = std::unique_ptr<IdentifyLayerResult>(rawIdentifyResult);
                                                           if (!identifyResult)
                                                           {
                                                             return;
                                                           }
                                                           if (!identifyResult->error().isEmpty())
                                                           {
                                                             qDebug() << "Identify error occurred: " << identifyResult->error().message();
                                                             return;
                                                           }
                                                           const auto geoElements = identifyResult->geoElements();
                                                           if (geoElements.isEmpty())
                                                           {
                                                             qDebug() << "No elements found";
                                                             return;
                                                           }
                                                           if (DynamicEntityObservation* observation = dynamic_cast<DynamicEntityObservation*>(geoElements.constFirst()); observation)
                                                           {
                                                             m_trackedEntity = observation->dynamicEntity();
                                                             if (!m_trackedEntity)
                                                             {
                                                               return;
                                                             }
                                                             m_trackedEntity->setParent(this);
                                                             const auto popup = new Popup(m_trackedEntity, this);
                                                             if (popup->title().isEmpty())
                                                             {
                                                               popup->popupDefinition()->setTitle(identifyResult->layerContent()->name());
                                                             }
                                                             setPopup(popup);

                                                             if (auto* deLayer = dynamic_cast<DynamicEntityLayer*>(layer))
                                                             {
                                                               deLayer->clearSelection();
                                                               deLayer->selectDynamicEntity(m_trackedEntity);
                                                             }
                                                           }
                                                         });
                                             });
        });
}
