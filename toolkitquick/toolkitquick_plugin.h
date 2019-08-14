#pragma once

#include <QQmlExtensionPlugin>

class ToolkitquickPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  void registerTypes(const char* uri);
};
