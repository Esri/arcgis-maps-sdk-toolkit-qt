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

#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ARCGISRUNTIMEENVIRONMENTPROXY_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ARCGISRUNTIMEENVIRONMENTPROXY_H

// Qt headers
#include <QObject>
#include <QString>

namespace Esri::ArcGISRuntime::Authentication
{
  class ArcGISAuthenticationChallengeHandler;
}

class ArcGISRuntimeEnvironmentProxy : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
public:
  Q_INVOKABLE explicit ArcGISRuntimeEnvironmentProxy(QObject* parent = nullptr);
  ~ArcGISRuntimeEnvironmentProxy() override;

  QString apiKey() const;
  void setApiKey(const QString& apiKey);

  Q_INVOKABLE void cacheCurrentChallengeHandler();
  Q_INVOKABLE void prepareForDemoSwitch();
  void resetAuthenticationState();

signals:
  void apiKeyChanged();

private:
  QString m_cachedApiKey;
  Esri::ArcGISRuntime::Authentication::ArcGISAuthenticationChallengeHandler* m_toolkitChallengeHandler = nullptr;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_ARCGISRUNTIMEENVIRONMENTPROXY_H
