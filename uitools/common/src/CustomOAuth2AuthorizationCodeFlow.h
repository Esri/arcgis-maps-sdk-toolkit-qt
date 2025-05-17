/*******************************************************************************
 *  Copyright 2012-2025 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_CUSTOMOAUTH2AUTHORIZATIONCODEFLOW_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_CUSTOMOAUTH2AUTHORIZATIONCODEFLOW_H

// Qt headers
#include <QAbstractOAuth2>
#include <QUrl>

namespace Esri::ArcGISRuntime::Toolkit {

class CustomOAuth2AuthorizationCodeFlow : public QAbstractOAuth2
{
  Q_OBJECT
public:
  CustomOAuth2AuthorizationCodeFlow(QUrl m_authorizeUrl, QObject* parent);
  ~CustomOAuth2AuthorizationCodeFlow() override;

  void grant() override;

  QString authorizationCode() const;
  void setAuthorizationCode(const QString& code);

private:
  Q_DISABLE_COPY_MOVE(CustomOAuth2AuthorizationCodeFlow)

  QString m_authorizationCode;
  const QUrl m_authorizeUrl;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_CUSTOMOAUTH2AUTHORIZATIONCODEFLOW_H
