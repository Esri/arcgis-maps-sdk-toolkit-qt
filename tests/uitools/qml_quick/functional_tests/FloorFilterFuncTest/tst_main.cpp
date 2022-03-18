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
#include "utils.h"

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QtQuickTest>

class Setup : public QObject
{
  Q_OBJECT

public:
  Setup()
  {
  }

public slots:
  void qmlEngineAvailable(QQmlEngine* engine)
  {
    engine->addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));
    Esri::ArcGISRuntime::Toolkit::registerComponents(*engine);
#ifdef ARCGIS_RUNTIME_IMPORT_PATH_2
    engine->addImportPath(ARCGIS_RUNTIME_IMPORT_PATH_2);
#endif
    engine->rootContext()->setContextProperty("credential_username", qgetenv("CREDENTIAL_USERNAME"));
    engine->rootContext()->setContextProperty("credential_password", qgetenv("CREDENTIAL_PASSWORD"));
    qmlRegisterSingletonType<Utils>("utils", 1, 0, "Utils", Utils::createSingletonInstance);
  }
};

QUICK_TEST_MAIN_WITH_SETUP(example, Setup)

#include "tst_main.moc"
