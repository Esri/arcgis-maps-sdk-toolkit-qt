#include "PopupViewDemoModel.h"

#include "GeoView.h"
#include "Map.h"
#include "MapQuickView.h"
#include "PopupManager.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "ServiceFeatureTable.h"

#include <QList>

using namespace Esri::ArcGISRuntime;

PopupViewDemoModel::PopupViewDemoModel(QObject* parent) :
  BaseDemoModel(parent)
{
  connect(this, &BaseDemoModel::geoViewChanged, this, &PopupViewDemoModel::setUp);
}

PopupViewDemoModel::~PopupViewDemoModel()
{
}

Esri::ArcGISRuntime::Map* PopupViewDemoModel::initMap_(QObject* parent) const
{
  return new Map(QUrl("https://runtime.maps.arcgis.com/home/webmap/"
                      "viewer.html?webmap=e4c6eb667e6c43b896691f10cc2f1580"),
                 parent);
}

Scene* PopupViewDemoModel::initScene_(QObject* parent) const
{
  Scene* scene = BaseDemoModel::initScene_(parent);
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

QObject* PopupViewDemoModel::popupManager_()
{
  return m_popupManager;
}

void PopupViewDemoModel::setPopupManager_(QObject* popupManager)
{
  if (popupManager == m_popupManager)
    return;

  auto oldManager = m_popupManager;
  m_popupManager = popupManager;
  emit popupManagerChanged();
  delete oldManager;
}

void PopupViewDemoModel::setUp()
{
  apply([this](auto geoView)
        {
          using ViewType = std::remove_pointer_t<decltype(geoView)>;
          connect(geoView, &ViewType::mouseClicked, this, [this, geoView](QMouseEvent& mouse)
                  {
                    m_featureLayer = static_cast<FeatureLayer*>(geoModel()->operationalLayers()->first());
                    geoView->identifyLayer(m_featureLayer, mouse.x(), mouse.y(), 12, false);
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
