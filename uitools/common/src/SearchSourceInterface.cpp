
/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#include "SearchSourceInterface.h"

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
    \class Esri::ArcGISRuntime::Toolkit::SearchSourceInterface
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  SearchSourceInterface::SearchSourceInterface(QObject* parent) :
    QObject(parent)
  {
  }

  SearchSourceInterface::~SearchSourceInterface()
  {
  }

  QString SearchSourceInterface::displayName() const
  {
    return m_displayName;
  }

  void SearchSourceInterface::setDisplayName(QString displayName)
  {
    if (displayName == m_displayName)
      return;

    m_displayName = std::move(displayName);
    emit displayNameChanged();
  }

} // Esri::ArcGISRuntime::Toolkit
