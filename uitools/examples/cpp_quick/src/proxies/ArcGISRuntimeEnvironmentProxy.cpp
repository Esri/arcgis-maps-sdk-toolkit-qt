// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
#include "ArcGISRuntimeEnvironmentProxy.h"

#include <ArcGISRuntimeEnvironment.h>

/*!
    \internal
    \class ArcGISRuntimeEnvironmentProxy
    \brief This class exposes the global `ArcGISRuntimeEnvironmentProxy.apiKey` property such
    that it is accessible to QML.
*/

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
