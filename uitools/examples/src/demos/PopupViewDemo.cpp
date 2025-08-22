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
#include "Map.h"
#include "MapTypes.h"
#include "MapQuickView.h"
#include "Popup.h"
#include "PopupDefinition.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "ServiceFeatureTable.h"
#include "SpatialReference.h"
#include "Viewpoint.h"

#include <QList>
#include <QMouseEvent>
#include <QUuid>
#include <QFuture>

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

Popup* PopupViewDemo::popup()
{
  return m_popup;
}

void PopupViewDemo::setPopup(Popup* popup)
{
  if (m_popup == popup)
    return;

  if (m_popup)
  {
    m_popup->deleteLater();
  }

  m_popup = popup;
  emit popupChanged();
}

void PopupViewDemo::setUp()
{
  apply([this](auto geoView)
        {
          using ViewType = std::remove_pointer_t<decltype(geoView)>;
          connect(geoView, &ViewType::mouseClicked, this, [this, geoView](QMouseEvent& mouse)
                  {
              auto layer = geoModel()->operationalLayers()->at(0);
                    if (layer->layerType() == LayerType::FeatureLayer)
                    {
                      m_featureLayer = static_cast<FeatureLayer*>(layer);
                      geoView->identifyLayerAsync(m_featureLayer, mouse.position(), 12, false).then(this, [this](IdentifyLayerResult* rawIdentifyResult)
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

                        if (geoElements.length() == 0)
                        {
                          qDebug() << "no geoElements";
                          return;
                        }

                        const auto popup = new Popup(geoElements.first(), this);
                        popup->setParent(this);

                        if (popup->title().isEmpty())
                        {
                          popup->popupDefinition()->setTitle(identifyResult->layerContent()->name());
                        }

                        if (auto element = popup->geoElement())
                        {
                          Feature* feature = static_cast<Feature*>(element);
                          m_featureLayer->selectFeature(feature);
                        }

                        setPopup(popup);
                        emit popupChanged();
                      });
                    }
                    else
                    {
                      qDebug() << "Unexpected layer type taken from click.";
                    }
                  });
        });
}
