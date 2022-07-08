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

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 \class Esri::ArcGISRuntime::Toolkit::AuthenticationController
 \inmodule EsriArcGISRuntimeToolkit
 \ingroup ArcGISQtToolkitUiCppControllers
 \brief In MVC architecture, this is the controller for the corresponding
 AuthenticationView.

 This controller is a thin wrapper around the AuthenticationManager. As
 AuthenticationManager challenges are queued, the controller holds onto a
 "current" challenge, which is the challenge the user is presented with, which
 will be discarded once the user chooses an action to perform on the challenge.
 */

/*!
  \brief Constructor
  \list
    \li \a parent Owning QObject.
  \endlist
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

      // Auth manager normally only emits a challenge after previous one has
      // been processed. But if it starts hammering us here we will clean up the
      // unprocessed challenges.
      if (m_currentChallenge)
      {
        m_currentChallenge->cancel();
        if (m_deleteChallengeOnProcessed)
          m_currentChallenge->deleteLater();
      }

      m_currentChallenge = challenge;
      emit currentChallengeChanged();
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
  // cancel the current challenge on destruction to ensure new challenges can be issued
  // for future requests (e.g. if AuthenticationView is declared within a StackView)
  if (m_currentChallenge)
    m_currentChallenge->cancel();
}

/*!
 \brief Adds a certificate and password to the AuthenticationManager.
 \sa clientCertificateInfos
 \list
 \li \a clientCertificate Url of certificate to add.
 \li \a password Optional password to open certificate.
 \endlist
 */
void AuthenticationController::addClientCertificate(const QUrl& clientCertificate, const QString& password)
{
  AuthenticationManager::addClientCertificate(clientCertificate, password);
}

/*!
 \brief Returns the list of ClientCertificateInfo strings currently held by the
 AuthenticationManager.
 \sa addClientCertificate
 */
QStringList AuthenticationController::clientCertificateInfos() const
{
  return AuthenticationManager::clientCertificateInfos();
}

/*!
 \brief Returns the authorization url of the current AuthenticationChallenge.
 */
QUrl AuthenticationController::currentChallengeUrl() const
{
    return m_currentChallenge ? m_currentChallenge->authorizationUrl() : QUrl{};
}

/*!
 \brief Returns the authorization hostname of the current
 AuthenticationChallenge.
 */
QString AuthenticationController::currentAuthenticatingHost() const
{
  return m_currentChallenge ? m_currentChallenge->authenticatingHost().toString(): QString{};
}

/*!
 \brief Returns the type of the current challenge as an int.

  \table
  \header
      \li Challenge type
      \li Constant
  \row
      \li \c Unknown
      \li \c 0
  \row
      \li \c UsernamePassword
      \li \c 1
  \row
      \li \c OAuth
      \li \c 2
  \row
      \li \c ClientCertificate
      \li \c 3
  \row
      \li \c SslHandshake
      \li \c 4
  \endtable

 */
int AuthenticationController::currentChallengeType() const
{
  if (m_currentChallenge)
      return static_cast<int>(m_currentChallenge->authenticationChallengeType());
  else
      return static_cast<int>(AuthenticationChallengeType::Unknown);
}

/*!
 \brief Returns the failure count of the current AuthenticationChallenge.
 */
int AuthenticationController::currentChallengeFailureCount() const
{
  return m_currentChallenge ? m_currentChallenge->failureCount() : 0;
}

/*!
 \brief Sets the \l deleteChallengeOnProcessed flag.

 \sa deleteChallengeOnProcessed

 \list
 \li \a deleteFlag When \c true, the current challenge is deleted after it has
     been processed. Otherwise the challenge is not deleted after processing.
 \endlist
 */
void AuthenticationController::setDeleteChallengeOnProcessed(bool deleteFlag)
{
  m_deleteChallengeOnProcessed = deleteFlag;
}

/*!
 \brief After a challenge has been processed, the AuthenticationController
 clears the reference to the currently held challenge. When the returned value
 is \c true then the challenge is also deleted when the reference is cleared.

 In most cases it is desired to for the controller to clean up challenges that
 have been processed, but this options may be turned off in cases where there
 may be additional references to the AuthenticationChallenge emitted by the
 AuthenticationManager in the codebase.

 Defaults to \c true.

 \sa setDeleteChallengeOnProcessed
 */
bool AuthenticationController::deleteChallengeOnProcessed() const
{
  return m_deleteChallengeOnProcessed;
}

/*!
 \brief Calls \c continueWithUsernamePassword on the current challenge.

 \list
 \li \a username Username string.
 \li \a password Password string.
 \endlist
 */
void AuthenticationController::continueWithUsernamePassword(const QString& username, const QString& password)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithUsernamePassword(username, password);
}

/*!
 \brief Calls \c continueWithOAuthAuthorizationCode on the current challenge.

 \list
 \li \a oAuthAuthorizationCode Authorization code.
 \endlist
 */
void AuthenticationController::continueWithOAuthAuthorizationCode(const QString& oAuthAuthorizationCode)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithOAuthAuthorizationCode(oAuthAuthorizationCode);
}

/*!
 \brief Calls \c continueWithClientCertificate on the current challenge.

 \list
 \li \a clientCertificateIndex The index is the index of the certificate
     as seen in \l clientCertificateInfos.
 \endlist
 */
void AuthenticationController::continueWithClientCertificate(int clientCertificateIndex)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithClientCertificate(clientCertificateIndex);
}

/*!
 \brief Calls \c continueWithSslHandshake on the current challenge.

 \list
 \li \a trust When true, trusts the misconfigured resource.
 \li \a remember Remember the user's preferences this session.
 \endlist
 */
void AuthenticationController::continueWithSslHandshake(bool trust, bool remember)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->continueWithSslHandshake(trust, remember);
}

/*!
 \brief Calls \c cancel on the current challenge.
 */
void AuthenticationController::cancel()
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->cancel();
}

/*!
 \brief Calls \c cancelWithError on the current challenge, with the \a title
 and the \a html content from the web view.
 */
void AuthenticationController::cancelWithError(const QString& title, const QString& html)
{
  if (!m_currentChallenge)
    return;

  auto c = m_currentChallenge;
  cleanup();
  c->cancelWithError(title, html);
}

/*!
 \internal
 \brief Clears the reference to the current challenge. Also deletes the current
 challenge where applicable.
 */
void AuthenticationController::cleanup()
{
  // This controller has the option of
  // "owning" the challenge and may delete
  // it after use.
  if (m_deleteChallengeOnProcessed)
    m_currentChallenge->deleteLater();

  m_currentChallenge = nullptr;
  emit currentChallengeChanged();
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::AuthenticationController::currentChallengeChanged()
  \brief Emitted when the reference to the current challenge changes.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::AuthenticationController::clientCertificateInfosChanged()
  \brief Emitted when the clientCertificateInfos updates.
  \sa clientCertificateInfos
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::AuthenticationController::clientCertificatePasswordRequired(QUrl certificate)
  \brief Emitted when a \a certificate that was added to the AuthenticationManager requires a password.
  \sa addClientCertificate
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::AuthenticationController::currentChallengeUrl
  \brief The Url of the current challenge.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::AuthenticationController::currentAuthenticatingHost
  \brief Authenticating host of current challenge.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::AuthenticationController::currentChallengeType
  \brief Type of current challenge.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::AuthenticationController::currentChallengeFailureCount
  \brief Failure count of current challenge.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::AuthenticationController::clientCertificateInfos
  \brief List of strings representing the certificates held by the AuthenticationManager.
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
