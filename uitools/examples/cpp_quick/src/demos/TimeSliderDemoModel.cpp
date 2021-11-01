#include "TimeSliderDemoModel.h"
#include "ArcGISMapImageLayer.h"
#include "Map.h"
#include "Scene.h"

TimeSliderDemoModel::TimeSliderDemoModel(QObject *parent) {}

TimeSliderDemoModel::~TimeSliderDemoModel() {}

Esri::ArcGISRuntime::Map *TimeSliderDemoModel::initMap_(QObject *parent) const
{
    using namespace Esri::ArcGISRuntime;
    auto map = new Map(BasemapStyle::ArcGISCommunity, parent);
    //add mapimage layer
    return map;
}

Esri::ArcGISRuntime::Scene *TimeSliderDemoModel::initScene_(QObject *parent) const
{
    using namespace Esri::ArcGISRuntime;
    auto scene = new Scene(BasemapStyle::ArcGISCommunity, parent);
    //add sceneimage layer
    return scene;
}
