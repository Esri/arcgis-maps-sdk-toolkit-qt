#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ENUMSPROXY_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ENUMSPROXY_H

#include <QObject>

#include <MapTypes.h>

class EnumsProxy : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit EnumsProxy(QObject* parent = nullptr);
  ~EnumsProxy() override;

  enum class LoadStatus 
  {
    LoadStatusLoaded = static_cast<int>(Esri::ArcGISRuntime::LoadStatus::Loaded),
    LoadStatusLoading = static_cast<int>(Esri::ArcGISRuntime::LoadStatus::Loading),
    LoadStatusFailedToLoad = static_cast<int>(Esri::ArcGISRuntime::LoadStatus::FailedToLoad),
    LoadStatusNotLoaded = static_cast<int>(Esri::ArcGISRuntime::LoadStatus::NotLoaded),
    LoadStatusUnknown = static_cast<int>(Esri::ArcGISRuntime::LoadStatus::Unknown),
  };
  Q_ENUM(LoadStatus)
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ENUMSPROXY_H