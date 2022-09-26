/*******************************************************************************
 *  Copyright 2012-2019 Esri
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

#include <qglobal.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "AppInfo.h"
#include "CppArExample.h"

#include "ArcGISRuntimeEnvironment.h"
#include "SceneQuickView.h"

#include <QDir>
#include <QGuiApplication>
#include <QMessageBox>
#include <QSettings>
#include <QSurfaceFormat>
#include <QQmlApplicationEngine>

// Include the AR view from toolkit
#include "ArcGISArView.h"

//------------------------------------------------------------------------------

#define kSettingsFormat                 QSettings::IniFormat

//------------------------------------------------------------------------------

#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

//------------------------------------------------------------------------------

using namespace Esri::ArcGISRuntime;

int main(int argc, char *argv[])
{
  // Enforce OpenGL
  qputenv("QSG_RHI_BACKEND", "opengl");

  // There are some conflicts between the AR frameworks and the Qt's rendering thread.
  // This code enables the non-threaded render loop mode in Qt.
  // See SceneView::renderFrame documentation and Qt's documentation
  // https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#non-threaded-render-loops-basic-and-windows
  // for more information.
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
  qputenv("QSG_RENDER_LOOP", "basic");
#endif

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
  // Linux requires 3.2 OpenGL Context
  // in order to instance 3D symbols
  QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
  fmt.setVersion(3, 2);
  QSurfaceFormat::setDefaultFormat(fmt);
#endif

  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);

  QCoreApplication::setApplicationName(kApplicationName);
  QCoreApplication::setApplicationVersion(kApplicationVersion);
  QCoreApplication::setOrganizationName(kOrganizationName);
#ifdef Q_OS_MAC
  QCoreApplication::setOrganizationDomain(kOrganizationName);
#else
  QCoreApplication::setOrganizationDomain(kOrganizationDomain);
#endif
  QSettings::setDefaultFormat(kSettingsFormat);

  // Before initializing ArcGIS Runtime, first set the
  // ArcGIS Runtime license setting required for your application.

  // ArcGISRuntimeEnvironment::setLicense("Place license string in here");

  //  use this code to check for initialization errors
  //  QObject::connect(ArcGISRuntimeEnvironment::instance(), &ArcGISRuntimeEnvironment::errorOccurred, [](const Error& error){
  //    QMessageBox msgBox;
  //    msgBox.setText(error.message);
  //    msgBox.exec();
  //  });

  //  if (ArcGISRuntimeEnvironment::initialize() == false)
  //  {
  //    application.quit();
  //    return 1;
  //  }

  // Register the scene view for QML
  qmlRegisterType<SceneQuickView>("Esri.CppArExample", 1, 0, "SceneView");

  // Register the CppArExample (QQuickItem) for QML
  qmlRegisterType<CppArExample>("Esri.CppArExample", 1, 0, "CppArExample");

  // Register the ArcGISArView
  Esri::ArcGISRuntime::Toolkit::ArcGISArView::qmlRegisterTypes();

  // Initialize application view
  QQmlApplicationEngine engine;

  // Add the import Path
  engine.addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));
  QString arcGISRuntimeImportPath = QUOTE(ARCGIS_RUNTIME_IMPORT_PATH);

#if defined(LINUX_PLATFORM_REPLACEMENT)
  // on some linux platforms the string 'linux' is replaced with 1
  // fix the replacement paths which were created
  QString replaceString = QUOTE(LINUX_PLATFORM_REPLACEMENT);
  arcGISRuntimeImportPath = arcGISRuntimeImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
#endif

  // Add the Runtime and Extras path
  engine.addImportPath(arcGISRuntimeImportPath);

  // Set the source
  engine.load(QUrl(kApplicationSourceUrl));

  return app.exec();
}

//------------------------------------------------------------------------------
