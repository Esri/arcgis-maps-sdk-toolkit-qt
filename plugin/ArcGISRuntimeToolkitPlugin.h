#ifndef ARCGISRUNTIME_TOOLKIT_PLUGIN_H
#define ARCGISRUNTIME_TOOLKIT_PLUGIN_H

#include <QQmlExtensionPlugin>

class ArcGISRuntimeToolkitPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  ArcGISRuntimeToolkitPlugin(QObject* parent = nullptr);

  ~ArcGISRuntimeToolkitPlugin() override;

  void registerTypes(const char *uri) override;
};

#endif