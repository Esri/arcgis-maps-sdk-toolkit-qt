#include "PopupViewDemoModel.h"
#include "GeoView.h"
#include "Map.h"
#include "MapQuickView.h"
#include "PopupManager.h"
#include "SceneQuickView.h"
#include <QList>

using namespace Esri::ArcGISRuntime;

PopupViewDemoModel::PopupViewDemoModel(QObject *parent) : m_popupManager(nullptr)
{
    connect(this, &BaseDemoModel::geoViewChanged, this, &PopupViewDemoModel::setUp);
}

PopupViewDemoModel::~PopupViewDemoModel() {}

Esri::ArcGISRuntime::Map *PopupViewDemoModel::initMap_(QObject *parent) const
{
    return new Map(QUrl("https://runtime.maps.arcgis.com/home/webmap/"
                        "viewer.html?webmap=e4c6eb667e6c43b896691f10cc2f1580"));
}

QObject *PopupViewDemoModel::popupManager_()
{
    return m_popupManager;
}

void PopupViewDemoModel::setPopupManager_(QObject *popupManager)
{
    m_popupManager = popupManager;
}

void PopupViewDemoModel::setUp()
{
    apply([this](auto geoView) {
        using ViewType = std::remove_pointer_t<decltype(geoView)>;
        auto &featureLayer = this->featureLayer;
        connect(geoView, &ViewType::mouseClicked, this, [&featureLayer, geoView](QMouseEvent &mouse) {
            if (MapQuickView *mapQuickView = qobject_cast<MapQuickView *>(geoView)) {
                qDebug() << "is a mapquickview";
                featureLayer = static_cast<FeatureLayer *>(
                    mapQuickView->map()->operationalLayers()->first());
                mapQuickView->identifyLayer(featureLayer, mouse.x(), mouse.y(), 12, false);
                qDebug() << (featureLayer == nullptr);
            } else {
                qDebug() << "not a mapquickview";
            }
        });

        connect(geoView,
                &ViewType::identifyLayerCompleted,
                this,
                [this, &featureLayer, geoView](QUuid, IdentifyLayerResult *rawIdentifyResult) {
                    qDebug() << (featureLayer == nullptr);
                    qDebug() << (this->featureLayer == nullptr);
                    // managed by smart pointer
                    auto identifyResult = std::unique_ptr<IdentifyLayerResult>(rawIdentifyResult);
                    if (!identifyResult)
                        return;

                    if (!identifyResult->error().isEmpty()) {
                        qDebug() << "Identify error occurred: "
                                 << identifyResult->error().message();
                        return;
                    }
                    //todo: check if the clicked is not a featurelayer. otherwise, dont clear
                    featureLayer->clearSelection();

                    for (int i = 0; i < identifyResult->geoElements().size(); i++) {
                        GeoElement *element = identifyResult->geoElements().at(i);
                        if (nullptr != element) {
                            // add the element to the list and take ownership of it.
                            Feature *feature = static_cast<Feature *>(element);
                            featureLayer->selectFeature(feature);
                        }
                    }

                    if (!identifyResult->popups().isEmpty()) {
                        auto popup = identifyResult->popups().first();
                        // create a popup manager
                        PopupManager *popupManager = new PopupManager{popup, this};
                        m_popupManager = popupManager;
                        //letting know qml that this property was changed, so update all the other linked to it
                        emit popupManagerChanged();
                    }
                });
    });
}
