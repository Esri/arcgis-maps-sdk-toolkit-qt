#include "register_plugins.h"

#include <QQmlEngine>

#include "NorthArrowController.h"

void registerArcGisQtToolkitPlugins()
{
  qmlRegisterType<NorthArrowController>("esri.arcgisruntime.toolkit.controller", 1, 0, "NorthArrowController");
}
