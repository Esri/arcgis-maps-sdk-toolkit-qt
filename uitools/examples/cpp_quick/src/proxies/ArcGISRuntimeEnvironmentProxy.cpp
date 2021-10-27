#include "ArcGISRuntimeEnvironmentProxy.h"

#include <ArcGISRuntimeEnvironment.h>

ArcGISRuntimeEnvironmentProxy::ArcGISRuntimeEnvironmentProxy(QObject* parent) :
  QObject(parent)
{
}

ArcGISRuntimeEnvironmentProxy::~ArcGISRuntimeEnvironmentProxy()
{
}

QString ArcGISRuntimeEnvironmentProxy::apiKey() const
{
  using namespace Esri::ArcGISRuntime;
  return ArcGISRuntimeEnvironment::apiKey();
}

void ArcGISRuntimeEnvironmentProxy::setApiKey(const QString& apiKey)
{
  using namespace Esri::ArcGISRuntime;
  const auto oldApiKey = ArcGISRuntimeEnvironment::apiKey();
  if (oldApiKey != apiKey)
  {
    ArcGISRuntimeEnvironment::setApiKey(apiKey);
    emit apiKeyChanged();
  }
}
