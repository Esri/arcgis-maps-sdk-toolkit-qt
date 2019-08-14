#include "toolkitquick_plugin.h"

#ifdef NORTHARROW
#include "NorthArrowController.h"
#endif

#include <qqml.h>

void ToolkitquickPlugin::registerTypes(const char* uri)
{
  // @uri esri.arcgisruntime.toolkit

#ifdef NORTHARROW
  qmlRegisterType<NorthArrowController>(uri, 1, 0, "NorthArrowController");
#endif
}

