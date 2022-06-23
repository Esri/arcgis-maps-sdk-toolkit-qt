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

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QtWebView>

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
    QCoreApplication::instance()->setProperty("Esri.ArcGISRuntime.apiKey", apiKey);
  }


  QQuickStyle::addStylePath("qrc:///esri.com/imports/");

  // Intialize application window
  QQmlApplicationEngine engine;
  Esri::ArcGISRuntime::Toolkit::registerComponents(engine);

#ifdef ARCGIS_RUNTIME_IMPORT_PATH_2
  engine.addImportPath(ARCGIS_RUNTIME_IMPORT_PATH_2);
#endif

  engine.load(QUrl("qrc:/demo_app/qml/main.qml"));
  return app.exec();
}
