#include "register_plugins.h"

#include <QQmlEngine>

#include "NorthArrowController.h"
#include "SouthArrowController.h"

void registerArcGisQtToolkitPlugins()
{
  qmlRegisterType<NorthArrowController>("esri.arcgisruntime.toolkit.controllers", 1, 0, "NorthArrowController");
  qmlRegisterType<SouthArrowController>("esri.arcgisruntime.toolkit.controllers", 1, 0, "SouthArrowController");
}
