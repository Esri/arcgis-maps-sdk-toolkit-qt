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
  QGuiApplication app(argc, argv);

  // Use of ArcGIS location services, such as basemap styles, geocoding, and routing services,
  // requires an access token. For more information see
  // https://links.esri.com/arcgis-runtime-security-auth.
  // The following methods grant an access token:
  // 1. User authentication: Grants a temporary access token associated with a user's ArcGIS account.
  // To generate a token, a user logs in to the app with an ArcGIS account that is part of an
  // organization in ArcGIS Online or ArcGIS Enterprise.
  // 2. API key authentication: Get a long-lived access token that gives your application access to
  // ArcGIS location services. Go to the tutorial at https://links.esri.com/create-an-api-key.
  // Copy the API Key access token.

  QString accessToken = QString("");
  if (accessToken.isEmpty())
  {
    accessToken = qgetenv("ARCGIS_RUNTIME_API_KEY");
  }

  if (!accessToken.isEmpty())
  {
    Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setApiKey(accessToken);
  }

  // Register ArcGIS types with QML.
  qmlRegisterExtendedType<Esri::ArcGISRuntime::MapQuickView,
                          MapQuickViewProxy>("Esri.ArcGISRuntime", 200, 0, "MapView");
  qmlRegisterExtendedType<Esri::ArcGISRuntime::SceneQuickView,
                          SceneQuickViewProxy>("Esri.ArcGISRuntime", 200, 0, "SceneView");
  qmlRegisterUncreatableType<GeoModelProxy>("Esri.ArcGISRuntime",
                                            200,
                                            0,
                                            "Map",
                                            "Map not creatable in QML.");
  qmlRegisterUncreatableType<GeoModelProxy>("Esri.ArcGISRuntime",
                                            200,
                                            0,
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
                                       200,
                                       0,
                                       "Enums",
                                       [](QQmlEngine* engine, QJSEngine*) -> QObject*
                                       {
                                         return new EnumsProxy(engine);
                                       });

  // Register Own types with QML.
  // Initialize application view
  QQmlApplicationEngine engine;
  Esri::ArcGISRuntime::Toolkit::registerComponents(engine);
  engine.load(QUrl("qrc:/main.qml"));
  return app.exec();
}

//------------------------------------------------------------------------------
