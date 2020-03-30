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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATIONCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATIONCONTROLLER_H

// ArcGISRuntime headers
#include <CoreTypes.h>

// Qt headers
#include <QObject>
#include <QPointer>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 * \brief In MVC architecture, this is the controller for the corresponding
 * PopupView.
 * 
 * This controller is a thin wrapper around a PopupManager. It re-exposes some
 * PopupManager properties, including the number of total rows to render as a 
 * property.
 */
class AuthenticationController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(ChallengeType currentChallenge READ currentChallenge NOTIFY challengeChanged)

public:
  /*!
   * \brief Constructor
   * \param parent Parent owning QObject.
   */
  explicit Q_INVOKABLE AuthenticationController(QObject* parent = nullptr);

  /*!
   * \brief Destructor.
   */
  ~AuthenticationController() override;

  AuthenticationChallenge* currentChallenge() const;

signals:
    void challengeChanged();

private:
    QPointer<AuthenticationChallenge> m_currentChallenge;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATIONCONTROLLER_H
