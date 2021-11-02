#include "AuthenticationViewDemoModel.h"
#include "Map.h"
#include "Scene.h"

AuthenticationViewDemoModel::AuthenticationViewDemoModel(QObject *parent) {}

AuthenticationViewDemoModel::~AuthenticationViewDemoModel() {}

Esri::ArcGISRuntime::Map *AuthenticationViewDemoModel::initMap_(QObject *parent) const
{
    //return a map that requires authentication
    return new Esri::ArcGISRuntime::Map(QUrl(QStringLiteral(
        "https://sampleserver6.arcgisonline.com/arcgis/rest/services/USA_secure_user1/MapServer")));
}

Esri::ArcGISRuntime::Scene *AuthenticationViewDemoModel::initScene_(QObject *parent) const
{
    return new Esri::ArcGISRuntime::Scene(QUrl(QStringLiteral(
        "https://sampleserver6.arcgisonline.com/arcgis/rest/services/USA_secure_user1/MapServer")));
}
