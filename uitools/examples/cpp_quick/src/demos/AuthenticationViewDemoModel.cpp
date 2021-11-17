#include "AuthenticationViewDemoModel.h"

#include "Map.h"
#include "Scene.h"

AuthenticationViewDemoModel::AuthenticationViewDemoModel(QObject* parent) :
  BaseDemoModel(parent)
{
}

AuthenticationViewDemoModel::~AuthenticationViewDemoModel()
{
}

Esri::ArcGISRuntime::Map* AuthenticationViewDemoModel::initMap_(QObject* parent) const
{
  //return a map that requires authentication
  return new Esri::ArcGISRuntime::Map(new Esri::ArcGISRuntime::PortalItem(QStringLiteral("e5039444ef3c48b8a8fdc9227f9be7c1"), parent));
}

Esri::ArcGISRuntime::Scene* AuthenticationViewDemoModel::initScene_(QObject* /*parent*/) const
{
  return nullptr;
}
