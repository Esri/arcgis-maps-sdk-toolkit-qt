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

#include <QDebug>
#include <QSettings>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QCommandLineParser>
#include <QDir>
#include <QSurfaceFormat>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "AppInfo.h"

// Include the AR view from toolkit
#include "QmlArcGISArView.h"

//------------------------------------------------------------------------------

#define kSettingsFormat                 QSettings::IniFormat

//------------------------------------------------------------------------------

#define kArgShowName                    "show"
#define kArgShowValueName               "showOption"
#define kArgShowDescription             "Show option maximized | minimized | fullscreen | normal | default"
#define kArgShowDefault                 "show"

#define kShowMaximized                  "maximized"
#define kShowMinimized                  "minimized"
#define kShowFullScreen                 "fullscreen"
#define kShowNormal                     "normal"
#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  // Enable OpenGL
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

  // Initialize license

#ifdef kLicense
  QCoreApplication::instance()->setProperty("Esri.ArcGISRuntime.license", kLicense);
#endif

  // Register the AR toolkit types
  Esri::ArcGISRuntime::Toolkit::QmlArcGISArView::qmlRegisterTypes();

  // Intialize application window
  QQmlApplicationEngine appEngine;
  appEngine.addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));

  QString arcGISRuntimeImportPath = QUOTE(ARCGIS_RUNTIME_IMPORT_PATH);

#if defined(LINUX_PLATFORM_REPLACEMENT)
  // On some Linux platforms the string 'linux' is replaced with 1
  // Fix the created replacement paths
  QString replaceString = QUOTE(LINUX_PLATFORM_REPLACEMENT);
  arcGISRuntimeImportPath = arcGISRuntimeImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
#endif

  // Add the Runtime and Extras paths
  appEngine.addImportPath(arcGISRuntimeImportPath);

  appEngine.load(QUrl(kApplicationSourceUrl));

  auto topLevelObject = appEngine.rootObjects().value(0);
  qDebug() << Q_FUNC_INFO << topLevelObject;

  auto window = qobject_cast<QQuickWindow *>(topLevelObject);
  if (!window)
  {
    qCritical("Error: Your root item has to be a Window.");

    return -1;
  }

#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
  // Process command line

  QCommandLineOption showOption(kArgShowName, kArgShowDescription, kArgShowValueName, kArgShowDefault);

  QCommandLineParser commandLineParser;

  commandLineParser.setApplicationDescription(kApplicationDescription);
  commandLineParser.addOption(showOption);
  commandLineParser.addHelpOption();
  commandLineParser.addVersionOption();
  commandLineParser.process(app);

  // Show app window

  auto showValue = commandLineParser.value(kArgShowName).toLower();

  if (showValue.compare(kShowMaximized) == 0)
  {
    window->showMaximized();
  }
  else if (showValue.compare(kShowMinimized) == 0)
  {
    window->showMinimized();
  }
  else if (showValue.compare(kShowFullScreen) == 0)
  {
    window->showFullScreen();
  }
  else if (showValue.compare(kShowNormal) == 0)
  {
    window->showNormal();
  }
  else
  {
    window->show();
  }

#else
  window->show();
#endif

  return app.exec();
}

//------------------------------------------------------------------------------
