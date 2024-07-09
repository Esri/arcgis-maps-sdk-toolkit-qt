/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickStyle>

#include "ArcGISRuntimeEnvironment.h"
#include "CalciteTest.h"

//------------------------------------------------------------------------------

int
main(int argc, char* argv[])
{
  qDebug() << "Initializing application";

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

  const QString accessToken = QString("");

  if (accessToken.isEmpty())
  {
    qWarning() << "Use of ArcGIS location services, such as the basemap styles service, requires" <<
                  "you to authenticate with an ArcGIS account or set the API Key property.";
  }
  else
  {
    Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setApiKey(accessToken);
  }

  // Production deployment of applications built with ArcGIS Maps SDK for Native Apps
  // requires you to license its functionality. For more information see
  // https://links.esri.com/arcgis-runtime-license-and-deploy.


  CalciteTest::init();

  // Intialize application window
  QQmlApplicationEngine appEngine;
  appEngine.addImportPath("qrc:///esri.com/imports/");

#ifdef ARCGIS_RUNTIME_IMPORT_PATH_2
  appEngine.addImportPath(ARCGIS_RUNTIME_IMPORT_PATH_2);
#endif

  appEngine.load(QUrl("qrc:/qml/main.qml"));
  return app.exec();
}
