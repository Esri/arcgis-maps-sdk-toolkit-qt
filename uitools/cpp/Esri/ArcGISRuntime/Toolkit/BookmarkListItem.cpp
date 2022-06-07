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

namespace Esri {
  namespace ArcGISRuntime {
    namespace Toolkit {

    /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::BookmarkListItem
    \brief An item contained within \l BookmarksViewController::bookmarks. This class wraps
    a \c Bookmark for easy manipulation/inspection within an AbstractItemModel.
   */

    /*!
    \brief Constructs a new empty BookmarkListItem object with a given \a parent.
   */
    BookmarkListItem::BookmarkListItem(QObject* parent) :
      BookmarkListItem(nullptr, parent)
    {
    }

    /*!
    \brief Constructs a new BookmarkListItem object with a given \a bookmark and
    \a parent.
   */
      BookmarkListItem::BookmarkListItem(Bookmark* bookmark, QObject* parent) :
        QObject(parent),
        m_bookmark(bookmark)
      {
        connect(this, &BookmarkListItem::bookmarkChanged, this, &BookmarkListItem::nameChanged);
      }

    /*!
    \brief Destructor.
   */
    BookmarkListItem::~BookmarkListItem() = default;

    /*!
    \brief Change the underlying item to \a bookmark.
   */
    void BookmarkListItem::setBookmark(Bookmark* bookmark)
    {
      if (m_bookmark == bookmark)
      {
        return;
      }

      m_bookmark = bookmark;
      emit bookmarkChanged();
    }

    /*!
    \brief Returns the current \c bookmark.
   */
    Bookmark* BookmarkListItem::bookmark() const
    {
      return m_bookmark;
    }

    /*!
    \property Esri::ArcGISRuntime::Toolkit::BookmarkListItem::name
    \brief Returns the name of the item.
   */
    QString BookmarkListItem::name() const
    {
      return m_bookmark ? m_bookmark->name() : QString{};
    }

    /*!
    \brief Sets the underlying item to \a name.
   */
    void BookmarkListItem::setName(const QString& name)
    {
      if (m_bookmark && m_bookmark->name() != name)
      {
        m_bookmark->setName(name);
        emit nameChanged();
      }
    }

    /*!
    \fn void Esri::ArcGISRuntime::Toolkit::BookmarkListItem::bookmarkChanged()
    \brief Signal emitted when the \l bookmark changes.
   */

    /*!
    \fn void Esri::ArcGISRuntime::Toolkit::BookmarkListItem::nameChanged()
    \brief Signal emitted when the \l bookmark name changes.
   */

    } // Toolkit
  } // ArcGISRuntime
} // Esri
