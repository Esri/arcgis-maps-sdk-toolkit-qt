#include "register_plugins.h"

#include <QQmlEngine>

#include "NorthArrowController.h"
#include "SouthArrowController.h"

void registerArcGisQtToolkitPlugins()
{
  qmlRegisterType<NorthArrowController>("esri.arcgisruntime.toolkit.NorthArrowController", 1, 0, "NorthArrowController");
  qmlRegisterType<SouthArrowController>("esri.arcgisruntime.toolkit.SouthArrowController", 1, 0, "SouthArrowController");
}
