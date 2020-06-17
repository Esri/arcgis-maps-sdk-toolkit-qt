/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#include "AuthenticationController.h"

// ArcGISRuntime headers
#include <AuthenticationManager.h>

#include <QDebug>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionController
 \inmodule EsriArcGISRuntimeToolkit
 \brief In MVC architecture, this is the controller for the corresponding
 AuthenticationView.
  
 This controller is a thin wrapper around a AuthenticationManager.
 */

/*!
  \brief Constructor
  \param parent Parent owning QObject.
 */
AuthenticationController::AuthenticationController(QObject* parent) :
  QObject(parent)
{
  connect(AuthenticationManager::instance(),
          &AuthenticationManager::authenticationChallenge, this,
    [this](AuthenticationChallenge* challenge)
    {
      if (challenge == m_currentChallenge)
          return;

      qDebug() << "CHALLENGE POINTER:" << challenge;
      // Auth manager normally only emits a challenge after previous one has
      // been processed. But if it starts hammering us here we will clean up the 
      // unprocessed challenges.
      if (m_currentChallenge) {
        m_currentChallenge->cancel();
        if (m_deleteChallengeOnProcessed)
          m_currentChallenge->deleteLater();
      }

      m_currentChallenge = challenge;
      emit challengeChanged();
    }, Qt::QueuedConnection
  );

  connect(AuthenticationManager::instance(), &AuthenticationManager::clientCertificateInfosChanged,
          this, &AuthenticationController::clientCertificateInfosChanged);

  connect(AuthenticationManager::instance(), &AuthenticationManager::clientCertificatePasswordRequired,
          this, &AuthenticationController::clientCertificatePasswordRequired);
}

/*!
  \brief Destructor.
 */
AuthenticationController::~AuthenticationController()
{
}

void AuthenticationController::addClientCertificate(const QUrl& clientCertificate, const QString& password)
{
  AuthenticationManager::instance()->addClientCertificate(clientCertificate, password);
}

QStringList AuthenticationController::clientCertificateInfos() const
{
  return AuthenticationManager::instance()->clientCertificateInfos();
}

AuthenticationChallenge* AuthenticationController::currentChallenge() const
{
  return m_currentChallenge.data();
}

QUrl AuthenticationController::currentChallengeUrl() const
{
    return m_currentChallenge ? m_currentChallenge->authorizationUrl() : QUrl{};
}

QString AuthenticationController::currentAuthenticatingHost() const
{
  return m_currentChallenge ? m_currentChallenge->authenticatingHost().toString(): QString{};
}

int AuthenticationController::currentChallengeType() const
{
  if (m_currentChallenge)
      return static_cast<int>(m_currentChallenge->authenticationChallengeType());
  else
      return static_cast<int>(AuthenticationChallengeType::Unknown);
}

int AuthenticationController::currentChallengeFailureCount() const
{
  return m_currentChallenge ? m_currentChallenge->failureCount() : 0;
}

void AuthenticationController::setDeleteChallengeOnProcessed(bool deleteFlag)
{
  m_deleteChallengeOnProcessed = deleteFlag;
}

bool AuthenticationController::deleteChallengeOnProcessed() const
{
  return m_deleteChallengeOnProcessed;
}

void AuthenticationController::continueWithUsernamePassword(const QString& username, const QString& password)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithUsernamePassword(username, password);
}

void AuthenticationController::continueWithOAuthAuthorizationCode(const QString& oAuthAuthorizationCode)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithOAuthAuthorizationCode(oAuthAuthorizationCode);
}

void AuthenticationController::continueWithClientCertificate(int clientCertificateIndex)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithClientCertificate(clientCertificateIndex);
}

void AuthenticationController::continueWithSslHandshake(bool trust, bool remember)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithSslHandshake(trust, remember);
}

void AuthenticationController::cancel()
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->cancel();
}

void AuthenticationController::cleanup()
{
  // This controller has the option of
  // "owning" the challenge and may delete
  // it after use.
  if (m_deleteChallengeOnProcessed) 
    m_currentChallenge->deleteLater();

  m_currentChallenge = nullptr;
  emit challengeChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
