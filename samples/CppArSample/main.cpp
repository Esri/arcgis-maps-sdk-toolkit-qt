
// Copyright 2019 ESRI
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

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "AppInfo.h"
#include "CppArSample.h"

#include "ArcGISRuntimeEnvironment.h"
#include "SceneQuickView.h"

#include <QDir>
#include <QGuiApplication>
#include <QMessageBox>
#include <QSettings>
#include <QSurfaceFormat>
#include <QQmlApplicationEngine>

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
  // There are some conflicts between the AR frameworks and the Qt's rendering thread.
  // This lines of code enable the non-threaded render loops mode in Qt.
  // See SceneView::renderFrame documentation and Qt's documentation
  // https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html#non-threaded-render-loops-basic-and-windows
  // for more informations.
#if defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
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
  qmlRegisterType<SceneQuickView>("Esri.CppArSample", 1, 0, "SceneView");

  // Register the CppArSample (QQuickItem) for QML
  qmlRegisterType<CppArSample>("Esri.CppArSample", 1, 0, "CppArSample");

  // Register the ArcGISArView
  qmlRegisterType<Esri::ArcGISRuntime::Toolkit::ArcGISArView>("Esri.CppArSample", 1, 0, "ArcGISArView");

  // Initialize application view
  QQmlApplicationEngine engine;

  // Add the import Path
  engine.addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));
  QString arcGISRuntimeImportPath = QUOTE(ARCGIS_RUNTIME_IMPORT_PATH);
  QString arcGISToolkitImportPath = QUOTE(ARCGIS_TOOLKIT_IMPORT_PATH);

#if defined(LINUX_PLATFORM_REPLACEMENT)
  // on some linux platforms the string 'linux' is replaced with 1
  // fix the replacement paths which were created
  QString replaceString = QUOTE(LINUX_PLATFORM_REPLACEMENT);
  arcGISRuntimeImportPath = arcGISRuntimeImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
  arcGISToolkitImportPath = arcGISToolkitImportPath.replace(replaceString, "linux", Qt::CaseSensitive);
#endif

  // Add the Runtime and Extras path
  engine.addImportPath(arcGISRuntimeImportPath);
  // Add the Toolkit path
  engine.addImportPath(arcGISToolkitImportPath);

  // Set the source
  engine.load(QUrl(kApplicationSourceUrl));

  return app.exec();
}

//------------------------------------------------------------------------------
