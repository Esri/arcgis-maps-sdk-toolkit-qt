/*******************************************************************************
 *  Copyright 2012-2026 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_IOSWEBAUTHENTICATIONSESSION_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_IOSWEBAUTHENTICATIONSESSION_H

// Qt headers
#include <QObject>
#include <QtGlobal>

class QUrl;
class QString;

#ifdef Q_OS_IOS
Q_FORWARD_DECLARE_OBJC_CLASS(ASWebAuthenticationSession);
Q_FORWARD_DECLARE_OBJC_CLASS(NSObject);
#endif

namespace Esri::ArcGISRuntime::Toolkit
{

#ifdef Q_OS_IOS

  class IOSWebAuthenticationSession : public QObject
  {
    Q_OBJECT
  public:
    explicit IOSWebAuthenticationSession(QObject* parent = nullptr);
    ~IOSWebAuthenticationSession() override;

    void start(const QUrl& authorizationUrl, const QString& callbackUrlScheme, bool prefersEphemeralWebBrowserSession = false);
    void cancel();

  signals:
    void completed(const QUrl& callbackUrl);
    void canceled();
    void failed(const QString& error);

  private:
    ASWebAuthenticationSession* m_session = nullptr;
    NSObject* m_presentationContextProvider = nullptr;
  };

#endif // Q_OS_IOS

} // namespace Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_IOSWEBAUTHENTICATIONSESSION_H
