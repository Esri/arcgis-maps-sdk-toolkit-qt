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

#include "IOSWebAuthenticationSession.h"

#ifdef Q_OS_IOS

#import <AuthenticationServices/AuthenticationServices.h>
#import <UIKit/UIKit.h>

#include <QMetaObject>
#include <QPointer>
#include <QUrl>

namespace
{
  static UIWindow* presentationWindow()
  {
    UIApplication* app = UIApplication.sharedApplication;

    for (UIScene* scene in app.connectedScenes)
    {
      if (scene.activationState != UISceneActivationStateForegroundActive)
      {
        continue;
      }

      if (![scene isKindOfClass:[UIWindowScene class]])
      {
        continue;
      }

      UIWindowScene* windowScene = (UIWindowScene*)scene;
      for (UIWindow* w in windowScene.windows)
      {
        if (w.isKeyWindow)
        {
          return w;
        }
      }

      if (windowScene.windows.count > 0)
      {
        return windowScene.windows.firstObject;
      }
    }

    return nil;
  }
} // namespace

@interface QRTWebAuthPresentationContextProvider : NSObject<ASWebAuthenticationPresentationContextProviding>
@end

@implementation QRTWebAuthPresentationContextProvider

- (ASPresentationAnchor)presentationAnchorForWebAuthenticationSession:(ASWebAuthenticationSession*)session
{
  Q_UNUSED(session);
  UIWindow* w = presentationWindow();
  return w ? w : (UIWindow*)nil;
}

@end

namespace Esri::ArcGISRuntime::Toolkit
{

  IOSWebAuthenticationSession::IOSWebAuthenticationSession(QObject* parent) :
    QObject(parent)
  {
    m_presentationContextProvider = [[QRTWebAuthPresentationContextProvider alloc] init];
  }

  IOSWebAuthenticationSession::~IOSWebAuthenticationSession()
  {
    cancel();

    if (m_presentationContextProvider)
    {
      [m_presentationContextProvider release];
      m_presentationContextProvider = nullptr;
    }
  }

  void IOSWebAuthenticationSession::start(const QUrl& authorizationUrl, const QString& callbackUrlScheme, bool prefersEphemeralWebBrowserSession)
  {
    cancel();

    const auto authUrlString = authorizationUrl.toString();
    NSURL* url = [NSURL URLWithString:authUrlString.toNSString()];

    if (!url)
    {
      emit failed(QStringLiteral("Invalid authorization URL"));
      return;
    }

    const QString scheme = callbackUrlScheme.trimmed();
    if (scheme.isEmpty())
    {
      emit failed(QStringLiteral("Redirect URI has no callback URL scheme"));
      return;
    }

    QPointer<IOSWebAuthenticationSession> guard(this);
    m_session =
      [[ASWebAuthenticationSession alloc] initWithURL:url
                                    callbackURLScheme:scheme.toNSString()
                                    completionHandler:^(NSURL* _Nullable callbackURL, NSError* _Nullable error) {
                                      if (!guard)
                                      {
                                        return;
                                      }

                                      IOSWebAuthenticationSession* self = guard.data();

                                      if (error)
                                      {
                                        const bool canceledLogin = [error.domain isEqualToString:ASWebAuthenticationSessionErrorDomain] &&
                                                                   error.code == ASWebAuthenticationSessionErrorCodeCanceledLogin;

                                        const QString message = QString::fromNSString(error.localizedDescription);

                                        QMetaObject::invokeMethod(self, [self, canceledLogin, message]()
                                        {
                                          if (canceledLogin)
                                          {
                                            emit self->canceled();
                                          }
                                          else
                                          {
                                            emit self->failed(message.isEmpty() ? QStringLiteral("ASWebAuthenticationSession failed") : message);
                                          }
                                        }, Qt::QueuedConnection);

                                        return;
                                      }

                                      if (!callbackURL)
                                      {
                                        QMetaObject::invokeMethod(self, [self]()
                                        {
                                          emit self->failed(QStringLiteral("ASWebAuthenticationSession returned no callback URL"));
                                        }, Qt::QueuedConnection);
                                        return;
                                      }

                                      NSString* absoluteString = callbackURL.absoluteString;
                                      const QUrl qtCallbackUrl{QString::fromNSString(absoluteString)};

                                      QMetaObject::invokeMethod(self, [self, qtCallbackUrl]()
                                      {
                                        emit self->completed(qtCallbackUrl);
                                      }, Qt::QueuedConnection);
                                    }];

    m_session.presentationContextProvider = (id<ASWebAuthenticationPresentationContextProviding>)m_presentationContextProvider;
    m_session.prefersEphemeralWebBrowserSession = prefersEphemeralWebBrowserSession;

    const BOOL started = [m_session start];
    if (!started)
    {
      emit failed(QStringLiteral("Unable to start ASWebAuthenticationSession"));
      cancel();
    }
  }

  void IOSWebAuthenticationSession::cancel()
  {
    if (m_session)
    {
      [m_session cancel];

      [m_session release];
      m_session = nullptr;
    }
  }

} // namespace Esri::ArcGISRuntime::Toolkit

#endif // Q_OS_IOS
