
/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
#include "BookmarkListItem.h"

// ArcGISRuntime headers
#include <Bookmark.h>

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
    \inmodule Esri.ArcGISRuntime.Toolkit
    \class Esri::ArcGISRuntime::Toolkit::BookmarkListItem
    \internal

    This class is an internal implementation detail and is subject to change.
   */

  BookmarkListItem::BookmarkListItem(QObject* parent) :
    BookmarkListItem(nullptr, parent)
  {
  }

  BookmarkListItem::BookmarkListItem(Bookmark* bookmark, QObject* parent) :
    QObject(parent),
    m_bookmark(bookmark)
  {
    connect(this, &BookmarkListItem::bookmarkChanged, this, &BookmarkListItem::nameChanged);
  }

  BookmarkListItem::~BookmarkListItem() = default;

  void BookmarkListItem::setBookmark(Bookmark* bookmark)
  {
    if (m_bookmark == bookmark)
    {
      return;
    }

    m_bookmark = bookmark;
    emit bookmarkChanged();
  }

  Bookmark* BookmarkListItem::bookmark() const
  {
    return m_bookmark;
  }

  QString BookmarkListItem::name() const
  {
    return m_bookmark ? m_bookmark->name() : QString{};
  }

  void BookmarkListItem::setName(const QString& name)
  {
    if (m_bookmark && m_bookmark->name() != name)
    {
      m_bookmark->setName(name);
      emit nameChanged();
    }
  }

} // Esri::ArcGISRuntime::Toolkit
