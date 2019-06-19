
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
  qDebug() << "Initializing application";

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

  // Intialize application window

  QQmlApplicationEngine appEngine;
  appEngine.addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));

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
  appEngine.addImportPath(arcGISRuntimeImportPath);
  // Add the Toolkit path
  appEngine.addImportPath(arcGISToolkitImportPath);

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
