// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "Esri/ArcGISRuntime/Toolkit/register.h"

#include "util/ArcGISRuntimeEnvironmentProxy.h"

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include <ArcGISRuntimeEnvironment.h>
#include <MapQuickView.h>
#include <SceneQuickView.h>

//------------------------------------------------------------------------------

using namespace Esri::ArcGISRuntime;

int main(int argc, char* argv[])
{
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);

  // Use of Esri location services, including basemaps and geocoding, requires
  // either an ArcGIS identity or an API key. For more information see
  // https://links.esri.com/arcgis-runtime-security-auth.

  // 1. ArcGIS identity: An ArcGIS named user account that is a member of an
  // organization in ArcGIS Online or ArcGIS Enterprise.

  // 2. API key: A permanent key that gives your application access to Esri
  // location services. Create a new API key or access existing API keys from
  // your ArcGIS for Developers dashboard (https://links.esri.com/arcgis-api-keys).

  const QString apiKey = QString("");
  if (!apiKey.isEmpty())
  {
    ArcGISRuntimeEnvironment::setApiKey(apiKey);
  }

  QQuickStyle::addStylePath("qrc:///esri.com/imports/");

  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.ArcGISRuntime", 100, 13, "MapView");
  qmlRegisterType<SceneQuickView>("Esri.ArcGISRuntime", 100, 13, "SceneView");
  qmlRegisterType<ArcGISRuntimeEnvironmentProxy>("Esri.ArcGISRuntime", 100, 13, "ArcGISRuntimeEnvironment");
  qmlRegisterType(QUrl("qrc:/shared/qml/DemoPage.qml"), "DemoPage", 1, 0, "DemoPage");
  // Initialize application view
  QQmlApplicationEngine engine;
  Esri::ArcGISRuntime::Toolkit::registerComponents(engine);

  engine.load(QUrl("qrc:/shared/qml/main.qml"));
  return app.exec();
}

//------------------------------------------------------------------------------
