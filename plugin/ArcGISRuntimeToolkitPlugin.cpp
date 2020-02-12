#include "ArcGISRuntimeToolkitPlugin.h"

// Qt headers
#include <QQmlEngine>

// Toolkit headers
#include "NorthArrowController.h"

ArcGISRuntimeToolkitPlugin::ArcGISRuntimeToolkitPlugin(QObject* parent) :
  QQmlExtensionPlugin(parent)
{
}

ArcGISRuntimeToolkitPlugin::~ArcGISRuntimeToolkitPlugin()
{
}

void ArcGISRuntimeToolkitPlugin::registerTypes(const char *uri)
{
  Q_ASSERT(uri == QLatin1String("Esri.ArcGISRuntime.Toolkit"));
  qmlRegisterType<NorthArrowController>(uri, 1, 0, "NorthArrow");
}
