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
#ifndef ESRI_ARCGISRUNTIME_CLIENTCERTIFICATEPASSWORDDIALOG_H
#define ESRI_ARCGISRUNTIME_CLIENTCERTIFICATEPASSWORDDIALOG_H

#include <QDialog>
#include <QUrl>

namespace Ui
{
class ClientCertificatePasswordDialog;
}

namespace Esri
{
namespace ArcGISRuntime
{

namespace Toolkit
{

class AuthenticationController;

class ClientCertificatePasswordDialog : public QDialog 
{
  Q_OBJECT
public:
  explicit ClientCertificatePasswordDialog(QUrl certificateFile, AuthenticationController* controller, QWidget* parent = nullptr);
  ~ClientCertificatePasswordDialog() override;

private:
  QUrl m_certificateFile{};
  AuthenticationController* m_controller;
  Ui::ClientCertificatePasswordDialog* m_ui = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_CLIENTCERTIFICATEPASSWORDDIALOG_H
