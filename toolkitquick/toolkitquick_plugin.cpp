#include "toolkitquick_plugin.h"
#include "NorthArrowController.h"

#include <qqml.h>

void ToolkitquickPlugin::registerTypes(const char* uri)
{
  // @uri esri.arcgisruntime.toolkitquick
  qmlRegisterType<NorthArrowController>(uri, 1, 0, "NorthArrowController");
}

