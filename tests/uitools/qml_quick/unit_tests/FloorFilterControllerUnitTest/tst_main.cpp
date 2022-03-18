#include "Esri/ArcGISRuntime/Toolkit/register.h"

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
  }
};

QUICK_TEST_MAIN_WITH_SETUP(example, Setup)

#include "tst_main.moc"
