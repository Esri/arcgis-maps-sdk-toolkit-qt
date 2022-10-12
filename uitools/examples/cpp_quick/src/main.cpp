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

#include "Esri/ArcGISRuntime/Toolkit/register.h"
#include "proxies/ArcGISRuntimeEnvironmentProxy.h"
#include "proxies/GeoModelProxy.h"
#include "proxies/MapQuickViewProxy.h"
#include "proxies/SceneQuickViewProxy.h"

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtWebView>

#include <ArcGISRuntimeEnvironment.h>
#include <Map.h>
#include <MapQuickView.h>
#include <Scene.h>
#include <SceneQuickView.h>

//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  QtWebView::initialize();
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

  QString apiKey = QString("");
  if (apiKey.isEmpty())
  {
    apiKey = qgetenv("ARCGIS_RUNTIME_API_KEY");
  }

  if (!apiKey.isEmpty())
  {
    Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setApiKey(apiKey);
  }

  QQuickStyle::addStylePath("qrc:///esri.com/imports/");

  // Register ArcGIS types with QML.
  qmlRegisterExtendedType<Esri::ArcGISRuntime::MapQuickView,
                          MapQuickViewProxy>("Esri.ArcGISRuntime", 100, 15, "MapView");
  qmlRegisterExtendedType<Esri::ArcGISRuntime::SceneQuickView,
                          SceneQuickViewProxy>("Esri.ArcGISRuntime", 100, 15, "SceneView");
  qmlRegisterUncreatableType<GeoModelProxy>("Esri.ArcGISRuntime",
                                            100,
                                            15,
                                            "Map",
                                            "Map not creatable in QML.");
  qmlRegisterUncreatableType<GeoModelProxy>("Esri.ArcGISRuntime",
                                            100,
                                            15,
                                            "Scene",
                                            "Scene not creatable in QML.");

  qmlRegisterSingletonType<ArcGISRuntimeEnvironmentProxy>(
      "Esri.ArcGISRuntime",
      100,
      15,
      "ArcGISRuntimeEnvironment",
      [](QQmlEngine* engine, QJSEngine*) -> QObject*
      {
        return new ArcGISRuntimeEnvironmentProxy(engine);
      });

  qmlRegisterSingletonType<EnumsProxy>("Esri.ArcGISRuntime",
                                       100,
                                       15,
                                       "Enums",
                                       [](QQmlEngine* engine, QJSEngine*) -> QObject*
                                       {
                                         return new EnumsProxy(engine);
                                       });

  // Register Own types with QML.
  // Initialize application view
  QQmlApplicationEngine engine;
  Esri::ArcGISRuntime::Toolkit::registerComponents(engine);
  engine.load(QUrl("qrc:/demo_app/qml/main.qml"));
  return app.exec();
}

//------------------------------------------------------------------------------
