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
#include "PopupViewController.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

PopupViewController::PopupViewController(QObject* parent):
  QObject(parent)
{
}

PopupViewController::~PopupViewController()
{
}

PopupManager* PopupViewController::popupManager() const
{
  return m_popupManager;
}

void PopupViewController::setPopupManager(PopupManager* popupManager)
{
  if (popupManager == m_popupManager)
    return;

  if (m_popupManager)
    disconnect(m_popupManager.data(), nullptr, this, nullptr);

  if (auto attachments = this->attachments())
    disconnect(attachments, nullptr, this, nullptr);

  if (auto displayFields = this->displayFields())
    disconnect(displayFields, nullptr, this, nullptr);

  m_popupManager = popupManager;

  if (m_popupManager)
    connect(m_popupManager.data(), &QObject::destroyed, this, &PopupViewController::popupManagerChanged);

  if (auto attachments = this->attachments())
  {
    connect(attachments, &QAbstractListModel::rowsInserted , this, &PopupViewController::attachmentCountChanged);
    connect(attachments, &QAbstractListModel::rowsRemoved , this, &PopupViewController::attachmentCountChanged);
  }

  if (auto displayFields = this->displayFields())
  {
    connect(displayFields, &QAbstractListModel::rowsInserted , this, &PopupViewController::fieldCountChanged);
    connect(displayFields, &QAbstractListModel::rowsRemoved , this, &PopupViewController::fieldCountChanged);
  }

  emit popupManagerChanged();
  emit fieldCountChanged();
  emit attachmentCountChanged();
}

QAbstractListModel* PopupViewController::displayFields() const
{
  return m_popupManager ? m_popupManager->displayedFields() : nullptr;
}

QAbstractListModel* PopupViewController::attachments() const
{
  if (!m_popupManager)
    return nullptr;

  auto attachmentManager = m_popupManager->attachmentManager();
  if (!attachmentManager)
    return nullptr;

  return attachmentManager->attachmentsModel();
}

int PopupViewController::fieldCount() const
{
  if (auto displayFields = this->displayFields())
    return displayFields->rowCount();

  return 0;
}

int PopupViewController::attachmentCount() const
{
  if (auto attachments = this->attachments())
    return attachments->rowCount();

  return 0;
}

QString PopupViewController::title() const
{
  // This is re-exposed from PopupManager as PopupManager does not have
  // NOTIFY/CONSTANT modifiers on its title property, so the Controller
  // re-exposes title to suppress warnings about this.
  return m_popupManager ? m_popupManager->title() : nullptr;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
