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

  if (auto a = attachments())
    disconnect(a, nullptr, this, nullptr);

  if (auto d = displayFields())
    disconnect(d, nullptr, this, nullptr);

  m_popupManager = popupManager;

  if (auto a = attachments())
  {
    connect(a, &QAbstractListModel::rowsInserted , this, &PopupViewController::attachmentCountChanged);
    connect(a, &QAbstractListModel::rowsRemoved , this, &PopupViewController::attachmentCountChanged);
  }

  if (auto d = displayFields())
  {
    connect(d, &QAbstractListModel::rowsInserted , this, &PopupViewController::fieldCountChanged);
    connect(d, &QAbstractListModel::rowsRemoved , this, &PopupViewController::fieldCountChanged);
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

int PopupViewController::fieldCount_() const
{
  if (auto d = displayFields()) {
    return d->rowCount();
  }
  return 0;
}

int PopupViewController::attachmentCount_() const
{
  if (auto a = attachments())
    return a->rowCount();

  return 0;
}

QString PopupViewController::title() const
{
  return m_popupManager ? m_popupManager->title() : nullptr;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
