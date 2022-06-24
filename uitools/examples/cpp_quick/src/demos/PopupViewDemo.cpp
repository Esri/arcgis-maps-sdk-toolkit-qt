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

#include "GeoView.h"
#include "Map.h"
#include "MapQuickView.h"
#include "PopupManager.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "ServiceFeatureTable.h"

#include <QList>

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
  return new Map(QUrl("https://runtime.maps.arcgis.com/home/webmap/"
                      "viewer.html?webmap=e4c6eb667e6c43b896691f10cc2f1580"),
                 parent);
}

Scene* PopupViewDemo::initScene_(QObject* parent) const
{
  Scene* scene = BaseDemo::initScene_(parent);
  Viewpoint viewPoint(Envelope(-122.5277, 37.7204, -122.3511, 37.7956, SpatialReference(4326)));
  scene->setInitialViewpoint(viewPoint);
  FeatureLayer* fl = new FeatureLayer(new ServiceFeatureTable(
                                          QUrl("https://sampleserver6.arcgisonline.com/arcgis/rest/services/"
                                               "SF311/FeatureServer/0"),
                                          parent),
                                      parent);
  scene->operationalLayers()->append(fl);
  return scene;
}

QObject* PopupViewDemo::popupManager_()
{
  return m_popupManager;
}

void PopupViewDemo::setPopupManager_(QObject* popupManager)
{
  if (popupManager == m_popupManager)
    return;

  auto oldManager = m_popupManager;
  m_popupManager = popupManager;
  emit popupManagerChanged();
  delete oldManager;
}

void PopupViewDemo::setUp()
{
  apply([this](auto geoView)
        {
          using ViewType = std::remove_pointer_t<decltype(geoView)>;
          connect(geoView, &ViewType::mouseClicked, this, [this, geoView](QMouseEvent& mouse)
                  {
                    auto layer = geoModel()->operationalLayers()->first();
                    if (layer->layerType() == LayerType::FeatureLayer)
                    {
                      m_featureLayer = static_cast<FeatureLayer*>(layer);
                      geoView->identifyLayer(m_featureLayer, mouse.x(), mouse.y(), 12, false);
                    }
                    else
                    {
                      qDebug() << "Unexpected layer type taken from click.";
                    }
                  });

          connect(geoView,
                  &ViewType::identifyLayerCompleted,
                  this,
                  [this](QUuid /*id*/, IdentifyLayerResult* rawIdentifyResult)
                  {
                    // managed by smart pointer
                    auto identifyResult = std::unique_ptr<IdentifyLayerResult>(rawIdentifyResult);
                    if (!identifyResult)
                      return;

                    if (!identifyResult->error().isEmpty())
                    {
                      qDebug() << "Identify error occurred: "
                               << identifyResult->error().message();
                      return;
                    }

                    m_featureLayer->clearSelection();

                    const auto geoElements = identifyResult->geoElements();
                    for (auto element : geoElements)
                    {
                      if (nullptr != element)
                      {
                        // add the element to the list and take ownership of it.
                        Feature* feature = static_cast<Feature*>(element);
                        m_featureLayer->selectFeature(feature);
                      }
                    }

                    if (identifyResult->geoElements().length() == 0)
                    {
                      qDebug() << "no geoElements";
                      return;
                    }

                    Popup* popup = new Popup(identifyResult->geoElements().first());
                    popup->popupDefinition()->setTitle(identifyResult->layerContent()->name());

                    PopupManager* popupManager = new PopupManager{popup, this};
                    popup->setParent(popupManager);

                    setPopupManager_(popupManager);
                    emit popupManagerChanged();
                  });
        });
}
