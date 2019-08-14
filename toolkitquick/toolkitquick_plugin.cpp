#include "toolkitquick_plugin.h"
#include "northarrow.h"

#include <qqml.h>

void ToolkitquickPlugin::registerTypes(const char *uri)
{
  // @uri esri.arcgisruntime.toolkitquick
  qmlRegisterType<NorthArrow>(uri, 1, 0, "NorthArrow");
}

