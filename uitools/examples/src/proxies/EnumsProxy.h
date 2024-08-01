/*******************************************************************************
 *  Copyright 2012-2022 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

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
